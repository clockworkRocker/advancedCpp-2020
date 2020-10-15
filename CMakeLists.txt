cmake_minimum_required(VERSION 3.12)
project(Task1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

include_directories(include)

#        ======== Building part ========        #
file(GLOB SRC_FILES src/*.c)
list(REMOVE_ITEM SRC_FILES src/main.c)

# -------- Create a lib for further purposes -------- #
add_library(${PROJECT_NAME}_lib ${SRC_FILES} src/utility.c)

add_executable(${PROJECT_NAME}
        src/main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)

#        ======== Testing part ========        #
file(GLOB TEST_SRC_FILES tests/*.cpp)

OPTION(TESTING "Builds tests" OFF)

if(TESTING)
    enable_testing()
    set(EXAMPLE_PATH Examples/example.py)

    find_package(GTest REQUIRED)

    add_executable(testTask1
            ${TEST_SRC_FILES})
    target_link_libraries(testTask1 ${PROJECT_NAME}_lib)
    target_link_libraries(testTask1 gtest)
    target_link_libraries(testTask1 gtest_main pthread)

    gtest_discover_tests(testTask1
            EXTRA_ARGS "${EXAMPLE_PATH}")
endif(TESTING)