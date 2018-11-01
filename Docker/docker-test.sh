#!/usr/bin/env bash

set -ex

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

# Build and run the unit tests
source ${DIR}/docker-build-unittests.sh

${unittests_build_dir}/test-check-cpp-api

# Build the application
source ${DIR}/docker-build.sh

# Run the integration tests
test_dir=/root/test_project
mkdir -p ${test_dir}

test_project_dir=${test_dir}/project1
ln -sfn ${llvm_root_dir}/llvm/tools/clang/tools/extra/${project_name}/test/project1 ${test_project_dir}

${llvm_build_dir}/bin/check-cpp-api \
	-json ${llvm_build_dir}/check-cpp-api_results.json \
	-p ${test_project_dir}/build/ \
	${test_project_dir}/src/TestClass.cpp

diff ${llvm_build_dir}/check-cpp-api_results.json ${test_project_dir}/json/check-cpp-api_results.json
