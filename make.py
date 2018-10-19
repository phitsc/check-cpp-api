#!/usr/bin/env python3

from helpers import docker_image_exists, docker_container_exists
from pathlib import Path
from subprocess import check_call

project_name = "check-cpp-api"
build_image_name = "libtooling"
build_container_name = "{}_build".format(project_name)


def main():
    if docker_image_exists(build_image_name):
        if docker_container_exists(build_container_name):
            check_call(["docker", "start", "-i", build_container_name])
        else:
            working_dir = Path.cwd()

            check_call(
                [
                    "docker",
                    "run",
                    "-it",
                    "--name",
                    build_container_name,
                    "-v",
                    "{}:/root/clang-llvm/llvm/tools/clang/tools/extra/{}".format(
                        working_dir.as_posix(), project_name
                    ),
                    build_image_name,
                    "/root/clang-llvm/llvm/tools/clang/tools/extra/{}/Docker/docker-build.sh".format(
                        project_name
                    )
                    #'--entrypoint', f"/bin/bash",
                ]
            )

        check_call(["docker", "commit", build_container_name, project_name])
    else:
        raise Exception(
            "{} Docker image not found. Run 'docker build -t {} .' first.".format(
                build_image_name, build_image_name
            )
        )


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print("Error:", e)
