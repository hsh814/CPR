#!/usr/bin/env python3
from typing import Union, List, Dict, Tuple, Optional, Set
import multiprocessing as mp
import subprocess

import os
import sys
import json
import time
import datetime

# import importlib
# PARENT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# import module from uni_klee.py
import uni_klee

ROOT_DIR = uni_klee.ROOT_DIR
GLOBAL_LOG_DIR = os.path.join(ROOT_DIR, "logs")

def execute(cmd: str, dir: str, log_file: str, log_dir: str, prefix: str, lock: mp.Lock):
  print(f"Executing: {cmd}")
  start = time.time()
  proc = subprocess.run(cmd, shell=True, cwd=dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  end = time.time()
  print(f"Done {prefix}: {end - start}s")
  with lock:
    with open(os.path.join(GLOBAL_LOG_DIR, "time.log"), "a") as f:
      f.write(f"{prefix},{end - start}\n")
  if proc.returncode != 0:
    print(f"Failed to execute: {cmd}")
    try:
      if not os.path.exists(os.path.join(GLOBAL_LOG_DIR, log_dir)):
        os.makedirs(os.path.join(GLOBAL_LOG_DIR, log_dir), exist_ok=True)
      with open(os.path.join(log_dir, log_file), "w") as f:
        f.write(proc.stderr.decode("utf-8", errors="ignore"))
        f.write("\n###############\n")
        f.write(proc.stdout.decode("utf-8", errors="ignore"))
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
    return f"uni-klee.py clean {self.meta['bug_id']} --lock=w"
  def get_filter_cmd(self) -> str:
    return f"uni-klee.py filter {self.meta['bug_id']} --lock=f"
  def get_exp_cmd(self) -> str:
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
    query = self.meta["bug_id"] + ":" + ",".join([str(x) for x in patches])
    cmd = f"uni-klee.py rerun {query} --lock=f --additional='--max-time=12h'"
    return cmd
  def get_cmd(self, opt: str) -> str:
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
      return self.get_exp_cmd()
    print(f"Unknown opt: {opt}")
    return None

def check_correct_exists(meta: dict) -> bool:
  if "correct" not in meta:
    return False
  if "no" not in meta["correct"]:
    return False
  return True

def run_cmd(opt: str, meta_data: List[dict]):
  core = 32
  m = mp.Manager()
  lock = m.Lock()
  pool = mp.Pool(core)
  args_list = list()
  for meta in meta_data:
    if not check_correct_exists(meta):
      continue
    rs = RunSingle(meta["id"])
    cmd = rs.get_cmd(opt)
    if cmd is None:
      continue
    args_list.append((cmd, ROOT_DIR, f"{meta['bug_id']}.log", opt, f"{opt},{meta['subject']}/{meta['bug_id']}", lock))
  print(f"Total {opt}: {len(args_list)}")
  pool.map(execute_wrapper, args_list)
  pool.close()
  pool.join()
  print(f"{opt} done")


def main(argv: List[str]):
  cmd = "exp"
  if len(argv) > 0:
    cmd = argv[0]
  with open(os.path.join(GLOBAL_LOG_DIR, "time.log"), "a") as f:
    f.write(f"\n#{datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
  meta_data = uni_klee.global_config.get_meta_data_list()
  print(f"Total meta data: {len(meta_data)}")
  run_cmd(cmd, meta_data)

if __name__ == "__main__":
  main(sys.argv[1:])