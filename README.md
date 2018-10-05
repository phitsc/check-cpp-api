# check-cpp-api

## Building Clang on Linux

1. Follow the build instructions in https://clang.llvm.org/get_started.html . Make sure to include step 4 'extra Clang tools'.

2. Clone the *cpp-check-api* repository into `llvm/tools/clang/tools/extra/`.

3. Add `add_subdirectory(check-cpp-api)` at the bottom of `llvm/tools/clang/tools/extra/CMakeLists.txt`

4. Run the make script.

    python3 make.py


## Building with Docker

1. Clone the *check-cpp-api* repository into a directory of your choice and `cd` to that directory.

2. Create the *libtooling* docker image for building Clang tools.

    sudo docker build -t libtooling .

3. Run the make script.

    python3 make.py




