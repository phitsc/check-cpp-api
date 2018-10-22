# check-cpp-api

[![Build Status](https://travis-ci.com/phitsc/check-cpp-api.svg?token=RqqkynutptY9gpoo5YZP&branch=master)](https://travis-ci.com/phitsc/check-cpp-api)


## Build instructions

### Option 1: Build together with Clang on Linux

1. Follow the build instructions on https://clang.llvm.org/get_started.html . Make sure to include step 4 'extra Clang tools'.

2. Clone the *check-cpp-api* repository into `llvm/tools/clang/tools/extra/`.

3. Add `add_subdirectory(check-cpp-api)` at the bottom of `llvm/tools/clang/tools/extra/CMakeLists.txt`

4. Build everything as described in step 7 'Build LLVM and Clang' on https://clang.llvm.org/get_started.html .

### Option 2: Build with Docker

1. Clone the *check-cpp-api* repository into a directory of your choice and change into that directory.

```bash
git clone https://github.com/phitsc/check-cpp-api.git
cd check-cpp-api
```

2. Create the *libtooling* docker image for building Clang tools.

```bash
sudo docker build -t libtooling Docker
```

3. Run the make script.

```bash
python3 make.py
```

# Using check-cpp-api

## Running check-cpp-api against the test project

1. Change into *check-cpp-api/test/project1*.

```bash
cd test/project1
```

2. Create the *builddir* directory and change into it.

```bash
mkdir builddir
cd builddir
```

3. Create the compilation database.

```bash
cmake -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
```

    Make sure the *compile_commands.json* file has been created in the *project1/builddir* directory.

4. Change back into the *check-cpp-api* directory and run check-cpp-api.

```bash
cd ../../..
python3 run.py -p test/project1/builddir test/project1/src/TestClass.cpp
```

