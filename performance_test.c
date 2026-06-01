#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long bubble_compare_count = 0;
long long merge_compare_count = 0;
long long quick_compare_count = 0;

void generate_random_array(int arr[], int n) {
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
    
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        merge_compare_count++;
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    
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
    int i = low - 1;
    for (int j = low; j < high; j++) {
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
    return i + 1;
}

void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

typedef struct {
    int weight;
    double value;
} Item;

double dynamic_programming_knapsack(Item items[], int n, int capacity) {
    int **dp = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int*)calloc(capacity + 1, sizeof(int));
    }
    
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            dp[i][w] = dp[i - 1][w];
            if (items[i - 1].weight <= w) {
                int val = dp[i - 1][w - items[i - 1].weight] + (int)(items[i - 1].value * 100);
                if (val > dp[i][w]) dp[i][w] = val;
            }
        }
    }
    
    double result = dp[n][capacity] / 100.0;
    for (int i = 0; i <= n; i++) free(dp[i]);
    free(dp);
    return result;
}

double greedy_knapsack(Item items[], int n, int capacity) {
    int *selected = (int*)calloc(n, sizeof(int));
    double total_value = 0;
    int remaining = capacity;
    
    for (int i = 0; i < n; i++) {
        if (items[i].weight <= remaining) {
            selected[i] = 1;
            remaining -= items[i].weight;
            total_value += items[i].value;
        }
    }
    
    free(selected);
    return total_value;
}

void test_sort_performance() {
    int sizes[] = {100, 500, 1000, 2000, 5000, 10000, 20000};
    int size_count = sizeof(sizes) / sizeof(sizes[0]);
    
    printf("=== Sort Algorithm Performance Test ===\n");
    printf("Size\tBubbleTime(ms)\tBubbleComparisons\tMergeTime(ms)\tMergeComparisons\tQuickTime(ms)\tQuickComparisons\n");
    
    for (int i = 0; i < size_count; i++) {
        int n = sizes[i];
        int *arr = (int*)malloc(n * sizeof(int));
        int *arr_copy1 = (int*)malloc(n * sizeof(int));
        int *arr_copy2 = (int*)malloc(n * sizeof(int));
        
        generate_random_array(arr, n);
        copy_array(arr_copy1, arr, n);
        copy_array(arr_copy2, arr, n);
        
        clock_t start, end;
        
        start = clock();
        bubble_sort(arr, n);
        end = clock();
        double bubble_time = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
        long long bubble_cmp = bubble_compare_count;
        
        start = clock();
        merge_compare_count = 0;
        merge_sort(arr_copy1, 0, n - 1);
        end = clock();
        double merge_time = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
        long long merge_cmp = merge_compare_count;
        
        start = clock();
        quick_compare_count = 0;
        quick_sort(arr_copy2, 0, n - 1);
        end = clock();
        double quick_time = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
        long long quick_cmp = quick_compare_count;
        
        printf("%d\t%.2f\t\t%lld\t\t\t%.2f\t\t%lld\t\t\t%.2f\t\t%lld\n",
               n, bubble_time, bubble_cmp, merge_time, merge_cmp, quick_time, quick_cmp);
        
        free(arr);
        free(arr_copy1);
        free(arr_copy2);
    }
}

void test_knapsack_performance() {
    int capacities[] = {1000, 5000, 10000};
    int item_counts[] = {50, 100, 200, 500, 1000};
    
    printf("\n=== Knapsack Algorithm Performance Test ===\n");
    printf("Capacity\tItems\tDPTime(ms)\tGreedyTime(ms)\n");
    
    for (int cap = 0; cap < 3; cap++) {
        int capacity = capacities[cap];
        for (int i = 0; i < 5; i++) {
            int n = item_counts[i];
            Item *items = (Item*)malloc(n * sizeof(Item));
            
            for (int j = 0; j < n; j++) {
                items[j].weight = rand() % 100 + 1;
                items[j].value = (rand() % 90000 + 10000) / 100.0;
            }
            
            clock_t start, end;
            
            start = clock();
            dynamic_programming_knapsack(items, n, capacity);
            end = clock();
            double dp_time = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
            
            start = clock();
            greedy_knapsack(items, n, capacity);
            end = clock();
            double greedy_time = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
            
            printf("%d\t\t%d\t%.2f\t\t%.2f\n", capacity, n, dp_time, greedy_time);
            
            free(items);
        }
    }
}

int main() {
    srand(time(NULL));
    
    test_sort_performance();
    test_knapsack_performance();
    
    return 0;
}