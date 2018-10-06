#!/usr/bin/env bash

set -ex

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

source ${DIR}/docker-build.sh

test_project_dir=${llvm_root_dir}/llvm/tools/clang/tools/extra/${project_name}/test/project1

${llvm_build_dir}/bin/check-cpp-api -json -p ${test_project_dir}/build/ ${test_project_dir}/src/TestClass.cpp

diff ${llvm_build_dir}/check-cpp-api_results.json ${test_project_dir}/json/check-cpp-api_results.json
