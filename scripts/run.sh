#!/usr/bin/bash
uvicorn server:app --port 8000 --host 0.0.0.0 --reload