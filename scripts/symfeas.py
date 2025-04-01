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

import enum

import pysmt.environment
from pysmt.shortcuts import Symbol, BVType, ArrayType, BV, Select, BVConcat, BVULT, Bool, Ite, And, Or, Symbol, Equals, Not, LE, LT, GE, GT, Int, is_sat
import pysmt.shortcuts as smt
from pysmt.smtlib.parser import SmtLibParser
from pysmt.smtlib.script import SmtLibScript
from pysmt.typing import BV32, BV8, BV64, INT

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def print_log(msg: str):
    print(msg, file=sys.stderr)

def print_out(msg: str):
    print(msg, file=sys.stdout)

def parse_smt2_file(file_path: str):
    pysmt.environment.push_env()
    parser = SmtLibParser()
    with open(file_path, "r") as f:
        script = parser.get_script(f)
    formulae = script.get_last_formula()
    symbols = script.get_declared_symbols()
    print_log("Formulae: ", formulae)
    print_log("Symbols: ", symbols)
    
def parse_mem_result_file(file_path: str) -> sbsv.parser:
    parser = sbsv.parser()
    parser.add_schema("[mem] [index: int] [u-addr: int] [a-addr: int]")
    parser.add_schema("[heap-check] [error] [no-mapping] [u-addr: int] [u-value: int]")
    parser.add_schema("[heap-check] [error] [value-mismatch] [u-addr: int] [u-value: int] [a-addr: int] [a-value: int]")
    parser.add_schema("[heap-check] [ok] [u-addr: int] [u-value: int] [a-addr: int] [a-value: int]")
    parser.add_schema("[heap-check] [begin]")
    parser.add_schema("[heap-check] [end]")
    parser.add_schema("[val] [arg] [index: int] [value: str] [size: int] [name: str] [num: int]")
    parser.add_schema("[val] [error] [no-mapping] [u-addr: int] [name: str]")
    parser.add_schema("[val] [error] [null-pointer] [addr: int] [name: str]")
    parser.add_schema("[val] [heap] [u-addr: int] [name: str] [value: str] [size: int] [num: int]")
    parser.add_schema("[global] [sym: str] [value: str]")
    parser.add_group("heap-check", "heap-check$begin", "heap-check$end")
    with open(file_path, "r") as f:
        parser.load(f)
        return parser


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


def run_fuzzer_multi(subject: dict, subject_dir: str, debug: bool = False):
    # Find subject
    conf = get_conf(subject, subject_dir)
    print_log(conf)
    runtime_dir = os.path.join(subject_dir, "runtime")
    out_no = find_num(runtime_dir, "aflrun-multi-out")
    out_dir = os.path.join(runtime_dir, f"aflrun-multi-out-{out_no}")

    in_dir = in_dir = os.path.join(subject_dir, "seed")
    if not os.path.exists(in_dir): # Single seed
        in_dir = os.path.join(runtime_dir, "in")
        if os.path.exists(in_dir):
            os.system(f"rm -rf {in_dir}")
        os.makedirs(in_dir)
        os.system(f"cp {os.path.join(subject_dir, conf['poc_path'])} {in_dir}/")
    
    env = os.environ.copy()
    env["AFL_NO_UI"] = "1"
    bin = os.path.basename(conf["binary_path"])
    opts = conf["test_input_list"].replace("$POC", "@@")
    cmd = f"timeout 24h /root/projects/AFLRun/afl-fuzz -C -i {in_dir} -o {out_dir} -m none -t 2000ms -- ./{bin}.aflrun {opts}"
    print_log(f"Running fuzzer: {cmd}")
    stdout = sys.stdout if debug else subprocess.DEVNULL
    stderr = sys.stderr # if debug else subprocess.DEVNULL
    proc = subprocess.run(cmd, shell=True, cwd=runtime_dir, env=env, stdout=stdout, stderr=stderr)
    if proc.returncode != 0:
        print_log(f"Fuzzer failed {proc.stderr}")
    print_log("Fuzzer finished")
    collect_val_runtime(subject_dir, out_dir)


def run_fuzzer(subject: dict, subject_dir: str, debug: bool = False):
    # Find subject
    conf = get_conf(subject, subject_dir)
    print_log(conf)
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
    cmd = f"timeout 12h /root/projects/AFLRun/afl-fuzz -C -i {in_dir} -o {out_dir} -m none -t 2000ms -- ./{bin}.aflrun {opts}"
    print_log(f"Running fuzzer: {cmd}")
    stdout = sys.stdout if debug else subprocess.DEVNULL
    stderr = sys.stderr # if debug else subprocess.DEVNULL
    proc = subprocess.run(cmd, shell=True, cwd=runtime_dir, env=env, stdout=stdout, stderr=stderr)
    if proc.returncode != 0:
        print_log(f"Fuzzer failed {proc.stderr}")
    print_log("Fuzzer finished")
    collect_val_runtime(subject_dir, out_dir)


def collect_val_runtime(subject_dir: str, out_dir: str):
    print_log(f"Collecting val runtime from {out_dir}")
    # Collect results in val-runtime
    conc_inputs_dir = os.path.join(subject_dir, "concrete-inputs")
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

def run_val(subject: dict, subject_dir: str, symvass_prefix: str, val_prefix: str, debug: bool = False):
    conf = get_conf(subject, subject_dir)
    val_runtime = os.path.join(subject_dir, "val-runtime")
    val_bin = os.path.join(val_runtime, os.path.basename(conf["binary_path"]))
    val_out_no = find_num(val_runtime, val_prefix)
    val_out_dir = os.path.join(val_runtime, f"{val_prefix}-{val_out_no}")
    
    out_no = find_num(os.path.join(subject_dir, "patched"), symvass_prefix)
    out_dir = os.path.join(subject_dir, "patched", f"{symvass_prefix}-{out_no - 1}")
    if os.path.exists(os.path.join(out_dir, "base-mem.symbolic-globals")):
        with open(os.path.join(out_dir, "base-mem.symbolic-globals")) as f:
            globals = f.readlines()
            if len(globals) > 1:
                # with open("/root/projects/CPR/out/out.txt", "a") as f:
                #     f.write(f"{subject_dir} {len(globals)}\n")
                pass
    symin_cluster_json = os.path.join(out_dir, "symin-cluster.json")
    if not os.path.exists(symin_cluster_json):
        print_log(f"symin-cluster.json not found in {out_dir}")
        return
    with open(symin_cluster_json, "r") as f:
        data = json.load(f)
    
    os.makedirs(val_out_dir, exist_ok=True)
    cluster = data["mem_cluster"]
    
    with open(os.path.join(val_out_dir, "val.json"), "w") as f:
        save_obj = dict()
        save_obj["uni_klee_out_dir"] = out_dir
        save_obj["val_out_dir"] = val_out_dir
        save_obj["cluster"] = cluster
        json.dump(save_obj, f, indent=2)
        
    conc_inputs_dir = os.path.join(subject_dir, "concrete-inputs")
    if val_prefix == "cludafl-queue":
        conc_inputs_dir = os.path.join(val_runtime, "..", "runtime", "cludafl-queue", "queue")
    elif val_prefix == "cludafl-memory":
        conc_inputs_dir = os.path.join(val_runtime, "..", "runtime", "cludafl-memory", "input")
    print_log(f"Conc inputs dir: {conc_inputs_dir}")
    cinputs = os.listdir(conc_inputs_dir)
    tmp_inputs_dir = os.path.join(val_out_dir, "inputs")
    os.makedirs(tmp_inputs_dir, exist_ok=True)
    
    for i, c in enumerate(cluster):
        print_log(f"Processing cluster {i}")
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
                print_log(f"Timeout for {target_cmd}")
                continue
            except Exception as e:
                print_log(f"Error for {target_cmd}: {e}")
                continue
            print_log(f"Finished {cid}: {env_str} {target_cmd}")
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


def read_val_out_file(parser: sbsv.parser, globals_list: List[Dict[str, str]], iter: int, index: Tuple[int, int], script: SmtLibScript, cur_env: pysmt.environment.Environment):
    # Check error
    if len(parser.get_result_by_index("heap-check$error$no-mapping", index)) > 0: #  or len(result["val"]["error"]["no-mapping"]) > 0
        print_log("Memory mismatch")
        return "MEM_MISMATCH"
    if len(parser.get_result_by_index("val$error$null-pointer", index)) > 0:
        print_log("Null pointer")
        return "NULL_PTR"
    if len(parser.get_result_by_index("heap-check$error$value-mismatch", index)) > 0:
        print_log("Value mismatch")
        return "VAL_MISMATCH"
    
    # Read values and check satisfiability
    formula = script.get_last_formula()
    for heap in parser.get_result_by_index("val$heap", index):
        addr = heap["u-addr"]
        name = heap["name"]
        value = heap["value"]
        size = heap["size"]
        num = heap["num"]
        print_log(f"Heap: {addr} {name} {value} {size} {num}")
        # Build the bitvector formula
        # value: hex string little endian
        # size: number of bytes
        # num: number of elements
        # Get variable already declared in script
        val = get_bv_const(cur_env, value, size)
        var = get_var_from_script(script, name, cur_env)
        if var is None:
            print_log(f"Variable {name} not found")
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
    
    for arg in parser.get_result_by_index("val$arg", index):
        index = arg["index"]
        value = arg["value"]
        size = arg["size"]
        name = arg["name"]
        print_log(f"Arg: {index} {value} {size} {name}")
        val = get_bv_const(cur_env, value, size)
        var = get_var_from_script(script, name, cur_env)
        if var is None:
            print_log(f"Variable {name} not found")
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
    
    if len(globals_list) > 0:
        for name, value in globals_list[iter].items():
            var = get_var_from_script(script, name, cur_env)
            if var is None:
                print_log(f"Variable {name} not found")
                continue
            size = len(value) // 2
            val = get_bv_const(cur_env, value, size)
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
        print_log("SAT")
        return "SAT"
    else:
        print_log("UNSAT")
        return "UNSAT"

def parse_symvass_result(file_path: str) -> Dict[int, List[int]]:
    result = dict()
    parser = sbsv.parser()
    parser.add_schema("[sym-in] [id: int] [base: int] [test: int] [cnt: int] [patches: str]")
    parser.add_schema("[sym-out] [best] [cnt: int] [patches: str]")
    parser.add_schema("[meta-data] [correct: int] [all-patches: int] [sym-input: int] [correct-input: int]")
    with open(file_path, "r") as f:
        parser.load(f)
    for sym_in in parser.get_result()["sym-in"]:
        test = sym_in["test"]
        patches = sym_in["patches"]
        result[test] = eval(patches)
    result[-1] = list(range(parser.get_result()["meta-data"][0]["all-patches"]))
    return result
        
def parse_val_results(val_out_dir: str):
    if not os.path.exists(val_out_dir) or not os.path.exists(os.path.join(val_out_dir, "val.json")):
        print_log(f"Val out dir or {val_out_dir}/val.json not found")
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
    rf = open(os.path.join(val_out_dir, "result.sbsv"), "w")
    remaining_base_states = list()
    result["remaining_patches"] = list()
    symvass_result = parse_symvass_result(os.path.join(uni_klee_out_dir, "table.sbsv"))
    for i, c in enumerate(cluster):
        print_log(f"Processing cluster {i}")
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
                local_out_file = os.path.join(group_out_dir, val)
                parser = parse_mem_result_file(local_out_file)
                indices = parser.get_group_index("heap-check")
                if len(indices) == 0:
                    print_log(f"No heap-check in {local_out_file}")
                    node_result["result"].append({"val_file": val, "result": "NOT_DONE"})
                    rf.write(f"[res] [c {i}] [n {node}] [res NOT_DONE] [val {val}]\n")
                    continue
                globals_list = list()
                globals_map = dict()
                for global_var in parser.get_result()["global"]:
                    name = global_var["sym"]
                    value = global_var["value"]
                    if name in globals_map:
                        globals_list.append(globals_map.copy())
                        globals_map.clear()
                    globals_map[name] = value
                globals_list.append(globals_map)
                if len(indices) > 1:
                    print_log(f"Multiple heap-check ({len(indices)}) in {local_out_file}")
                for iter, index in enumerate(indices):
                    if succ:
                        break
                    script, cur_env = load_smt_file(node_file) # push_env
                    res = read_val_out_file(parser, globals_list, iter, index, script, cur_env)
                    pysmt.environment.pop_env()
                    if res is not None:
                        node_result["result"].append({"val_file": val, "result": res})
                        rf.write(f"[res] [c {i}] [n {node}] [res {res}] [val {val}]\n")
                        if res == "SAT":
                            succ = True
                            break
                    else:
                        node_result["result"].append({"val_file": val, "result": "ERROR"})
                        rf.write(f"[res] [c {i}] [n {node}] [res ERROR] [val {val}]\n")
            node_result["success"] = succ
            if succ:
                rf.write(f"[success] [c {i}] [n {node}]\n")
                remaining_base_states.append(node)
        print_log(f"Finished cluster {i}")
        remaining_patches = set(symvass_result[-1])
        remaining_sym_inputs = list()
        for symin in remaining_base_states:
            if symin in symvass_result:
                remaining_patches = remaining_patches.intersection(set(symvass_result[symin]))
                rf.write(f"[remaining] [input] [input {symin}] [patches {symvass_result[symin]}]\n")
        result["remaining_patches"] = sorted(list(remaining_patches))
        rf.write(f"[remaining] [patch] [patches {result['remaining_patches']}]\n")
    rf.close()
    with open(os.path.join(val_out_dir, "result.json"), "w") as f:
        json.dump(result, f, indent=2)


def analyze(subject: dict, val_out_dir: str, output: str):
    subject_dir = os.path.join(ROOT_DIR, "patches", subject["benchmark"], subject["subject"], subject["bug_id"])
    filter_json = os.path.join(subject_dir, "patched", "filter", "filtered.json")
    if not os.path.exists(filter_json):
        print_log(f"Filter file {filter_json} not found")
        return
    with open(filter_json, "r") as f:
        filter_data = json.load(f)
    filter_result = set(filter_data["remaining"])
    result_file = os.path.join(val_out_dir, "result.sbsv")
    if not os.path.exists(result_file):
        print_log(f"Result file {result_file} not found")
        return
    parser = sbsv.parser()
    parser.add_schema("[res] [c: int] [n: int] [res: str] [val: str]")
    parser.add_schema("[success] [c: int] [n: int]")
    parser.add_schema("[remaining] [input] [input: int] [patches: str]")
    parser.add_schema("[remaining] [patch] [patches: str]")
    with open(result_file, "r") as f:
        result = parser.load(f)
    remaining_input_num = len(result["remaining"]["input"])
    remaining_patches = eval(result["remaining"]["patch"][0]["patches"])
    remaining_patches_filtered = list()
    for patch in remaining_patches:
        if patch in filter_result:
            remaining_patches_filtered.append(patch)

    correct_patch = subject["correct"]["no"]
    found = correct_patch in remaining_patches_filtered
    # val_inputs val_remaining_patches val_filtered val_found
    res = f"{subject['subject']}\t{subject['bug_id']}\t{remaining_input_num}\t{len(remaining_patches)}\t{len(remaining_patches_filtered)}\t{found}"
    if output != "":
        with open(os.path.join(f"{ROOT_DIR}/out", output), "a") as f:
            f.write(res + "\n")
    else:
        print_out(res)

# --- Updated TokenType ---
class TokenType(enum.Enum):
    IDENTIFIER = 'IDENTIFIER'
    NUMBER = 'NUMBER'
    LE = 'LE'          # <=
    GE = 'GE'          # >=
    LT = 'LT'          # <
    GT = 'GT'          # >
    EQ = 'EQ'          # ==
    NE = 'NE'          # !=
    AND = 'AND'        # &&
    OR = 'OR'          # ||
    # NOT = 'NOT'      # ! (optional)
    LPAREN = 'LPAREN'  # (
    RPAREN = 'RPAREN'  # )
    PLUS = 'PLUS'      # +
    MINUS = 'MINUS'    # -
    MUL = 'MUL'        # *
    DIV = 'DIV'        # /
    EOF = 'EOF'        # End of File/Input

class Token:
    def __init__(self, type, value=None, lineno=1, col=1):
        self.type = type
        self.value = value
        self.lineno = lineno
        self.col = col

    def __str__(self):
        return f"Token({self.type.name}, {repr(self.value)}, L{self.lineno} C{self.col})"

class Lexer:
    def __init__(self, text):
        self.text = text
        self.pos = 0
        self.current_char = self.text[self.pos] if self.pos < len(self.text) else None
        self.lineno = 1
        self.col = 1

    def advance(self):
        if self.current_char == '\n':
            self.lineno += 1
            self.col = 0

        self.pos += 1
        if self.pos < len(self.text):
            self.current_char = self.text[self.pos]
            self.col += 1
        else:
            self.current_char = None # EOF

    def peek(self):
        peek_pos = self.pos + 1
        if peek_pos < len(self.text):
            return self.text[peek_pos]
        else:
            return None

    def skip_whitespace(self):
        while self.current_char is not None and self.current_char.isspace():
            self.advance()

    def number(self):
        result = ''
        start_col = self.col
        if self.current_char == '-':
            result += self.current_char
            self.advance()

        if self.current_char is None or not self.current_char.isdigit():
             self._error(f"Expected digit after '-' at L{self.lineno} C{start_col}")

        while self.current_char is not None and self.current_char.isdigit():
            result += self.current_char
            self.advance()
        return Token(TokenType.NUMBER, int(result), self.lineno, start_col)

    def identifier(self):
        result = ''
        start_col = self.col
        while self.current_char is not None and (self.current_char.isalnum() or self.current_char == '_'):
            result += self.current_char
            self.advance()
        return Token(TokenType.IDENTIFIER, result, self.lineno, start_col)

    def _error(self, message="Invalid character"):
        raise ValueError(f"{message}: '{self.current_char}' at L{self.lineno} C{self.col}")

    def get_next_token(self):
        while self.current_char is not None:
            start_col = self.col

            if self.current_char.isspace():
                self.skip_whitespace()
                continue

            # Handle potential negative numbers vs. MINUS operator
            # Check if '-' is followed by a digit - if so, it's part of a number
            if self.current_char == '-' and self.peek() is not None and self.peek().isdigit():
                 # Let the number() method handle it
                 return self.number()
            # If '-' is not followed by a digit, treat it as a MINUS operator
            elif self.current_char == '-':
                 self.advance()
                 return Token(TokenType.MINUS, '-', self.lineno, start_col)

            if self.current_char.isdigit():
                # Must be a positive number
                return self.number()

            if self.current_char.isalpha() or self.current_char == '_':
                return self.identifier()

            # Two-character operators
            if self.current_char == '<':
                if self.peek() == '=':
                    self.advance()
                    self.advance()
                    return Token(TokenType.LE, '<=', self.lineno, start_col)
                else:
                    self.advance()
                    return Token(TokenType.LT, '<', self.lineno, start_col)

            if self.current_char == '>':
                if self.peek() == '=':
                    self.advance()
                    self.advance()
                    return Token(TokenType.GE, '>=', self.lineno, start_col)
                else:
                    self.advance()
                    return Token(TokenType.GT, '>', self.lineno, start_col)

            if self.current_char == '=':
                if self.peek() == '=':
                    self.advance()
                    self.advance()
                    return Token(TokenType.EQ, '==', self.lineno, start_col)
                else:
                    self._error("Expected '=' after '=' for '=='")

            if self.current_char == '!':
                if self.peek() == '=':
                    self.advance()
                    self.advance()
                    return Token(TokenType.NE, '!=', self.lineno, start_col)
                # else: # Optional NOT '!'
                #    self.advance()
                #    return Token(TokenType.NOT, '!', self.lineno, start_col)
                else:
                     self._error("Expected '=' after '!' for '!='")

            if self.current_char == '&':
                if self.peek() == '&':
                    self.advance()
                    self.advance()
                    return Token(TokenType.AND, '&&', self.lineno, start_col)
                else:
                    self._error("Expected '&' after '&' for '&&'")

            if self.current_char == '|':
                if self.peek() == '|':
                    self.advance()
                    self.advance()
                    return Token(TokenType.OR, '||', self.lineno, start_col)
                else:
                    self._error("Expected '|' after '|' for '||'")

            # Single-character tokens
            if self.current_char == '+':
                self.advance()
                return Token(TokenType.PLUS, '+', self.lineno, start_col)
            # MINUS already handled above
            if self.current_char == '*':
                self.advance()
                return Token(TokenType.MUL, '*', self.lineno, start_col)
            if self.current_char == '/':
                self.advance()
                return Token(TokenType.DIV, '/', self.lineno, start_col)
            if self.current_char == '(':
                self.advance()
                return Token(TokenType.LPAREN, '(', self.lineno, start_col)
            if self.current_char == ')':
                self.advance()
                return Token(TokenType.RPAREN, ')', self.lineno, start_col)

            # If none of the above matched
            self._error()

        # End of input
        return Token(TokenType.EOF, None, self.lineno, self.col)

class Parser:
    # (__init__, _get_or_create_variable, _error, eat, get_parsed_variables remain mostly the same)
    def __init__(self, lexer, variable_type=INT):
        self.lexer = lexer
        self.current_token = self.lexer.get_next_token()
        self.variable_type = variable_type
        self.variables = {} # Stores SMT variable objects

    def _get_or_create_variable(self, name):
        if name not in self.variables:
            # Assuming all variables are INT for now, adjust if needed
            self.variables[name] = smt.Symbol(name, self.variable_type)
        return self.variables[name]

    def _error(self, expected_type=None):
        msg = f"Syntax error: Unexpected token {self.current_token}"
        if expected_type:
            msg += f" (expected {expected_type})"
        # Ensure lineno and col are accessed safely, even for EOF token
        lineno = self.current_token.lineno if self.current_token else '?'
        col = self.current_token.col if self.current_token else '?'
        raise SyntaxError(msg + f" at L{lineno} C{col}")


    def eat(self, token_type):
        if self.current_token.type == token_type:
            # print(f"Eating: {self.current_token}") # Debugging
            self.current_token = self.lexer.get_next_token()
        else:
            self._error(expected_type=token_type.name)

    # --- Grammar Rules ---
    # expression   ::= logic_term ( (AND | OR) logic_term )*
    # logic_term   ::= comparison | LPAREN expression RPAREN | (NOT logic_term)
    # comparison   ::= arith_expr ( (LE | GE | LT | GT | EQ | NE) arith_expr )?  <- Made optional to allow single arith_expr? No, comparison requires operator.
    # comparison   ::= arith_expr (LE | GE | LT | GT | EQ | NE) arith_expr
    # arith_expr   ::= term ( (PLUS | MINUS) term )*
    # term         ::= factor ( (MUL | DIV) factor )*
    # factor       ::= NUMBER | IDENTIFIER | LPAREN expression RPAREN  <- Incorrect, should be LPAREN arith_expr RPAREN for arithmetic
    # factor       ::= NUMBER | IDENTIFIER | PLUS factor | MINUS factor | LPAREN expression RPAREN <- Still not quite right for arith precedence
    # Let's redo factor/term/arith_expr more conventionally:
    # factor       ::= NUMBER | IDENTIFIER | LPAREN arith_expr RPAREN | PLUS factor | MINUS factor
    # term         ::= factor ( (MUL | DIV) factor )*
    # arith_expr   ::= term ( (PLUS | MINUS) term )*

    # --- Parsing Methods ---

    # factor ::= NUMBER | IDENTIFIER | PLUS factor | MINUS factor | LPAREN arith_expr RPAREN
    def parse_factor(self):
        token = self.current_token
        if token.type == TokenType.NUMBER:
            self.eat(TokenType.NUMBER)
            return smt.Int(token.value)
        elif token.type == TokenType.IDENTIFIER:
            self.eat(TokenType.IDENTIFIER)
            return self._get_or_create_variable(token.value)
        elif token.type == TokenType.LPAREN:
            self.eat(TokenType.LPAREN)
            # *** Important: Parentheses inside arithmetic should contain arithmetic ***
            node = self.parse_arith_expr() # Changed from parse_expression
            self.eat(TokenType.RPAREN)
            return node
        elif token.type == TokenType.PLUS: # Unary plus
            self.eat(TokenType.PLUS)
            node = self.parse_factor()
            return node # SMT usually doesn't need explicit unary plus
        elif token.type == TokenType.MINUS: # Unary minus
            self.eat(TokenType.MINUS)
            node = self.parse_factor()
            # SMT representation of unary minus: 0 - node or Times(-1, node)
            # Using Times(-1, node) is generally safer for type consistency if dealing with reals
            return smt.Times(smt.Int(-1), node) # Or smt.Minus(smt.Int(0), node)
        else:
            self._error(expected_type="NUMBER, IDENTIFIER, '+', '-', or '('")

    # term ::= factor ( (MUL | DIV) factor )*
    def parse_term_arith(self):
        node = self.parse_factor()
        while self.current_token.type in (TokenType.MUL, TokenType.DIV):
            token = self.current_token
            if token.type == TokenType.MUL:
                self.eat(TokenType.MUL)
                node = smt.Times(node, self.parse_factor())
            elif token.type == TokenType.DIV:
                self.eat(TokenType.DIV)
                right_factor = self.parse_factor()
                node = smt.Times(node, right_factor)
        return node

    # arith_expr ::= term ( (PLUS | MINUS) term )*
    def parse_arith_expr(self):
        node = self.parse_term_arith()
        while self.current_token.type in (TokenType.PLUS, TokenType.MINUS):
            token = self.current_token
            if token.type == TokenType.PLUS:
                self.eat(TokenType.PLUS)
                node = smt.Plus(node, self.parse_term_arith())
            elif token.type == TokenType.MINUS:
                self.eat(TokenType.MINUS)
                node = smt.Minus(node, self.parse_term_arith())
        return node

    # comparison ::= arith_expr (LE | GE | LT | GT | EQ | NE) arith_expr
    def parse_comparison(self):
        # *** Update: Use parse_arith_expr instead of parse_value ***
        left_node = self.parse_arith_expr()

        if self.current_token.type in (TokenType.LE, TokenType.GE, TokenType.LT, TokenType.GT, TokenType.EQ, TokenType.NE):
            op_token = self.current_token
            self.eat(op_token.type)
            # *** Update: Use parse_arith_expr instead of parse_value ***
            right_node = self.parse_arith_expr()

            op_type = op_token.type
            if op_type == TokenType.LE:
                return smt.LE(left_node, right_node)
            elif op_type == TokenType.GE:
                return smt.GE(left_node, right_node)
            elif op_type == TokenType.LT:
                return smt.LT(left_node, right_node)
            elif op_type == TokenType.GT:
                return smt.GT(left_node, right_node)
            elif op_type == TokenType.EQ:
                # Ensure we use smt.Equals for comparisons, not Python's ==
                return smt.Equals(left_node, right_node)
            elif op_type == TokenType.NE:
                return smt.Not(smt.Equals(left_node, right_node))
        else:
             # It's possible a logic term is just a single comparison (handled below)
             # Or maybe just a boolean variable/constant if allowed? Current grammar doesn't show this.
             # If the grammar strictly means comparison *must* have an operator, raise error.
             # If an arith_expr alone *cannot* be a logic_term, then this branch is an error.
             # Based on the `logic_term` rule, if it's not LPAREN, it *must* be a comparison.
             self._error(expected_type="Comparison Operator (<=, >=, <, >, ==, !=)")
             # If we allowed single arith_expr to evaluate to bool (e.g., if x is bool), we'd return left_node here.
             # return left_node

    # logic_term ::= comparison | LPAREN expression RPAREN | (NOT logic_term)
    def parse_logic_term(self): # Renamed from parse_term_logic for clarity
        token = self.current_token
        if token.type == TokenType.LPAREN:
            self.eat(TokenType.LPAREN)
            # *** Parentheses around logical expressions should contain logical expressions ***
            node = self.parse_expression() # Correct: parse_expression inside logic parens
            self.eat(TokenType.RPAREN)
            return node
        # elif token.type == TokenType.NOT: # Optional NOT
        #     self.eat(TokenType.NOT)
        #     node = smt.Not(self.parse_logic_term())
        #     return node
        else:
            # If not parentheses (or NOT), it must be a comparison
            return self.parse_comparison()

    # expression ::= logic_term ( (AND | OR) logic_term )*
    def parse_expression(self):
        node = self.parse_logic_term() # Use the renamed function

        while self.current_token.type in (TokenType.AND, TokenType.OR):
            token = self.current_token
            if token.type == TokenType.AND:
                self.eat(TokenType.AND)
                node = smt.And(node, self.parse_logic_term()) # Use the renamed function
            elif token.type == TokenType.OR:
                self.eat(TokenType.OR)
                node = smt.Or(node, self.parse_logic_term()) # Use the renamed function
        return node

    # --- Main parse method ---
    def parse(self):
        # Reset state if parsing multiple times with the same parser instance
        # self.lexer = Lexer(self.lexer.text) # Pass new text if needed
        # self.current_token = self.lexer.get_next_token()
        self.variables.clear() # Clear tracked variables for a fresh parse

        node = self.parse_expression()
        if self.current_token.type != TokenType.EOF:
            self._error(expected_type="EOF")

        return node

    def get_parsed_variables(self):
        return self.variables

def code_to_formula(code_str):
    substitutions = {}
    result_expr = None
    
    for line in code_str.split(';'):
        line = line.strip()
        if not line: continue
        
        if line.startswith('result'):
            result_expr = line.split('=', 1)[1].strip().rstrip(';')
        elif '=' in line:
            var, value = map(str.strip, line.split('=', 1))
            substitutions[var] = value
    
    if substitutions and result_expr:
        for var, val in substitutions.items():
            result_expr = re.sub(rf'\b{var}\b', val, result_expr)
    print_log(f"Code: {result_expr}")
    if result_expr in ["(0)", "0"]:
        return smt.Bool(False)
    if result_expr in ["(1)", "1"]:
        return smt.Bool(True)
    lexer = Lexer(result_expr)
    parser = Parser(lexer)
    expr = parser.parse()
    return expr

def group_patches(subject_dir: str):
    with open(os.path.join(subject_dir, "meta-program.json"), "r") as f:
        meta = json.load(f)
    patches = list()
    done = set()
    equivalences = dict()
    correct = -1
    for patch in meta["patches"]:
        id = patch["id"]
        if patch["name"] == "correct":
            correct = id
        code = patch["code"]
        formula = code_to_formula(code)
        patches.append((id, formula, formula.get_free_variables()))
        print_log(f"Patch {id}: {formula.serialize()}")
    for i in range(len(patches)):
        id, formula, vars = patches[i]
        if id in done:
            continue
        for j in range(i + 1, len(patches)):
            id2, formula2, vars2 = patches[j]
            if id2 in done:
                continue
            if vars != vars2:
                continue
            equivalence = smt.Iff(formula, formula2)
            if smt.is_valid(equivalence):
                if id not in equivalences:
                    equivalences[id] = list()
                equivalences[id].append(id2)
                done.add(id2)
                print_log(f"Equivalence: {id} == {id2} ({formula.serialize()} == {formula2.serialize()})")
    print_log(f"Equivalences: {equivalences}")
    result = list()
    for id, eqs in equivalences.items():
        data = list()
        data.append(id)
        for eq in eqs:
            data.append(eq)
        result.append(data)
    obj = dict()
    obj["equivalences"] = result
    obj["correct"] = correct
    with open(os.path.join(subject_dir, "concrete", "equivalences.json"), "w") as f:
        json.dump(obj, f, indent=2)
    
def main():
    parser = argparse.ArgumentParser(description="Symbolic Input Feasibility Analysis")
    parser.add_argument("cmd", help="Command to run", choices=["fuzz", "fuzz-seeds", "check", "fuzz-build", "val-build", "build", "collect-inputs", "group-patches", "val", "feas", "analyze"])
    parser.add_argument("subject", help="Subject to run", default="")
    parser.add_argument("-i", "--input", help="Input file", default="")
    parser.add_argument("-o", "--output", help="Output file", default="")
    parser.add_argument("-d", "--debug", help="Debug mode", action="store_true")
    parser.add_argument("-s", "--symvass-prefix", help="SymVass prefix", default="uni-m-out")
    parser.add_argument("-v", "--val-prefix", help="Val prefix", default="")
    parser.add_argument("-p", "--prefix", help="Prefix of fuzzer out: default aflrun-multi-out", default="aflrun-multi-out")
    # parser.add_argument("-s", "--subject", help="Subject", default="")
    args = parser.parse_args(sys.argv[1:])
    subject = get_metadata(args.subject)
    subject_dir = os.path.join(ROOT_DIR, "patches", subject["benchmark"], subject["subject"], subject["bug_id"])
    val_prefix = args.val_prefix if args.val_prefix != "" else args.symvass_prefix
    if args.cmd == "fuzz":
        run_fuzzer(subject, subject_dir, args.debug)
    elif args.cmd == "fuzz-seeds":
        run_fuzzer_multi(subject, subject_dir, args.debug)
    elif args.cmd == "check":
        parse_smt2_file(args.input)
    elif args.cmd == "fuzz-build":
        subprocess.run(f"./aflrun.sh", cwd=subject_dir, shell=True)
    elif args.cmd == "val-build":
        symvass_prefix = args.symvass_prefix
        if symvass_prefix == "":
            print_log(f"SymVass prefix not found (use -s or --symvass-prefix)")
            return
        no = find_num(os.path.join(subject_dir, "patched"), symvass_prefix)
        if no == 0:
            print_log(f"SymVass output not found for {symvass_prefix}: did you run symvass?")
            return
        out_dir = os.path.join(subject_dir, "patched", f"{symvass_prefix}-{no - 1}")
        env = os.environ.copy()
        env["UNI_KLEE_SYMBOLIC_GLOBALS_FILE_OVERRIDE"] = os.path.join(out_dir, "base-mem.symbolic-globals")
        subprocess.run(f"./val.sh", cwd=subject_dir, shell=True, env=env)
    elif args.cmd == "build":
        subprocess.run(f"./init.sh", cwd=subject_dir, shell=True)
    elif args.cmd == "collect-inputs":
        out_no = find_num(os.path.join(subject_dir, "runtime"), "aflrun-out")
        collect_val_runtime(subject_dir, os.path.join(subject_dir, "runtime", f"aflrun-out-{out_no - 1}"))
    elif args.cmd == "group-patches":
        # Group patches
        group_patches(subject_dir)
    elif args.cmd == "val":
        run_val(subject, subject_dir, args.symvass_prefix, val_prefix, args.debug)
    elif args.cmd == "feas":
        val_dir = os.path.join(subject_dir, "val-runtime")
        no = find_num(val_dir, val_prefix)
        val_out_dir = os.path.join(val_dir, f"{val_prefix}-{no - 1}")
        parse_val_results(val_out_dir)
    elif args.cmd == "analyze":
        val_dir = os.path.join(subject_dir, "val-runtime")
        no = find_num(val_dir, val_prefix)
        val_out_dir = os.path.join(val_dir, f"{val_prefix}-{no - 1}")
        analyze(subject, val_out_dir, args.output)

if __name__ == "__main__":
    main()