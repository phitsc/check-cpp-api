cmake_minimum_required(VERSION 3.11)

set(PROJECT_NAME test-check-cpp-api)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/TestDetermineCaseType.cpp
)

target_compile_features(${PROJECT_NAME}
    PRIVATE cxx_std_17)
