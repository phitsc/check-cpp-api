#!/usr/bin/env bash

set -ex

project_name="check-cpp-api"

llvm_root_dir="/root/clang-llvm"
llvm_build_dir=${llvm_root_dir}/build

if [ ! -d "${llvm_root_dir}/llvm/tools/clang/tools/extra" ]
then
    echo "Run this script only within a libtooling docker container."
    exit 1
fi

if [ ! -d "${llvm_build_dir}" ]
then
    mkdir ${llvm_build_dir}
fi

cd ${llvm_build_dir}

# this script assumes we are in a sibling directory of llvm,
# e.g. build

grep -q -F "add_subdirectory(${project_name})" \
    ../llvm/tools/clang/tools/extra/CMakeLists.txt \
    || echo "add_subdirectory(${project_name})" \
    >> ../llvm/tools/clang/tools/extra/CMakeLists.txt

cmake \
    -G Ninja \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_USE_LINKER=gold \
    ../llvm \
    && ninja -j12
