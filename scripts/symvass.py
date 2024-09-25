#!/usr/bin/env python3
import os
import sys
import argparse
from typing import List, Tuple, Set, Dict
import multiprocessing as mp
import subprocess
import json
import re
import signal

import traceback
import networkx as nx
import graphviz
import sbsv

import uni_klee
import sympatch


def get_trace(dir: str, id: int):
    dp = uni_klee.DataLogParser(dir)
    dp.read_data_log("data.log")
    result = dp.generate_table_v2(dp.cluster())
    graph = result["graph"]
    state_id = id
    parent_states = dp.get_parent_states(graph, state_id)
    state_filter = set(parent_states)
    state_filter.add(state_id)
    done = set()
    prev = -1
    trace = list()
    with open(os.path.join(dir, "trace.log"), "r") as f:
        lines = f.readlines()
        for line in lines:
            if line.startswith("[state"):
                tokens = dp.parser_level_1(line)
                if len(tokens) < 3:
                    continue
                state = dp.parse_state_id(tokens[0])
                if tokens[1] == "B":
                    continue
                if state in state_filter:
                    if state in done:
                        continue
                    if prev != state:
                        if prev > state:
                            continue
                        trace.append(f"[state {prev}] -> [state {state}]")
                        done.add(prev)
                        prev = state
                    trace.append(line.strip())
    with open(os.path.join(dir, f"state-{id}.log"), "w") as f:
        for line in trace:
            f.write(line + "\n")


def convert_to_sbsv(dir: str):
    with open(f"{dir}/run.stats", "r") as f:
        lines = f.readlines()
        header = lines[0].strip().replace("(", "").replace(")", "").split(",")
        with open(f"{dir}/run.stats.sbsv", "w") as f:
            for i in range(1, len(lines)):
                line = lines[i].strip().replace("(", "").replace(")", "")
                tokens = line.split(",")
                f.write(
                    f"[stats] [inst {tokens[0]}] [fb {tokens[1]}] [pb {tokens[2]}] [nb {tokens[3]}] [ut {tokens[4]}] [ns {tokens[5]}] [mu {tokens[6]}] [nq {tokens[7]}] [nqc {tokens[8]}] [no {tokens[9]}] [wt {tokens[10]}] [ci {tokens[11]}] [ui {tokens[12]}] [qt {tokens[13]}] [st {tokens[14]}] [cct {tokens[15]}] [ft {tokens[16]}] [rt {tokens[17]}] [qccm {tokens[18]}] [ccch {tokens[19]}]\n"
                )


class ConfigFiles(uni_klee.ConfigFiles):
    def __init__(self):
        self.root_dir = uni_klee.ROOT_DIR
        self.global_config = uni_klee.global_config

    def set(self, bug_info: dict):
        patches_dir = os.path.join(self.root_dir, "patches")
        self.bid = bug_info["bug_id"]
        self.benchmark = bug_info["benchmark"]
        self.subject = bug_info["subject"]
        self.project_dir = os.path.join(
            patches_dir, self.benchmark, self.subject, self.bid
        )
        self.work_dir = os.path.join(self.project_dir, "patched")
        self.repair_conf = os.path.join(self.project_dir, "repair.conf")
        self.meta_program = os.path.join(self.project_dir, "meta-program.json")
        sympatch.compile(os.path.join(self.project_dir, "concrete"))
        self.meta_patch_obj_file = os.path.join(
            self.project_dir, "concrete", "libuni_klee_runtime_new.bca"
        )

    def set_out_dir(self, out_dir: str, out_dir_prefix: str, bug_info: dict, snapshot_prefix: str, filter_prefix: str, use_last: bool):
        self.out_dir_prefix = out_dir_prefix
        self.snapshot_prefix = snapshot_prefix
        self.filter_prefix = filter_prefix
        if out_dir == "":
            self.out_base_dir = self.work_dir
        elif out_dir == "out":
            self.out_base_dir = os.path.join(self.root_dir, "out", self.benchmark, self.subject, self.bid)
        else:
            self.out_base_dir = out_dir
        os.makedirs(self.out_base_dir, exist_ok=True)
        no = self.find_num(self.out_base_dir, out_dir_prefix)
        if use_last:
            self.out_dir = os.path.join(self.out_base_dir, f"{out_dir_prefix}-{no-1}")
        else:
            self.out_dir = os.path.join(self.out_base_dir, f"{out_dir_prefix}-{no}")
        self.snapshot_dir = os.path.join(self.out_base_dir, self.snapshot_prefix)
        self.filter_dir = os.path.join(self.out_base_dir, filter_prefix)
        print(f"Use snapshot {self.bid} snapshot-last.json ...")
        self.snapshot_file = os.path.join(self.snapshot_dir, "snapshot-last.json")

class Config(uni_klee.Config):
    conf_files: ConfigFiles

    def __init__(
        self, cmd: str, query: str, debug: bool, sym_level: str, max_fork: str
    ):
        self.cmd = cmd
        self.query = query
        self.debug = debug
        self.sym_level = sym_level
        self.max_fork = max_fork
        self.conf_files = ConfigFiles()

    def append_snapshot_cmd(self, cmd: List[str]):
        snapshot_dir = self.conf_files.snapshot_dir
        patch_str = ",".join(self.snapshot_patch_ids)
        if self.cmd == "filter":
            cmd.append("--no-exit-on-error")
            snapshot_dir = self.conf_files.filter_dir
            all_patches = [str(patch["id"]) for patch in self.meta_program["patches"]]
            patch_str = ",".join(all_patches)
        cmd.append(f"--output-dir={snapshot_dir}")
        cmd.append(f"--patch-id={patch_str}")

    def append_cmd(self, cmd: List[str], patch_str: str, opts: List[str]):
        out_dir = self.conf_files.out_dir
        default_opts = [
            "--no-exit-on-error",
            "--simplify-sym-indices",
            f"--symbolize-level={self.sym_level}",
            f"--max-forks-per-phases={self.max_fork}",
        ]
        cmd.extend(default_opts)
        cmd.extend(opts)
        cmd.append(f"--output-dir={out_dir}")
        cmd.append(f"--patch-id={patch_str}")
        cmd.append(f"--snapshot={self.conf_files.snapshot_file}")

    def get_cmd_opts(self, is_snapshot: bool) -> str:
        target_function = self.bug_info["target"]
        link_opt = f"--link-llvm-lib={self.conf_files.meta_patch_obj_file}"
        result = [
            "uni-klee",
            "--libc=uclibc",
            "--posix-runtime",
            "--external-calls=all",
            "--allocate-determ",
            "--write-smt2s",
            "--write-kqueries",
            "--log-trace",
            "--max-memory=0",
            "--lazy-patch",
            f"--target-function={target_function}",
            link_opt,
        ]
        if "klee_flags" in self.project_conf:
            link_opt = self.project_conf["klee_flags"]
            result.append(link_opt)
        if self.additional != "":
            result.extend(self.additional.split(" "))
        if is_snapshot:
            self.append_snapshot_cmd(result)
        else:
            add_opts = list()
            if self.cmd == "cmp":
                add_opts.append("--start-from-snapshot")
            self.append_cmd(result, ",".join(self.patch_ids), add_opts)
        bin_file = os.path.basename(self.project_conf["binary_path"])
        target = bin_file + ".bc"
        result.append(target)
        if "test_input_list" in self.project_conf:
            poc_path = "exploit"
            if "poc_path" in self.project_conf:
                poc_path = self.project_conf["poc_path"]
            target_cmd = self.project_conf["test_input_list"].replace("$POC", poc_path)
            result.append(target_cmd)
        return " ".join(result)

class SymvassDataLogSbsvParser():
    dir: str
    parser: sbsv.parser
    data: Dict[str, List[dict]]

    def __init__(self, dir: str, name: str = "data.log"):
        self.dir = dir
        self.parser = sbsv.parser()
        self.set_schema(self.parser)
        with open(os.path.join(dir, name), "r") as f:
            self.data = self.parser.load(f)

    def set_schema(self, parser: sbsv.parser):
        parser.add_schema("[meta-data] [state: int] [crashId: int] [patchId: int] [stateType: str] [isCrash: bool] [actuallyCrashed: bool] [exitLoc: str] [exit: str]")
        parser.add_schema("[fork] [state$from: int] [state$to: int]")
        parser.add_schema("[fork-map] [fork] [state$from: int] [type$from: str] [base: int] [base-type: str] [state$to: int] [type$to: str] [fork-count: str]")
        parser.add_schema("[fork-map] [sel-patch] [state$base: int] [state$base_after: int]")
        parser.add_schema("[fork-map] [fork-parent] [state: int] [type: str]")
        parser.add_schema("[fork-map] [merge] [state$base: int] [state$crash_test: int] [patch: int]")
        parser.add_schema("[fork-loc] [br] [state$from: int] [loc$from: str] -> [state$to_a: int] [loc$to_a: str] [state$to_b: int] [loc$to_b: str]")
        parser.add_schema("[fork-loc] [sw] [state$from: int] [loc$from: str] -> [state$to_a: int] [loc$to_a: str] [state$to_b: int] [loc$to_b: str]")
        parser.add_schema("[fork-loc] [lazy] [state$from: int] [state$to: int] [loc: str] [name?: str]")
        parser.add_schema("[patch-base] [trace] [state: int] [res: bool] [iter: int]")
        parser.add_schema("[patch-base] [fork] [state$true: int] [state$false: int] [iter: int]")
        parser.add_schema("[regression-trace] [state: int] [n: int] [res: bool] [loc: str]")
        parser.add_schema("[patch] [trace] [state: int] [iter: int] [res: bool] [patches: str]")
        parser.add_schema("[patch] [trace-rand] [state: int] [iter: int] [res: bool] [patches: str]")
        parser.add_schema("[patch] [fork] [state$true: int] [state$false: int] [iter: int] [patches: str]")
        parser.add_schema("[regression] [state: int] [reg?: str]")
        parser.add_schema("[lazy-trace] [state: int] [reg?: str] [patches?: str]")
        parser.add_schema("[stack-trace] [state: int] [reg?: str]")
    
    def get_data(self) -> Dict[str, List[dict]]:
        return self.data


class DataAnalyzer():
    dp: SymvassDataLogSbsvParser
    data: Dict[str, List[dict]]
    meta_data: Dict[int, dict]
    graph: nx.DiGraph
    symbolic_inputs: Dict[int, List[int]]
    
    def __init__(self, dp: SymvassDataLogSbsvParser):
        self.dp = dp
        self.data = self.dp.get_data()
        self.meta_data = dict()
        self.graph = nx.DiGraph()
        self.symbolic_inputs = dict()
        
    def analyze(self):
        self.construct_graph()
        self.draw_graph()

    def to_list(self, s: str) -> List[int]:
        ls = s.strip("[]").strip(", ")
        ls = ls.split(", ")
        result = list()
        for x in ls:
            try:
                if len(x) > 0:
                    result.append(int(x))
            except:
                pass
        return result

    def construct_graph(self):
        meta_data = self.meta_data
        graph = self.graph
        for meta in self.data["meta-data"]:
            state = meta["state"]
            state_type = meta["stateType"]
            graph.add_node(state, fork_parent=False)
            meta_data[state] = meta.data
            meta_data[state]["sel-patch"] = False
            meta_data[state]["input"] = False
            meta_data[state]["patches"] = list()
            meta_data[state]["reg"] = list()
        for lt in self.data["lazy-trace"]:
            state = lt["state"]
            reg = lt["reg"]
            patches = lt["patches"]
            if patches is None:
                patches = list()
            else:
                patches = self.to_list(patches)
            if reg is None:
                reg = list()
            else:
                reg = self.to_list(reg)
            meta_data[state]["patches"] = patches
            meta_data[state]["reg"] = reg
        for fp in self.data["fork-map"]["fork-parent"]:
            state = fp["state"]
            state_type = fp["type"]
            graph.add_node(state, fork_parent=True)
        # Add edges
        for fork in self.data["fork-map"]["fork"]:
            fork_from = fork["state$from"]
            type_from = fork["type$from"]
            fork_to = fork["state$to"]
            type_to = fork["type$to"]
            graph.add_edge(fork_from, fork_to, type="fork")
        for sel in self.data["fork-map"]["sel-patch"]:
            base = sel["state$base"]
            base_after = sel["state$base_after"]
            if base not in meta_data:
                if base_after not in meta_data:
                    continue
                meta_data[base] = meta_data[base_after].copy()
                meta_data[base]["state"] = base
                meta_data[base]["stateType"] = "1"
                meta_data[base]["sel-patch"] = True
            graph.add_edge(base, base_after, type="sel-patch")
        for merge in self.data["fork-map"]["merge"]:
            base = merge["state$base"]
            crash_test = merge["state$crash_test"]
            patch = merge["patch"]
            meta_data[base]["input"] = True
            graph.add_edge(base, crash_test, type="merge", patch=patch)
            if base not in self.symbolic_inputs:
                self.symbolic_inputs[base] = list()
            self.symbolic_inputs[base].append(crash_test)

    def draw_graph(self):
        dot = graphviz.Digraph()
        for state in self.graph.nodes():
            fillcolor = "white"
            color = "black"
            if state in self.meta_data:
                meta = self.meta_data[state]
                if meta["actuallyCrashed"]:
                    color = "red"
                else:
                    color = "green"
                if "early" in meta["exit"]:
                    color = "grey"
                    fillcolor = "grey"
                else:
                    if meta["sel-patch"]:
                        fillcolor = "pink"
                    elif meta["input"]:
                        fillcolor = "blue"
                    elif meta["stateType"] == "1":
                        fillcolor = "red"
                    elif meta["stateType"] == "2":
                        fillcolor = "skyblue"
                    elif meta["stateType"] == "4":
                        fillcolor = "yellow"
            shape = "ellipse"
            if "fork_parent" in self.graph.nodes[state]:
                if self.graph.nodes[state]["fork_parent"]:
                    shape = "box"
            dot.node(str(state), shape=shape, style="filled", color=color, fillcolor=fillcolor)
        
        for edge in self.graph.edges():
            src = edge[0]
            dst = edge[1]
            style = "solid"
            color = "black"
            if "type" in self.graph[src][dst]:
                if self.graph[src][dst]["type"] == "merge":
                    style = "dashed"
                    color = "red"
                if self.graph[src][dst]["type"] == "sel-patch":
                    style = "dotted"
                    color = "blue"
            dot.edge(str(src), str(dst), style=style, color=color)
        
        dot.render("fork-graph", self.dp.dir, format="png")
        dot.render("fork-graph", self.dp.dir, format="pdf")


class PatchSorter:
    # dir: str
    # patch_ids: list
    # meta_data: dict
    dp: DataAnalyzer
    dp_filter: DataAnalyzer
    cluster: Dict[int, Dict[int, List[Tuple[int, bool]]]]
    patch_ids: List[int]
    patch_filter: Set[int]

    def __init__(
        self, dp: DataAnalyzer, dp_filter: DataAnalyzer = None
    ):
        # self.dir = dir
        # self.patch_ids = patch_ids
        # self.meta_data = config.bug_info
        self.dp = dp
        self.dp_filter = dp_filter
        self.cluster = dict()
        self.patch_ids = list()

    def check_correctness(self, base: dict, patch: dict) -> bool:
        if base["isCrash"]:
            return not patch["actuallyCrashed"]
        base_trace = base["lazyTrace"].split() if "lazyTrace" in base else []
        patch_trace = patch["lazyTrace"].split() if "lazyTrace" in patch else []
        return base_trace == patch_trace

    def filter_patch(self) -> Dict[int, bool]:
        self.patch_filter = dict()
        result = dict()
        for state_id, data in self.dp_filter.meta_data.items():
            patch_id = data["patchId"]
            if data["stateType"] == "3":
                self.patch_filter[patch_id] = data
                result[patch_id] = not data["actuallyCrashed"]
        # original = self.patch_filter[0]
        # if not original["actuallyCrashed"]:
        #     print("Original patch does not crash")
        return result

    def analyze_cluster(self) -> Dict[int, Dict[int, List[Tuple[int, bool]]]]:
        self.cluster: Dict[int, Dict[int, List[Tuple[int, bool]]]] = dict()
        self.cluster[0] = dict()
        filter_result = self.filter_patch()
        patch_set = set()
        for patch_id, result in filter_result.items():
            if result:
                patch_set.add(patch_id)
            self.cluster[0][patch_id] = [(-1, result)]
            print(f"Patch {patch_id} crashed: {result}")
        temp_patches = set()
        for crash_id, data_list in self.dp.cluster().items():
            if crash_id == 0:
                continue
            self.cluster[crash_id] = dict()
            base = data_list[0]
            for data in data_list:
                if data["stateType"] == "2":
                    base = data
                    break
            for data in data_list:
                if data["stateType"] == "2":
                    continue
                patch = data["patchId"]
                temp_patches.add(patch)
                id = data["state"]
                check = self.check_correctness(base, data)
                if patch not in self.cluster[crash_id]:
                    self.cluster[crash_id][patch] = list()
                self.cluster[crash_id][patch].append((id, check))
        if len(self.patch_ids) == 0:
            self.patch_ids = list(temp_patches)
            self.patch_ids.sort()
        return self.cluster

    def get_score(self, patch: int) -> float:
        score = 0.0
        for crash_id, patch_map in self.cluster.items():
            patch_score = 0.0
            if patch not in patch_map:
                print(f"Patch {patch} not found in crash {crash_id}")
                continue
            for patch_result in patch_map[patch]:
                if patch_result[1]:
                    patch_score += 1.0
            score += patch_score / len(patch_map[patch])
        return score

    def sort(self) -> List[Tuple[int, float]]:
        scores: List[Tuple[int, float]] = list()
        for patch in self.patch_ids:
            score = self.get_score(patch)
            scores.append((patch, score))
        scores.sort(key=lambda x: x[1], reverse=True)
        return scores


class SymvassAnalyzer:
    dir: str
    filter_dir: str
    bug_info: dict

    def __init__(self, dir: str, bug_info: dict):
        self.dir = dir
        self.bug_info = bug_info

    def save_sorted_patches(
        self,
        dp: DataAnalyzer,
        sorted_patches: List[Tuple[int, float]],
        cluster: Dict[int, Dict[int, List[Tuple[int, bool]]]],
    ):
        patch_list = sorted([patch for patch, score in sorted_patches])
        with open(os.path.join(self.dir, "patch-rank.md"), "w") as f:
            f.write("## Patch Rank\n")
            f.write(f"| Rank | Patch | Score |\n")
            f.write(f"|------|-------|-------|\n")
            rank = 1
            patch_filter = set()
            for patch_id, result in cluster[0].items():
                if result[0][1]:
                    patch_filter.add(patch_id)
            for patch, score in sorted_patches:
                if patch in patch_filter:
                    f.write(f"| {rank} | {patch} | {score:.2f} |\n")
                    rank += 1
            f.write(f"\n## Removed patch list\n")
            f.write(f"| Rank | Patch | Score |\n")
            f.write(f"|------|-------|-------|\n")
            rank = 1
            for patch, score in sorted_patches:
                if patch not in patch_filter:
                    f.write(f"| {rank} | {patch} | {score:.2f} |\n")
                    rank += 1
            f.write("\n## Patch Result\n")
            cluster_list = list(cluster.items())
            cluster_list.sort(key=lambda x: x[0])
            for crash_id, patch_map in cluster_list:
                f.write(f"### Input {crash_id}\n")
                f.write(f"| Input id | Patch id | state id | Correctness | crashed |\n")
                f.write(f"|----------|----------|----------|-------------|---------|\n")
                for patch in patch_list:
                    result = patch_map[patch]
                    local_input_id = 0
                    for patch_result in result:
                        state = patch_result[0]
                        if state < 0:
                            f.write(
                                f"| {patch}-{local_input_id} | {patch} | {state} | {'O' if patch_result[1] else 'X'} | - |\n"
                            )
                        else:
                            actually_crashed = dp.meta_data[state]["actuallyCrashed"]
                            f.write(
                                f"| {patch}-{local_input_id} | {patch} | {state} | {'O' if patch_result[1] else 'X'} | {actually_crashed} |\n"
                            )
                        local_input_id += 1
        print(f"Saved to {os.path.join(self.dir, 'patch-rank.md')}")
        
    def cluster(self, analyzer: DataAnalyzer) -> Dict[int, Set[int]]:
        cluster: Dict[int, Set[int]] = dict()
        for crash_id in analyzer.symbolic_inputs:
            cluster[crash_id] = set()
            for crash_test in analyzer.symbolic_inputs[crash_id]:
                successors = set(nx.dfs_preorder_nodes(analyzer.graph, crash_test))
                cluster[crash_id].update(successors)
        return cluster
    
    def get_predecessors(self, analyzer: DataAnalyzer, state: int):
        trace = list()
        state_type = set()
        if state not in analyzer.meta_data:
            return []
        stop_type = ""
        if analyzer.meta_data[state]["stateType"] == "4":
            stop_type = "2"
        stack = [state]
        while stack:
            node = stack.pop()
            preds = analyzer.graph.predecessors(node)
            stack += preds
            if len(list(preds)) > 1:
                print(f"[warn] [state {node}] [preds {preds}]")
            for pred in preds:
                if pred in analyzer.meta_data and analyzer.meta_data[pred]["stateType"] in state_type:
                    trace.append(pred)
        return trace
    
    def symbolic_trace(self, analyzer: DataAnalyzer) -> Dict[int, List[Tuple[bool, int, List[Tuple[int, str]]]]]:
        patch_data = analyzer.dp.parser.get_result_in_order(["patch$trace", "patch$trace-rand", "patch$fork"])
        trace_original = dict()
        for patch in reversed(patch_data):
            patches_str = patch["patches"]
            patches_str = patches_str.strip("[]").strip(", ")
            pairs = patches_str.split(", ")
            patches_result = list()
            for pair in pairs:
                key, value = pair.split(":")
                patches_result.append((int(key), value))
            iter = patch["iter"]
            if patch.get_name() == "patch$fork":
                true_state = patch["state$true"]
                false_state = patch["state$false"]
                if true_state not in trace_original:
                    trace_original[true_state] = list()
                if false_state not in trace_original:
                    trace_original[false_state] = list()
                trace_original[true_state].append((True, iter, patches_result))
                trace_original[false_state].append((False, iter, patches_result))
            else:
                res = patch["res"]
                state = patch["state"]
                if state not in trace_original:
                    trace_original[state] = list()
                trace_original[state].append((res, iter, patches_result))
        trace = dict()
        for state in trace_original:
            trace[state] = list()
            check_1 = False
            for res, iter, patches in trace_original[state]:
                if iter == 1:
                    check_1 = True
                    break
            if check_1:
                trace[state] = trace_original[state]
            else:
                pred = self.get_predecessors(analyzer, state)
                cur_iter = -1
                for p in pred:
                    if p not in trace_original:
                        continue
                    for res, iter, patches in trace_original[p]:
                        if cur_iter > 0 and iter >= cur_iter:
                            continue
                        if cur_iter > 0 and cur_iter != iter + 1:
                            print(f"[warn] [error] [state {state}] [iter {cur_iter-1}] [actual {iter}]")
                        cur_iter = iter
                        trace[state].append((res, iter, patches))
            # print(f"[state {state}] {trace[state]}")
        return trace
    
    def buggy_trace(self, analyzer: DataAnalyzer):
        trace_original = dict()
        bt = analyzer.dp.parser.get_result_in_order(["patch-base$trace", "patch-base$fork"])
        for patch_base in reversed(bt):
            iter = patch_base["iter"]
            if patch_base.get_name() == "patch-base$trace":
                state = patch_base["state"]
                res = patch_base["res"]
                if state not in trace_original:
                    trace_original[state] = list()
                trace_original[state].append((res, iter))
            else:
                true_state = patch_base["state$true"]
                false_state = patch_base["state$false"]
                if true_state not in trace_original:
                    trace_original[true_state] = list()
                if false_state not in trace_original:
                    trace_original[false_state] = list()
                trace_original[true_state].append((True, iter))
                trace_original[false_state].append((False, iter))
        trace = dict()
        for state in trace_original:
            trace[state] = list()
            check_1 = False
            for res, iter in trace_original[state]:
                if iter == 1:
                    check_1 = True
                    break
            if check_1:
                trace[state] = trace_original[state]
            else:
                pred = self.get_predecessors(analyzer, state)
                cur_iter = -1
                for p in pred:
                    if p not in trace_original:
                        continue
                    for res, iter in trace_original[p]:
                        if cur_iter > 0 and iter >= cur_iter:
                            continue
                        if cur_iter > 0 and cur_iter != iter + 1:
                            print(f"[warn] [error] [state {state}] [iter {cur_iter-1}] [actual {iter}]")
                        cur_iter = iter
                        trace[state].append((res, iter))
        print(trace)
        return trace

    def get_patch(self, state: int, st: List[Tuple[bool, int, List[Tuple[int, str]]]]) -> List[int]:
        trace = dict()
        patches = set()
        for s in st:
            trace[s[1]] = s[0]
            for p in s[2]:
                if s[0] and p[1] != "0":
                    patches.add(p[0])
                elif not s[0] and p[1] != "1":
                    patches.add(p[0])
        for s in st:
            for p in s[2]:
                if s[0] and p[1] == "0":
                    patches.remove(p[0])
                elif not s[0] and p[1] == "1":
                    patches.remove(p[0])
        l = list()
        for i in range(len(trace)):
            l.append(trace[i+1])
        print(f"[state {state}] [trace {l}] [patches {patches}]")
        return patches
    
    def generate_table(self, cluster: Dict[int, list], result: List[Tuple[int, int, int, List[int]]]) -> str:
        with open(os.path.join(self.dir, "table.sbsv"), "w") as f:
            all_patches = set()
            for res in result:
                crash_id, base, test, patches = res
                for patch in patches:
                    all_patches.add(patch)
                f.write(f"[sym-in] [id {crash_id}] [base {base}] [test {test}] [cnt {len(patches)}] [patches {patches}]\n")
            # Current result: assume all are feasible
            current_result = all_patches.copy()
            for res in result:
                crash_id, base, test, patches = res
                res_patches = set(patches)
                for patch in all_patches:
                    if patch not in res_patches and patch in current_result:
                        current_result.remove(patch)
            current_result_list = sorted(list(current_result))
            f.write(f"[sym-out] [default] [cnt {len(current_result_list)}] [patches {current_result_list}]\n")
            # Best result: use symbolic inputs those do not filter out correct patches
            if self.bug_info is not None:
                correct_patch = self.bug_info["correct"]["no"]
                best_result = all_patches.copy()
                correct_input_num = 0
                for res in result:
                    crash_id, base, test, patches = res
                    res_patches = set(patches)
                    if correct_patch not in res_patches:
                        continue
                    correct_input_num += 1
                    for patch in all_patches:
                        if patch not in res_patches and patch in best_result:
                            best_result.remove(patch)
                best_result_list = sorted(list(best_result))
                f.write(f"[sym-out] [best] [cnt {len(best_result_list)}] [patches {best_result_list}]\n")
                meta_data_info = uni_klee.global_config.get_meta_data_info_by_id(self.bug_info["id"])
                f.write(f"[meta-data] [correct {correct_patch}] [all-patches {len(meta_data_info['meta_program']['patches'])}] [sym-input {len(result)}] [correct-input {correct_input_num}]")

        with open(os.path.join(self.dir, "table.md"), "w") as md:
            md.write("# Symvass Result\n")
            md.write(f"| crashId | base | test | patches |\n")
            md.write("| ------- | ---- | ---- | ------- |\n")
            for res in result:
                crash_id, base, test, patches = res
                md.write(f"| {crash_id} | {base} | {test} | {patches} |\n")
            md.write("\n")
    
    def analyze_v2(self):
        dp = SymvassDataLogSbsvParser(self.dir)
        analyzer = DataAnalyzer(dp)
        analyzer.analyze()
        cluster = self.cluster(analyzer)
        # symbolic_trace = self.symbolic_trace(analyzer)
        # buggy_trace = self.buggy_trace(analyzer)
        result = list()
        for crash_state in cluster:
            base_meta = analyzer.meta_data[crash_state]
            crash_id = base_meta["crashId"]
            base = base_meta["patches"]
            base_reg = base_meta["reg"]
            is_crash = base_meta["isCrash"]
            if not is_crash:
                result.append((crash_id, base_meta["state"], base_meta["state"], base))
            for crash_test in cluster[crash_state]:
                if crash_test not in analyzer.meta_data:
                    continue
                crash_meta = analyzer.meta_data[crash_test]
                crash = crash_meta["patches"]
                crash_reg = crash_meta["reg"]
                crashed = crash_meta["actuallyCrashed"]
                # If input is feasible:
                # crash -> not crash
                # not crash -> not crash + preserve behavior
                if is_crash:
                    if not crashed:
                        result.append((crash_id, base_meta["state"], crash_meta["state"], crash))
                else:
                    if not crashed and base_reg == crash_reg:
                        result.append((crash_id, base_meta["state"], crash_meta["state"], crash))
        self.generate_table(cluster, result)

def arg_parser(argv: List[str]) -> Config:
    # Remaining: c, e, h, i, j, n, q, t, u, v, w, x, y
    parser = argparse.ArgumentParser(description="Test script for uni-klee")
    parser.add_argument("cmd", help="Command to execute", choices=["run", "rerun", "snapshot", "clean", "kill", "filter", "analyze"])
    parser.add_argument("query", help="Query for bugid and patch ids: <bugid>[:<patchid>] # ex) 5321:1,2,3,r5-10")
    parser.add_argument("-a", "--additional", help="Additional arguments", default="")
    parser.add_argument("-d", "--debug", help="Debug mode", action="store_true")
    parser.add_argument("-o", "--outdir", help="Output directory", default="")
    parser.add_argument("-p", "--outdir-prefix", help="Output directory prefix(\"out\" for out dir)", default="uni-m-out")
    parser.add_argument("-b", "--snapshot-base-patch", help="Patches for snapshot", default="buggy")
    parser.add_argument("-s", "--snapshot-prefix", help="Snapshot directory prefix", default="snapshot")
    parser.add_argument("-f", "--filter-prefix", help="Filter directory prefix", default="filter")
    parser.add_argument("-l", "--sym-level", help="Symbolization level", default="medium")
    parser.add_argument("-m", "--max-fork", help="Max fork", default="64,64,64")
    parser.add_argument("-k", "--lock", help="Handle lock behavior", default="i", choices=["i", "w", "f"])
    parser.add_argument("-r", "--rerun", help="Rerun last command with same option", action="store_true")
    parser.add_argument("-z", "--analyze", help="Analyze symvass data", action="store_true")
    parser.add_argument("-g", "--use-last", help="Use last output directory", action="store_true")
    args = parser.parse_args(argv[1:])
    conf = Config(args.cmd, args.query, args.debug, args.sym_level, args.max_fork)
    if args.analyze:
        conf.conf_files.out_dir = args.query
        return conf
    conf.init(args.snapshot_base_patch, args.rerun, args.additional, args.lock)
    conf.conf_files.set_out_dir(args.outdir, args.outdir_prefix, conf.bug_info, args.snapshot_prefix, args.filter_prefix, args.use_last)
    return conf


class Runner(uni_klee.Runner):
    config: Config

    def __init__(self, conf: Config):
        self.config = conf

    def execute(self, cmd: str, dir: str, log_prefix: str, env: dict = None):
        cmd = cmd.replace('-S$(printf \'\\t\\t\\t\')', '-S$(printf "\\t\\t\\t")')
        if "/tmp/out.tiff" in cmd and os.path.exists("/tmp/out.tiff"):
            os.remove("/tmp/out.tiff")
        print(f"Change directory to {dir}")
        print(f"Executing: {cmd}")
        if env is None:
            env = os.environ
        proc = subprocess.run(cmd, shell=True, cwd=dir, env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if self.config.debug or proc.returncode != 0:
            log_file = os.path.join(
                self.config.conf_files.get_log_dir(), f"{log_prefix}.log"
            )
            if proc.returncode != 0:
                print("!!!!! Error !!!!")
                print("Save error log to " + log_file)
            try:
                print(proc.stderr.decode("utf-8", errors="ignore"))
                os.makedirs(self.config.conf_files.get_log_dir(), exist_ok=True)
                with open(log_file, "w") as f:
                    f.write(proc.stderr.decode("utf-8", errors="ignore"))
                    f.write("\n###############\n")
                    f.write(proc.stdout.decode("utf-8", errors="ignore"))
                print(
                    f"Save error log to {self.config.conf_files.get_log_dir()}/{log_prefix}.log"
                )
            except:
                pass
        return proc.returncode

    def execute_snapshot(self, cmd: str, dir: str, env: dict = None):
        if self.config.cmd in ["rerun", "snapshot"]:
            self.execute("rm -rf " + self.config.conf_files.snapshot_dir, dir, "rm")
        if self.config.cmd == "filter":
            self.execute("rm -rf " + self.config.conf_files.filter_dir, dir, "rm")
            self.execute(cmd, dir, "filter", env)
            return
        if not os.path.exists(self.config.conf_files.snapshot_file):
            if self.config.debug:
                print(
                    f"snapshot file {self.config.conf_files.snapshot_file} does not exist"
                )
            self.execute(cmd, dir, "snapshot", env)
    
    def print_list(self, l: List[Tuple[str, int]]):
        for item, index in l:
            print(f"{index}) {item}")

    def interactive_select(self, l: List[Tuple[str, int]], msg: str) -> Tuple[str, int]:
        print("Select from here: ")
        self.print_list(l)
        default = l[-1][1]
        while True:
            tmp = input(f"Select {msg}(default: {default}): ").strip()
            res = default
            if tmp == "q":
                return ("", -1)
            if tmp != "":
                res = int(tmp)
            for item, index in l:
                if res == index:
                    return (item, index)

    def get_dir(self):
        print("Set_dir")
        dir = self.config.conf_files.out_dir
        # self.filter_dir = self.config.conf_files.filter_dir
        if not os.path.exists(dir):
            print(f"{dir} does not exist")
            out_dirs = self.config.conf_files.find_all_nums(
                self.config.conf_files.out_base_dir,
                self.config.conf_files.out_dir_prefix,
            )
            out_dir = self.interactive_select(out_dirs, "dir")[0]
            if out_dir == "":
                print("Exit")
                return
            dir = os.path.join(self.config.conf_files.out_base_dir, out_dir)
        print(f"Using {dir}")
        return dir

    def run(self):
        if self.config.cmd == "analyze":
            analyzer = SymvassAnalyzer(self.get_dir(), self.config.bug_info)
            analyzer.analyze_v2()
            return
        if self.config.cmd in ["clean", "kill"]:
            # 1. Find all processes
            processes = uni_klee.global_config.get_current_processes()
            for proc in processes:
                if proc == self.config.bug_info["id"]:
                    with open(uni_klee.global_config.get_lock_file(self.config.bug_info["bug_id"]),"r") as f:
                        lines = f.readlines()
                        if len(lines) > 1:
                            print(f"Kill process {lines[0]}")
                            try:
                                os.kill(int(lines[0]), signal.SIGTERM)
                            except OSError as e:
                                print(e.errno)
                    # 2. Remove lock file
                    os.remove(
                        uni_klee.global_config.get_lock_file(
                            self.config.bug_info["bug_id"]
                        )
                    )
            # 3. Remove output directory
            if self.config.cmd == "clean":
                out_dirs = self.config.conf_files.find_all_nums(
                    self.config.conf_files.out_base_dir,
                    self.config.conf_files.out_dir_prefix,
                )
                for out_dir in out_dirs:
                    print(f"Remove {out_dir[0]}")
                    os.system(
                        f"rm -rf {os.path.join(self.config.conf_files.out_base_dir, out_dir[0])}"
                    )
            return
        lock_file = uni_klee.global_config.get_lock_file(self.config.bug_info["bug_id"])
        lock = uni_klee.acquire_lock(lock_file, self.config.lock, self.config.conf_files.out_dir)
        try:
            if lock < 0:
                print(f"Cannot acquire lock {lock_file}")
                return
            cmd = self.config.get_cmd_opts(True)
            self.execute_snapshot(cmd, self.config.workdir)
            if self.config.cmd not in ["snapshot", "filter"]:
                cmd = self.config.get_cmd_opts(False)
                self.execute(cmd, self.config.workdir, "uni-klee")
                analyzer = SymvassAnalyzer(self.get_dir(), self.config.bug_info)
                analyzer.analyze_v2()
        except Exception as e:
            print(f"Exception: {e}")
            print(traceback.format_exc())
        finally:
            uni_klee.release_lock(lock_file, lock)


def main(argv: list) -> int:
    os.chdir(uni_klee.ROOT_DIR)
    cmd = argv[1]
    if cmd != "trace":
        conf = arg_parser(argv)
        runner = Runner(conf)
        runner.run()
    elif cmd == "trace":
        get_trace(argv[2], int(argv[3]))


if __name__ == "__main__":
    main(sys.argv)
