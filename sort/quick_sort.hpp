/**
 * @file Multiple versions of quick sort
 *
 * @note Lomuto partition scheme with last element as the pivot will get stack overflow if the input is a large sorted
 * array
 * @note Other than dutch flag partition scheme, all schemes will get stack overflow if the input is a large array
 * with all elements being the same value
 */

#include <algorithm>

namespace internal {
    enum PIVOT_POLICY {
        LAST, MIDDLE, MO3,
    };

    enum PARTITION_SCHEME {
        LOMUTO, HOARE, DUTCH_FLAG,
    };

    template<typename T, PIVOT_POLICY PP>
    struct ChoosePivot {
        static int choose_pivot(T *arr, int low, int high) { abort(); }
    };

    template<typename T>
    struct ChoosePivot<T, LAST> {
        static int choose_pivot(T *arr, int low, int high) {
            return high;
        }
    };

    template<typename T>
    struct ChoosePivot<T, MIDDLE> {
        static int choose_pivot(T *arr, int low, int high) {
            return (low + high) / 2;
        }
    };

    template<typename T>
    struct ChoosePivot<T, MO3> {
        static int choose_pivot(T *arr, int low, int high) {
            // estimate median, and place it in arr[high] and use it as the pivot
            int mid = (low + high) / 2;
            if (arr[mid] < arr[low])
                std::swap(arr[low], arr[mid]);
            if (arr[high] < arr[low])
                std::swap(arr[low], arr[high]);
            if (arr[mid] < arr[high])
                std::swap(arr[mid], arr[high]);
            return high;
        }
    };

    template<typename T, PARTITION_SCHEME PS>
    struct PartitionScheme {
        static int partition(T *arr, int low, int high) { abort(); }
    };

    template<typename T>
    struct PartitionScheme<T, LOMUTO> {
        static int partition(T *arr, int low, int high, int pivot_idx) {
            std::swap(arr[pivot_idx], arr[high]);
            T pivot = arr[high]; // Lomuto scheme assumes that pivot is the last element

            int i = (low - 1);    // index of smaller element
            for (int j = low; j < high; j++) {
                // if current element is smaller than or equal to pivot
                if (arr[j] <= pivot)
                    std::swap(arr[++i], arr[j]);
            }
            // no need to check if i <= high
            std::swap(arr[i + 1], arr[high]);
            return i + 1;
        }
    };

    template<typename T>
    struct PartitionScheme<T, HOARE> {
        static int partition(T *arr, int low, int high, int pivot_idx) {
            T pivot = arr[pivot_idx];
            while (true) {
                while (arr[low] < pivot)
                    ++low;
                while (arr[high] > pivot)
                    --high;

                if (low < high &&
                    arr[low] == arr[high]) { // in case something like 4 2 3 4 1 7 4 where pivot value is 4
                    --high;
                }

                if (low >= high) return low;

                std::swap(arr[low], arr[high]);
            }
        }
    };

    /**
     * This makes sorting a large array with all elements being the same value much much much faster
     */
    template<typename T>
    struct PartitionScheme<T, DUTCH_FLAG> {
        static std::pair<int, int> partition(T *arr, int low, int high, int pivot_idx) {
            T pivot = arr[pivot_idx];

            int smaller = low; // for i in [low, smaller), arr[i] < pivot
            int equal = low; // for i in [smaller, equal), arr[i] == pivot
            int larger = high; // for i in [larger, high], arr[i] > pivot

            while (equal <= larger) {
                if (arr[equal] < pivot) {
                    std::swap(arr[equal++], arr[smaller++]);
                } else if (arr[equal] > pivot) {
                    std::swap(arr[equal], arr[larger--]);
                } else {
                    ++equal;
                }
            }

            return std::make_pair(smaller, larger);
        }
    };

    template<typename T, PIVOT_POLICY PP, PARTITION_SCHEME PS>
    struct Sort {
        static void quick_sort(T *arr, int low, int high) {
            if (low < high) {
                int pivot = ChoosePivot<T, PP>::choose_pivot(arr, low, high);
                int p = PartitionScheme<T, PS>::partition(arr, low, high, pivot);
                quick_sort(arr, low, p - 1);
                quick_sort(arr, p + 1, high);
            }
        }
    };

    // specialization for dutch flag partition scheme
    template<typename T, PIVOT_POLICY PP>
    struct Sort<T, PP, DUTCH_FLAG> {
        static void quick_sort(T *arr, int low, int high) {
            if (low < high) {
                int pivot = ChoosePivot<T, PP>::choose_pivot(arr, low, high);
                std::pair<int, int> par = PartitionScheme<T, DUTCH_FLAG>::partition(arr, low, high, pivot);
                quick_sort(arr, low, par.first - 1);
                quick_sort(arr, par.second + 1, high);
            }
        }
    };
}

template<typename T>
void quick_sort_lomuto_last(T *arr, int n) {
    using namespace internal;
    Sort<T, LAST, LOMUTO>::quick_sort(arr, 0, n - 1);
}

template<typename T>
void quick_sort_lomuto_middle(T *arr, int n) {
    using namespace internal;
    Sort<T, MIDDLE, LOMUTO>::quick_sort(arr, 0, n - 1);
}

template<typename T>
void quick_sort_hoare_middle(T *arr, int n) {
    using namespace internal;
    Sort<T, MIDDLE, HOARE>::quick_sort(arr, 0, n - 1);
}

template<typename T>
void quick_sort_lomuto_mo3(T *arr, int n) {
    using namespace internal;
    Sort<T, MO3, LOMUTO>::quick_sort(arr, 0, n - 1);
}

template<typename T>
void quick_sort_hoare_mo3(T *arr, int n) {
    using namespace internal;
    Sort<T, MO3, HOARE>::quick_sort(arr, 0, n - 1);
}

template<typename T>
void quick_sort_dutch_flag_mo3(T *arr, int n) {
    using namespace internal;
    Sort<T, MO3, DUTCH_FLAG>::quick_sort(arr, 0, n - 1);
}
