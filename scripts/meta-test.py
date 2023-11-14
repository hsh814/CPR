#! /usr/bin/env python3
import os
import time
import sys
from typing import List, Set, Dict, Tuple
import json
import csv
import subprocess
import argparse
import patch
import glob
import re
import signal
import random

import graphviz

import networkx as nx
import difflib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

FILE_LOC = os.path.abspath(__file__) if not os.path.islink(os.path.abspath(__file__)) else os.path.abspath(os.readlink(os.path.abspath(__file__)))

ROOT_DIR = os.path.dirname(os.path.dirname(FILE_LOC)) 

class GloablConfig:
  root_dir: str
  patch_dir: str
  meta_data_file: str
  meta_data: dict
  def __init__(self):
    self.root_dir = ROOT_DIR
    self.patch_dir = os.path.join(self.root_dir, "patches")
    self.meta_data_file = os.path.join(self.patch_dir, "meta-data.json")
    with open(self.meta_data_file, "r") as f:
      self.meta_data = json.load(f)
  def get_meta_data_list(self) -> List[dict]:
    return self.meta_data
  def get_meta_data_info(self, id: str) -> dict:
    for data in self.meta_data:
      if str(data["id"]) == id:
        conf_files = ConfigFiles()
        conf_files.set(data)
        result = dict()
        result['conf'] = conf_files.read_conf_file()
        result['meta-program'] = conf_files.read_meta_program()
        return result
    return dict()

global_config = GloablConfig()


class ConfigFiles:
  root_dir: str
  global_config: GloablConfig
  repair_conf: str
  meta_program: str
  patch_dir: str
  work_dir: str
  project_dir: str
  meta_patch_obj_file: str
  benchmark: str
  subject: str
  bid: str
  out_base_dir: str
  out_dir: str
  out_dir_prefix: str
  snapshot_dir: str
  snapshot_file: str
  snapshot_prefix: str
  
  def __init__(self):
    self.root_dir = ROOT_DIR
    self.global_config = global_config
  def set(self, bug_info: dict):
    patches_dir = os.path.join(self.root_dir, "patches")
    self.bid = bug_info["bug_id"]
    self.benchmark = bug_info["benchmark"]
    self.subject = bug_info["subject"]
    self.project_dir = os.path.join(patches_dir, self.benchmark, self.subject, self.bid)
    self.work_dir = os.path.join(self.project_dir, "patched")
    self.repair_conf = os.path.join(self.project_dir, "repair.conf")
    self.meta_program = os.path.join(self.project_dir, "meta-program.json")
    patch.compile(os.path.join(self.project_dir, "concrete"))
    self.meta_patch_obj_file = os.path.join(self.project_dir, "concrete", "libuni_klee_runtime.bca")
  def set_out_dir(self, out_dir: str, out_dir_prefix: str, bug_info: dict, snapshot_prefix: str):
    self.out_dir_prefix = out_dir_prefix
    self.snapshot_prefix = snapshot_prefix
    if out_dir == "":
      self.out_base_dir = self.work_dir
    elif out_dir == "out":
      self.out_base_dir = os.path.join(self.root_dir, "out", self.benchmark, self.subject, self.bid)
    else:
      self.out_base_dir = out_dir
    os.makedirs(self.out_base_dir, exist_ok=True)
    no = self.find_num(self.out_base_dir, out_dir_prefix)
    self.out_dir = os.path.join(self.out_base_dir, f"{out_dir_prefix}-{no}")
    self.snapshot_dir = os.path.join(self.out_base_dir, self.snapshot_prefix)
    if "snapshot" in bug_info:
      print(f"Use snapshot {bug_info['snapshot']}")
      self.snapshot_file = os.path.join(self.snapshot_dir, bug_info["snapshot"])
    else:
      print(f"Use snapshot {self.bid}-last.json ...")
      self.snapshot_file = os.path.join(self.snapshot_dir, "snapshot-last.json")
  def find_num(self, dir: str, name: str) -> int:
    result = 0
    dirs = os.listdir(dir)
    while True:
      if f"{name}-{result}" in dirs:
        result += 1
      else:
        break
    return result
  def sorting_key(self, item: str):
      # Extract the numeric value at the end of the string using regular expression
      match = re.search(r'(\d+)$', item)
      if match:
          # If a numeric value is found, return it for sorting
          return int(match.group(1))
      else:
          # If no numeric value is found, treat as -1
          return -1
  def find_all_nums(self, dir: str, name: str) -> List[Tuple[str, int]]:
    result = list()
    dirs = os.listdir(dir)
    index = 0
    print(name)
    print(dirs)
    dirs.sort(key=self.sorting_key)
    print(dirs)
    for d in dirs:
      if name not in d:
        continue
      index += 1
      result.append((d, index))
    return result

  def read_conf_file(self) -> dict:
    with open(self.repair_conf, "r") as f:
      lines = f.readlines()
    result = dict()
    for line in lines:
      line = line.strip()
      if len(line) == 0:
        continue
      if line.startswith("#"):
        continue
      key, value = line.split(":", 1)
      result[key] = value
    return result
  def read_meta_data(self) -> dict:
    return self.global_config.meta_data
  def read_meta_program(self) -> dict:
    with open(self.meta_program, "r") as f:
      return json.load(f)
  def get_log_dir(self) -> str:
    return os.path.join(self.root_dir, "logs", self.bid)
  def get_lock(self) -> str:
    return os.path.join(self.root_dir, "logs", ".meta-test", self.bid)


class Config:
  cmd: str
  patch_ids: List[str]
  snapshot_patch_ids: List[str]
  meta: dict
  bug_info: dict
  meta_program: dict
  query: str
  debug: bool
  outdir: str
  workdir: str
  project_conf: dict
  conf_files: ConfigFiles
  additional: str
  sym_level: str
  max_fork: str
  lock: str
  rerun: bool
  
  def get_bug_info(self, bugid: str) -> dict:
    def check_int(s: str) -> bool:
      try:
        int(s)
        return True
      except ValueError:
        return False
    num = -1
    if check_int(bugid):
      num = int(bugid)
    for data in self.meta:
      bid = data["bug_id"]
      benchmark = data["benchmark"]
      subject = data["subject"]
      id = data["id"]
      if num != -1:
        if num == id:
          return data
      if bugid.lower() in bid.lower():
        return data
    return None
  
  def get_patch_ids(self, patch_ids: list) -> List[str]:
    self.meta_program = self.conf_files.read_meta_program()
    result = list()
    for patch_id in patch_ids:
      for patch in self.meta_program["patches"]:
        if str(patch["id"]) == patch_id:
          result.append(str(patch["id"]))
        elif patch["name"] == patch_id:
          result.append(str(patch["id"]))
    return result
  
  def parse_query(self, snapshot_patch_ids: str) -> Tuple[dict, list]:
    parsed: List[str] = None
    if ":" in self.query:
      parsed = self.query.rsplit(":", 1)
    else:
      parsed = self.query.rsplit("/", 1)
    bugid = parsed[0]
    self.bug_info = self.get_bug_info(bugid)
    if self.bug_info is None:
      print(f"Cannot find patch for {self.query} - {bugid}")
      sys.exit(1)
    # Set config files
    self.conf_files.set(self.bug_info)
    # Set patch ids
    patchid = "buggy"
    if len(parsed) > 1:
      patchid = parsed[1]
    self.patch_ids = self.get_patch_ids(patchid.split(","))
    if snapshot_patch_ids != "":
      self.snapshot_patch_ids = self.get_patch_ids(snapshot_patch_ids.split(","))
    else:
      self.snapshot_patch_ids = list()
    print(f"query: {self.query} => bugid {self.bug_info}, patchid {self.patch_ids}")
    
  def init(self, snapshot_patch_ids: str, rerun: bool, additional: str, lock: str):
    self.meta = self.conf_files.read_meta_data()
    self.parse_query(snapshot_patch_ids)
    self.project_conf = self.conf_files.read_conf_file()
    self.workdir = self.conf_files.work_dir
    self.rerun = rerun
    self.additional = additional
    self.lock = lock
  
  def __init__(self, cmd: str, query: str, debug: bool, sym_level: str, max_fork: str):
    self.cmd = cmd
    self.query = query    
    self.debug = debug
    self.sym_level = sym_level
    self.max_fork = max_fork
    self.conf_files = ConfigFiles()
  
  def append_snapshot_cmd(self, cmd: List[str]):
    snapshot_dir = self.conf_files.snapshot_dir
    patch_str = ",".join(self.snapshot_patch_ids)
    cmd.append(f"--output-dir={snapshot_dir}")
    cmd.append(f"--patch-id={patch_str}")
  
  def append_cmd(self, cmd: List[str], patch_str: str, opts: List[str]):
    out_dir = self.conf_files.out_dir
    default_opts = ["--no-exit-on-error", "--simplify-sym-indices", 
                    f"--symbolize-level={self.sym_level}", f"--max-forks-per-phases={self.max_fork}", 
                     "--max-memory=0"]
    cmd.extend(default_opts)
    cmd.extend(opts)
    cmd.append(f"--output-dir={out_dir}")
    cmd.append(f"--patch-id={patch_str}")
    cmd.append(f"--snapshot={self.conf_files.snapshot_file}")
  
  def get_cmd_opts(self, is_snapshot: bool) -> str:
    target_function = self.bug_info["target"]
    link_opt = f"--link-llvm-lib={self.conf_files.meta_patch_obj_file}"
    result = ["uni-klee", "--libc=uclibc", "--posix-runtime", "--external-calls=all",
              "--allocate-determ", "--write-smt2s", "--write-kqueries", "--log-trace",
              f"--target-function={target_function}", link_opt]
    if "klee_flags" in self.project_conf:
      link_opt = self.project_conf['klee_flags']
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

class Runner:
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
      if proc.returncode != 0:
        print("!!!!! Error !!!!")
      try:
        print(proc.stderr.decode("utf-8", errors="ignore"))
        os.makedirs(self.config.conf_files.get_log_dir(), exist_ok=True)
        with open(os.path.join(self.config.conf_files.get_log_dir(), f"{log_prefix}.log"), "w") as f:
          f.write(proc.stderr.decode("utf-8", errors="ignore"))
          f.write("\n###############\n")
          f.write(proc.stdout.decode("utf-8", errors="ignore"))
        print(f"Save error log to {self.config.conf_files.get_log_dir()}/{log_prefix}.log")
      except:
        pass
    return proc.returncode
  def execute_snapshot(self, cmd: str, dir: str, env: dict = None):
    if self.config.cmd in ["rerun", "snapshot"]:
      self.execute("rm -rf " + self.config.conf_files.snapshot_dir, dir, "rm")
    if not os.path.exists(self.config.conf_files.snapshot_file):
      if self.config.debug:
        print(f"snapshot file {self.config.conf_files.snapshot_file} does not exist")
      self.execute(cmd, dir, "snapshot", env)
  def run(self):
    if self.config.cmd == "analyze":
      analyzer = Analyzer(self.config)
      analyzer.analyze()
      return
    cmd = self.config.get_cmd_opts(True)
    self.execute_snapshot(cmd, self.config.workdir)
    if self.config.cmd != "snapshot":
      cmd = self.config.get_cmd_opts(False)
      self.execute(cmd, self.config.workdir, "uni-klee")
      analyzer = Analyzer(self.config)
      analyzer.analyze()

class DataLogParser:
  dir: str
  fork_map: Dict[int, List[int]]
  fork_graph: Dict[int, List[int]]
  state_type_map: Dict[int, str]
  meta_data: Dict[int, Dict[str, int]]
  merge_edge_map: Dict[int, List[int]]
  merge_patch_map: Dict[int, int]
  def __init__(self, dir: str):
    self.dir = dir
    self.fork_map = dict()
    self.fork_graph = dict()
    self.state_type_map = dict()
    self.meta_data = dict()
    self.merge_edge_map = dict()
    self.merge_patch_map = dict()
  def parse_dict(self, line: str) -> dict:
    result = dict()
    for item in line.split(","):
      item = item.strip()
      if len(item) == 0:
        continue
      key, value = item.split(":", 1)
      result[key.strip()] = value.strip()
    return result
  def parser_level_1(self, line: str) -> list:
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
  def parse_state_id(self, state: str, prefix: str = "state") -> int:
    return int(state.removeprefix(prefix).strip())
  def add_meta_data(self, line: str):
    # metadata_pattern = r"\[meta-data\] \[state (\d+)\] \[crashId: (\d+), patchId: (\d+), stateType: (\w+), isCrash: (\w+), actuallyCrashed: (\d+), exitLoc: ([^,]+), exit: ([^\]]+)\]"
    tokens = self.parser_level_1(line)
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
    if state not in self.meta_data:
      self.meta_data[state] = dict()
    result = self.meta_data[state]
    result["state"] = state
    result["crashId"] = crash_id
    result["patchId"] = patch_id
    result["stateType"] = state_type
    result["isCrash"] = is_crash
    result["actuallyCrashed"] = actually_crashed
    result["exitLoc"] = exit_loc
    result["exit"] = exit_code    
  def add_fork(self, line: str):
    # fork_pattern = r"\[fork\] \[state (\d+)\] -> \[state (\d+)\]"
    tokens = self.parser_level_1(line)
    if len(tokens) < 4:
      print(f"Unknown fork: {line}")
      return
    source_state = self.parse_state_id(tokens[1])
    target_state = self.parse_state_id(tokens[3])
    if source_state not in self.fork_graph:
      self.fork_graph[source_state] = list()
    self.fork_graph[source_state].append(target_state)
  def add_fork_map(self, line: str):
    # [fork-map] [fork] [state 1] 1 [base 0] 1 [state 5] 1 [fork-count 5/-1]
    # fork_map_pattern = r"\[fork-map\] \[fork\] \[state (\d+)\] (\d+) \[base (\d+)\] (\d+) \[state (\d+)\] (\d+) \[fork-count .+\]"
    # fork_merge_pattern = r"\[fork-map\] \[merge\] \[state (\d+)\] -> \[state (\d+)\] \[patch (\d+)\]"
    tokens = self.parser_level_1(line)
    if len(tokens) < 3:
      print(f"Unknown fork-map: {line}")
      return
    if tokens[1] == "fork":
      if len(tokens) < 8:
        print(f"Unknown fork-map: {line}")
        return
      state = self.parse_state_id(tokens[2])
      base_state = self.parse_state_id(tokens[4], "base")
      forked_state = self.parse_state_id(tokens[6])
      if base_state not in self.fork_map:
        self.fork_map[base_state] = list()
      self.fork_map[base_state].append(forked_state)
      self.state_type_map[state] = tokens[3]
      self.state_type_map[forked_state] = tokens[7]
    elif tokens[1] == "merge":
      if len(tokens) < 6:
        print(f"Unknown fork-map: {line}")
        return
      source_state = self.parse_state_id(tokens[2])
      target_state = self.parse_state_id(tokens[4])
      patch_id = self.parse_state_id(tokens[5], "patch")
      if source_state not in self.merge_edge_map:
        self.merge_edge_map[source_state] = list()
      self.merge_edge_map[source_state].append(target_state)
      self.merge_patch_map[target_state] = patch_id
  def add_regression(self, line: str):
    # reg_pattern = r"\[regression\] \[state (\d+)\] \[([^\]]*)\]"
    tokens = self.parser_level_1(line)
    if len(tokens) < 3:
      print(f"Unknown regression: {line}")
      return
    state = self.parse_state_id(tokens[1])
    reg = tokens[2]
    self.meta_data[state]["regressionTrace"] = reg
  def add_lazy_trace(self, line: str):
    # reg_pattern = r"\[lazy-trace\] \[state (\d+)\] \[([^\]]*)\]"
    tokens = self.parser_level_1(line)
    if len(tokens) < 3:
      print(f"Unknown lazy-trace: {line}")
      return
    state = self.parse_state_id(tokens[1])
    reg = tokens[2]
    self.meta_data[state]["lazyTrace"] = reg
  def add_stack_trace(self, line: str):
    # st_pattern = r"\[stack-trace\] \[state (\d+)\] \[([^\]]*)\]"
    tokens = self.parser_level_1(line)
    if len(tokens) < 3:
      print(f"Unknown stack-trace: {line}")
      return
    state = self.parse_state_id(tokens[1])
    st = tokens[2]
    self.meta_data[state]["stackTrace"] = st
  def read_data_log(self, name: str) -> dict:
    with open(os.path.join(self.dir, name), "r") as f:
      for line in f.readlines():
        line = line.strip()
        if line.startswith("[fork]"):
          self.add_fork(line)
        elif line.startswith("[fork-map]"):
          self.add_fork_map(line)
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
        else:
          print(f"Unknown line: {line}")
  def cluster(self) -> Dict[int, list]:
    cluster_by_crash_id = dict()
    for state, data in self.meta_data.items():
      crash_id = data["crashId"]
      if crash_id not in cluster_by_crash_id:
        cluster_by_crash_id[crash_id] = list()
      cluster_by_crash_id[crash_id].append(data)
    return cluster_by_crash_id
  def analyze_cluster(self, cluster: Dict[int, list]):
    result: Dict[int, Tuple[List[Dict[str, int]], List[Dict[str, int]], List[Dict[str, int]]]] = dict()
    for crash_id, data_list in cluster.items():
      removed = list()
      removed_if_feasible = list()
      removed_if_infeasible = list()
      result[crash_id] = (removed, removed_if_feasible, removed_if_infeasible)
      # get base state
      base = data_list[0]
      for data in data_list:
        if data["stateType"] == '2':
          base = data
          break
      is_crash = base["isCrash"]
      # 0. remove most likely incorrect patches
      if not is_crash:
        for data in data_list:
          if data["actuallyCrashed"]:
            removed.append({"id": data["state"], "patch": data["patchId"]})
      # 1. if input is feasible: 
      # crash -> not crash
      # not crash -> not crash (preserve regression)
      for data in data_list:
        if data["actuallyCrashed"]:
          removed_if_feasible.append({"id": data["state"], "patch": data["patchId"]})
        elif not is_crash:
          reg = base["lazyTrace"].split() if "lazyTrace" in base else []
          data_reg = data["lazyTrace"].split() if "lazyTrace" in data else []
          if reg != data_reg:
            removed_if_feasible.append({"id": data["state"], "patch": data["patchId"]})
      # 2. if input is infeasible:
      # crash -> crash (preserve regression)
      # not crash -> crash (?)
      for data in data_list:
        if is_crash and not data["actuallyCrashed"]:
          removed_if_infeasible.append({"id": data["state"], "patch": data["patchId"]})
    return result
  def generate_table(self, cluster: Dict[int, list]):
    def to_str(patch: int, state: int) -> str:
      return f"{patch} ({state})"
    def list_to_str(l: list) -> str:
      return ", ".join(map(lambda x: to_str(x["patch"], x["id"]), l))
    removed = self.analyze_cluster(cluster)
    with open(os.path.join(self.dir, "table.md"), "w") as md:
      md.write("# Table\n")
      md.write(f"| crashId | num | removed patch (state) | feasible | infeasible |\n")
      md.write(f"| ------- | --- | --------------------- | -------- | ---------- |\n")
      for base, data_list in cluster.items():
        rm, rm_if_feasible, rm_if_infeasible = removed[base]
        md.write(f"| {base} | {len(data_list)} | {list_to_str(rm)} | {list_to_str(rm_if_feasible)} | {list_to_str(rm_if_infeasible)} |\n")
      for base, data_list in cluster.items():
        md.write(f"## Crash ID: {base}, len {len(data_list)}\n")
        md.write("### removed:\n")
        md.write("| patchId | state |\n")
        md.write("| ------- | ----- |\n")
        rm, rm_if_feasible, rm_if_infeasible = removed[base]
        for data in rm:
          md.write(f"| {data['patch']} | {data['id']} |\n")
        md.write("### removed if feasible:\n")
        md.write("| patchId | state |\n")
        md.write("| ------- | ----- |\n")
        for data in rm_if_feasible:
          md.write(f"| {data['patch']} | {data['id']} |\n")
        md.write("### removed if infeasible:\n")
        md.write("| patchId | state |\n")
        md.write("| ------- | ----- |\n")
        for data in rm_if_infeasible:
          md.write(f"| {data['patch']} | {data['id']} |\n")
        md.write("### base\n")
        md.write("| id | patchId | stateType | isCrash | actuallyCrashed | regression | exit | stackTrace |\n")
        md.write("| -- | ------- | --------- | ------- | --------------- | ---------- | ---- | ---------- |\n")
        for data in data_list:
          if data["stateType"] == '2':
            reg = data["lazyTrace"] if "lazyTrace" in data else ""
            st = data["stackTrace"] if "stackTrace" in data else ""
            md.write(f"| {data['state']} | {data['patchId']} | {data['stateType']} | {data['isCrash']} | {data['actuallyCrashed']} | [{reg}] | {data['exit']} | {st} |\n")
        md.write("### correct:\n")
        md.write("| id | patchId | stateType | isCrash | actuallyCrashed | regression | exit | stackTrace |\n")
        md.write("| -- | ------- | --------- | ------- | --------------- | ---------- | ---- | ---------- |\n")
        for data in data_list:
          if data["patchId"] == 2:
            reg = data["lazyTrace"] if "lazyTrace" in data else ""
            st = data["stackTrace"] if "stackTrace" in data else ""
            md.write(f"| {data['state']} | {data['patchId']} | {data['stateType']} | {data['isCrash']} | {data['actuallyCrashed']} | [{reg}] | {data['exit']} | {st} |\n")
        md.write("### data:\n")
        md.write("| id | patchId | stateType | isCrash | actuallyCrashed | regression | exit | stackTrace |\n")
        md.write("| -- | ------- | --------- | ------- | --------------- | ---------- | ---- | ---------- |\n")
        for data in data_list:
          reg = data["lazyTrace"] if "lazyTrace" in data else ""
          st = data["stackTrace"] if "stackTrace" in data else ""
          md.write(f"| {data['state']} | {data['patchId']} | {data['stateType']} | {data['isCrash']} | {data['actuallyCrashed']} | [{reg}] | {data['exit']} | {st} |\n")
  def generate_fork_graph(self, name: str):
    dot = graphviz.Digraph()
    done = set(self.meta_data.keys())
    for state in done:
      meta_data = self.meta_data[state]
      state_type = meta_data["stateType"]
      if state_type == '4':
        dot.node(str(state), style="filled", fillcolor="blue")
      elif state_type == '2':
        dot.node(str(state), style="filled", fillcolor="green")
      elif state_type == '1':
        dot.node(str(state), style="filled", fillcolor="red")
    for source, state_type in self.state_type_map.items():
      if source not in done:
        if state_type == '4':
          dot.node(str(source), style="filled", fillcolor="skyblue")
        elif state_type == '2':
          dot.node(str(source), style="filled", fillcolor="lightgreen")
        elif state_type == '1':
          dot.node(str(source), style="filled", fillcolor="pink")
    for source, targets in self.merge_edge_map.items():
      for target in targets:
        dot.edge(str(source), str(target), style="dashed", color="red", label=f"patch {self.merge_patch_map[target]}")
    for source, targets in self.fork_graph.items():
      for target in targets:
        dot.edge(str(source), str(target))
    dot.render(name, self.dir, view=False, format="svg")
    dot.render(name, self.dir, view=False, format="png")
    dot.render(name, self.dir, view=False, format="pdf")
    # dot.attr(size="10,10")
  def generate(self):
    self.read_data_log("data.log")
    self.generate_table(self.cluster())
    self.generate_fork_graph("fork-graph")
    print(f"Saved table to {os.path.join(self.dir, 'table.md')}")
    
        

class Analyzer:
  config: Config
  dir: str
  def __init__(self, conf: Config):
    self.config = conf
  def sort_key(self, filename: str):
    # Extract the number from the filename using regular expression
    match = re.search(r'snapshot-(\d+)', filename)
    if match:
        return int(match.group(1))
    else:
        # For filenames without numbers, assign a higher number
        return float('inf')
  def collect_snapshot_names(self) -> List[Tuple[str, int]]:
    snapshot_files = glob.glob(os.path.join(self.dir, "snapshot-*.json"))
    snapshot_files = sorted(snapshot_files, key=self.sort_key)
    result = list()
    for i in range(len(snapshot_files)):
      result.append((os.path.basename(snapshot_files[i]), i))
    return result
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
  def compare_trace(self, trace_a: List[str], trace_b: List[str]):
    diff = list(difflib.ndiff(trace_a, trace_b))
    fig, ax = plt.subplots(figsize=(10, 40))
    # Define chunk size and output directory
    chunk_size = 200  # Number of lines to display in each chunk
    output_directory = os.path.join(self.dir, "trace")  # Change this to your desired output directory
    os.makedirs(output_directory, exist_ok=True)
    # Generate and save PNG files for each chunk
    print(f"Generate {len(diff)} diff.pdf in {output_directory}")
    with PdfPages(os.path.join(output_directory, 'diff.pdf')) as pdf:
      for i in range(0, len(diff), chunk_size):
          ax.clear()
          count = 0
          end = min(i + chunk_size, len(diff))
          for j, line in enumerate(diff[i:end]):
              if line.startswith('-'):
                count += 1
                ax.annotate(line[2:], xy=(0, j/chunk_size), xytext=(0, j/chunk_size), textcoords='offset points', color='red')
              elif line.startswith('+'):
                count += 1
                ax.annotate(line[2:], xy=(0, j/chunk_size), xytext=(0, j/chunk_size), textcoords='offset points', color='green')
          ax.set_xlim(0, 1)
          ax.set_ylim(0, 1)
          output_filename = f"{output_directory}/diff_chunk_{i // chunk_size}.png"
          if count > 0:
            pdf.savefig(fig, dpi=300, bbox_inches='tight')
          plt.close()
  def compare_snapshots(self, snapshot_files: List[Tuple[str, int]]):
    print(f"Snapshot files:")
    while True:
      snapshot_name_a, index = self.interactive_select(snapshot_files, "snapshot a")
      if snapshot_name_a == "":
        cmd = input("Extract snapshot?(y/n): ").strip()
        if cmd == "y":
          for snapshot_name, index in snapshot_files:
            with open(os.path.join(self.dir, snapshot_name), "r") as f:
              snapshot_a = json.load(f)
              if not os.path.exists(os.path.join(self.dir, "trace", snapshot_name + ".trace")):
                with open(os.path.join(self.dir, "trace", snapshot_name + ".trace"), "w") as tf:
                  for line in snapshot_a["trace"]:
                    tf.write(line)
                    tf.write("\n")
        print("Exit")
        return
      with open(os.path.join(self.dir, snapshot_name_a), "r") as f:
        snapshot_a = json.load(f)
      snapshot_name_b, index = self.interactive_select(snapshot_files, "snapshot b")
      if snapshot_name_b == "" or snapshot_name_a == snapshot_name_b:
        print("Exit")
        return
      with open(os.path.join(self.dir, snapshot_name_b), "r") as f:
        snapshot_b = json.load(f)
        
      self.compare_trace(snapshot_a["trace"], snapshot_b["trace"])
      cmd = input("Continue? (y/n): ").strip()
      if cmd == "n":
        print("Exit")
        return
  def analyze(self):
    self.dir = self.config.conf_files.out_dir
    if not os.path.exists(self.dir):
      print(f"{self.dir} does not exist")
      out_dirs = self.config.conf_files.find_all_nums(self.config.conf_files.out_base_dir, self.config.conf_files.out_dir_prefix)
      out_dir = self.interactive_select(out_dirs, "dir")[0]
      if out_dir == "":
        print("Exit")
        return
      self.dir = os.path.join(self.config.conf_files.out_base_dir, out_dir)
    dp = DataLogParser(self.dir)
    dp.generate()
    # Removed because of the performance issue
    # snapshot_files = self.collect_snapshot_names()
    # tg = TableGenerator(self.dir, [name for name, index in snapshot_files])
    # tg.generate()
    # ra = RegressionAnalysis(self.dir)
    # for snapshot_name, index in snapshot_files:
    #   with open(os.path.join(self.dir, snapshot_name), "r") as f:
    #     snapshot = json.load(f)
    #     ra.add_trace(snapshot["regressionTrace"], snapshot_name)
    # ra.analyze()    
    # self.compare_snapshots(snapshot_files)
      

class RegressionAnalysis:
  result_dir: str
  trace_groups: dict
  def __init__(self, result_dir: str):
    self.result_dir = result_dir
    self.trace_groups = dict()
  def add_trace(self, trace: list, name: str):
    key = tuple(trace)
    if key not in self.trace_groups:
      self.trace_groups[key] = [name]
    else:
      self.trace_groups[key].append(name)
  def get_state_id(self, name: str) -> str:
    # Extract the number from the filename using regular expression
    match = re.search(r'snapshot-(\d+)', name)
    if match:
        return match.group(1)
    else:
        # For filenames without numbers, assign a higher number
        return "-1"
  def analyze(self):
    grouped_traces = list(self.trace_groups.values())
    group_labels = [self.get_state_id(traces[0]) for traces in grouped_traces]
    plt.figure(figsize=(10, 20))
    plt.bar(range(len(grouped_traces)), [len(traces) for traces in grouped_traces])
    plt.xticks(range(len(grouped_traces)), group_labels, rotation=45)
    plt.xlabel('Trace Groups')
    plt.ylabel('Count')
    plt.title('Visualization of Trace Groups')
    plt.savefig(os.path.join(self.result_dir, "trace_groups.png"))
    with open(os.path.join(self.result_dir, "trace_groups.csv"), "w") as f:
      group_id = 0
      f.write("group_id,state_id,trace\n")
      for group in self.trace_groups:
        group_id += 1
        names = self.trace_groups[group]
        for name in names:
          f.write(f"{group_id},{self.get_state_id(name)},")
          f.write(",".join(map(str, group)))
          f.write("\n")
    # Create a graph
    plt.clf()
    G = nx.Graph()
    for key, names in self.trace_groups.items():
      G.add_node(self.get_state_id(names[0]), traces=key)
    for node1 in G.nodes():
      for node2 in G.nodes():
        if node1 != node2 and not G.has_edge(node1, node2):
          group1 = G.nodes[node1]['traces']
          group2 = G.nodes[node2]['traces']
          # similarity = self.jaccard_similarity(group1, group2)
          G.add_edge(node1, node2)
    # Draw the graph
    pos = nx.spring_layout(G, seed=42)
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw(G, pos, with_labels=True, node_size=500, node_color='skyblue')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)
    plt.title('Similarity Graph of Trace Groups')
    plt.savefig(os.path.join(self.result_dir, "trace_groups_graph.png"))
    
  def hamming_distance(self, vector1: list, vector2: list):
    # Check if the vectors have the same length
    
    # Calculate the Hamming distance
    distance = 0
    for bit1, bit2 in zip(vector1, vector2):
      if bit1 != bit2:
        distance += 1
    return distance

    
class TableGenerator:
  result_dir: str
  state_names: list
  def __init__(self, result_dir: str, state_names: list):
    self.result_dir = result_dir
    self.state_names = state_names
  def parse_name(self, name: str) -> Dict[str, str]:
    # name = "snapshot-8-r0-c5c-p0.overshift.err.json"
    name = name.removeprefix("snapshot-")
    name = name.replace(".json", "")
    tokens = name.split("-")
    if len(tokens) < 4:
      return None
    result = dict()
    result["id"] = tokens[0]
    result["ret"] = tokens[1].replace("r", "")
    result["base_exit"] = tokens[2][0] == "e"
    result["base"] = tokens[2].replace("c", "").replace("e", "")
    result["exit"] = tokens[3][-1] == "e"
    result["patch"] = tokens[3].split(".")[0].replace("p", "")
    return result
  def get_regression(self, name: str) -> str:
    with open(os.path.join(self.result_dir, name), "r") as f:
      lines = f.readlines()
      for index in range(len(lines)):
        line = lines[index].strip()
        if line.startswith('"regressionTrace"'):
          tmp = line.split(":", 1)
          if len(tmp) > 1 and tmp[1].strip() != "":
            return json.loads(tmp[1].strip(","))
          content = ""
          jline = index + 1
          while not line.startswith('],'):
            line = lines[jline].strip()
            content += line
            jline += 1
          return json.loads(content.strip(","))
    return None
  def generate(self):
    fileds = ["id", "patch", "ret", "base_exit", "base", "exit", "regression"]
    csv_file = os.path.join(self.result_dir, "table.csv")
    markdown_file = os.path.join(self.result_dir, "table.md")
    with open(csv_file, "w") as f, open(markdown_file, "w") as md:
        writer = csv.DictWriter(f, fieldnames=fileds)
        writer.writeheader()
        data_by_base = {}
        for name in self.state_names:
            data = self.parse_name(name)
            if data is None:
                continue
            data["regression"] = self.get_regression(name)
            base = data["base"]
            if base not in data_by_base:
                data_by_base[base] = []
            data_by_base[base].append(data)
        for base, data_list in data_by_base.items():
            md.write(f"## {base}\n\n")
            md.write("| id | patch | ret | base_exit | exit | regression |\n")
            md.write("| -- | ----- | --- | --------- | ---- | ---------- |\n")
            for data in data_list:
                writer.writerow(data)
                md.write(f"| {data['id']} | {data['patch']} | {data['ret']} | {data['base_exit']} | {data['exit']} | {data['regression']} |\n")
            md.write("\n")   

def acquire_lock(lock_file: str, lock_behavior: str) -> int:
  timeout = 10  # Maximum time (in seconds) to wait for the lock
  interval = 0.1  # Time (in seconds) between attempts

  start_time = time.time()
  while True:
    try:
      # Try to create a lock file (this will fail if the file already exists)
      lock_fd = os.open(lock_file, os.O_CREAT | os.O_EXCL | os.O_RDWR)
      os.write(lock_fd, str(os.getpid()).encode("utf-8"))
      return lock_fd
    except FileExistsError:
      if lock_behavior == "w":
        print("Lock file exists, waiting...")
        # wait random time
        time.sleep(random.random())
        continue
      elif lock_behavior == "i":
        lock_behavior = input("Lock file exists, [w]ait or [f]orce kill?: ").lower()
      elif lock_behavior == "f":
        print("Force to kill the previous process and acquire the lock")
        with open(lock_file, "r") as f:
          pid = int(f.read())
          os.kill(pid, signal.SIGKILL)
        os.remove(lock_file)
        continue
      if time.time() - start_time > timeout:
        return None  # Lock not acquired within the timeout
      time.sleep(interval)

def release_lock(lock_file: str, lock_fd: int):
  os.close(lock_fd)
  os.remove(lock_file)

def log(args: List[str]):
  if len(args) < 2:
    return
  cmd = args[1]
  log_file = "logs/.meta-test/log"
  lock_file = "logs/.meta-test/lock"
  if cmd == "log":
    if os.path.exists(log_file):
      with open(log_file, "r") as f:
        lines = f.readlines()
        # print last 10 lines
        for line in lines[-10:]:
          print(line.strip())
    exit(0)
  os.makedirs("logs/.meta-test", exist_ok=True)
  lock = acquire_lock(lock_file, "w")
  try:
    with open(log_file, "a") as f:
      f.write(" ".join(args) + "\n")
  except:
    print("Cannot write log")
  finally:
    release_lock(lock_file, lock)

def arg_parser(argv: List[str]) -> Config:
  parser = argparse.ArgumentParser(description="Test script for uni-klee")
  parser.add_argument("cmd", help="Command to execute", choices=["run", "rerun", "cmp", "fork", "snapshot", "batch", "filter", "analyze"])
  parser.add_argument("query", help="Query for bugid and patch ids: <bugid>[:<patchid>] # ex) 5321:1,2,3")
  parser.add_argument("-a", "--additional", help="Additional arguments", default="")
  parser.add_argument("-d", "--debug", help="Debug mode", action="store_true")
  parser.add_argument("-o", "--outdir", help="Output directory", default="")
  parser.add_argument("-p", "--outdir-prefix", help="Output directory prefix(\"out\" for out dir)", default="uni-m-out")
  parser.add_argument("-b", "--snapshot-base-patch", help="Patches for snapshot", default="buggy")
  parser.add_argument("-s", "--snapshot-prefix", help="Snapshot directory prefix", default="snapshot")
  parser.add_argument("-l", "--sym-level", help="Symbolization level", default="medium")
  parser.add_argument("-f", "--max-fork", help="Max fork", default="64,64,4")
  parser.add_argument("-k", "--lock", help="Handle lock behavior", default="i", choices=["i", "w", "f"])
  parser.add_argument("-r", "--rerun", help="Rerun last command with same option", action="store_true")
  args = parser.parse_args(argv[1:])
  conf = Config(args.cmd, args.query, args.debug, args.sym_level, args.max_fork)
  conf.init(args.snapshot_base_patch, args.rerun, args.additional, args.lock)
  conf.conf_files.set_out_dir(args.outdir, args.outdir_prefix, conf.bug_info, args.snapshot_prefix)
  return conf

def main(args: List[str]):
  root_dir = ROOT_DIR
  os.chdir(root_dir)
  log(args)
  conf = arg_parser(args)
  lock_file = conf.conf_files.get_lock()
  if conf.cmd == "analyze":
    runner = Runner(conf)
    runner.run()
    exit(0)
  lock = acquire_lock(lock_file, conf.lock)
  try:
    runner = Runner(conf)
    runner.run()
  except Exception as e:
    print(f"Error: {e}")
  finally:
    release_lock(lock_file, lock)

if __name__ == "__main__":
  main(sys.argv)