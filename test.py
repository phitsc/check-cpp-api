#!/usr/bin/env python3

import argparse
from pathlib import Path
from subprocess import call
from helpers import resolve_path


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--json",
        help="Export test results to JSON file (in builddir)",
        action="store_true",
    )
    parser.add_argument(
        "--diff",
        help="Diff exported JSON file against the one in ./json",
        action="store_true",
    )
    args = parser.parse_args()

    if args.json or args.diff:
        jsonFile = "/root/test_project/project1/builddir/check-cpp-api_results.json"
    else:
        jsonFile = None

    call_args = (
        ["python3", "run.py"]
        + ([("-json " + jsonFile)] if jsonFile else [])
        + [
            "-p",
            str(resolve_path(Path("test/project1/builddir"))),
            str(resolve_path(Path("test/project1/src/TestClass.cpp"))),
        ]
    )

    call(call_args)

    if args.diff:
        call(
            [
                "diff",
                str(
                    resolve_path(
                        Path("test/project1/builddir/check-cpp-api_results.json")
                    )
                ),
                str(
                    resolve_path(Path("test/project1/json/check-cpp-api_results.json"))
                ),
            ]
        )


if __name__ == "__main__":
    try:
        main()
    except RuntimeError as error:
        print("Error:", error)
