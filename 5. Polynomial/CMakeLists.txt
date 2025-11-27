cmake_minimum_required(VERSION 3.20)
project(OOPC5_POLYNOMIAL CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -pedantic)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g)
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()

add_executable(OOPC5_POLYNOMIAL
        src/main.cpp
        src/Poly.cpp
)
target_include_directories(OOPC5_POLYNOMIAL PRIVATE include)

#[[add_executable(poly_tests
        src/Poly.cpp
)
target_include_directories(poly_tests PRIVATE include)
target_link_libraries(poly_tests PRIVATE GTest::gtest GTest::gtest_main)

gtest_discover_tests(poly_tests)]]
