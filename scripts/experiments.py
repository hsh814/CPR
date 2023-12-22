#!/usr/bin/env python3
from typing import Union, List, Dict, Tuple, Optional, Set
import multiprocessing as mp
import subprocess

import os
import sys
import json
import time

# import importlib
# PARENT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# import module from ../meta-test.py
# sys.path.append(PARENT_DIR)
# meta_test = importlib.import_module("meta-test", package=None)
import meta_test

ROOT_DIR = meta_test.ROOT_DIR
GLOBAL_LOG_DIR = os.path.join(ROOT_DIR, "logs")

def execute(cmd: str, dir: str, log_file: str, log_dir: str, prefix: str, lock: mp.Lock):
  print(f"Executing: {cmd}")
  start = time.time()
  proc = subprocess.run(cmd, shell=True, cwd=dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  end = time.time()
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
    res = meta_test.global_config.get_meta_data_info_by_id(id)
    self.meta = res["meta"]
    self.meta_program = res["meta_program"]
    self.conf = res["conf"]
  def get_filter_cmd(self) -> str:
    return f"meta-test.py filter {self.meta['bug_id']} --lock=w"
  def get_cmd(self) -> str:
    if "correct" not in self.meta:
      print("No correct patch")
      return None
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
    cmd = f"meta-test.py rerun {query} --lock=w"
    print(cmd)
    return cmd

def check_correct_exists(meta: dict) -> bool:
  if "correct" not in meta:
    return False
  if "no" not in meta["correct"]:
    return False
  return True

def main():
  core = 32
  meta_data = meta_test.global_config.get_meta_data_list()
  print(f"Total meta data: {len(meta_data)}")
  m = mp.Manager()
  lock = m.Lock()
  run_clean = False
  if run_clean:
    pool = mp.Pool(core)
    args_list = list()
    for meta in meta_data:
      if not check_correct_exists(meta):
        continue
      cmd = f"meta-test.py clean {meta['bug_id']} --lock=w"
      if cmd is None:
        continue
      args_list.append((cmd, ROOT_DIR, f"{meta['id']}.log", "clean", f"clean,{meta['id']}", lock))
    print(f"Total clean: {len(args_list)}")
    pool.map(execute_wrapper, args_list)
    pool.close()
    pool.join()
    print(f"Cleaning done")
    return
  run_filter = False
  if run_filter:
    pool = mp.Pool(core)
    args_list = list()
    for meta in meta_data:
      if not check_correct_exists(meta):
        continue
      rs = RunSingle(meta["id"])
      cmd = rs.get_filter_cmd()
      if cmd is None:
        continue
      args_list.append((cmd, ROOT_DIR, f"{meta['id']}.log", "filter", f"filter,{meta['id']}", lock))
    print(f"Total filter: {len(args_list)}")
    pool.map(execute_wrapper, args_list)
    pool.close()
    pool.join()
    print(f"Filtering done")
  pool = mp.Pool(core)
  args_list = list()
  for meta in meta_data:
    if not check_correct_exists(meta):
      continue
    rs = RunSingle(meta["id"])
    cmd = rs.get_cmd()
    if cmd is None:
      continue
    args_list.append((cmd, ROOT_DIR, f"{meta['id']}.log", "experiment-100", f"experiment-100,{meta['id']}", lock))
  print(f"Total experiment: {len(args_list)}")
  pool.map(execute_wrapper, args_list)
  pool.close()
  pool.join()

if __name__ == "__main__":
  main()