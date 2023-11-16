#!/usr/bin/env python3
from typing import Union, List, Dict, Tuple, Optional, Set
from fastapi import FastAPI
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

@app.get("/")
def read_root():
    return FileResponse("frontend/build/index.html")

@app.get("/meta-data/list")
def meta_data_list():
    return meta_test.global_config.get_meta_data_list()

@app.get("/meta-data/info/{id}")
def meta_data_info(id: str):
    return meta_test.global_config.get_meta_data_info(id)

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