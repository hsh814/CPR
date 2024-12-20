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

import pysmt.environment
from pysmt.shortcuts import Symbol, BVType, ArrayType, And, BV, Select, BVConcat, BVULT, Bool, Not, Ite, Equals
from pysmt.smtlib.parser import SmtLibParser
from pysmt.typing import BV32, BV8, BV64

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def parse_smt2_file(file_path: str):
    pysmt.environment.push_env()
    parser = SmtLibParser()
    with open(file_path, "r") as f:
        script = parser.get_script(f)
    formulae = script.get_last_formula()
    symbols = script.get_declared_symbols()
    print("Formulae: ", formulae)
    print("Symbols: ", symbols)
    
def parse_mem_result_file(file_path: str) -> sbsv.parser:
    parser = sbsv.parser()
    parser.add_schema("[mem] [index: int] [u-addr: int] [a-addr: int]")
    parser.add_schema("[heap-check] [error] [no-mapping] [u-addr: int] [u-value: int]")
    parser.add_schema("[heap-check] [error] [value-mismatch] [u-addr: int] [u-value: int] [a-addr: int] [a-value: int]")
    parser.add_schema("[heap-check] [ok] [u-addr: int] [u-value: int] [a-addr: int] [a-value: int]")
    parser.add_schema("[val] [arg] [index: int] [value: int] [size: int] [name: str]")
    parser.add_schema("[val] [error] [no-mapping] [u-addr: int] [name: str]")
    parser.add_schema("[mem] [error] [null-pointer] [addr: int] [name: str]")
    parser.add_schema("[val] [heap] [u-addr: int] [name: str] [value: int] [size: int]")
    parser.add_schema("[val] [obj] [u-addr: int] [name: str] [value: str] [size: int]")
    with open(file_path, "r") as f:
        return parser.load(f)


def find_num(dir: str, name: str) -> int:
    result = 0
    dirs = set(os.listdir(dir))
    while True:
        if f"{name}-{result}" in dirs:
            result += 1
        else:
            break
    return result

def read_config_file(file_path: str) -> Dict[str, str]:
    result = dict()
    if os.path.exists(file_path):
        with open(file_path, "r") as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith("#"):
                    continue
                parts = line.split("=")
                if len(parts) == 2:
                    result[parts[0].strip()] = parts[1].strip()
    return result

def get_metadata(subject_name: str) -> dict:
    subject = None
    for sub in uni_klee.global_config.get_meta_data_list():
        if subject_name.lower() in sub["bug_id"].lower():
            subject = sub
            break
    return subject

def run_fuzzer(subject_name: str, debug: bool = False):
    # Find subject
    subject = get_metadata(subject_name)
    conf = uni_klee.global_config.get_meta_data_info_by_id(subject["id"])["conf"]
    # Run fuzzer
    subject_dir = os.path.join(ROOT_DIR, "patches", subject["benchmark"], subject["subject"], subject["bug_id"])
    config_for_fuzz = read_config_file(os.path.join(subject_dir, "config"))
    if len(config_for_fuzz) > 0:
        conf["test_input_list"] = config_for_fuzz["cmd"].replace("<exploit>", "$POC")
        conf["poc_path"] = os.path.basename(config_for_fuzz["exploit"])
    print(conf)
    runtime_dir = os.path.join(subject_dir, "runtime")
    out_no = find_num(runtime_dir, "aflrun-out")
    out_dir = os.path.join(runtime_dir, f"aflrun-out-{out_no}")
    in_dir = os.path.join(runtime_dir, "in")
    if os.path.exists(in_dir):
        os.system(f"rm -rf {in_dir}")
    os.makedirs(in_dir)
    os.system(f"cp {os.path.join(subject_dir, conf['poc_path'])} {in_dir}/")
    env = os.environ.copy()
    env["AFL_NO_UI"] = "1"
    bin = os.path.basename(conf["binary_path"])
    opts = conf["test_input_list"].replace("$POC", "@@")
    cmd = f"timeout 12h /root/projects/AFLRun/afl-fuzz -C -i ./in -o {out_dir} -m none -t 2000ms -- ./{bin}.aflrun {opts}"
    print(f"Running fuzzer: {cmd}")
    stdout = sys.stdout if debug else subprocess.DEVNULL
    stderr = sys.stderr # if debug else subprocess.DEVNULL
    proc = subprocess.run(cmd, shell=True, cwd=runtime_dir, env=env, stdout=stdout, stderr=stderr)
    if proc.returncode != 0:
        print(f"Fuzzer failed {proc.stderr}")
    print("Fuzzer finished")
    collect_val_runtime(subject_dir, out_dir)


def collect_val_runtime(subject_dir: str, out_dir: str):
    print(f"Collecting val runtime from {out_dir}")
    # Collect results in val-runtime
    val_dir = os.path.join(subject_dir, "val-runtime")
    conc_inputs_dir = val_dir + "/concrete-inputs"
    if os.path.exists(conc_inputs_dir):
        os.system(f"rm -rf {conc_inputs_dir}")
    os.makedirs(conc_inputs_dir, exist_ok=True)
    # Copy from crashes
    # for seed in os.listdir(f"{out_dir}/default/crashes"):
    #     if seed == "README.txt":
    #         continue
    os.system(f"rsync -az {out_dir}/default/crashes/ {conc_inputs_dir}/")
    # Copy from queue
    # for seed in os.listdir(f"{out_dir}/default/queue"):
    #     if os.path.isdir(f"{out_dir}/default/queue/{seed}"):
    #         continue
    os.system(f"rsync -az {out_dir}/default/queue/ {conc_inputs_dir}/")
    

def main():
    parser = argparse.ArgumentParser(description="Symbolic Input Feasibility Analysis")
    parser.add_argument("cmd", help="Command to run", choices=["fuzz", "check", "fuzz-build", "val-build", "build", "collect-inputs"])
    parser.add_argument("subject", help="Subject to run", default="")
    parser.add_argument("-i", "--input", help="Input file", default="")
    parser.add_argument("-o", "--output", help="Output file", default="")
    parser.add_argument("-d", "--debug", help="Debug mode", action="store_true")
    # parser.add_argument("-s", "--subject", help="Subject", default="")
    args = parser.parse_args(sys.argv[1:])
    if args.cmd == "fuzz":
        run_fuzzer(args.subject, args.debug)
    elif args.cmd == "check":
        parse_smt2_file(args.input)
    elif args.cmd == "fuzz-build":
        subject = get_metadata(args.subject)
        subject_dir = os.path.join(ROOT_DIR, "patches", subject["benchmark"], subject["subject"], subject["bug_id"])
        subprocess.run(f"./aflrun.sh", cwd=subject_dir, shell=True)
    elif args.cmd == "val-build":
        subject = get_metadata(args.subject)
        subject_dir = os.path.join(ROOT_DIR, "patches", subject["benchmark"], subject["subject"], subject["bug_id"])
        subprocess.run(f"./val.sh", cwd=subject_dir, shell=True)
    elif args.cmd == "build":
        subject = get_metadata(args.subject)
        subject_dir = os.path.join(ROOT_DIR, "patches", subject["benchmark"], subject["subject"], subject["bug_id"])
        subprocess.run(f"./init.sh", cwd=subject_dir, shell=True)
    elif args.cmd == "collect-inputs":
        subject = get_metadata(args.subject)
        subject_dir = os.path.join(ROOT_DIR, "patches", subject["benchmark"], subject["subject"], subject["bug_id"])
        out_no = find_num(os.path.join(subject_dir, "runtime"), "aflrun-out")
        collect_val_runtime(subject_dir, os.path.join(subject_dir, "runtime", f"aflrun-out-{out_no - 1}"))
    

if __name__ == "__main__":
    main()