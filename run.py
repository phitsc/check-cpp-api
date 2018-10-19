#!/usr/bin/env python3

"""
Runs check-cpp-api within a Docker container
if a suitable Docker image named check-cpp-api exists.
"""

import argparse
import fileinput

from os import remove, rename
from pathlib import Path
from subprocess import call
from helpers import docker_image_exists

PROJECT_NAME = "check-cpp-api"


def redirect_paths(unknown_args, name, directory):
    """ For every argument in unknown_args which is an existing
        file path containing name, the file path is redirected
        to directory"""
    ret = []

    for arg in unknown_args:
        if Path(arg).exists:
            pos = arg.find(name)
            if pos >= 0:
                ret.append(directory + arg[pos + len(name) :])
            else:
                ret.append(arg)
        else:
            ret.append(arg)

    return ret

# pylint: disable=C0330
def patch_compile_commands(
    compile_commands_file, test_project_dir, container_project_dir
):
    """ Replaces occurrences of test_project_dir in the compilation
        database file with container_project_dir. Also replaces \
        by / """
    with fileinput.FileInput(
        str(compile_commands_file), inplace=True, backup=".bak"
    ) as file:
        for line in file:
            print(
                line.replace("\\\\", "/").replace(
                    test_project_dir, container_project_dir
                ),
                end="",
            )


def fix_wsl_path(path):
    """ Replaces /mnt/... by /host_mnt/... in paths starting with /mnt/ """
    # workaround for the problem described here:
    # https://forums.docker.com/t/volume-mounts-in-windows-does-not-work/10693/169
    parts = list(Path(path).parts)
    parts[1] = "host_mnt" if len(parts) > 1 and parts[1] == "mnt" else parts[1]
    return Path(*parts)


def main():
    """ The main function """
    if docker_image_exists(PROJECT_NAME):

        # extract test project name and directory from -p argument
        parser = argparse.ArgumentParser(add_help=False)
        parser.add_argument("-p")
        args, unknown_args = parser.parse_known_args()

        test_project_dir = Path(args.p).parent if args.p else None
        test_project_name = Path(args.p).parent.stem if args.p else None
        test_project_build_dir_name = Path(args.p).stem if args.p else None
        container_project_dir = (
            "/root/test_project/" + test_project_name if args.p else None
        )
        compile_commands_file = (
            Path(args.p) / "compile_commands.json" if args.p else None
        )

        # redirect local paths to their respective directory within the container
        if container_project_dir:
            unknown_args = redirect_paths(
                unknown_args, test_project_name, container_project_dir
            )

        # project_volume required to run docker-run.sh script
        project_volume = "{}:/root/clang-llvm/llvm/tools/clang/tools/extra/{}".format(
            fix_wsl_path(Path.cwd()), PROJECT_NAME
        )

        # test project volume and arguments
        if test_project_dir:
            test_project_volume = "{}:{}".format(
                fix_wsl_path(test_project_dir), container_project_dir
            )

            test_project_args = "-p /root/test_project/{}/{} {}".format(
                test_project_name, test_project_build_dir_name, " ".join(unknown_args)
            )
        else:
            # if -p was not supplied, just pass same volume twice
            # to specify a valid volume
            test_project_volume = project_volume
            test_project_args = " ".join(unknown_args)

        if compile_commands_file and compile_commands_file.exists:
            patch_compile_commands(
                compile_commands_file,
                test_project_dir.as_posix(),
                container_project_dir,
            )

        call(
            [
                "docker",
                "run",
                "-it",  # use container interactively
                "--rm",  # remove container when it exits
                "-v",
                project_volume,
                "-v",
                test_project_volume,
                PROJECT_NAME,  # project name = image name
                "/root/clang-llvm/llvm/tools/clang/tools/extra/{}/Docker/docker-run.sh".format(
                    PROJECT_NAME
                ),
                test_project_args  # pass on command line arguments
                # "/bin/bash"
            ]
        )

        if compile_commands_file and compile_commands_file.exists:
            remove(str(compile_commands_file))
            rename(
                str(compile_commands_file.with_suffix(".json.bak")),
                str(compile_commands_file),
            )

    else:
        raise RuntimeError(
            "{} Docker image not found. Run make.py first.".format(PROJECT_NAME)
        )


if __name__ == "__main__":
    try:
        main()
    except RuntimeError as error:
        print("Error:", error)
