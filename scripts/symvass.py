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


class Config(uni_klee.Config):

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
        parser.add_schema("[fork-map] [fork-parent] [state: int] [type: str]")
        parser.add_schema("[fork-map] [merge] [state$base: int] [state$crash_test: int] [patch: int]")
        parser.add_schema("[fork-loc] [br] [state$from: int] [loc$from: str] -> [state$to_a: int] [loc$to_a: str] [state$to_b: int] [loc$to_b: str]")
        parser.add_schema("[fork-loc] [sw] [state$from: int] [loc$from: str] -> [state$to_a: int] [loc$to_a: str] [state$to_b: int] [loc$to_b: str]")
        parser.add_schema("[fork-loc] [lazy] [state$from: int] [state$to: int] [loc: str] [name: str]")
        parser.add_schema("[patch-base] [trace] [state: int] [res: bool]")
        parser.add_schema("[patch-base] [fork] [state$true: int] [state$false: int]")
        parser.add_schema("[regression-trace] [state: int] [n: int] [res: bool] [loc: str]")
        parser.add_schema("[patch] [trace] [state: int] [res: bool] [patches: str]")
        parser.add_schema("[patch] [fork] [state$true: int] [state$false: int] [patches: str]")
        parser.add_schema("[regression] [state: int] [reg: str]")
        parser.add_schema("[lazy-trace] [state: int] [reg: str]")
        parser.add_schema("[stack-trace] [state: int] [reg: str]")
    
    def get_data(self) -> Dict[str, List[dict]]:
        return self.data

class SymvassDataLogParser():
    dir: str
    data: Dict[str, List[dict]]
    index: int
    def __init__(self, dir: str):
        self.dir = dir
        self.data = dict()
        self.index = 0
    
    def parse_dict(self, line: str) -> dict:
        result = dict()
        for item in line.split(","):
            item = item.strip()
            if len(item) == 0:
                continue
            key, value = item.split(":", 1)
            result[key.strip()] = value.strip()
        return result
    
    def parse_state_id(self, state: str, prefix: str = "state") -> int:
        if state.startswith(prefix):
            state = state[len(prefix) :]
        return int(state.strip())
    
    def tokenize(self, line: str) -> List[str]:
        # input: "[fork-map] [fork] [state 1] 1 [base 0] 1 [state 5] 1 [fork-count 5/-1]"
        # output: ["fork-map", "fork", "state 1", "1", "base 0", "1", "state 5", "1", "fork-count 5/-1"]
        result = list()
        level = 0
        current = ""
        for char in line:
            if char == '[':
                level += 1
                if level == 1:
                    if len(current.strip()) > 0:
                        result.append(current.strip())
                    current = ""
                    continue
            elif char == ']':
                level -= 1
                if level == 0:
                    result.append(current.strip())
                    current = ""
                    continue
            current += char
        if len(current.strip()) > 0:
            result.append(current.strip())
        return result
    
    def add_fork(self, line: str):
        tokens = self.tokenize(line)
        if len(tokens) < 4:
            print(f"Unknown fork: {line}")
            return
        source_state = self.parse_state_id(tokens[1])
        target_state = self.parse_state_id(tokens[3])
        self.data["fork"].append({
            "source": source_state,
            "target": target_state,
            "type": "fork",
            "loc": dict()
        })
    
    def add_fork_loc(self, line: str):
        tokens = self.tokenize(line)
        if len(tokens) < 3:
            print(f"Unknown fork-loc: {line}")
            return
        opt = tokens[1]
        if opt == "br":
            # [fork-loc] [br] [state 2] [/root/projects/CPR/patches/extractfix/libtiff/CVE-2016-5314/src/libtiff/tif_pixarlog.c:799:5:28680] -> [state 2] [/root/projects/CPR/patches/extractfix/libtiff/CVE-2016-5314/src/libtiff/tif_pixarlog.c:799:43:28683] [state 23] [/root/projects/CPR/patches/extractfix/libtiff/CVE-2016-5314/src/libtiff/tif_pixarlog.c:800:2:28687]
            if len(tokens) < 9:
                print(f"Unknown fork-loc: {line}")
                return
            state_from = self.parse_state_id(tokens[2])
            loc_from = tokens[3]
            state_to_a = self.parse_state_id(tokens[5])
            loc_to_a = tokens[6]
            state_to_b = self.parse_state_id(tokens[7])
            loc_to_b = tokens[8]
            self.data["fork-loc"]["br"].append({
                "state": state_from,
                "loc": loc_from,
                "to": [
                    { "state": state_to_a, "loc": loc_to_a },
                    { "state": state_to_b, "loc": loc_to_b },
                ]
            })

        elif opt == "lazy":
            if len(tokens) < 7:
                print(f"Unknown fork-loc: {line}")
                return
            state_from = self.parse_state_id(tokens[2])
            state_to = self.parse_state_id(tokens[4])
            loc = tokens[5]
            name = tokens[6]
            self.data["fork-loc"]["lazy"].append({
                "state": state_from,
                "to": state_to,
                "loc": loc,
                "name": name
            })

        elif opt == "sw":
            if len(tokens) < 9:
                print(f"Unknown fork-loc: {line}")
                return
            state_from = self.parse_state_id(tokens[2])
            loc_from = tokens[3]
            state_to_a = self.parse_state_id(tokens[5])
            loc_to_a = tokens[6]
            state_to_b = self.parse_state_id(tokens[7])
            loc_to_b = tokens[8]
            self.data["fork-loc"]["sw"].append({
                "state": state_from,
                "loc": loc_from,
                "to": [
                    { "state": state_to_a, "loc": loc_to_a },
                    { "state": state_to_b, "loc": loc_to_b },
                ]
            })
    
    def add_fork_map(self, line: str):
        tokens = self.tokenize(line)
        if len(tokens) < 4:
            print(f"Unknown fork-map: {line}")
            return
        if tokens[1] == "fork":
            state = self.parse_state_id(tokens[2])
            state_type = tokens[3]
            base_state = self.parse_state_id(tokens[4], "base")
            base_state_type = tokens[5]
            forked_state = self.parse_state_id(tokens[6])
            forked_state_type = tokens[7]
            self.data["fork-map"]["fork"].append({
                "state": state,
                "state_type": state_type,
                "base": base_state,
                "base_type": base_state_type,
                "forked": forked_state,
                "forked_type": forked_state_type,
            })
        elif tokens[1] == "merge":
            source_state = self.parse_state_id(tokens[2])
            target_state = self.parse_state_id(tokens[4])
            patch_id = self.parse_state_id(tokens[5], "patch")
            self.data["fork-map"]["merge"].append({
                "source": source_state,
                "target": target_state,
                "patch": patch_id
            })
        elif tokens[1] == "fork-parent":
            state = self.parse_state_id(tokens[2])
            state_type = tokens[3]
            self.data["fork-map"]["fork-parent"].append({
                "state": state,
                "type": state_type
            })
    
    def add_meta_data(self, line: str):
        tokens = self.tokenize(line)
        if len(tokens) < 3:
            print(f"Unknown meta-data: {line}")
            return
        state = self.parse_state_id(tokens[1])
        data = self.parse_dict(tokens[2])
        crash_id = int(data["crashId"])
        patch_id = int(data["patchId"])
        state_type = data["stateType"]
        is_crash = data["isCrash"] == "true"
        actually_crashed = data["actuallyCrashed"] == "true"
        exit_loc = data["exitLoc"]
        exit_code = data["exit"]
        self.data["meta-data"].append({
            "state": state,
            "crash_id": crash_id,
            "patch_id": patch_id,
            "state_type": state_type,
            "is_crash": is_crash,
            "actually_crashed": actually_crashed,
            "exit_loc": exit_loc,
            "exit_code": exit_code
        })
    
    def add_regression(self, line: str):
        tokens = self.tokenize(line)
        if len(tokens) < 3:
            print(f"Unknown regression: {line}")
            return
        state = self.parse_state_id(tokens[1])
        reg = tokens[2]
        self.data["regression"].append({
            "state": state,
            "trace": reg
        })
    
    def add_lazy_trace(self, line: str):
        tokens = self.tokenize(line)
        if len(tokens) < 3:
            print(f"Unknown lazy-trace: {line}")
            return
        state = self.parse_state_id(tokens[1])
        trace = tokens[2]
        self.data["lazy-trace"].append({
            "state": state,
            "trace": trace
        })
    
    def add_stack_trace(self, line: str):
        tokens = self.tokenize(line)
        if len(tokens) < 3:
            print(f"Unknown stack-trace: {line}")
            return
        state = self.parse_state_id(tokens[1])
        trace = tokens[2]
        self.data["stack-trace"].append({
            "state": state,
            "trace": trace
        })
        
    def add_patch(self, line: str):
        tokens = self.tokenize(line)
        opt = tokens[1]
        if opt == "fork":
            state_true = self.parse_state_id(tokens[2])
            state_false = self.parse_state_id(tokens[3])
            patches = tokens[4]
            patches_data = patches[patches.find('[')+1 : patches.find(']')]
            elements = patches_data.split(",")
            patches_map = dict()
            for element in elements:
                tokens = element.strip().split(":")
                patch_id = int(tokens[0])
                patch_state = int(tokens[1])
                patches_map[patch_id] = patch_state
            self.data["patch"]["fork"].append({
                "state_true": state_true,
                "state_false": state_false,
                "patches": patches_map
            })
        elif opt == "trace":
            state = self.parse_state_id(tokens[2])
            trace = tokens[3]
            patches = tokens[4]
            patches_data = patches[patches.find('[')+1 : patches.find(']')]
            elements = patches_data.split(",")
            patches_map = dict()
            for element in elements:
                tokens = element.strip().split(":")
                patch_id = int(tokens[0])
                patch_state = int(tokens[1])
                patches_map[patch_id] = patch_state
            self.data["patch"]["trace"].append({
                "state": state,
                "trace": trace,
                "patches": patches_map
            })
        
    def parse_line(self, line: str):
        if line.startswith("[fork]"):
            self.add_fork(line)
        elif line.startswith("[fork-map]"):
            self.add_fork_map(line)
        elif line.startswith("[fork-loc]"):
            self.add_fork_loc(line)
        elif line.startswith("[meta-data]"):
            self.add_meta_data(line)
        elif line.startswith("[regression]"):
            self.add_regression(line)
        elif line.startswith("[regression-trace]"):
            pass
        elif line.startswith("[lazy-trace]"):
            self.add_lazy_trace(line)
        elif line.startswith("[stack-trace]"):
            self.add_stack_trace(line)
        elif line.startswith("[patch]"):
            self.add_patch(line)
        elif line.startswith("[stats]"):
            pass
        
    def read_data_log(self, name: str = "data.log"):
        self.data["fork"] = list()
        self.data["fork-map"] = { "fork": [], "merge": [], "fork-parent": [] }
        self.data["fork-loc"] = { "br": [], "lazy": [], "sw": [] }
        self.data["meta-data"] = list()
        self.data["regression"] = list()
        self.data["regression-trace"] = list()
        self.data["lazy-trace"] = list()
        self.data["stack-trace"] = list()
        self.data["patch"] = { "fork": [], "trace": [] }
        with open(os.path.join(self.dir, name), "r") as f:
            lines = f.readlines()
            for line in lines:
                line = line.strip()
                if len(line) == 0 or line.startswith("#"):
                    continue
                self.parse_line(line)


class DataAnalyzer():
    dp: SymvassDataLogSbsvParser
    data: Dict[str, List[dict]]
    meta_dat: Dict[int, dict]
    graph: nx.DiGraph
    
    def __init__(self, dp: SymvassDataLogSbsvParser):
        self.dp = dp
        self.data = self.dp.get_data()
        self.meta_data = dict()
        self.graph = nx.DiGraph()
        
    def analyze(self):
        self.construct_graph()
    
    def dump_graph(self):
        dot = graphviz.Digraph()
        for node in self.graph.nodes():
            state = node[0]
            nt = node[1]
            fillcolor = "white"
            color = "black"
            if state in self.meta_data:
                meta = self.meta_data[state]
                if meta["isCrash"]:
                    color = "red"
                else:
                    color = "green"
                if meta["stateType"] == "1":
                    fillcolor = "red"
                if meta["stateType"] == "2":
                    fillcolor = "blue"
                elif meta["stateType"] == "4":
                    fillcolor = "yellow"
            dot.node(f"{state},{nt}", style="filled", color=color, fillcolor=fillcolor)
        
        for edge in self.graph.edges():
            src = edge[0]
            dst = edge[1]
            src_state = src[0]
            src_type = src[1]
            dst_state = dst[0]
            dst_type = dst[1]
            style = "solid"
            color = "black"
            label = ""
            
            dot.edge(f"{src[0]},{src[1]}", f"{dst[0]},{dst[1]}")
            
    
    def construct_graph(self):
        meta_data = self.meta_data
        graph = self.graph
        for meta in self.data["meta-data"]:
            state = meta["state"]
            state_type = meta["stateType"]
            graph.add_node((state, state_type))
            meta_data[state] = meta
            if state_type == "2":
                graph.add_node((state, "1"))
                graph.add_edge((state, "1"), (state, "2"))
        for fp in self.data["fork-map"]["fork-parent"]:
            state = fp["state"]
            state_type = fp["type"]
            graph.add_node((state, state_type), type="fork-parent")
        # Add edges
        for fork in self.data["fork-map"]["fork"]:
            fork_from = fork["state$from"]
            type_from = fork["type$from"]
            fork_to = fork["state$to"]
            type_to = fork["type$to"]
            graph.add_edge((fork_from, type_from), (fork_to, type_to), type="fork")
        for merge in self.data["fork-map"]["merge"]:
            base = merge["state$base"]
            crash_test = merge["state$crash_test"]
            patch = merge["patch"]
            graph.add_edge((base, "2"), (crash_test, "4"), type="merge", patch=patch)
        
        

class PatchSorter:
    # dir: str
    # patch_ids: list
    # meta_data: dict
    dp: SymvassDataLogParser
    dp_filter: SymvassDataLogParser
    cluster: Dict[int, Dict[int, List[Tuple[int, bool]]]]
    patch_ids: List[int]
    patch_filter: Set[int]

    def __init__(
        self, dp: SymvassDataLogParser, dp_filter: SymvassDataLogParser = None
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
    config: Config
    dir: str
    filter_dir: str

    def __init__(self, conf: Config):
        self.config = conf

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

    def set_dir(self):
        print("Set_dir")
        self.dir = self.config.conf_files.out_dir
        self.filter_dir = self.config.conf_files.filter_dir
        if not os.path.exists(self.dir):
            print(f"{self.dir} does not exist")
            out_dirs = self.config.conf_files.find_all_nums(
                self.config.conf_files.out_base_dir,
                self.config.conf_files.out_dir_prefix,
            )
            out_dir = self.interactive_select(out_dirs, "dir")[0]
            if out_dir == "":
                print("Exit")
                return
            self.dir = os.path.join(self.config.conf_files.out_base_dir, out_dir)
        print(f"Using {self.dir}")

    def save_sorted_patches(
        self,
        dp: SymvassDataLogParser,
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

    def analyze(self):
        run_filter = not os.path.exists(self.filter_dir)
        if run_filter:
            print(f"{self.filter_dir} does not exist")
            proc = mp.Process(
                target=uni_klee.main,
                args=(["uni-klee.py", "filter", self.config.query,
                        f"-f={self.config.conf_files.filter_prefix}",
                        "--lock=w",], ))
            proc.start()
            proc.join()
        dp = SymvassDataLogParser(self.dir)
        dp.read_data_log()
        dp_filter = SymvassDataLogParser(self.filter_dir)
        dp_filter.read_data_log()
        ps = PatchSorter(dp, dp_filter)
        cluster = ps.analyze_cluster()
        print("Cluster analyzed", cluster)
        sorted_patches = ps.sort()
        print("Sorted patches", sorted_patches)
        self.save_sorted_patches(dp, sorted_patches, cluster)
    
    def analyze_v2(self):
        dp = SymvassDataLogSbsvParser(self.dir)
        analyzer = DataAnalyzer(dp)
        analyzer.analyze()


def arg_parser(argv: List[str]) -> Config:
    # Remaining: c, e, g, h, i, j, n, q, t, u, v, w, x, y, z
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
    parser.add_argument("-m", "--max-fork", help="Max fork", default="64,64,4")
    parser.add_argument("-k", "--lock", help="Handle lock behavior", default="i", choices=["i", "w", "f"])
    parser.add_argument("-r", "--rerun", help="Rerun last command with same option", action="store_true")
    args = parser.parse_args(argv[1:])
    conf = Config(args.cmd, args.query, args.debug, args.sym_level, args.max_fork)
    conf.init(args.snapshot_base_patch, args.rerun, args.additional, args.lock)
    conf.conf_files.set_out_dir(args.outdir, args.outdir_prefix, conf.bug_info, args.snapshot_prefix, args.filter_prefix)
    return conf


class Runner(uni_klee.Runner):
    config: Config

    def __init__(self, conf: Config):
        self.config = conf

    def execute(self, cmd: str, dir: str, log_prefix: str, env: dict = None):
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

    def run(self):
        if self.config.cmd == "analyze":
            analyzer = SymvassAnalyzer(self.config)
            analyzer.set_dir()
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
        lock = uni_klee.acquire_lock(
            lock_file, self.config.lock, self.config.conf_files.out_dir
        )
        try:
            if lock < 0:
                print(f"Cannot acquire lock {lock_file}")
                return
            cmd = self.config.get_cmd_opts(True)
            self.execute_snapshot(cmd, self.config.workdir)
            if self.config.cmd not in ["snapshot", "filter"]:
                cmd = self.config.get_cmd_opts(False)
                self.execute(cmd, self.config.workdir, "uni-klee")
                analyzer = SymvassAnalyzer(self.config)
                analyzer.set_dir()
                analyzer.analyze()
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
