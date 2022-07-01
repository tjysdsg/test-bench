cmake_minimum_required(VERSION 3.2)

project(sort_test_bench CXX)
set(CMAKE_CXX_STANDARD 17)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
add_subdirectory(thirdparty/benchmark)

add_library(sort
        sort/stub.cpp)
target_include_directories(sort PUBLIC sort)

foreach (sort_name heap_sort quick_sort_lomuto_middle quick_sort_hoare_middle)
    set(_target_name test_${sort_name})

    # test
    add_executable(${_target_name} test.cpp sort)
    target_link_libraries(${_target_name} PUBLIC sort)
    target_compile_definitions(${_target_name} PUBLIC SORT_FUNC=${sort_name})

    # benchmark
    set(_target_name bench_${sort_name})
    add_executable(${_target_name} bench.cpp)
    target_link_libraries(${_target_name} PUBLIC benchmark::benchmark sort)
    target_compile_definitions(${_target_name} PUBLIC SORT_FUNC=${sort_name})
endforeach ()

