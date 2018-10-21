cmake_minimum_required(VERSION 3.11)

enable_testing()

set(PROJECT_NAME test-check-cpp-api)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/TestDetermineCaseType.cpp
)

target_compile_features(${PROJECT_NAME}
    PRIVATE cxx_std_17)

add_test(unittests ${PROJECT_NAME})
