#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;


const int TEST_RUNS = 5;        
const int MAX_SIZE = 30000;    
const int STEP_SIZE = 5000;     
const int NUM_TESTS = 5;         


void generate_data(int arr[], int size, string data_type) {
    if (data_type == "random") {
        for (int i = 0; i < size; i++) {
            arr[i] = rand() % 1000000;
        }
    }
    else if (data_type == "sorted") {
        for (int i = 0; i < size; i++) {
            arr[i] = i + 1;
        }
    }
    else if (data_type == "reverse") {
        for (int i = 0; i < size; i++) {
            arr[i] = size - i;
        }
    }
}

// Bubble Sort
void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Merge Sort
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void merge_sort(int arr[], int l, int r) {
    if (l >= r) return;
    int m = l + (r - l)/2;
    merge_sort(arr, l, m);
    merge_sort(arr, m+1, r);
    merge(arr, l, m, r);
}

// Quick Sort 
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

double measure_time(void (*sort_func)(int[], int), int original[], int size) {
    double total_time = 0;
    int* test_arr = new int[size];
    
    for (int i = 0; i < TEST_RUNS; i++) {
        copy(original, original + size, test_arr);
        
        clock_t start = clock();
        sort_func(test_arr, size);
        clock_t end = clock();
        
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }
    
    delete[] test_arr;
    return total_time / TEST_RUNS;
}

double measure_time_recursive(void (*sort_func)(int[], int, int), int original[], int size) {
    double total_time = 0;
    int* test_arr = new int[size];
    
    for (int i = 0; i < TEST_RUNS; i++) {
        copy(original, original + size, test_arr);
        
        clock_t start = clock();
        sort_func(test_arr, 0, size-1);
        clock_t end = clock();
        
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }
    
    delete[] test_arr;
    return total_time / TEST_RUNS;
}

void save_results(const string& filename, const int sizes[], 
                 const double bubble[], const double merge[], const double quick[]) {
    ofstream file(filename);
    file << "Size,Bubble Sort,Merge Sort,Quick Sort\n";
    for (int i = 0; i < NUM_TESTS; i++) {
        file << sizes[i] << "," << fixed << setprecision(6)
             << bubble[i] << "," << merge[i] << "," << quick[i] << "\n";
    }
    file.close();
    cout << "Results saved to " << filename << endl;
}

int main() {
    srand(time(0)); 
    
    int test_sizes[NUM_TESTS];
    for (int i = 0; i < NUM_TESTS; i++) {
        test_sizes[i] = (i+1) * STEP_SIZE;
    }
    
    const string data_types[] = {"random", "sorted", "reverse"};
    
    double bubble_results[NUM_TESTS];
    double merge_results[NUM_TESTS];
    double quick_results[NUM_TESTS];
    
    for (const string& type : data_types) {
        cout << "\n\nPerformance analysis for " << type << " data:\n";
        cout << "Size\tBubble Sort\tMerge Sort\tQuick Sort\n";
        
        for (int i = 0; i < NUM_TESTS; i++) {
            int size = test_sizes[i];
            int* data = new int[size];
            generate_data(data, size, type);
            
            bubble_results[i] = measure_time(bubble_sort, data, size);
            merge_results[i] = measure_time_recursive(merge_sort, data, size);
            quick_results[i] = measure_time_recursive(quick_sort, data, size);
            
            cout << size << "\t" << fixed << setprecision(4)
                 << bubble_results[i] << "s\t\t"
                 << merge_results[i] << "s\t\t"
                 << quick_results[i] << "s\n";
            
            delete[] data;
        }
        
        string filename = "results_" + type + ".csv";
        save_results(filename, test_sizes, bubble_results, merge_results, quick_results);
    }
    
    cout << "\n\nTheoretical Analysis:\n";
    cout << "Algorithm\tBest Case\tAverage Case\tWorst Case\tStable\n";
    cout << "Bubble Sort\tO(n)\t\tO(n^2)\t\tO(n^2)\t\tYes\n";
    cout << "Merge Sort\tO(n log n)\tO(n log n)\tO(n log n)\tYes\n";
    cout << "Quick Sort\tO(n log n)\tO(n log n)\tO(n^2)\t\tNo\n";

    cout << "\nConclusions:\n";
    cout << "1. Bubble Sort is impractical for large datasets\n";
    cout << "2. Merge Sort provides consistent performance\n";
    cout << "3. Quick Sort is fastest in most practical cases\n";
    cout << "4. For pre-sorted data, Bubble Sort performs better\n";
    
    return 0;
}