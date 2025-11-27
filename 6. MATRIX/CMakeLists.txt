cmake_minimum_required(VERSION 3.20)
project(OOPC6_MATRIX CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add global compiler flxags
add_compile_options(-Wall -pedantic)
# Add debug flag only for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g)
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
        # DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include GoogleTest integration utilities
include(GoogleTest)

# Main application
add_executable(OOPC6_MATRIX
        src/main.cpp
        src/Matrix.cpp
        src/MatrixExceptions.cpp
)
target_include_directories(OOPC6_MATRIX PRIVATE include)

enable_testing()

# Unit tests
add_executable(matrix_tests
        tests/MatrixTest.cpp
        src/Matrix.cpp
        src/MatrixExceptions.cpp
)
target_include_directories(matrix_tests PRIVATE include)

# Link GoogleTest libraries
target_link_libraries(matrix_tests PRIVATE GTest::gtest GTest::gtest_main)
# Automatically discover and register tests
gtest_discover_tests(matrix_tests)