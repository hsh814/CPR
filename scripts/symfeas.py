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
from pysmt.shortcuts import Symbol, BVType, ArrayType, And, BV, Select, BVConcat, BVULT, Bool, Not, Ite, Equals, is_sat
from pysmt.smtlib.parser import SmtLibParser
from pysmt.smtlib.script import SmtLibScript
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
    
def parse_mem_result_file(file_path: str) -> dict:
    parser = sbsv.parser()
    parser.add_schema("[mem] [index: int] [u-addr: int] [a-addr: int]")
    parser.add_schema("[heap-check] [error] [no-mapping] [u-addr: int] [u-value: int]")
    parser.add_schema("[heap-check] [error] [value-mismatch] [u-addr: int] [u-value: int] [a-addr: int] [a-value: int]")
    parser.add_schema("[heap-check] [ok] [u-addr: int] [u-value: int] [a-addr: int] [a-value: int]")
    parser.add_schema("[val] [arg] [index: int] [value: str] [size: int] [name: str] [num: int]")
    parser.add_schema("[val] [error] [no-mapping] [u-addr: int] [name: str]")
    parser.add_schema("[val] [error] [null-pointer] [addr: int] [name: str]")
    parser.add_schema("[val] [heap] [u-addr: int] [name: str] [value: str] [size: int] [num: int]")
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

def get_conf(subject: dict, subject_dir: str) -> Dict[str, str]:
    conf = uni_klee.global_config.get_meta_data_info_by_id(subject["id"])["conf"]
    config_for_fuzz = read_config_file(os.path.join(subject_dir, "config"))
    if len(config_for_fuzz) > 0:
        conf["test_input_list"] = config_for_fuzz["cmd"].replace("<exploit>", "$POC")
        conf["poc_path"] = os.path.basename(config_for_fuzz["exploit"])
    return conf

def run_fuzzer(subject: dict, subject_dir: str, debug: bool = False):
    # Find subject
    conf = get_conf(subject, subject_dir)
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
    os.system(f"rsync -az {out_dir}/default/crashes/ {conc_inputs_dir}/")
    # Copy from queue
    os.system(f"rsync -az {out_dir}/default/queue/ {conc_inputs_dir}/")
    
def clear_val(dir: str):
    files = os.listdir(dir)
    for f in files:
        file = os.path.join(dir, f)
        if f.startswith("core.") and os.path.isfile(file):
            os.remove(file)

def run_val(subject: dict, subject_dir: str, uni_klee_prefix: str, debug: bool = False):
    conf = get_conf(subject, subject_dir)
    val_runtime = os.path.join(subject_dir, "val-runtime")
    val_bin = os.path.join(val_runtime, os.path.basename(conf["binary_path"]))
    val_out_no = find_num(val_runtime, "val-out")
    val_out_dir = os.path.join(val_runtime, f"val-out-{val_out_no}")
    
    out_no = find_num(os.path.join(subject_dir, "patched"), uni_klee_prefix)
    out_dir = os.path.join(subject_dir, "patched", f"{uni_klee_prefix}-{out_no - 1}")
    symin_cluster_json = os.path.join(out_dir, "symin-cluster.json")
    if not os.path.exists(symin_cluster_json):
        print(f"symin-cluster.json not found in {out_dir}")
        return
    with open(symin_cluster_json, "r") as f:
        cluster = json.load(f)
    
    os.makedirs(val_out_dir, exist_ok=True)
    
    with open(os.path.join(val_out_dir, "val.json"), "w") as f:
        save_obj = dict()
        save_obj["uni_klee_out_dir"] = out_dir
        save_obj["val_out_dir"] = val_out_dir
        save_obj["cluster"] = cluster
        json.dump(save_obj, f, indent=2)
        
    conc_inputs_dir = os.path.join(val_runtime, "concrete-inputs")
    cinputs = os.listdir(conc_inputs_dir)
    tmp_inputs_dir = os.path.join(val_runtime, "inputs")
    os.makedirs(tmp_inputs_dir, exist_ok=True)
    
    for i, c in enumerate(cluster):
        print(f"Processing cluster {i}")
        file = c["file"]
        nodes = c["nodes"]
        group_out_dir = os.path.join(val_out_dir, f"group-{i}")
        os.makedirs(group_out_dir, exist_ok=True)
        env = os.environ.copy()
        env["UNI_KLEE_MEM_BASE_FILE"] = os.path.join(out_dir, "base-mem.graph")
        env["UNI_KLEE_MEM_FILE"] = file
        for cid, cinput in enumerate(cinputs):
            original_file = os.path.join(conc_inputs_dir, cinput)
            if os.path.isdir(original_file):
                continue
            if cinput == "README.txt":
                continue
            c_file = os.path.join(tmp_inputs_dir, f"val{cid}")
            if os.path.exists(c_file):
                os.unlink(c_file)
            os.link(original_file, c_file)
            env_local = env.copy()
            local_out_file = os.path.join(group_out_dir, f"val-{cid}.txt")
            env_local["UNI_KLEE_MEM_RESULT"] = local_out_file
            env_str = f"UNI_KLEE_MEM_BASE_FILE={env['UNI_KLEE_MEM_BASE_FILE']} UNI_KLEE_MEM_FILE={env['UNI_KLEE_MEM_FILE']} UNI_KLEE_MEM_RESULT={local_out_file}"
            if "test_input_list" in conf:
                use_stdin = conf["test_input_list"].find("$POC") == -1
                target_cmd = conf["test_input_list"].replace("$POC", c_file)
                if use_stdin:
                    target_cmd = f"cat {c_file} | {val_bin} {target_cmd}"
                else:
                    target_cmd = f"{val_bin} {target_cmd}"
            else:
                target_cmd = val_bin
            try:
                subprocess.run(target_cmd, shell=True, env=env_local, cwd=val_runtime, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=10)
            except subprocess.TimeoutExpired:
                print(f"Timeout for {target_cmd}")
                continue
            except Exception as e:
                print(f"Error for {target_cmd}: {e}")
                continue
            print(f"Finished {cid}: {env_str} {target_cmd}")
            if os.path.exists(local_out_file):
                with open(local_out_file, "a") as f:
                    f.write(f"[input] [id {cid}] [symgroup {i}] [file {cinput}]")
        clear_val(val_runtime)


def load_smt_file(file_path: str) -> Tuple[SmtLibScript, pysmt.environment.Environment]:
    pysmt.environment.push_env()
    cur_env = pysmt.environment.get_env()
    parser = SmtLibParser(cur_env)
    with open(file_path, "r") as f:
        script = parser.get_script(f)
    return script, cur_env

def get_var_from_script(script: SmtLibScript, name: str, cur_env: pysmt.environment.Environment):
    # Access the parser's cache which contains declared variables
    existing_symbol = None
    for s in script.get_declared_symbols():
        if s.symbol_name() == name:
            existing_symbol = s
            break
    return existing_symbol


def get_bv_const(cur_env: pysmt.environment.Environment, value: str, size: int):
    bytes_data = bytearray.fromhex(value)
    int_val = int.from_bytes(bytes_data, byteorder="little")
    bv = cur_env.formula_manager.BV(int_val, size * 8)
    return bv


def read_val_out_file(file_path: str, script: SmtLibScript, cur_env: pysmt.environment.Environment):
    result = parse_mem_result_file(file_path)
    if result is None:
        return None
    if len(result["heap-check"]["error"]["no-mapping"]) > 0 or len(result["val"]["error"]["no-mapping"]) > 0:
        print("Memory mismatch")
        return None
    if len(result["val"]["error"]["null-pointer"]) > 0:
        print("Null pointer")
        return None
    if len(result["heap-check"]["error"]["value-mismatch"]) > 0:
        print("Value mismatch")
        return None
    formula = script.get_last_formula()
    for heap in result["val"]["heap"]:
        addr = heap["u-addr"]
        name = heap["name"]
        value = heap["value"]
        size = heap["size"]
        num = heap["num"]
        print(f"Heap: {addr} {name} {value} {size} {num}")
        # Build the bitvector formula
        # value: hex string little endian
        # size: number of bytes
        # num: number of elements
        # Get variable already declared in script
        val = get_bv_const(cur_env, value, size)
        var = get_var_from_script(script, name, cur_env)
        if var is None:
            print(f"Variable {name} not found")
            continue
        bv = None
        for i in range(size):
            index_bv = cur_env.formula_manager.BV(i, 32)
            byte_bv = cur_env.formula_manager.Select(var, index_bv)
            if bv is None:
                bv = byte_bv
            else:
                bv = cur_env.formula_manager.BVConcat(byte_bv, bv)
        # Add constraint
        eq = Equals(bv, val)
        formula = And(formula, eq)
    for arg in result["val"]["arg"]:
        index = arg["index"]
        value = arg["value"]
        size = arg["size"]
        name = arg["name"]
        print(f"Arg: {index} {value} {size} {name}")
        val = get_bv_const(cur_env, value, size)
        var = get_var_from_script(script, name, cur_env)
        if var is None:
            print(f"Variable {name} not found")
            continue
        bv = None
        for i in range(size):
            index_bv = cur_env.formula_manager.BV(i, 32)
            byte_bv = cur_env.formula_manager.Select(var, index_bv)
            if bv is None:
                bv = byte_bv
            else:
                bv = cur_env.formula_manager.BVConcat(byte_bv, bv)
        eq = cur_env.formula_manager.Equals(bv, val)
        formula = cur_env.formula_manager.And(formula, eq)
    if is_sat(formula):
        print("SAT")
        return "SAT"
    else:
        print("UNSAT")
        return "UNSAT"
        
        
def parse_val_results(val_out_dir: str):
    if not os.path.exists(val_out_dir) or not os.path.exists(os.path.join(val_out_dir, "val.json")):
        print(f"Val out dir or {val_out_dir}/val.json not found")
        return
    with open(os.path.join(val_out_dir, "val.json"), "r") as f:
        result = json.load(f)
    uni_klee_out_dir = result["uni_klee_out_dir"]
    val_out_dir = result["val_out_dir"]
    cluster = result["cluster"]
    result = dict()
    result["uni_klee_out_dir"] = uni_klee_out_dir
    result["val_out_dir"] = val_out_dir
    result["val"] = list()
    for i, c in enumerate(cluster):
        print(f"Processing cluster {i}")
        nodes = c["nodes"]
        group_out_dir = os.path.join(val_out_dir, f"group-{i}")
        vals = os.listdir(group_out_dir)
        group_result = dict()
        group_result["group_id"] = i
        group_result["nodes"] = nodes
        group_result["nodes_result"] = list()
        result["val"].append(group_result)
        for node in nodes:
            node_file = os.path.join(uni_klee_out_dir, f"test{node:06d}.smt2")
            node_result = dict()
            node_result["node_id"] = node
            node_result["result"] = list()
            group_result["nodes_result"].append(node_result)
            succ = False
            for val in vals:
                script, cur_env = load_smt_file(node_file)
                local_out_file = os.path.join(group_out_dir, val)
                res = read_val_out_file(local_out_file, script, cur_env)
                pysmt.environment.pop_env()
                if res is not None:
                    node_result["result"].append({"val_file": val, "result": res})
                    if res == "SAT":
                        succ = True
                        break
                else:
                    node_result["result"].append({"val_file": val, "result": "ERROR"})
            node_result["success"] = succ
        print(f"Finished cluster {i}")
    with open(os.path.join(val_out_dir, "result.json"), "w") as f:
        json.dump(result, f, indent=2)

def main():
    parser = argparse.ArgumentParser(description="Symbolic Input Feasibility Analysis")
    parser.add_argument("cmd", help="Command to run", choices=["fuzz", "check", "fuzz-build", "val-build", "build", "collect-inputs", "val", "feas"])
    parser.add_argument("subject", help="Subject to run", default="")
    parser.add_argument("-i", "--input", help="Input file", default="")
    parser.add_argument("-o", "--output", help="Output file", default="")
    parser.add_argument("-d", "--debug", help="Debug mode", action="store_true")
    parser.add_argument("-u", "--uni-klee-prefix", help="UniKlee prefix", default="uni-m-out")
    # parser.add_argument("-s", "--subject", help="Subject", default="")
    args = parser.parse_args(sys.argv[1:])
    subject = get_metadata(args.subject)
    subject_dir = os.path.join(ROOT_DIR, "patches", subject["benchmark"], subject["subject"], subject["bug_id"])
    if args.cmd == "fuzz":
        run_fuzzer(subject, subject_dir, args.debug)
    elif args.cmd == "check":
        parse_smt2_file(args.input)
    elif args.cmd == "fuzz-build":
        subprocess.run(f"./aflrun.sh", cwd=subject_dir, shell=True)
    elif args.cmd == "val-build":
        subprocess.run(f"./val.sh", cwd=subject_dir, shell=True)
    elif args.cmd == "build":
        subprocess.run(f"./init.sh", cwd=subject_dir, shell=True)
    elif args.cmd == "collect-inputs":
        out_no = find_num(os.path.join(subject_dir, "runtime"), "aflrun-out")
        collect_val_runtime(subject_dir, os.path.join(subject_dir, "runtime", f"aflrun-out-{out_no - 1}"))
    elif args.cmd == "val":
        run_val(subject, subject_dir, args.uni_klee_prefix, args.debug)
    elif args.cmd == "feas":
        val_dir = os.path.join(subject_dir, "val-runtime")
        no = find_num(val_dir, "val-out")
        val_out_dir = os.path.join(val_dir, f"val-out-{no - 1}")
        parse_val_results(val_out_dir)

if __name__ == "__main__":
    main()