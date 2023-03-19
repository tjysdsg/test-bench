# Sorting algorithms

Benchmark and testing code is heavily adapted
from https://github.com/llvm/llvm-project/blob/main/libcxx/benchmarks/algorithms
and https://github.com/llvm/llvm-project/blob/main/libcxx/test/std/algorithms/alg.sorting/alg.sort/sort/sort.pass.cpp

# Results

- [Heap sort](results/heap_sort.txt)
- [Quick sort using Dutch flag partition scheme and median of three pivot](results/quick_sort_dutch_flag_mo3.txt)
- FIXME: other quick sort variants get stackoverflow for some edge cases
