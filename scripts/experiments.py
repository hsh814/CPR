#!/usr/bin/env python3
from typing import Union, List, Dict, Tuple, Optional, Set
import multiprocessing as mp
import subprocess

import os
import sys
import json
import time
import datetime
import sbsv
import argparse
import psutil

# import importlib
# PARENT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# import module from uni_klee.py
import uni_klee
import symvass

ROOT_DIR = uni_klee.ROOT_DIR
GLOBAL_LOG_DIR = os.path.join(ROOT_DIR, "logs")
OUTPUT_DIR = "out"
PREFIX = ""
SYMVASS_PREFIX = "uni-m-out"

def kill_proc_tree(pid: int, including_parent: bool = True):
  parent = psutil.Process(pid)
  children = parent.children(recursive=True)
  for child in children:
    child.kill()
  psutil.wait_procs(children, timeout=5)
  if including_parent:
    parent.kill()
    parent.wait(5)

def execute(cmd: str, dir: str, log_file: str, log_dir: str, prefix: str, meta: dict):
  print(f"Executing: {cmd}")
  start = time.time()
  timeout = 12 * 3600 + 600 # 12 hours + 10 minutes for analysis
  proc = subprocess.Popen(cmd, shell=True, cwd=dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  try:
    stdout, stderr = proc.communicate(timeout=timeout)
  except subprocess.TimeoutExpired:
    print(f"Timeout: {cmd}")
    kill_proc_tree(proc.pid)
    stdout, stderr = proc.communicate()
  finally:
    end = time.time()
  print(f"Done {prefix}: {end - start}s")
  with open(os.path.join(GLOBAL_LOG_DIR, "time.log"), "a") as f:
    f.write(f"{prefix},{end - start}\n")
  collect_result(meta)
  if proc.returncode != 0:
    print(f"Failed to execute: {cmd}")
    try:
      if not os.path.exists(os.path.join(GLOBAL_LOG_DIR, log_dir)):
        os.makedirs(os.path.join(GLOBAL_LOG_DIR, log_dir), exist_ok=True)
      with open(os.path.join(log_dir, log_file), "w") as f:
        f.write(stderr.decode("utf-8", errors="ignore"))
        f.write("\n###############\n")
        f.write(stdout.decode("utf-8", errors="ignore"))
    except Exception as e:
      print(f"Failed to write log file: {log_file}")
      print(e)
    return False
  return True

def execute_wrapper(args):
  return execute(*args)

class RunSingle():
  meta: dict = None
  meta_program: dict = None
  conf: dict = None  
  def __init__(self, id: int):
    res = uni_klee.global_config.get_meta_data_info_by_id(id)
    self.meta = res["meta"]
    self.meta_program = res["meta_program"]
    self.conf = res["conf"]
  def get_clean_cmd(self) -> str:
    return f"symvass.py clean {self.meta['bug_id']} --lock=w"
  def get_filter_cmd(self) -> str:
    return f"symvass.py filter {self.meta['bug_id']} --lock=f"
  def get_analyze_cmd(self) -> str:
    return f"symvass.py analyze {self.meta['bug_id']} --use-last -p {SYMVASS_PREFIX}"
  def get_exp_cmd(self, extra: str = "") -> str:
    if "correct" not in self.meta:
      print("No correct patch")
      return None
    if "no" not in self.meta["correct"]:
      for patch in self.meta_program["patches"]:
        if patch["name"] == "correct":
          self.meta["correct"]["no"] = patch["id"]
          break
    if "no" not in self.meta["correct"]:
      print("No correct patch")
      return None
    correct = self.meta["correct"]["no"]
    patches = list()
    cnt = 0
    limit = 100
    for patch in self.meta_program["patches"]:
      patches.append(patch["id"])
      cnt += 1
      if cnt >= limit:
        if correct not in patches:
          patches.append(correct)
          patches.append(correct + 1)
          patches.append(correct - 1)
        break
    print(patches)
    query = self.meta["bug_id"] + ":0" # ",".join([str(x) for x in patches])
    cmd = f"symvass.py rerun {query} --lock=f --outdir-prefix={SYMVASS_PREFIX} "
    if extra == "k2-high":
      cmd += " --sym-level=high --additional='--symbolize-bound=2' --max-fork=1024,1024,1024"
    if extra == "high":
      cmd += " --sym-level=high --max-fork=1024,1024,128"
    if extra == "k2":
      cmd += " --additional='--symbolize-bound=2' --max-fork=1024,1024,1024"
    if extra == "low":
      cmd += " --sym-level=low --max-fork=1024,1024,1024"
    return cmd
  def get_cmd(self, opt: str, extra: str) -> str:
    # if "correct" not in self.meta:
    #   print("No correct patch")
    #   return None
    if "no" not in self.meta["correct"]:
      print("No correct patch")
      return None
    if opt == "filter":
      return self.get_filter_cmd()
    if opt == "clean":
      return self.get_clean_cmd()
    if opt == "exp":
      return self.get_exp_cmd(extra)
    if opt == "analyze":
      return self.get_analyze_cmd()
    print(f"Unknown opt: {opt}")
    return None

def check_correct_exists(meta: dict) -> bool:
  if "correct" not in meta:
    return False
  if "no" not in meta["correct"]:
    return False
  return True

def check_use_high_level(meta: dict) -> bool:
  conf = symvass.Config("analyze", meta["bug_id"], False, "high", "64,64,64")
  conf.init("snapshot", False, "", "f")
  conf.conf_files.set_out_dir("", "uni-m-out", conf.bug_info, "snapshot", "filter", True)
  if not os.path.exists(conf.conf_files.out_dir):
    return False
  if not os.path.exists(os.path.join(conf.conf_files.out_dir, "table.sbsv")):
    # Run analysis
    run_cmd("analyze", [meta])
  if not os.path.exists(os.path.join(conf.conf_files.out_dir, "table.sbsv")):
    return True
  parser = parse_result(os.path.join(conf.conf_files.out_dir, "table.sbsv"))
  result = parser.get_result()
  if result is None:
    return True
  if len(result["sym-in"]) > 16:
    print(f"Skip high level: {meta['bug_id']}")
    return False
  return True

def collect_result(meta: dict):
  conf = symvass.Config("analyze", meta["bug_id"], False, "high", "64,64,64")
  conf.init("snapshot", False, "", "f")
  conf.conf_files.set_out_dir("", SYMVASS_PREFIX, conf.bug_info, "snapshot", "filter", True)
  save_dir = os.path.join(OUTPUT_DIR, PREFIX, meta["subject"], meta["bug_id"])
  os.makedirs(save_dir, exist_ok=True)
  if not os.path.exists(conf.conf_files.out_dir):
    return False
  if not os.path.exists(os.path.join(conf.conf_files.out_dir, "table.sbsv")):
    return False
  print(f"save to {save_dir}")
  save_file = os.path.join(save_dir, f"table.sbsv")
  if os.path.exists(save_file):
    os.unlink(save_file)
  # copy file
  with open(os.path.join(conf.conf_files.out_dir, "table.sbsv"), "r") as f:
    with open(save_file, "w") as f2:
      f2.write(f.read())

def parse_result(file: str) -> sbsv.parser:
  parser = sbsv.parser()
  parser.add_schema("[sym-in] [id: int] [base: int] [test: int] [cnt: int] [patches: str]")
  parser.add_schema("[sym-out] [default] [cnt: int] [patches: str]")
  parser.add_schema("[sym-out] [best] [cnt: int] [patches: str]")
  parser.add_schema("[meta-data] [correct: int] [all-patches: int] [sym-input: int] [correct-input: int]")
  with open(file, "r") as f:
    parser.load(f)
  return parser

def str_to_list(s: str) -> List[int]:
  ss = s.strip('[]')
  res = list()
  for x in ss.split(", "):
    if x.strip() == "":
      continue
    res.append(int(x))
  return res

def final_analysis(dir: str):
  results = list()
  for project in sorted(os.listdir(dir)):
    project_path = os.path.join(dir, project)
    if not os.path.isdir(project_path):
      continue
    for bug in sorted(os.listdir(project_path)):
      bug_path = os.path.join(project_path, bug)
      if not os.path.isdir(bug_path):
        continue
      target_file = os.path.join(bug_path, "table.sbsv")
      if not os.path.exists(target_file):
        continue
      parser = parse_result(target_file)
      result = parser.get_result()
      if result is None:
        continue
      meta_data = result["meta-data"]
      correct_patch = meta_data[0]["correct"]
      default = result["sym-out"]["default"][0]["cnt"]
      default_patches = str_to_list(result["sym-out"]["default"][0]["patches"])
      default_found = correct_patch in default_patches
      best = result["sym-out"]["best"][0]["cnt"]
      best_patches = str_to_list(result["sym-out"]["best"][0]["patches"])
      best_found = correct_patch in best_patches
      if len(result["sym-in"]) == 0:
        default = meta_data[0]["all-patches"]
      if meta_data[0]["correct-input"] == 0:
        best = meta_data[0]["all-patches"]
      results.append({
        "project": project,
        "bug": bug,
        "correct": correct_patch,
        "all": meta_data[0]["all-patches"],
        "inputs": len(result["sym-in"]),
        "default": default,
        "default_found": default_found,
        "best_inputs": meta_data[0]["correct-input"],
        "best": best,
        "best_found": best_found,
      })
  with open(os.path.join(dir, "final.json"), "w") as f:
    json.dump(results, f, indent=2)
  with open(os.path.join(dir, "final.csv"), "w") as f:
    f.write(f"project\tbug\tcorrect_patch\tall_patches\tinputs\tdefault_remaining_patches\tdefault_found\tbest_inputs\tbest_remaining_patches\tbest_found\n")
    for result in results:
      f.write(f"{result['project']}\t{result['bug']}\t{result['correct']}\t{result['all']}\t{result['inputs']}\t{result['default']}\t{result['default_found']}\t{result['best_inputs']}\t{result['best']}\t{result['best_found']}\n")

def run_cmd(opt: str, meta_data: List[dict], extra: str):
  core = 32
  pool = mp.Pool(core)
  args_list = list()
  for meta in meta_data:
    if not check_correct_exists(meta):
      continue
    # if is_high and not check_use_high_level(meta):
    #   continue
    rs = RunSingle(meta["id"])
    cmd = rs.get_cmd(opt, extra)
    if cmd is None:
      continue
    args_list.append((cmd, ROOT_DIR, f"{meta['bug_id']}.log", opt, f"{opt},{meta['subject']}/{meta['bug_id']}", meta))
  print(f"Total {opt}: {len(args_list)}")
  pool.map(execute_wrapper, args_list)
  pool.close()
  pool.join()
  print(f"{opt} done")


def main(argv: List[str]):
  parser = argparse.ArgumentParser(description="Run symvass experiments")
  parser.add_argument("cmd", type=str, help="Command to run", choices=["filter", "exp", "high", "k2", "k2-high", "analyze", "final"], default="exp")
  parser.add_argument("-e", "--extra", type=str, help="Subcommand", default="exp")
  parser.add_argument("-o", "--output", type=str, help="Output directory", default="out", required=False)
  parser.add_argument("-p", "--prefix", type=str, help="Output prefix", default="", required=False)
  parser.add_argument("-s", "--symvass-prefix", type=str, help="Symvass prefix", default="", required=False)
  args = parser.parse_args(argv)
  global OUTPUT_DIR, PREFIX, SYMVASS_PREFIX
  OUTPUT_DIR = os.path.join(ROOT_DIR, args.output)
  if args.prefix != "":
    PREFIX = args.prefix
  else:
    PREFIX = time.strftime("%Y-%m-%d_%H-%M-%S", time.localtime())
  if args.cmd == "exp":
    if args.extra != "exp":
      SYMVASS_PREFIX = args.extra
  if args.cmd == "filter":
    SYMVASS_PREFIX = "filter"
  if args.symvass_prefix != "":
    SYMVASS_PREFIX = args.symvass_prefix
  if args.cmd == "final":
    final_analysis(os.path.join(OUTPUT_DIR, PREFIX))
    return
  with open(os.path.join(GLOBAL_LOG_DIR, "time.log"), "a") as f:
    f.write(f"\n#{datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
  meta_data = uni_klee.global_config.get_meta_data_list()
  print(f"Total meta data: {len(meta_data)}")
  run_cmd(args.cmd, meta_data, args.extra)

if __name__ == "__main__":
  main(sys.argv[1:])