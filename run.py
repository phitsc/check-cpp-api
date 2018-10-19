#!/usr/bin/env python3

import argparse
import fileinput
import sys

from helpers import docker_image_exists
from os import remove, rename
from pathlib import Path
from subprocess import call

project_name="check-cpp-api"

def redirectPaths(unknownArgs, name, dir):
    ret = []

    for arg in unknownArgs:
        if Path(arg).exists:
            pos = arg.find(name)
            if pos >= 0:
                ret.append(dir + arg[pos + len(name):])
            else:
                ret.append(arg)
        else:
            ret.append(arg)

    return ret


def patchCompileCommands(compile_commands_file, test_project_dir, container_project_dir):
    with fileinput.FileInput(str(compile_commands_file), inplace=True, backup='.bak') as file:
        for line in file:
            print(line.replace("\\\\", "/").replace(test_project_dir, container_project_dir), end='')


def fixWslPath(path):
    # workaround for the problem described here:
    # https://forums.docker.com/t/volume-mounts-in-windows-does-not-work/10693/169
    parts = list(Path(path).parts)
    parts[1] = "host_mnt" if len(parts) > 1 and parts[1] == "mnt" else parts[1]
    return Path(*parts)


def main():
    if (docker_image_exists(project_name)):

        # extract test project name and directory from -p argument
        parser = argparse.ArgumentParser(add_help=False)
        parser.add_argument('-p')
        args, unknownArgs = parser.parse_known_args()

        test_project_dir = Path(args.p).parent if args.p else None
        test_project_name = Path(args.p).parent.stem if args.p else None
        test_project_build_dir_name = Path(args.p).stem if args.p else None
        container_project_dir = "/root/test_project/" + test_project_name if args.p else None
        compile_commands_file = Path(args.p) / 'compile_commands.json' if args.p else None

        # redirect local paths to their respective directory within the container
        if container_project_dir:
            unknownArgs = redirectPaths(unknownArgs, test_project_name, container_project_dir)

        # project_volume required to run docker-run.sh script
        project_volume = ("{}:/root/clang-llvm/llvm/tools/clang/tools/extra/{}"
            .format(fixWslPath(Path.cwd()), project_name))

        # test project volume and arguments
        if test_project_dir:
            test_project_volume = ("{}:{}"
                .format(fixWslPath(test_project_dir), container_project_dir))

            test_project_args = ("-p /root/test_project/{}/{} {}"
                .format(
                    test_project_name,
                    test_project_build_dir_name,
                    ' '.join(unknownArgs)
                )
            )
        else:
            # if -p was not supplied, just pass same volume twice
            # to specify a valid volume
            test_project_volume = project_volume
            test_project_args = ' '.join(unknownArgs)


        if compile_commands_file and compile_commands_file.exists:
            patchCompileCommands(compile_commands_file, test_project_dir.as_posix(), container_project_dir)

        call([
            'docker',
            'run',
            '-it', # use container interactively
            '--rm', # remove container when it exits
            '-v', project_volume,
            '-v', test_project_volume,
            project_name, # project name = image name
            "/root/clang-llvm/llvm/tools/clang/tools/extra/{}/Docker/docker-run.sh"
            .format(project_name),
            test_project_args # pass on command line arguments
            # "/bin/bash"
        ])

        if compile_commands_file and compile_commands_file.exists:
            remove(compile_commands_file)
            rename(compile_commands_file.with_suffix(".json.bak"), compile_commands_file)

    else:
        raise Exception("{} Docker image not found. Run make.py first."
            .format(project_name)
        )

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
       print("Error:", e)