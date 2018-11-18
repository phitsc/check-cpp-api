cmake_minimum_required(VERSION 3.11)

set(LLVM_LINK_COMPONENTS support)

set(TARGET check-cpp-api)

add_clang_executable(${TARGET}
    src/Heuristic_KC_1.cpp
    src/Heuristic_KCE_1.cpp
    src/Heuristic_KCE_2.cpp
    src/Heuristic_KM_1.cpp
    src/Heuristic_RU_2.cpp
    src/HeuristicsCheckAction.cpp
    src/main.cpp
)

target_link_libraries(${TARGET} PRIVATE
    clangTooling
    clangBasic
    clangASTMatchers
    stdc++fs
)

target_compile_definitions(${TARGET}
    PRIVATE CLANG_BUILTIN_INCLUDES_DIR="${CLANG_BUILTIN_INCLUDES_DIR}"
)

target_compile_features(${TARGET}
    PRIVATE cxx_std_17
)

set (CMAKE_EXE_LINKER_FLAGS "-static-libstdc++")

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(${TARGET}
        PRIVATE -Wno-covered-switch-default
    )
endif()