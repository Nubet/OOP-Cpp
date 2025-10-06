cmake_minimum_required(VERSION 3.10)
project(OOPC4_COMPLEX_NUMBER CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# PATH TO .h files
include_directories(${CMAKE_SOURCE_DIR}/include)

# MAIN APPLICATION
add_executable(OOPC4_COMPLEX_NUMBER
        src/main.cpp
        src/Complex.cpp
)

# ADD googletest
add_subdirectory(third_party/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
enable_testing()

# TESTS
add_executable(complex_tests
        tests/ComplexTest.cpp
        src/Complex.cpp
)

target_link_libraries(complex_tests PRIVATE gtest gtest_main)
add_test(NAME complex_tests COMMAND complex_tests)