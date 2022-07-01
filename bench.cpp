#include <benchmark/benchmark.h>
#include <algorithm>
#include "common.h"
#include "config.h"
#include "heap_sort.hpp"
#include "quick_sort.hpp"

template<class ValueType, class Order>
struct Sort {
    size_t Quantity;

    void run(benchmark::State &state) const {
        runOpOnCopies<ValueType>(
                state, Quantity, Order(), BatchSize::CountElements,
                [](auto &Copy) {
                    SORT_FUNC(Copy.data(), Copy.size());
                });
    }

    bool skip() const { return Order() == ::Order::Heap; }

    std::string name() const {
        return "BM_Sort" + ValueType::name() + Order::name() + "_" +
               std::to_string(Quantity);
    };
};

int main(int argc, char **argv) {
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv))
        return 1;
    makeCartesianProductBenchmark<Sort, AllValueTypes, AllOrders>(Quantities);
    benchmark::RunSpecifiedBenchmarks();
}