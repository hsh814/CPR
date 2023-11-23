#!/usr/bin/env python3
from typing import Union, List, Dict, Tuple, Optional, Set
from fastapi import FastAPI, Query, BackgroundTasks
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel
from starlette.responses import RedirectResponse, FileResponse

import os
import sys

# import importlib
# PARENT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# import module from ../meta-test.py
# sys.path.append(PARENT_DIR)
# meta_test = importlib.import_module("meta-test", package=None)
import meta_test

app = FastAPI()

app.mount("/_app", StaticFiles(directory="frontend/build/_app"), name="_app")

class Item(BaseModel):
    name: str
    price: float
    is_offer: Optional[bool] = None

# Serve static files from frontend/build
@app.get("/")
def read_root():
    return FileResponse("frontend/build/index.html")
@app.get("/benchmark")
def read_benchmark():
    return FileResponse("frontend/build/benchmark.html")
@app.get("/benchmark/{path}")
def read_benchmark_path(path: str):
    return FileResponse(f"frontend/build/benchmark/{path}.html")

# APIs
@app.get("/meta-data/list")
def meta_data_list():
    return meta_test.global_config.get_meta_data_list()

@app.get("/meta-data/info/{id}")
def meta_data_info(id: str):
    print(f"meta_data_info: {id}")
    return meta_test.global_config.get_meta_data_info_by_id(id)

@app.get("/meta-data/out-dir")
def meta_data_out_dir(id: int = Query(0), prefix = Query("uni-m-out")):
    print(f"meta_data_out_dir: {id} & {prefix}")
    config = meta_test.global_config.get_config_for_analyzer(id, prefix)
    print(f"meta_data_out_dir: {config.conf_files.out_base_dir}, {config.conf_files.out_dir_prefix}")
    dirs = config.conf_files.find_all_nums(config.conf_files.out_base_dir, config.conf_files.out_dir_prefix)
    result = list()
    for dir, id in dirs:
        result.append({"id": dir, "full": os.path.join(config.conf_files.out_base_dir, dir)})
    return result

@app.get("/meta-data/data-log-parser")
def meta_data_data_log_parser(dir: str = Query("")):
    print(f"meta_data_data_log_parser: {dir}")
    if not os.path.exists(dir):
        return {"table": ""}
    dp = meta_test.DataLogParser(dir)
    dp.read_data_log("data.log")
    result_table = dp.generate_table(dp.cluster())
    with open(result_table, "r") as f:
        result = f.read()
    fork_map_nodes, fork_map_edges = dp.generate_fork_graph_v2()
    input_map_nodes, input_map_edges = dp.generate_input_graph()
    return {"table": result, 
            "fork_graph": {"nodes": fork_map_nodes, "edges": fork_map_edges}, 
            "input_graph": {"nodes": input_map_nodes, "edges": input_map_edges}}

def run_cmd(cmd: List[str]):
    final_cmd = ["meta-test.py"] + cmd + ["--lock=w"]
    meta_test.main(final_cmd)

@app.get("/meta-data/run/status")
def meta_data_run_status():
    return meta_test.global_config.get_current_processes()

@app.get("/meta-data/run/cmds")
def meta_data_run_cmds(id: int = Query(0), limit: int = Query(10)):
    return meta_test.global_config.get_last_command(id, limit)

@app.post("/meta-data/run")
async def meta_data_run(cmd: List[str], background_tasks: BackgroundTasks):
    background_tasks.add_task(run_cmd, cmd)
    return {"message": f"run {cmd} in background"}

@app.get("/analyze")
def analyze():
    return {"message": "analyze"}

@app.get("/hello")
def hello():
    return {"message": "Hello World"}

@app.get("/items/{item_id}")
def read_item(item_id: int, q: Union[str, None] = None):
    return {"item_id": item_id, "q": q}

@app.put("/items/{item_id}")
def update_item(item_id: int, item: Item):
    return {"item_name": item.name, "item_id": item_id}