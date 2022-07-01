#include <algorithm>

template<typename T>
int partition(T *arr, int low, int high) {
    T pivot = arr[high];  // taking the last element as pivot
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

template<typename T>
void quick_sort(T *arr, int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);
        quick_sort(arr, low, p - 1);
        quick_sort(arr, p + 1, high);
    }
}

template<typename T>
void quick_sort(T *arr, int n) { quick_sort(arr, 0, n - 1); }
