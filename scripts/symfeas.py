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

def run_fuzzer(subject_name: str):
    # Find subject
    subject = None
    for sub in uni_klee.global_config.get_meta_data_list():
        if subject_name.lower() in sub["bug_id"].lower():
            subject = sub
            break
    conf = uni_klee.global_config.get_meta_data_info_by_id(subject["id"])["conf"]
    # Run fuzzer
    subject_dir = os.path.join(ROOT_DIR, "patches", subject["benchmark"], subject["subject"], subject["bug_id"])
    runtime_dir = os.path.join(subject_dir, "runtime")
    out_no = find_num(runtime_dir, "aflrun-out")
    out_dir = os.path.join(runtime_dir, f"aflrun-out-{out_no}")
    
    

def main():
    parser = argparse.ArgumentParser(description="Symbolic Input Feasibility Analysis")
    parser.add_argument("cmd", help="Command to run", choices=["fuzz", "check"])
    parser.add_argument("-i", "--input", help="Input file", default="")
    parser.add_argument("-o", "--output", help="Output file", default="")
    parser.add_argument("-s", "--subject", help="Subject", default="")
    args = parser.parse_args(sys.argv[1:])
    if args.cmd == "fuzz":
        run_fuzzer(args.subject)
    elif args.cmd == "check":
        parse_smt2_file(args.input)
    

if __name__ == "__main__":
    main()