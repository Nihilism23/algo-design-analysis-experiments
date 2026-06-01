#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

typedef struct {
    int weight;
    double value;
    int index;
} Item;

void generate_items(Item items[], int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        items[i].weight = rand() % 100 + 1;
        items[i].value = (rand() % 90000 + 10000) / 100.0;
        items[i].index = i + 1;
    }
}

int cmp_by_value_density(const void *a, const void *b) {
    Item *itemA = (Item*)a;
    Item *itemB = (Item*)b;
    double densityA = itemA->value / itemA->weight;
    double densityB = itemB->value / itemB->weight;
    if (densityB > densityA) return 1;
    if (densityB < densityA) return -1;
    return 0;
}

double brute_force_knapsack(Item items[], int n, int capacity, int *selected) {
    double max_value = 0;
    int max_mask = 0;
    
    for (int mask = 0; mask < (1 << n); mask++) {
        int total_weight = 0;
        double total_value = 0;
        
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                total_weight += items[i].weight;
                if (total_weight > capacity) break;
                total_value += items[i].value;
            }
        }
        
        if (total_weight <= capacity && total_value > max_value) {
            max_value = total_value;
            max_mask = mask;
        }
    }
    
    for (int i = 0; i < n; i++) {
        selected[i] = (max_mask & (1 << i)) ? 1 : 0;
    }
    
    return max_value;
}

double dynamic_programming_knapsack(Item items[], int n, int capacity, int *selected) {
    int **dp = (int**)malloc((n + 1) * sizeof(int*));
    int **keep = (int**)malloc((n + 1) * sizeof(int*));
    
    for (int i = 0; i <= n; i++) {
        dp[i] = (int*)malloc((capacity + 1) * sizeof(int));
        keep[i] = (int*)malloc((capacity + 1) * sizeof(int));
        for (int j = 0; j <= capacity; j++) {
            dp[i][j] = 0;
            keep[i][j] = 0;
        }
    }
    
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            dp[i][w] = dp[i - 1][w];
            if (items[i - 1].weight <= w) {
                int value_with = dp[i - 1][w - items[i - 1].weight] + (int)(items[i - 1].value * 100);
                if (value_with > dp[i][w]) {
                    dp[i][w] = value_with;
                    keep[i][w] = 1;
                }
            }
        }
    }
    
    for (int i = 0; i < n; i++) {
        selected[i] = 0;
    }
    
    int w = capacity;
    for (int i = n; i >= 1; i--) {
        if (keep[i][w] == 1) {
            selected[i - 1] = 1;
            w -= items[i - 1].weight;
        }
    }
    
    double result = dp[n][capacity] / 100.0;
    
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
        free(keep[i]);
    }
    free(dp);
    free(keep);
    
    return result;
}

double greedy_knapsack(Item items[], int n, int capacity, int *selected) {
    Item *sorted_items = (Item*)malloc(n * sizeof(Item));
    for (int i = 0; i < n; i++) {
        sorted_items[i] = items[i];
    }
    
    qsort(sorted_items, n, sizeof(Item), cmp_by_value_density);
    
    int *original_selected = (int*)calloc(n, sizeof(int));
    int remaining_capacity = capacity;
    double total_value = 0;
    
    for (int i = 0; i < n; i++) {
        if (sorted_items[i].weight <= remaining_capacity) {
            original_selected[sorted_items[i].index - 1] = 1;
            remaining_capacity -= sorted_items[i].weight;
            total_value += sorted_items[i].value;
        }
    }
    
    for (int i = 0; i < n; i++) {
        selected[i] = original_selected[i];
    }
    
    free(sorted_items);
    free(original_selected);
    return total_value;
}

double backtracking_knapsack(Item items[], int n, int capacity, int *selected, 
                             int current, int current_weight, double current_value,
                             double *max_value, int *best_selection) {
    if (current == n) {
        if (current_value > *max_value) {
            *max_value = current_value;
            for (int i = 0; i < n; i++) {
                best_selection[i] = selected[i];
            }
        }
        return *max_value;
    }
    
    if (current_weight + items[current].weight <= capacity) {
        selected[current] = 1;
        backtracking_knapsack(items, n, capacity, selected, current + 1,
                              current_weight + items[current].weight,
                              current_value + items[current].value,
                              max_value, best_selection);
    }
    
    selected[current] = 0;
    backtracking_knapsack(items, n, capacity, selected, current + 1,
                          current_weight, current_value, max_value, best_selection);
    
    return *max_value;
}

double solve_backtracking(Item items[], int n, int capacity, int *selected) {
    int *temp_selected = (int*)calloc(n, sizeof(int));
    int *best_selection = (int*)calloc(n, sizeof(int));
    double max_value = 0;
    
    backtracking_knapsack(items, n, capacity, temp_selected, 0, 0, 0, &max_value, best_selection);
    
    for (int i = 0; i < n; i++) {
        selected[i] = best_selection[i];
    }
    
    free(temp_selected);
    free(best_selection);
    return max_value;
}

void test_knapsack(int item_counts[], int count, int capacities[], int cap_count) {
    SetConsoleOutputCP(65001);
    
    for (int cap_idx = 0; cap_idx < cap_count; cap_idx++) {
        int capacity = capacities[cap_idx];
        printf("\n背包容量: %d\n", capacity);
        printf("========================================\n");
        printf("物品数量\t蛮力法\t\t动态规划\t贪心\t\t回溯\n");
        printf("========================================\n");
        
        for (int i = 0; i < count; i++) {
            int n = item_counts[i];
            if (n > 20 && cap_idx == 0) {
                printf("%d\t\t-\t\t", n);
            } else {
                Item *items = (Item*)malloc(n * sizeof(Item));
                generate_items(items, n);
                
                int *selected = (int*)calloc(n, sizeof(int));
                double result;
                
                if (n <= 20) {
                    clock_t start = clock();
                    result = brute_force_knapsack(items, n, capacity, selected);
                    clock_t end = clock();
                    printf("%d\t\t%.2fms\t\t", n, (double)(end - start) * 1000 / CLOCKS_PER_SEC);
                } else {
                    printf("%d\t\t-\t\t", n);
                }
                
                clock_t start_dp = clock();
                result = dynamic_programming_knapsack(items, n, capacity, selected);
                clock_t end_dp = clock();
                printf("%.2fms\t\t", (double)(end_dp - start_dp) * 1000 / CLOCKS_PER_SEC);
                
                clock_t start_greedy = clock();
                result = greedy_knapsack(items, n, capacity, selected);
                clock_t end_greedy = clock();
                printf("%.2fms\t\t", (double)(end_greedy - start_greedy) * 1000 / CLOCKS_PER_SEC);
                
                if (n <= 20) {
                    clock_t start_back = clock();
                    result = solve_backtracking(items, n, capacity, selected);
                    clock_t end_back = clock();
                    printf("%.2fms", (double)(end_back - start_back) * 1000 / CLOCKS_PER_SEC);
                } else {
                    printf("-");
                }
                
                free(items);
                free(selected);
            }
            printf("\n");
        }
        printf("========================================\n");
    }
}

void print_selected_items(Item items[], int n, int *selected) {
    printf("Selected items:\n");
    int total_weight = 0;
    double total_value = 0;
    for (int i = 0; i < n; i++) {
        if (selected[i]) {
            printf("Item %d: weight=%d, value=%.2f\n", 
                   items[i].index, items[i].weight, items[i].value);
            total_weight += items[i].weight;
            total_value += items[i].value;
        }
    }
    printf("Total: weight=%d, value=%.2f\n", total_weight, total_value);
}

int main() {
    int n = 5;
    int capacity = 10;
    Item items[] = {
        {2, 6.00, 1},
        {2, 3.00, 2},
        {6, 5.00, 3},
        {5, 4.00, 4},
        {4, 6.00, 5}
    };
    
    printf("Test case: n=%d, capacity=%d\n", n, capacity);
    printf("Items:\n");
    for (int i = 0; i < n; i++) {
        printf("Item %d: weight=%d, value=%.2f\n", 
               items[i].index, items[i].weight, items[i].value);
    }
    
    int *selected = (int*)calloc(n, sizeof(int));
    
    printf("\n--- Brute Force ---\n");
    double bf_value = brute_force_knapsack(items, n, capacity, selected);
    print_selected_items(items, n, selected);
    
    printf("\n--- Dynamic Programming ---\n");
    double dp_value = dynamic_programming_knapsack(items, n, capacity, selected);
    print_selected_items(items, n, selected);
    
    printf("\n--- Greedy ---\n");
    double greedy_value = greedy_knapsack(items, n, capacity, selected);
    print_selected_items(items, n, selected);
    
    printf("\n--- Backtracking ---\n");
    double back_value = solve_backtracking(items, n, capacity, selected);
    print_selected_items(items, n, selected);
    
    free(selected);
    return 0;
}