cmake_minimum_required(VERSION 3.20)
project(OOPC1_STACK C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# Include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Main application
add_executable(OOPC1_STACK src/main.c src/Stack.c)

# Add googletest
add_subdirectory(third_party/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Tests
enable_testing()
add_executable(stack_tests
        tests/StackTest.cpp
        src/Stack.c
)
target_include_directories(stack_tests PRIVATE include)
target_link_libraries(stack_tests gtest gtest_main)

add_test(NAME stack_tests COMMAND stack_tests)
