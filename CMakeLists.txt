cmake_minimum_required(VERSION 3.2)

project(sort_test_bench CXX)
set(CMAKE_CXX_STANDARD 17)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
add_subdirectory(thirdparty/benchmark)

add_executable(bench bench.cpp)
target_link_libraries(bench PUBLIC benchmark::benchmark)

add_executable(test test.cpp)
