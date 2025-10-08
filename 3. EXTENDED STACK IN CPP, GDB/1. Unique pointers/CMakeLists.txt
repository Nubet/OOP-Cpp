cmake_minimum_required(VERSION 3.10)
project(OOPC3_STACK_CPP CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# PATH TO .h files
include_directories(${CMAKE_SOURCE_DIR}/include)

# turn on debugging
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# MAIN APPLICATION
add_executable(OOPC3_STACK_CPP
    src/main.cpp
    src/Stack.cpp
)

# ADD googletest
add_subdirectory(third_party/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
enable_testing()

# TESTS
add_executable(stack_tests
    tests/StackTest.cpp
    src/Stack.cpp
)
target_link_libraries(stack_tests gtest gtest_main)
add_test(NAME stack_tests COMMAND stack_tests)
