#! /usr/bin/env python3
import os
import sys
from typing import List, Set, Dict, Tuple
import json
import subprocess
import argparse
import patch
import glob
import re

import networkx as nx
import difflib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
class ConfigFiles:
  root_dir: str
  meta_data: str
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
    self.meta_data = os.path.join(self.root_dir, "patches", "meta-data.json")
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
      self.out_base_dir = os.path.join(self.root_dir, "out", self.benchmark, self.subject, self.bid)
    elif out_dir == "out":
      self.out_base_dir = self.work_dir
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
    with open(self.meta_data, "r") as f:
      return json.load(f)
  def read_meta_program(self) -> dict:
    with open(self.meta_program, "r") as f:
      return json.load(f)

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
    
  def init(self, snapshot_patch_ids: str, additional: str):
    self.meta = self.conf_files.read_meta_data()
    self.parse_query(snapshot_patch_ids)
    self.project_conf = self.conf_files.read_conf_file()
    self.workdir = self.conf_files.work_dir
    self.additional = additional
  
  def __init__(self, cmd: str, query: str, debug: bool, sym_level: str):
    self.cmd = cmd
    self.query = query    
    self.debug = debug
    self.sym_level = sym_level
    self.conf_files = ConfigFiles()
  
  @staticmethod  
  def parser(argv: List[str]) -> 'Config':
    parser = argparse.ArgumentParser(description="Test script for uni-klee")
    parser.add_argument("cmd", help="Command to execute", choices=["run", "cmp", "fork", "snapshot", "batch", "filter", "analyze"])
    parser.add_argument("query", help="Query for bugid and patch ids: <bugid>[:<patchid>] # ex) 5321:1,2,3")
    parser.add_argument("-a", "--additional", help="Additional arguments", default="")
    parser.add_argument("-d", "--debug", help="Debug mode", action="store_true")
    parser.add_argument("-o", "--outdir", help="Output directory", default="")
    parser.add_argument("-p", "--outdir-prefix", help="Output directory prefix(\"out\" for work dir)", default="uni-m-out")
    parser.add_argument("-b", "--snapshot-base-patch", help="Patches for snapshot", default="buggy")
    parser.add_argument("-s", "--snapshot-prefix", help="Snapshot directory prefix", default="snapshot")
    parser.add_argument("-l", "--sym-level", help="Symbolization level", default="low")
    args = parser.parse_args(argv[1:])
    conf = Config(args.cmd, args.query, args.debug, args.sym_level)
    conf.init(args.snapshot_base_patch, args.additional)
    conf.conf_files.set_out_dir(args.outdir, args.outdir_prefix, conf.bug_info, args.snapshot_prefix)
    return conf
  
  def append_snapshot_cmd(self, cmd: List[str]):
    snapshot_dir = self.conf_files.snapshot_dir
    patch_str = ",".join(self.snapshot_patch_ids)
    cmd.append(f"--output-dir={snapshot_dir}")
    cmd.append(f"--patch-id={patch_str}")
  
  def append_cmd(self, cmd: List[str], patch_str: str, opts: List[str]):
    out_dir = self.conf_files.out_dir
    default_opts = ["--no-exit-on-error", "--simplify-sym-indices", f"--symbolize-level={self.sym_level}", "--dump-snapshot"]
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
  def execute(self, cmd: str, dir: str, env: dict = None):
    print(f"Change directory to {dir}")
    print(f"Executing: {cmd}")
    if env is None:
      env = os.environ
    if self.config.debug:
      proc = subprocess.run(cmd, shell=True, cwd=dir, env=env)
    else:
      proc = subprocess.run(cmd, shell=True, cwd=dir, env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if proc.returncode != 0:
      print("!!!!! Error !!!!")
      print(proc.stderr.decode("utf-8"))
      with open(os.path.join(self.config.conf_files.out_dir, "error.log"), "w") as f:
        f.write(proc.stderr.decode("utf-8"))
        f.write("\n###############\n")
        f.write(proc.stdout.decode("utf-8"))
      print(f"Save error log to {self.config.conf_files.out_dir}/error.log")
    print(f"Exit code: {proc.returncode}")
    return proc.returncode
  def execute_snapshot(self, cmd: str, dir: str, env: dict = None):
    if self.config.cmd == "snapshot":
      self.execute("rm -rf " + self.config.conf_files.snapshot_dir, dir)
    if not os.path.exists(self.config.conf_files.snapshot_file):
      if self.config.debug:
        print(f"snapshot file {self.config.conf_files.snapshot_file} does not exist")
      self.execute(cmd, dir, env)
  def run(self):
    if self.config.cmd == "analyze":
      analyzer = Analyzer(self.config)
      analyzer.analyze()
      return
    cmd = self.config.get_cmd_opts(True)
    self.execute_snapshot(cmd, self.config.workdir)
    if self.config.cmd != "snapshot":
      cmd = self.config.get_cmd_opts(False)
      self.execute(cmd, self.config.workdir)
      analyzer = Analyzer(self.config)
      analyzer.analyze()

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
      if self.config.additional != "":
        self.dir = self.config.additional
        print(f"Use {self.dir} instead")
      else:
        out_dirs = self.config.conf_files.find_all_nums(self.config.conf_files.out_base_dir, self.config.conf_files.out_dir_prefix)
        out_dir = self.interactive_select(out_dirs, "dir")[0]
        if out_dir == "":
          print("Exit")
          return
        self.dir = os.path.join(self.config.conf_files.out_base_dir, out_dir)
    snapshot_files = self.collect_snapshot_names()
    ra = RegressionAnalysis(self.dir)
    for snapshot_name, index in snapshot_files:
      with open(os.path.join(self.dir, snapshot_name), "r") as f:
        snapshot = json.load(f)
        ra.add_trace(snapshot["regressionTrace"], snapshot_name)
    ra.analyze()    
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

    
      

def main(args: List[str]):
  root_dir = ROOT_DIR
  os.chdir(root_dir)
  conf = Config.parser(args)
  runner = Runner(conf)
  runner.run()

if __name__ == "__main__":
  main(sys.argv)