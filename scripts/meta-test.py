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
  def set_out_dir(self, out_dir: str, out_dir_prefix: str, bug_info: dict):
    self.out_dir_prefix = out_dir_prefix
    if out_dir == "":
      self.out_base_dir = os.path.join(self.root_dir, "out", self.benchmark, self.subject, self.bid)
    elif out_dir == "out":
      self.out_base_dir = self.work_dir
    else:
      self.out_base_dir = out_dir
    os.makedirs(self.out_base_dir, exist_ok=True)
    no = self.find_num(self.out_base_dir, out_dir_prefix)
    self.out_dir = os.path.join(self.out_base_dir, f"{out_dir_prefix}-{no}")
    self.snapshot_dir = os.path.join(self.out_base_dir, "snapshot")
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
  def find_all_nums(self, dir: str, name: str) -> List[Tuple[str, int]]:
    result = list()
    dirs = os.listdir(dir)
    index = 0
    for d in dirs:
      if f"{name}-{index}" in dirs:
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
    for patch in self.meta_program["patches"]:
      if str(patch["id"]) in patch_ids:
        result.append(str(patch["id"]))
      elif patch["name"] in patch_ids:
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
  
  def __init__(self, cmd: str, query: str, debug: bool):
    self.cmd = cmd
    self.query = query    
    self.debug = debug
    self.conf_files = ConfigFiles()
  
  @staticmethod  
  def parser(argv: List[str]) -> 'Config':
    parser = argparse.ArgumentParser(description="Test script for uni-klee")
    parser.add_argument("cmd", help="Command to execute", choices=["run", "cmp", "fork", "snapshot", "batch", "filter", "analyze"])
    parser.add_argument("query", help="Query for bugid and patch ids: <bugid>[:<patchid>] # ex) 5321:1,2,3")
    parser.add_argument("-a", "--additional", help="Additional arguments", default="")
    parser.add_argument("-d", "--debug", help="Debug mode", action="store_true")
    parser.add_argument("-o", "--outdir", help="Output directory", default="")
    parser.add_argument("-p", "--outdir-prefix", help="Output directory prefix", default="uni-m-out")
    parser.add_argument("-s", "--snapshot", help="Patches for snapshot", default="buggy")
    args = parser.parse_args(argv[1:])
    conf = Config(args.cmd, args.query, args.debug)
    conf.init(args.snapshot, args.additional)
    conf.conf_files.set_out_dir(args.outdir, args.outdir_prefix, conf.bug_info)
    return conf
  
  def append_snapshot_cmd(self, cmd: List[str]):
    snapshot_dir = self.conf_files.snapshot_dir
    patch_str = ",".join(self.snapshot_patch_ids)
    cmd.append(f"--output-dir={snapshot_dir}")
    cmd.append(f"--patch-id={patch_str}")
  
  def append_cmd(self, cmd: List[str], patch_str: str, opts: List[str]):
    out_dir = self.conf_files.out_dir
    default_opts = ["--no-exit-on-error", "--simplify-sym-indices", "--make-lazy", "--dump-snapshot"]
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
      result.append((snapshot_files[i], i))
    return result
  def print_list(self, l: List[Tuple[str, int]]):
    for item, index in l:
      print(f"{index}) {item}")
  def interactive_select(self, l: List[Tuple[str, int]]) -> Tuple[str, int]:
    print("Select from here: ")
    self.print_list(l)
    default = l[-1][1]
    while True:
      tmp = input(f"Select(default: {default}): ").strip()
      res = default
      if tmp != "":
        res = int(tmp)
      for item, index in l:
        if res == index:
          return (item, index)
  def analyze(self):
    self.dir = self.config.conf_files.out_dir
    if not os.path.exists(self.dir):
      print(f"{self.dir} does not exist")
      if self.config.additional != "":
        self.dir = self.config.additional
        print(f"Use {self.dir} instead")
      else:
        out_dirs = self.config.conf_files.find_all_nums(self.config.conf_files.out_base_dir, self.config.conf_files.out_dir_prefix)
        out_dir = self.interactive_select(out_dirs)[0]
        self.dir = os.path.join(self.config.conf_files.out_base_dir, out_dir)
    snapshot_files = self.collect_snapshot_names()
    print(f"Snapshot files:")
    self.print_list(snapshot_files)    

def main(args: List[str]):
  root_dir = ROOT_DIR
  os.chdir(root_dir)
  conf = Config.parser(args)
  runner = Runner(conf)
  runner.run()

if __name__ == "__main__":
  main(sys.argv)