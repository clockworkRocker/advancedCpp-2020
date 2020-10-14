cmake_minimum_required(VERSION 3.9)
project(Task1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(Task1
        src/main.c
        src/vector.c
        src/intVector.c)

enable_testing()

add_executable(testTask1
        tests/testVector.cpp)

#gtest_discover_tests(testTask1)
