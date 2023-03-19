#include <algorithm>
#include <numeric>
#include <tuple>
#include <vector>
#include <utility>
#include <random>
#include <climits>


static const char Letters[] = {
        '0', '1', '2', '3', '4',
        '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F',
        'G', 'H', 'I', 'J', 'K',
        'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U',
        'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k',
        'l', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u',
        'v', 'w', 'x', 'y', 'z'
};
static const std::size_t LettersSize = sizeof(Letters);

inline std::default_random_engine &getRandomEngine() {
    static std::default_random_engine RandEngine(std::random_device{}());
    return RandEngine;
}


inline char getRandomChar() {
    std::uniform_int_distribution<> LettersDist(0, LettersSize - 1);
    return Letters[LettersDist(getRandomEngine())];
}

template<class IntT>
inline IntT getRandomInteger(IntT Min, IntT Max) {
    std::uniform_int_distribution<unsigned long long> dist(Min, Max);
    return static_cast<IntT>(dist(getRandomEngine()));
}

inline std::string getRandomString(std::size_t Len) {
    std::string str(Len, 0);
    std::generate_n(str.begin(), Len, &getRandomChar);
    return str;
}

template<class IntT>
inline std::vector<IntT> getDuplicateIntegerInputs(size_t N) {
    std::vector<IntT> inputs(N, static_cast<IntT>(-1));
    return inputs;
}

template<class IntT>
inline std::vector<IntT> getSortedIntegerInputs(size_t N) {
    std::vector<IntT> inputs;
    for (size_t i = 0; i < N; i += 1)
        inputs.push_back(i);
    return inputs;
}

template<class IntT>
std::vector<IntT> getSortedLargeIntegerInputs(size_t N) {
    std::vector<IntT> inputs;
    for (size_t i = 0; i < N; ++i) {
        inputs.push_back(i + N);
    }
    return inputs;
}

template<class IntT>
std::vector<IntT> getSortedTopBitsIntegerInputs(size_t N) {
    std::vector<IntT> inputs = getSortedIntegerInputs<IntT>(N);
    for (auto &E: inputs) E <<= ((sizeof(IntT) / 2) * CHAR_BIT);
    return inputs;
}

template<class IntT>
inline std::vector<IntT> getReverseSortedIntegerInputs(size_t N) {
    std::vector<IntT> inputs;
    std::size_t i = N;
    while (i > 0) {
        --i;
        inputs.push_back(i);
    }
    return inputs;
}

template<class IntT>
std::vector<IntT> getPipeOrganIntegerInputs(size_t N) {
    std::vector<IntT> v;
    v.reserve(N);
    for (size_t i = 0; i < N / 2; ++i) v.push_back(i);
    for (size_t i = N / 2; i < N; ++i) v.push_back(N - i);
    return v;
}


template<class IntT>
std::vector<IntT> getRandomIntegerInputs(size_t N) {
    std::vector<IntT> inputs;
    for (size_t i = 0; i < N; ++i) {
        inputs.push_back(getRandomInteger<IntT>(0, std::numeric_limits<IntT>::max()));
    }
    return inputs;
}

inline std::vector<std::string> getDuplicateStringInputs(size_t N) {
    std::vector<std::string> inputs(N, getRandomString(1024));
    return inputs;
}

inline std::vector<std::string> getRandomStringInputs(size_t N) {
    std::vector<std::string> inputs;
    for (size_t i = 0; i < N; ++i) {
        inputs.push_back(getRandomString(1024));
    }
    return inputs;
}

inline std::vector<std::string> getSortedStringInputs(size_t N) {
    std::vector<std::string> inputs = getRandomStringInputs(N);
    std::sort(inputs.begin(), inputs.end());
    return inputs;
}

inline std::vector<std::string> getReverseSortedStringInputs(size_t N) {
    std::vector<std::string> inputs = getSortedStringInputs(N);
    std::reverse(inputs.begin(), inputs.end());
    return inputs;
}

inline std::vector<const char *> getRandomCStringInputs(size_t N) {
    static std::vector<std::string> inputs = getRandomStringInputs(N);
    std::vector<const char *> cinputs;
    for (auto const &str: inputs)
        cinputs.push_back(str.c_str());
    return cinputs;
}

namespace internal {
    template<class D, class E, size_t I>
    struct EnumValue : std::integral_constant<E, static_cast<E>(I)> {
        static std::string name() { return std::string("_") + D::Names[I]; }
    };

    template<class D, class E, size_t ...Idxs>
    constexpr auto makeEnumValueTuple(std::index_sequence<Idxs...>) {
        return std::make_tuple(EnumValue<D, E, Idxs>{}...);
    }

    template<class B>
    static auto skip(const B &Bench, int) -> decltype(Bench.skip()) {
        return Bench.skip();
    }

    template<class B>
    static auto skip(const B &Bench, char) {
        return false;
    }

    template<class B, class Args, size_t... Is>
    void makeBenchmarkFromValuesImpl(const Args &A, std::index_sequence<Is...>) {
        for (auto &V: A) {
            B Bench{std::get<Is>(V)...};
            if (!internal::skip(Bench, 0)) {
                benchmark::RegisterBenchmark(Bench.name().c_str(),
                                             [=](benchmark::State &S) { Bench.run(S); });
            }
        }
    }

    template<class B, class... Args>
    void makeBenchmarkFromValues(const std::vector<std::tuple<Args...> > &A) {
        makeBenchmarkFromValuesImpl<B>(A, std::index_sequence_for<Args...>());
    }

    template<template<class...> class B, class Args, class... U>
    void makeBenchmarkImpl(const Args &A, std::tuple<U...> t) {
        makeBenchmarkFromValues<B<U...> >(A);
    }

    template<template<class...> class B, class Args, class... U,
            class... T, class... Tuples>
    void makeBenchmarkImpl(const Args &A, std::tuple<U...>, std::tuple<T...>,
                           Tuples... rest) {
        (internal::makeBenchmarkImpl<B>(A, std::tuple<U..., T>(), rest...), ...);
    }

    template<class R, class T>
    void allValueCombinations(R &Result, const T &Final) {
        return Result.push_back(Final);
    }

    template<class R, class T, class V, class... Vs>
    void allValueCombinations(R &Result, const T &Prev, const V &Value,
                              const Vs &... Values) {
        for (const auto &E: Value) {
            allValueCombinations(Result, std::tuple_cat(Prev, std::make_tuple(E)),
                                 Values...);
        }
    }

}  // namespace internal

// CRTP class that enables using enum types as a dimension for
// makeCartesianProductBenchmark below.
// The type passed to `B` will be a std::integral_constant<E, e>, with the
// additional static function `name()` that returns the stringified name of the
// label.
//
// Eg:
// enum class MyEnum { A, B };
// struct AllMyEnum : EnumValuesAsTuple<AllMyEnum, MyEnum, 2> {
//   static constexpr absl::string_view Names[] = {"A", "B"};
// };
template<class Derived, class EnumType, size_t NumLabels>
using EnumValuesAsTuple =
        decltype(internal::makeEnumValueTuple<Derived, EnumType>(
                std::make_index_sequence<NumLabels>{}));

// Instantiates B<T0, T1, ..., TN> where <Ti...> are the combinations in the
// cartesian product of `Tuples...`, and pass (arg0, ..., argN) as constructor
// arguments where `(argi...)` are the combination in the cartesian product of
// the runtime values of `A...`.
// B<T...> requires:
//  - std::string name(args...): The name of the benchmark.
//  - void run(benchmark::State&, args...): The body of the benchmark.
// It can also optionally provide:
//  - bool skip(args...): When `true`, skips the combination. Default is false.
//
// Returns int to facilitate registration. The return value is unspecified.
template<template<class...> class B, class... Tuples, class... Args>
int makeCartesianProductBenchmark(const Args &... A) {
    std::vector<std::tuple<typename Args::value_type...> > V;
    internal::allValueCombinations(V, std::tuple<>(), A...);
    internal::makeBenchmarkImpl<B>(V, std::tuple<>(), Tuples()...);
    return 0;
}

template<class B, class... Args>
int makeCartesianProductBenchmark(const Args &... A) {
    std::vector<std::tuple<typename Args::value_type...> > V;
    internal::allValueCombinations(V, std::tuple<>(), A...);
    internal::makeBenchmarkFromValues<B>(V);
    return 0;
}

enum class ValueType {
    Uint32, Uint64, Pair, Tuple, String, Float
};
struct AllValueTypes : EnumValuesAsTuple<AllValueTypes, ValueType, 6> {
    static constexpr const char *Names[] = {"uint32", "uint64", "pair<uint32, uint32>", "tuple<uint32, uint64, uint32>",
                                            "string", "float"};
};

using Types = std::tuple<uint32_t, uint64_t, std::pair<uint32_t, uint32_t>, std::tuple<uint32_t, uint64_t, uint32_t>,
        std::string, float>;

template<class V>
using Value = std::tuple_element_t<(int) V::value, Types>;

enum class Order {
    Random,
    Ascending,
    Descending,
    SingleElement,
    PipeOrgan,
    Heap,
    QuickSortAdversary,
};
struct AllOrders : EnumValuesAsTuple<AllOrders, Order, 7> {
    static constexpr const char *Names[] = {"Random", "Ascending",
                                            "Descending", "SingleElement",
                                            "PipeOrgan", "Heap",
                                            "QuickSortAdversary"};
};

// fillAdversarialQuickSortInput fills the input vector with N int-like values.
// These values are arranged in such a way that they would invoke O(N^2)
// behavior on any quick sort implementation that satisifies certain conditions.
// Details are available in the following paper:
// "A Killer Adversary for Quicksort", M. D. McIlroy, Software—Practice &
// ExperienceVolume 29 Issue 4 April 10, 1999 pp 341–344.
// https://dl.acm.org/doi/10.5555/311868.311871.
template<class T>
void fillAdversarialQuickSortInput(T &V, size_t N) {
    assert(N > 0);
    // If an element is equal to gas, it indicates that the value of the element
    // is still to be decided and may change over the course of time.
    const unsigned int gas = N - 1;
    V.resize(N);
    for (unsigned int i = 0; i < N; ++i) {
        V[i] = gas;
    }
    // Candidate for the pivot position.
    int candidate = 0;
    int nsolid = 0;
    // Populate all positions in the generated input to gas.
    std::vector<int> ascVals(V.size());
    // Fill up with ascending values from 0 to V.size()-1.  These will act as
    // indices into V.
    std::iota(ascVals.begin(), ascVals.end(), 0);
    std::sort(ascVals.begin(), ascVals.end(), [&](int x, int y) {
        if (V[x] == gas && V[y] == gas) {
            // We are comparing two inputs whose value is still to be decided.
            if (x == candidate) {
                V[x] = nsolid++;
            } else {
                V[y] = nsolid++;
            }
        }
        if (V[x] == gas) {
            candidate = x;
        } else if (V[y] == gas) {
            candidate = y;
        }
        return V[x] < V[y];
    });
}

template<typename T>
void fillValues(std::vector<T> &V, size_t N, Order O) {
    if (O == Order::SingleElement) {
        V.resize(N, 0);
    } else if (O == Order::QuickSortAdversary) {
        fillAdversarialQuickSortInput(V, N);
    } else {
        while (V.size() < N)
            V.push_back(V.size());
    }
}

template<typename T>
void fillValues(std::vector<std::pair<T, T>> &V, size_t N, Order O) {
    if (O == Order::SingleElement) {
        V.resize(N, std::make_pair(0, 0));
    } else {
        while (V.size() < N)
            // Half of array will have the same first element.
            if (V.size() % 2) {
                V.push_back(std::make_pair(V.size(), V.size()));
            } else {
                V.push_back(std::make_pair(0, V.size()));
            }
    }
}

template<typename T1, typename T2, typename T3>
void fillValues(std::vector<std::tuple<T1, T2, T3>> &V, size_t N, Order O) {
    if (O == Order::SingleElement) {
        V.resize(N, std::make_tuple(0, 0, 0));
    } else {
        while (V.size() < N)
            // One third of array will have the same first element.
            // One third of array will have the same first element and the same second element.
            switch (V.size() % 3) {
                case 0:
                    V.push_back(std::make_tuple(V.size(), V.size(), V.size()));
                    break;
                case 1:
                    V.push_back(std::make_tuple(0, V.size(), V.size()));
                    break;
                case 2:
                    V.push_back(std::make_tuple(0, 0, V.size()));
                    break;
            }
    }
}

inline void fillValues(std::vector<std::string> &V, size_t N, Order O) {
    if (O == Order::SingleElement) {
        V.resize(N, getRandomString(64));
    } else {
        while (V.size() < N)
            V.push_back(getRandomString(64));
    }
}

template<class T>
void sortValues(T &V, Order O) {
    switch (O) {
        case Order::Random: {
            std::random_device R;
            std::mt19937 M(R());
            std::shuffle(V.begin(), V.end(), M);
            break;
        }
        case Order::Ascending:
            std::sort(V.begin(), V.end());
            break;
        case Order::Descending:
            std::sort(V.begin(), V.end(), std::greater<>());
            break;
        case Order::SingleElement:
            // Nothing to do
            break;
        case Order::PipeOrgan:
            std::sort(V.begin(), V.end());
            std::reverse(V.begin() + V.size() / 2, V.end());
            break;
        case Order::Heap:
            std::make_heap(V.begin(), V.end());
            break;
        case Order::QuickSortAdversary:
            // Nothing to do
            break;
    }
}

constexpr size_t TestSetElements = 1 << 18;

template<class ValueType>
std::vector<std::vector<Value<ValueType> >> makeOrderedValues(size_t N,
                                                              Order O) {
    std::vector<std::vector<Value<ValueType> >> Ret;
    const size_t NumCopies = std::max(size_t{1}, TestSetElements / N);
    Ret.resize(NumCopies);
    for (auto &V: Ret) {
        fillValues(V, N, O);
        sortValues(V, O);
    }
    return Ret;
}

enum class BatchSize {
    CountElements,
    CountBatch,
};

template<class ValueType, class F>
void runOpOnCopies(benchmark::State &state, size_t Quantity, Order O,
                   BatchSize Count, F Body) {
    auto Copies = makeOrderedValues<ValueType>(Quantity, O);
    auto Orig = Copies;

    const size_t Batch = Count == BatchSize::CountElements
                         ? Copies.size() * Quantity
                         : Copies.size();
    while (state.KeepRunningBatch(Batch)) {
        for (auto &Copy: Copies) {
            Body(Copy);
            benchmark::DoNotOptimize(Copy);
        }
        state.PauseTiming();
        Copies = Orig;
        state.ResumeTiming();
    }
}


const std::vector<size_t> Quantities = {
        1 << 0, 1 << 2, 1 << 4, 1 << 6,
        1 << 8, 1 << 10, 1 << 14,
        // Running each benchmark in parallel consumes too much memory with
        // MSAN and can lead to the test process being killed.
        1 << 18
};
