#!/usr/bin/env python3

from pathlib import Path
from subprocess import call
from helpers import resolve_path

PROJECT_NAME = "check-cpp-api"

call(
    [
        "python3",
        "run.py",
        "-p",
        str(resolve_path(Path("test/project1/builddir"))),
        str(resolve_path(Path("test/project1/src/TestClass.cpp"))),
    ]
)
