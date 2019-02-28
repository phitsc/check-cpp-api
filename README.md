# check-cpp-api

[![Build Status](https://travis-ci.com/phitsc/check-cpp-api.svg?token=RqqkynutptY9gpoo5YZP&branch=master)](https://travis-ci.com/phitsc/check-cpp-api)

## Try out check-cpp-api without building

*check-cpp-api* can be tried out on the libbitcoin libraries using the *phitsc/check-libbitcoin-api* Docker image. On any system with Docker installed, run the following command to start a Bash shell in a Docker container based on the *phitsc/check-libbitcoin-api* Docker image.

        docker run -it --rm phitsc/check-libbitcoin-api:1.3 /bin/bash

Then, within the Bash shell in the newly started Docker container, run:

        cd ~/libbitcoin-all
        find . -name '*.hpp' -exec check-cpp-api -kc-1-1-case-type=snake {} +

## Build instructions

### Option 1: Build together with Clang on Linux

1. Follow the build instructions on https://clang.llvm.org/get_started.html . Make sure to include step 4 'extra Clang tools'.

2. Clone the *check-cpp-api* repository into `llvm/tools/clang/tools/extra/`.

3. Add `add_subdirectory(check-cpp-api)` at the bottom of `llvm/tools/clang/tools/extra/CMakeLists.txt`

4. Build everything as described in step 7 'Build LLVM and Clang' on https://clang.llvm.org/get_started.html .

### Option 2: Build with Docker

1. Clone the *check-cpp-api* repository into a directory of your choice and change into that directory.

        git clone https://github.com/phitsc/check-cpp-api.git
        cd check-cpp-api

2. Create the *libtooling* docker image for building Clang tools.

        sudo docker build -t libtooling api-utils-common/Docker

3. Run the make script.

        ./make

# Using check-cpp-api

## Running check-cpp-api against the test project

1. Change into *check-cpp-api/test/integration_tests*.

        cd test/integration_tests

2. Create the *builddir* directory and change into it.

        mkdir builddir
        cd builddir

3. Create the compilation database.

        cmake -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

    Make sure the *compile_commands.json* file has been created in the *integration_tests/builddir* directory.

4. Change back into the *check-cpp-api* directory and run check-cpp-api.

        cd ../../..
        python3 run.py -p test/integration_tests/builddir test/integration_tests/src/TestClass.cpp

