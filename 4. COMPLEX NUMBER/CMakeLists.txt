cmake_minimum_required(VERSION 3.20)
project(OOPC4_COMPLEX_NUMBER CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add global compiler flags
add_compile_options(-Wall -pedantic)
# Add debug flag only for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g)
endif()

set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include GoogleTest integration utilities
include(GoogleTest)

# Main application
add_executable(OOPC4_COMPLEX_NUMBER
        src/main.cpp
        src/Complex.cpp
)
target_include_directories(OOPC4_COMPLEX_NUMBER PRIVATE include)

enable_testing()

# Unit tests
add_executable(complex_tests
        tests/ComplexTest.cpp
        src/Complex.cpp
)
target_include_directories(complex_tests PRIVATE include)

# Link GoogleTest libraries
target_link_libraries(complex_tests PRIVATE GTest::gtest GTest::gtest_main)
# Automatically discover and register tests
gtest_discover_tests(complex_tests)