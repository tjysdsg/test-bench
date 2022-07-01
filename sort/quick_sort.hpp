#include <algorithm>

namespace internal {
    enum PIVOT_POLICY {
        LAST, MIDDLE,
    };

    enum PARTITION_SCHEME {
        LOMUTO, HOARE,
    };

    template<typename T, PIVOT_POLICY PP>
    struct ChoosePivot {
        static T choose_pivot(T *arr, int low, int high) { abort(); }
    };

    template<typename T>
    struct ChoosePivot<T, LAST> {
        static T choose_pivot(T *arr, int low, int high) {
            return arr[high];
        }
    };

    template<typename T>
    struct ChoosePivot<T, MIDDLE> {
        static T choose_pivot(T *arr, int low, int high) {
            return arr[(low + high) / 2];
        }
    };

    template<typename T, PARTITION_SCHEME PS>
    struct PartitionScheme {
        static int partition(T *arr, int low, int high) { abort(); }
    };

    /**
     * @note Lomuto partition only works when pivot is the last element
     */
    template<typename T>
    struct PartitionScheme<T, LOMUTO> {
        static int partition(T *arr, int low, int high, T pivot) {
            pivot = arr[high]; /// ignore the pivot policy
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
        static int partition(T *arr, int low, int high, T pivot) {
            while (true) {
                while (arr[low] < pivot)
                    ++low;
                while (arr[high] > pivot)
                    --high;

                if (low != high &&
                    arr[low] == arr[high]) { // = in case something like 4 2 3 4 1 7 4 where pivot value is 4
                    --high;
                }

                if (low >= high) return low;

                std::swap(arr[low], arr[high]);
            }
        }
    };

    template<typename T, PIVOT_POLICY PP, PARTITION_SCHEME PS>
    void _quick_sort(T *arr, int low, int high) {
        if (low < high) {
            T pivot = ChoosePivot<T, PP>::choose_pivot(arr, low, high);
            int p = PartitionScheme<T, PS>::partition(arr, low, high, pivot);
            _quick_sort<T, PP, PS>(arr, low, p - 1);
            _quick_sort<T, PP, PS>(arr, p + 1, high);
        }
    }
}

template<typename T>
void quick_sort_lomuto(T *arr, int n) {
    using namespace internal;
    _quick_sort<T, LAST, LOMUTO>(arr, 0, n - 1);
}

template<typename T>
void quick_sort_hoare_middle(T *arr, int n) {
    using namespace internal;
    _quick_sort<T, MIDDLE, HOARE>(arr, 0, n - 1);
}
