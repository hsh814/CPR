#! /usr/bin/env python3
import os
import sys
from typing import List, Set, Dict, Tuple
import json

import patch

def select_from_meta(meta: dict, query: str) -> dict:
  def check_int(s: str) -> bool:
    try:
      int(s)
      return True
    except ValueError:
      return False
  num = -1
  if check_int(query):
    num = int(query)
  for data in meta:
    bid = data["bug_id"]
    benchmark = data["benchmark"]
    subject = data["subject"]
    id = data["id"]
    if num != -1:
      if num == id:
        return data
    if query.lower() in bid.lower():
      return data
  return None

def read_conf_file(conf_file: str) -> dict:
  with open(conf_file, "r") as f:
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

def find_num(dir: str, name: str) -> int:
  result = 0
  dirs = os.listdir(dir)
  while True:
    if f"{name}-{result}" in dirs:
      result += 1
    else:
      break
  return result

def execute(cmd: str, dir: str) -> int:
  cwd = os.getcwd()
  os.chdir(dir)
  print(f"Change directory to {dir}")
  print(f"Executing: {cmd}")
  exitcode = os.system(cmd)
  os.chdir(cwd)
  print(f"Exit code: {exitcode}")
  return exitcode

def run(root_dir: str, bug_info: dict, patchid: str, outdir: str, cmd: str, additional: dict):
  bid = bug_info["bug_id"]
  benchmark = bug_info["benchmark"]
  subject = bug_info["subject"]
  id = bug_info["id"]
  target_function = bug_info["target"]
  snapshot_file = "snapshot-last.json"
  if "snapshot" in bug_info:
    snapshot_file = bug_info["snapshot"]
  subdir = os.path.join(benchmark, subject, bid)
  output_dir = os.path.join(outdir, subdir, patchid)
  os.makedirs(output_dir, exist_ok=True)
  patches = os.path.join(root_dir, "patches")
  patch_dir = os.path.join(patches, "concrete", subdir, patchid)
  patch.compile(patch_dir)
  patch_file = os.path.join(patch_dir, "libuni_klee_runtime.bca")
  repair_conf_file = os.path.join(patches, subdir, "repair.conf")
  file_check = [patch_file, repair_conf_file]
  for file in file_check:
    if not os.path.exists(file):
      print(f"Cannot find {file}")
      sys.exit(1)
  conf = read_conf_file(repair_conf_file)
  poc_path = "exploit"
  if "poc_path" in conf:
    poc_path = conf["poc_path"]
  target = os.path.join(conf["src_directory"], conf["binary_path"] + ".bc")
  test_cmd = f"{target} "
  if "test_input_list" in conf:
    test_cmd += conf['test_input_list'].replace("$POC", poc_path)
  no = find_num(output_dir, "uni-out")
  snapshot_dir = os.path.join(output_dir, "snapshot")
  uni_out_dir = os.path.join(output_dir, f"uni-out-{no}")
  SNAPSHOT_DEFAULT_OPTS = f"--output-dir={snapshot_dir} --write-smt2s --libc=uclibc --allocate-determ --posix-runtime --external-calls=all --target-function={target_function}"
  UNI_KLEE_DEFAULT_OPTS = f"--output-dir={uni_out_dir} --write-smt2s --write-kqueries --libc=uclibc --allocate-determ --posix-runtime --external-calls=all --no-exit-on-error --dump-snapshot --log-trace --simplify-sym-indices --make-lazy --start-from-snapshot --target-function={target_function} --snapshot={snapshot_dir}/{snapshot_file}"
  link_opts = f"--link-llvm-lib={patch_file}"
  if "klee_flags" in conf:
    link_opts += f" {conf['klee_flags']}"
  data_dir = os.path.join(root_dir, "data", subdir)

  if not os.path.exists(os.path.join(data_dir, target)):
    execute("extract-bc " + conf["binary_path"], os.path.join(data_dir, conf["src_directory"]))

  if cmd == "snapshot":
    os.system(f"rm -rf {snapshot_dir}")
  if not os.path.exists(snapshot_dir):
    execute(f"uni-klee {link_opts} {SNAPSHOT_DEFAULT_OPTS} {test_cmd}", data_dir)
  if cmd in ["run", "all"]:
    execute(f"uni-klee {link_opts} {UNI_KLEE_DEFAULT_OPTS} {test_cmd}", data_dir)
  elif cmd in ["cmp"]:
    new_patch_id = additional["patch"]
    new_patch_file = os.path.join(patches, "concrete", subdir, new_patch_id, "libuni_klee_runtime.bca")
    new_link_opts = f"--link-llvm-lib={new_patch_file}"
    if "klee_flags" in conf:
      new_link_opts += f" {conf['klee_flags']}"
    new_output_dir = os.path.join(outdir, subdir, new_patch_id)
    os.makedirs(new_output_dir, exist_ok=True)
    new_no = find_num(new_output_dir, "uni-out")
    new_snapshot_dir = os.path.join(new_output_dir, "snapshot")
    new_uni_out_dir = os.path.join(new_output_dir, f"uni-out-{new_no}")
    NEW_UNI_KLEE_DEFAULT_OPTS = f"--make-lazy --output-dir={new_uni_out_dir} --start-from-snapshot --write-smt2s --write-kqueries --libc=uclibc --allocate-determ --posix-runtime --external-calls=all --no-exit-on-error --dump-snapshot --log-trace --simplify-sym-indices --target-function={target_function} --snapshot={snapshot_dir}/{snapshot_file}"
    execute(f"uni-klee {new_link_opts} {NEW_UNI_KLEE_DEFAULT_OPTS} {test_cmd}", data_dir)

def main(args: List[str]):
  root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
  if len(args) < 2:
    print(f"Usage: {args[0]} <cmd> <query>")
    print("Ex) test.py run 5321:buggy")
    sys.exit(1)
  cmd = args[1]
  query = args[2]
  additional = dict()
  if cmd == "cmp":
    if len(args) < 4:
      print(f"Usage: {args[0]} cmp <query> <patchid>")
      print("Ex) test.py cmp 5321:buggy 1-0")
      sys.exit(1)
    additional["patch"] = args[3]
  patches = os.path.join(root_dir, "patches")
  outdir = os.path.join(root_dir, "out")
  print(f"outdir: {outdir}")
  with open(f"{patches}/meta-data.json", "r") as f:
    data = json.load(f)
  if ":" in query:
    parsed = query.rsplit(":", 1)
  else:
    parsed = query.rsplit("/", 1)
  bugid = parsed[0]
  if len(parsed) < 2:
    patchid = "buggy"
  else:
    patchid = parsed[1]
  if len(parsed) < 2:
    print(f"Invalid query: {query}: should be 'bugid:patchid'")
    sys.exit(1)
  bug_info = select_from_meta(data, bugid)
  if bug_info is None:
    print(f"Cannot find patch for {bugid}")
    sys.exit(1)
  run(root_dir, bug_info, patchid, outdir, cmd, additional)


if __name__ == "__main__":
  main(sys.argv)