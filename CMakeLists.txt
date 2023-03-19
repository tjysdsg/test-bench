cmake_minimum_required(VERSION 3.2)

project(test_bench CXX)
set(CMAKE_CXX_STANDARD 17)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
add_subdirectory(thirdparty/benchmark)

add_subdirectory(sort)
