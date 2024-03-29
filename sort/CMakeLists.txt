add_library(sort stub.cpp)
target_include_directories(sort PUBLIC sort)

set(SORTING_ALGORITHMS
        heap_sort
        quick_sort_lomuto_middle
        quick_sort_hoare_middle
        quick_sort_lomuto_mo3
        quick_sort_hoare_mo3
        quick_sort_dutch_flag_mo3
        )
foreach (sort_name ${SORTING_ALGORITHMS})
    set(_target_name test_${sort_name})

    # test
    add_executable(${_target_name} test.cpp)
    target_link_libraries(${_target_name} PUBLIC sort)
    target_compile_definitions(${_target_name} PUBLIC SORT_FUNC=${sort_name})

    # benchmark
    set(_target_name bench_${sort_name})
    add_executable(${_target_name} bench.cpp)
    target_link_libraries(${_target_name} PUBLIC benchmark::benchmark sort)
    target_compile_definitions(${_target_name} PUBLIC SORT_FUNC=${sort_name})
endforeach ()
