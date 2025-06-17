#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cstring>
using namespace std;

const int NUM_RUNS = 5;
const int NUM_SIZES = 4;
const int sizes[NUM_SIZES] = {100, 1000, 10000, 20000};

const int NUM_TYPES = 10;
const char* types[NUM_TYPES] = {
    "random", "sorted", "reverse", "partial", "same",
    "even", "odd", "duplicates", "unique", "zigzag"
};

// دالة توليد البيانات حسب النوع
void generate_data(int arr[], int size, const char* type) {
    if (strcmp(type, "random") == 0) {
        for (int i = 0; i < size; ++i)
            arr[i] = rand() % 100000;
    }
    else if (strcmp(type, "sorted") == 0) {
        for (int i = 0; i < size; ++i)
            arr[i] = i;
    }
    else if (strcmp(type, "reverse") == 0) {
        for (int i = 0; i < size; ++i)
            arr[i] = size - i;
    }
    else if (strcmp(type, "partial") == 0) {
        for (int i = 0; i < size / 2; ++i)
            arr[i] = i;
        for (int i = size / 2; i < size; ++i)
            arr[i] = rand() % 100000;
    }
    else if (strcmp(type, "same") == 0) {
        for (int i = 0; i < size; ++i)
            arr[i] = 1;
    }
    else if (strcmp(type, "even") == 0) {
        for (int i = 0; i < size; ++i)
            arr[i] = (rand() % 50000) * 2;
    }
    else if (strcmp(type, "odd") == 0) {
        for (int i = 0; i < size; ++i)
            arr[i] = (rand() % 50000) * 2 + 1;
    }
    else if (strcmp(type, "duplicates") == 0) {
        int vals[] = {5, 10, 20, 30, 40};
        for (int i = 0; i < size; ++i)
            arr[i] = vals[rand() % 5];
    }
    else if (strcmp(type, "unique") == 0) {
        for (int i = 0; i < size; ++i)
            arr[i] = i * 3;
    }
    else if (strcmp(type, "zigzag") == 0) {
        int low = 1, high = size * 2;
        for (int i = 0; i < size; ++i)
            arr[i] = (i % 2 == 0) ? low++ : high--;
    }
}

// Bubble Sort
void bubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; ++i)
        for (int j = 0; j < size - i - 1; ++j)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

// Merge Sort
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int* L = new int[n1], *R = new int[n2];
    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];
    for (int i = 0; i < n2; ++i) R[i] = arr[m + 1 + i];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    delete[] L;
    delete[] R;
}
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Quick Sort
int partition(int arr[], int low, int high) {
    int pivot = arr[high], i = low - 1;
    for (int j = low; j < high; ++j)
        if (arr[j] < pivot)
            swap(arr[++i], arr[j]);
    swap(arr[i + 1], arr[high]);
    return i + 1;
}
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// قياس الوقت
double measure_time_bubble(void (*sort_func)(int[], int), int arr[], int size) {
    clock_t start = clock();
    sort_func(arr, size);
    clock_t end = clock();
    return double(end - start) / CLOCKS_PER_SEC;
}
double measure_time_other(void (*sort_func)(int[], int, int), int arr[], int l, int r) {
    clock_t start = clock();
    sort_func(arr, l, r);
    clock_t end = clock();
    return double(end - start) / CLOCKS_PER_SEC;
}

int main() {
    srand(time(0));
    cout << "Sorting Algorithm Average Execution Time (over " << NUM_RUNS << " runs)\n";
    cout << "Size\tType\t\tBubble Sort\tMerge Sort\tQuick Sort\n";

    for (int s = 0; s < NUM_SIZES; s++) {
        int size = sizes[s];
        int* original = new int[size];

        for (int t = 0; t < NUM_TYPES; t++) {
            const char* type = types[t];
            double bubble_sum = 0, merge_sum = 0, quick_sum = 0;

            for (int run = 0; run < NUM_RUNS; run++) {
                generate_data(original, size, type);

                int* bubble_arr = new int[size];
                int* merge_arr = new int[size];
                int* quick_arr = new int[size];

                memcpy(bubble_arr, original, size * sizeof(int));
                memcpy(merge_arr, original, size * sizeof(int));
                memcpy(quick_arr, original, size * sizeof(int));

                bubble_sum += measure_time_bubble(bubbleSort, bubble_arr, size);
                merge_sum  += measure_time_other(mergeSort, merge_arr, 0, size - 1);
                quick_sum  += measure_time_other(quickSort, quick_arr, 0, size - 1);

                delete[] bubble_arr;
                delete[] merge_arr;
                delete[] quick_arr;
            }

            double bubble_avg = bubble_sum / NUM_RUNS;
            double merge_avg = merge_sum / NUM_RUNS;
            double quick_avg = quick_sum / NUM_RUNS;

            cout << size << "\t" << type << "\t\t" << fixed << setprecision(6)
                 << bubble_avg << " s\t" << merge_avg << " s\t" << quick_avg << " s\n";
        }

        delete[] original;
    }

    return 0;
}
