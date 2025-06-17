#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

// ========= Sorting Algorithms =========
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for(int i = 0; i < n-1; i++)
        for(int j = 0; j < n-i-1; j++)
            if(arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
}

void merge(vector<int>& arr, int l, int m,int r) {
    vector<int> L(arr.begin() + l, arr.begin() + m + 1);
    vector<int> R(arr.begin() + m + 1, arr.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while(i < L.size() && j < R.size())
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while(i < L.size()) arr[k++] = L[i++];
    while(j < R.size()) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int l, int r) {
    if(l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high], i = low - 1;
    for(int j = low; j < high; j++)
        if(arr[j] <= pivot)
            swap(arr[++i], arr[j]);
    swap(arr[i+1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if(low < high) {
        int p = partition(arr, low, high);
        quickSort(arr, low, p - 1);
        quickSort(arr, p + 1, high);
    }
}

// ========= Data Generation =========
vector<int> generateData(int size, string type) {
    vector<int> data(size);
    if(type == "random") {
        for(int i = 0; i < size; ++i)
            data[i] = rand() % 100000;
    } else if(type == "sorted") {
        for(int i = 0; i < size; ++i)
            data[i] = i;
    } else if(type == "reversed") {
        for(int i = 0; i < size; ++i)
            data[i] = size - i;
    } else if(type == "partially_sorted") {
        for(int i = 0; i < size; ++i)
            data[i] = i;
        for(int i = 0; i < size / 4; ++i)
            swap(data[rand() % size], data[rand() % size]);
    } else if(type == "few_unique") {
        int vals[] = {10, 20, 30, 40, 50};
        for(int i = 0; i < size; ++i)
            data[i] = vals[rand() % 5];
    } else if(type == "nearly_sorted") {
        for(int i = 0; i < size; ++i)
            data[i] = i;
        for(int i = 0; i < size / 20; ++i)
            swap(data[rand() % size], data[rand() % size]);
    } else if(type == "single_value") {
        for(int i = 0; i < size; ++i)
            data[i] = 5;
    } else if(type == "zigzag") {
        for(int i = 0; i < size; ++i)
            data[i] = (i % 2 == 0) ? i / 2 : size - i / 2;
    } else if(type == "ascending_with_noise") {
        for(int i = 0; i < size; ++i)
            data[i] = i;
        for(int i = 0; i < size / 100; ++i)
            swap(data[rand() % size], data[rand() % size]);
    } else if(type == "descending_with_duplicates") {
        for(int i = 0; i < size; ++i)
            data[i] = (size - i) / 10;
    }
    return data;
}

// ========= Timing Functions =========
double averageTime(void (*sortFunc)(vector<int>&), vector<int> original, int runs = 5) {
    double total = 0;
    for(int i = 0; i < runs; ++i) {
        vector<int> copy = original;
        clock_t start = clock();
        sortFunc(copy);
        clock_t end = clock();
        total += double(end - start) / CLOCKS_PER_SEC;
    }
    return total / runs;
}

double averageTimeRecursive(void (*sortFunc)(vector<int>&, int, int), vector<int> original, int runs = 5) {
    double total = 0;
    for(int i = 0; i < runs; ++i) {
        vector<int> copy = original;
        clock_t start = clock();
        sortFunc(copy, 0, copy.size() - 1);
        clock_t end = clock();
        total += double(end - start) / CLOCKS_PER_SEC;
    }
    return total / runs;
}

// ========= Main =========
int main() {
    srand(time(0));
    vector<pair<string, int>> tests = {
        {"random", 1000},
        {"sorted", 5000},
        {"reversed", 10000},
        {"few_unique", 2000},
        {"partially_sorted", 8000},
        {"nearly_sorted", 6000},
        {"single_value", 3000},
        {"zigzag", 7000},
        {"ascending_with_noise", 4000},
        {"descending_with_duplicates", 1000}
    };

    cout << "Algorithm\tSize\tType\t\t\tAverage Time (s)\n";
    cout << "--------------------------------------------------------------\n";

    for(auto& test : tests) {
        string type = test.first;
        int size = test.second;

        vector<int> data = generateData(size, type);

        double t1 = averageTime(bubbleSort, data);
        double t2 = averageTimeRecursive(mergeSort, data);
        double t3 = averageTimeRecursive(quickSort, data);

        cout << "BubbleSort\t" << size << "\t" << type << "\t\t" << t1 << endl;
        cout << "MergeSort\t" << size << "\t" << type << "\t\t" << t2 << endl;
        cout << "QuickSort\t" << size << "\t" << type << "\t\t" << t3 << endl;
        cout << "--------------------------------------------------------------\n";
    }

    return 0;
}
