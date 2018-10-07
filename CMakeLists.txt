cmake_minimum_required(VERSION 3.11)

set(LLVM_LINK_COMPONENTS support)

set(PROJECT_NAME check-cpp-api)

add_clang_executable(${PROJECT_NAME}
	src/Heuristic_KCE_1.cpp
	src/Heuristic_KM_1.cpp
	src/HeuristicsCheckAction.cpp
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    clangTooling
    clangBasic
    clangASTMatchers
    stdc++fs
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE CLANG_BUILTIN_INCLUDES_DIR="${CLANG_BUILTIN_INCLUDES_DIR}")

target_compile_features(${PROJECT_NAME}
    PRIVATE cxx_std_17)

set (CMAKE_EXE_LINKER_FLAGS "-static-libstdc++")
