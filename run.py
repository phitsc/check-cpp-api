#!/usr/bin/env python3

import argparse
import sys

from helpers import docker_image_exists
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

        # redirect local paths to their respective directory within the container
        if container_project_dir:
            unknownArgs = redirectPaths(unknownArgs, test_project_name, container_project_dir)

        # project_volume required to run docker-run.sh script
        project_volume = ("{}:/root/clang-llvm/llvm/tools/clang/tools/extra/{}"
            .format(Path.cwd().as_posix(), project_name))

        # test project volume and arguments
        if test_project_dir:
            test_project_volume = ("{}:{}"
                .format(test_project_dir.as_posix(), container_project_dir))

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

    else:
        raise Exception("{} Docker image not found. Run make.py first."
            .format(project_name)
        )

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
       print("Error:", e)