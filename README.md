# check-cpp-api

[![Build Status](https://travis-ci.com/phitsc/check-cpp-api.svg?token=RqqkynutptY9gpoo5YZP&branch=master)](https://travis-ci.com/phitsc/check-cpp-api)

## Build instructions

### Option 1: Build together with Clang on Linux

1. Follow the build instructions on https://clang.llvm.org/get_started.html . Make sure to include step 4 'extra Clang tools'.

2. Clone the *check-cpp-api* repository into `llvm/tools/clang/tools/extra/`.

3. Add `add_subdirectory(check-cpp-api)` at the bottom of `llvm/tools/clang/tools/extra/CMakeLists.txt`

4. Build everything as described in step 7 'Build LLVM and Clang' on https://clang.llvm.org/get_started.html .


### Option 2: Build with Docker

1. Clone the *check-cpp-api* repository into a directory of your choice and `cd` into that directory.

2. Create the *libtooling* docker image for building Clang tools.

```shell
sudo docker build -t libtooling Docker
```

3. Run the make script.

```shell
python3 make.py
```

