#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;
using namespace std::chrono;

const int NUM_RUNS = 5; 

void generate_data(int arr[], int size, const string& data_type) {
    if (data_type == "random") {
        for (int i = 0; i < size; i++) {
            arr[i] = rand() % 100000 + 1;
        }
    } else if (data_type == "sorted") {
        for (int i = 0; i < size; i++) {
            arr[i] = i + 1;
        }
    } else if (data_type == "reverse") {
        for (int i = 0; i < size; i++) {
            arr[i] = size - i;
        }
    } else if (data_type == "partial") {
        for (int i = 0; i < size; i++) {
            arr[i] = i + 1;
        }
        for (int i = 0; i < size / 10; i++) {
            swap(arr[rand() % size], arr[rand() % size]);
        }
    }
}

// Bubble Sort
void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Merge Sort
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Quick Sort
int partition(int arr[], int low, int high) {
    int pivot = arr[high], i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            swap(arr[++i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

double measure_time(void (*sort_func)(int[], int), int arr[], int size) {
    auto start = high_resolution_clock::now();
    sort_func(arr, size);
    auto stop = high_resolution_clock::now();
    return duration_cast<microseconds>(stop - start).count() / 1e6;
}

// قياس الوقت للـ void sort(int[], int, int)
double measure_time(void (*sort_func)(int[], int, int), int arr[], int l, int r) {
    auto start = high_resolution_clock::now();
    sort_func(arr, l, r);
    auto stop = high_resolution_clock::now();
    return duration_cast<microseconds>(stop - start).count() / 1e6;
}

int main() {
    srand(time(0));
    const int sizes[] = {100, 1000, 10000};
    const char* types[] = {"random", "sorted", "reverse", "partial"};

    cout << "Sorting Algorithm Average Execution Time (over " << NUM_RUNS << " runs)\n";
    cout << "Size\tType\t\tBubble Sort\tMerge Sort\tQuick Sort\n";

    for (int size : sizes) {
        int* original = new int[size];

        for (const char* type : types) {
            double bubble_sum = 0, merge_sum = 0, quick_sum = 0;

            for (int run = 0; run < NUM_RUNS; run++) {
                generate_data(original, size, type);

                int* bubble_arr = new int[size];
                int* merge_arr = new int[size];
                int* quick_arr = new int[size];
                copy(original, original + size, bubble_arr);
                copy(original, original + size, merge_arr);
                copy(original, original + size, quick_arr);

                bubble_sum += measure_time(bubble_sort, bubble_arr, size);
                merge_sum  += measure_time(merge_sort, merge_arr, 0, size - 1);
                quick_sum  += measure_time(quick_sort, quick_arr, 0, size - 1);

                delete[] bubble_arr;
                delete[] merge_arr;
                delete[] quick_arr;
            }

            double bubble_avg = bubble_sum / NUM_RUNS;
            double merge_avg = merge_sum / NUM_RUNS;
            double quick_avg = quick_sum / NUM_RUNS;

            cout << size << "\t" << type << "\t" << fixed << setprecision(6)
                 << bubble_avg << " s\t" << merge_avg << " s\t" << quick_avg << " s\n";
        }

        delete[] original;
    }

    return 0;
}
