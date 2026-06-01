#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

long long bubble_compare_count = 0;
long long merge_compare_count = 0;
long long quick_compare_count = 0;

void generate_random_array(int arr[], int n, int seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100000;
    }
}

void copy_array(int dest[], int src[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

void bubble_sort(int arr[], int n) {
    bubble_compare_count = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            bubble_compare_count++;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        merge_compare_count++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
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
    
    free(L);
    free(R);
}

void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    
    for (int j = low; j <= high - 1; j++) {
        quick_compare_count++;
        if (arr[j] <= pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

void test_sort_algorithms(int sizes[], int size_count) {
    printf("Sort Algorithm Comparison Count\n");
    printf("========================================\n");
    printf("Size\t\tBubble\t\tMerge\t\tQuick\n");
    printf("========================================\n");
    
    for (int i = 0; i < size_count; i++) {
        int n = sizes[i];
        int *arr = (int*)malloc(n * sizeof(int));
        int *arr_copy1 = (int*)malloc(n * sizeof(int));
        int *arr_copy2 = (int*)malloc(n * sizeof(int));
        
        generate_random_array(arr, n, time(NULL));
        copy_array(arr_copy1, arr, n);
        copy_array(arr_copy2, arr, n);
        
        bubble_sort(arr, n);
        merge_compare_count = 0;
        merge_sort(arr_copy1, 0, n - 1);
        quick_compare_count = 0;
        quick_sort(arr_copy2, 0, n - 1);
        
        printf("%d\t\t%lld\t\t%lld\t\t%lld\n", 
               n, bubble_compare_count, merge_compare_count, quick_compare_count);
        
        free(arr);
        free(arr_copy1);
        free(arr_copy2);
    }
    
    printf("========================================\n");
}

int main() {
    int sizes[] = {10, 100, 1000, 2000, 5000, 10000};
    int size_count = sizeof(sizes) / sizeof(sizes[0]);
    
    test_sort_algorithms(sizes, size_count);
    
    return 0;
}