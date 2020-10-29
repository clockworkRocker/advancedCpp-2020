cmake_minimum_required(VERSION 3.14)
project(FakeNeuralNetwork C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# ------------ DIRECTORIES ------------ #
include_directories(include)
set(CMAKE_MODULE_PATH              ${PROJECT_SOURCE_DIR}/CMakeModules)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(PROJECT_TEST_DIR               ${PROJECT_SOURCE_DIR}/tests)

# ------------ FLAGS ------------ #
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic")

# ------------ SOURCE FILE SETS ------------ #
file(GLOB MATRIX_SRC ${PROJECT_SOURCE_DIR}/src/matrix/*.c)
file(GLOB NEURAL_SRC ${PROJECT_SOURCE_DIR}/src/neural/*.c)
file(GLOB TEST_SRC   ${PROJECT_TEST_DIR}/*.cpp)

# ------------ LIBRARIES ------------ #
add_library(matrix ${MATRIX_SRC})
add_library(neural ${NEURAL_SRC})
target_link_libraries(neural matrix)
add_library(neural_with_threads SHARED ${PROJECT_SOURCE_DIR}/src/NR_process_parallel.c)
target_link_libraries(neural_with_threads neural)
target_link_libraries(neural_with_threads pthread)

# ------------ EXECUTABLES ------------ #
add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} neural)
target_link_libraries(${PROJECT_NAME} neural_with_threads)
target_compile_definitions(${PROJECT_NAME} PUBLIC USE_THREADS)


add_executable(teachNetwork src/teachNetwork.cpp)
target_link_libraries(teachNetwork neural)
target_compile_definitions(teachNetwork
        PUBLIC INPUT_PATH="neural_network_files/input/")

# ------------ ANALYSIS OPTIONS ------------ #
option(ENABLE_TESTING  NO)
option(ENABLE_COVERAGE NO)

# ------------ TESTS ------------ #

    find_package(GTest)                                                         # Try to find GTest
if(ENABLE_TESTING)
    if(NOT GTEST_FOUND)
        include(FetchContent)
        FetchContent_Declare(                                                   # Get GTest from GitHub
                googletest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG        release-1.8.0
                )
        if(NOT googletest_POPULATED)
            FetchContent_Populate(googletest)                                   # Install GTest
            add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
        endif()

        FetchContent_MakeAvailable(googletest)
    endif(NOT GTEST_FOUND)

    include(GoogleTest)

    enable_testing()

    add_executable(test_${PROJECT_NAME} ${TEST_SRC})
    target_link_libraries(test_${PROJECT_NAME} gtest)
    target_link_libraries(test_${PROJECT_NAME} gtest_main pthread)
    target_link_libraries(test_${PROJECT_NAME} neural)
    target_link_libraries(test_${PROJECT_NAME} neural_with_threads)
    target_compile_definitions(test_${PROJECT_NAME}
            PUBLIC INPUT_PATH="${PROJECT_TEST_DIR}/test_inputs/"
                   STRESS_TEST_PATH="${PROJECT_SOURCE_DIR}/neural_network_files/"
            )

    gtest_discover_tests(test_${PROJECT_NAME})

    if(ENABLE_COVERAGE)
        include(CodeCoverage)
        set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g -O0 -fprofile-arcs -ftest-coverage")
        setup_target_for_coverage_gcovr_xml(
                NAME            test_coverage
                EXECUTABLE test_${PROJECT_NAME}
                BASE_DIRECTORY  ${PROJECT_TEST_DIR}
        )

    endif(ENABLE_COVERAGE)
endif(ENABLE_TESTING)
