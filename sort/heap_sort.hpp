#include <algorithm>

/**
 * Make tree rooted at arr[i] a max-heap
 * @param arr Array representation of a heap
 * @param n Number of elements
 * @param i Root node to heapify
 *
 * @note This assumes the children of arr[i] are already heapified
 */
template<typename T>
void heapify(T *arr, int n, int i) {
    int largest = i; // the index of the largest node in {arr[i], left child, right child}
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest])
        largest = l;

    if (r < n && arr[r] > arr[largest])
        largest = r;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

template<typename T>
void heap_sort(T *arr, int n) {
    // repeatedly heapify sub-tree, starting from the last non-leaf node
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);

    // the region after i is sorted, before i is the heap
    // | tree | a[n-3] | a[n-2] | a[n-1] |
    //         --------------------------
    //                   ^ sorted
    for (int i = n - 1; i >= 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}
