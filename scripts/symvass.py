#!/usr/bin/env python3
import os
import sys
import argparse
from typing import List, Tuple, Set, Dict
import multiprocessing as mp

import uni_klee


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
        header = lines[0].strip().replace('(', '').replace(')', '').split(",")
        with open(f"{dir}/run.stats.sbsv", "w") as f:
            for i in range(1, len(lines)):
                line = lines[i].strip().replace('(', '').replace(')', '')
                tokens = line.split(",")
                f.write(f"[stats] [inst {tokens[0]}] [fb {tokens[1]}] [pb {tokens[2]}] [nb {tokens[3]}] [ut {tokens[4]}] [ns {tokens[5]}] [mu {tokens[6]}] [nq {tokens[7]}] [nqc {tokens[8]}] [no {tokens[9]}] [wt {tokens[10]}] [ci {tokens[11]}] [ui {tokens[12]}] [qt {tokens[13]}] [st {tokens[14]}] [cct {tokens[15]}] [ft {tokens[16]}] [rt {tokens[17]}] [qccm {tokens[18]}] [ccch {tokens[19]}]\n")
        

class PatchSorter:
    # dir: str
    # patch_ids: list
    # meta_data: dict
    dp: uni_klee.DataLogParser
    dp_filter: uni_klee.DataLogParser
    cluster: Dict[int, Dict[int, List[Tuple[int, bool]]]]
    patch_ids: List[int]
    patch_filter: Set[int]
    
    def __init__(self, dp: uni_klee.DataLogParser, dp_filter: uni_klee.DataLogParser = None):
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
            if data["stateType"] == '3':
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
                if data["stateType"] == '2':
                    base = data
                    break
            for data in data_list:
                if data["stateType"] == '2':
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


class Analyzer:
    config: uni_klee.Config
    dir: str
    filter_dir: str
    
    def __init__(self, conf: uni_klee.Config):
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
        self.dir = self.config.conf_files.out_dir
        self.filter_dir = self.config.conf_files.filter_dir
        if not os.path.exists(self.dir):
            print(f"{self.dir} does not exist")
            out_dirs = self.config.conf_files.find_all_nums(self.config.conf_files.out_base_dir, self.config.conf_files.out_dir_prefix)
            out_dir = self.interactive_select(out_dirs, "dir")[0]
            if out_dir == "":
                print("Exit")
                return
            self.dir = os.path.join(self.config.conf_files.out_base_dir, out_dir)
        print(f"Using {self.dir}")
        
    def save_sorted_patches(self, dp: uni_klee.DataLogParser, sorted_patches: List[Tuple[int, float]], cluster: Dict[int, Dict[int, List[Tuple[int, bool]]]]):
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
            for (patch, score) in sorted_patches:
                if patch in patch_filter:
                    f.write(f"| {rank} | {patch} | {score:.2f} |\n")
                    rank += 1
            f.write(f"\n## Removed patch list\n")
            f.write(f"| Rank | Patch | Score |\n")
            f.write(f"|------|-------|-------|\n")
            rank = 1
            for (patch, score) in sorted_patches:
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
                            f.write(f"| {patch}-{local_input_id} | {patch} | {state} | {'O' if patch_result[1] else 'X'} | - |\n")
                        else:
                            actually_crashed = dp.meta_data[state]["actuallyCrashed"]
                            f.write(f"| {patch}-{local_input_id} | {patch} | {state} | {'O' if patch_result[1] else 'X'} | {actually_crashed} |\n")
                        local_input_id += 1
        print(f"Saved to {os.path.join(self.dir, 'patch-rank.md')}")

    def analyze(self):
        run_filter = not os.path.exists(self.filter_dir)
        if run_filter:
            print(f"{self.filter_dir} does not exist")
            proc = mp.Process(target=uni_klee.main, args=(["uni-klee.py", "filter", self.config.query, f"-f={self.config.conf_files.filter_prefix}", "--lock=w"],))
            proc.start()
            proc.join()
        dp = uni_klee.DataLogParser(self.dir)
        dp.read_data_log()
        dp_filter = uni_klee.DataLogParser(self.filter_dir)
        dp_filter.read_data_log()
        ps = PatchSorter(dp, dp_filter)
        cluster = ps.analyze_cluster()
        print("Cluster analyzed", cluster)
        sorted_patches = ps.sort()
        print("Sorted patches", sorted_patches)
        self.save_sorted_patches(dp, sorted_patches, cluster)
        
def arg_parser(argv: List[str]) -> uni_klee.Config:
    parser = argparse.ArgumentParser(description="SymVass")
    parser.add_argument("cmd", type=str, choices=["analyze"], help="Command")
    parser.add_argument("query", type=str, help="Query for bugid and patch ids: <bugid>[:<patchid>] # ex) 5321:1,2,3")
    parser.add_argument("-a", "--additional", help="Additional options for the command", default="")
    parser.add_argument("-p", "--outdir-prefix", help="Output directory prefix(\"out\" for out dir)", default="uni-m-out")
    parser.add_argument("-o", "--output", help="Output directory", default="")
    parser.add_argument("-f", "--filter-prefix", help="Filter prefix for output directory", default="filter")
    args = parser.parse_args(argv[1:])
    conf = uni_klee.Config(args.cmd, args.query, False, "medium", "64,64,0")
    conf.init("buggy", False, args.additional, "w")
    conf.conf_files.set_out_dir("", args.outdir_prefix, conf.bug_info, "snapshot", args.filter_prefix)
    return conf

def main(argv: list) -> int:
    os.chdir(uni_klee.ROOT_DIR)
    cmd = argv[1]
    if cmd == "analyze":
        conf = arg_parser(argv)
        analyzer = Analyzer(conf)
        analyzer.set_dir()
        analyzer.analyze()
    elif cmd == "trace":
        get_trace(argv[2], int(argv[3]))
    
if __name__ == "__main__":
    main(sys.argv)