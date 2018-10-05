#!/usr/bin/env bash

exe_name="check-cpp-api"

if [ -f /root/clang-llvm/build/bin/${exe_name} ]
then
    echo "/root/clang-llvm/build/bin/${exe_name} $@"
    /root/clang-llvm/build/bin/${exe_name} "$@"
else
    echo "Error: ${exe_name} not found. Call make.py first."
fi
