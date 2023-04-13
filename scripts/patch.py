#!/usr/bin/env python3
import sys
import os
import re
from typing import List, Set, Dict, Tuple
import json
import multiprocessing as mp

UNI_KLEE_RUNTIME = ""
UNI_KLEE_RUNTIME_H = ""

def formula_to_code(formula: str, concrete_range: list, vars: List[str]) -> List[str]:
  variable_names = set(re.findall(r'\b([a-zA-Z_]\w*)\b', formula))
  code = ""
  for i in range(len(vars)):
    code += f"  int {vars[i]} = rvals[{i}];\n"
  result = list()
  if len(concrete_range) == 0:
    code += f"  result = {formula};\n"
    result.append(code)
    return result
  for i in concrete_range:
    tmp = f"{code}  int constant_a = {i};\n"
    tmp += f"  result = {formula};\n"
    result.append(tmp)
  return result

def to_concrete_patch(patch: dict, meta: dict) -> dict:
  num = patch["num"]
  lid = patch["lid"]
  formula = patch["patch"]
  result = { "num": num, "lid": lid, "patch": formula }
  concrete_range = list()
  if "Partition" in patch:
    for part in patch["Partition"]:
      if "Range" in part:
        range_str = part["Range"]
        start, con, end = range_str.split("<=")
        for i in range(int(start), int(end) + 1):
          concrete_range.append(i)
  code = formula_to_code(formula, concrete_range, meta["vars"])
  result["codes"] = code
  return result

def apply_patch_to_file(outdir, code):
  lines = UNI_KLEE_RUNTIME.splitlines()
  os.system(f"rm -rf {outdir}")
  os.makedirs(outdir, exist_ok=True)
  contents = list()
  print(lines)
  for line in lines:
    if "// REPLACE" in line:
      contents.append(code)
    else:
      contents.append(line + "\n")
  with open(os.path.join(outdir, "uni_klee_runtime.c"), "w") as f:
    f.writelines(contents)
  with open(os.path.join(outdir, "uni_klee_runtime.h"), "w") as f:
    f.write(UNI_KLEE_RUNTIME_H)

def save_to_file(dir: str, patches: list):
  for patch in patches:
    patch_no = patch["num"]
    codes = patch["codes"]
    for i in range(len(codes)):
      patch_id = f"{patch_no}-{i}"
      outdir = os.path.join(dir, patch_id)
      os.makedirs(outdir, exist_ok=True)
      apply_patch_to_file(outdir, codes[i])

def lazy_compile(dir: str, cmd: str, file_a: str, file_b: str):
  cwd = os.getcwd()
  os.chdir(dir)
  if os.path.exists(file_b):
    if os.path.getmtime(file_a) <= os.path.getmtime(file_b):
      os.chdir(cwd)
      return
  os.system(f"{cmd}")
  os.chdir(cwd)

def compile(dir: str):
  KLEE_INCLUDE_PATH = "/root/projects/uni-klee/include"
  cmd = f"wllvm -g -fPIC -O0 -c -o uni_klee_runtime.o uni_klee_runtime.c -I{KLEE_INCLUDE_PATH}"
  lazy_compile(dir, cmd, "uni_klee_runtime.c", "uni_klee_runtime.o")
  cmd = "llvm-ar rcs libuni_klee_runtime.a uni_klee_runtime.o"
  lazy_compile(dir, cmd, "uni_klee_runtime.o", "libuni_klee_runtime.a")
  cmd = "extract-bc libuni_klee_runtime.a"
  lazy_compile(dir, cmd, "libuni_klee_runtime.a", "libuni_klee_runtime.bca")

def main(args: List[str]):
  if len(args) != 3:
    print("Usage: patch.py <opt> <patch-dir>")
    sys.exit(1)
  opt_map = {
    "single": "Compile a single patch",
    "compile": "Compile all patches",
    "concrete": "Generate concrete patches",
    "buggy": "Generate buggy patches from meta",
  }
  opt = args[1]
  if opt not in opt_map:
    print("Invalid option")
    for k, v in opt_map.items():
      print(f"  {k}: {v}")
    sys.exit(1)
  global UNI_KLEE_RUNTIME, UNI_KLEE_RUNTIME_H
  with open("/root/projects/CPR/lib/uni_klee_runtime.c", "r") as f:
    UNI_KLEE_RUNTIME = f.read()
  with open("/root/projects/CPR/lib/uni_klee_runtime.h", "r") as f:
    UNI_KLEE_RUNTIME_H = f.read()
  patch_dir = args[2]
  pool = mp.Pool(mp.cpu_count() * 2 // 3)
  if opt == "single":
    compile(patch_dir)
    sys.exit(0)
  with open(os.path.join(patch_dir, "meta-data.json"), "r") as f:
    meta_data = json.load(f)
  for meta in meta_data:
    bug_id = meta["bug_id"]
    benchmark = meta["benchmark"]
    subject = meta["subject"]
    concrete_dir = os.path.join(patch_dir, "concrete", benchmark, subject, bug_id)
    if opt == "compile":
      if os.path.exists(concrete_dir):
        for dir in os.listdir(concrete_dir):
          if os.path.isdir(os.path.join(concrete_dir, dir)):
            pool.apply_async(compile, args=(os.path.join(concrete_dir, dir),))
      continue
    outdir = os.path.join(patch_dir, benchmark, subject, bug_id)
    if "vars" not in meta:
      continue
    vars = meta["vars"]
    if "buggy" in meta:
      buggy_dir = os.path.join(concrete_dir, "buggy")
      os.system(f"cp /root/projects/CPR/lib/uni_klee_runtime.c {buggy_dir}")
      os.system(f"cp /root/projects/CPR/lib/uni_klee_runtime.h {buggy_dir}")
      concrete = formula_to_code(meta["buggy"]["code"], [], vars)
      print(f"Bug: {bug_id}")
      print(concrete)
      apply_patch_to_file(buggy_dir, concrete[0])
      compile(buggy_dir)
      if opt == "buggy":
        continue
    patch_file = os.path.join(outdir, "results", "output", "patch-set-ranked")
    if not os.path.exists(patch_file):
      print(f"Patch file does not exist: {patch_file}")
      continue
    with open(patch_file, "r") as f:
      lines = f.readlines()
    patch_num = 0
    patch_list = list()
    pattern = r'^L?\d+'
    patch = None
    partition_indent = 0
    partition = None
    for line in lines:
      indent = len(line) - len(line.lstrip())
      line = line.strip()
      if line.startswith("Patch #"):
        patch_num = int(line.split("#")[1])
        patch = dict()
        patch_list.append(patch)
        patch["num"] = patch_num
      elif bool(re.search(pattern, line)):
        patch["patch"] = line.split(":")[1].strip()
        patch["lid"] = line.split(":")[0].strip()
      elif line.startswith("Partition: "):
        partition_indent = indent
        if "Partition" not in patch:
          patch["Partition"] = list()
        partition = dict()
        patch["Partition"].append(partition)
        partition["id"] = int(line.split(":")[1].strip())
      elif indent > partition_indent:
        key, val = line.split(":")
        partition[key.strip()] = val.strip()
      else:
        key, val = line.split(":")
        patch[key.strip()] = val.strip()
    with open(f"{outdir}/abs-patches.json", "w") as f:
      print(f"Writing to {outdir}/abs-patches.json")
      json.dump(patch_list, f, indent=2)
    final_patch_list = list()
    for patch in patch_list:
      concrete_patches = to_concrete_patch(patch, meta)
      final_patch_list.append(concrete_patches)
    with open(f"{outdir}/concrete-patches.json", "w") as f:
      print(f"Writing to {outdir}/concrete-patches.json")
      json.dump(final_patch_list, f, indent=2)
    save_to_file(concrete_dir, final_patch_list)
  if opt == "compile":
    pool.close()
    pool.join()

if __name__ == "__main__":
  main(sys.argv)