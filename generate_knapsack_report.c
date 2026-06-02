#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int index;
    int weight;
    double value;
} Item;

void generate_items(Item items[], int n) {
    srand(12345);
    for (int i = 0; i < n; i++) {
        items[i].index = i + 1;
        items[i].weight = rand() % 100 + 1;
        items[i].value = (rand() % 90000 + 10000) / 100.0;
    }
}

int main() {
    int n = 1000;
    Item *items = (Item*)malloc(n * sizeof(Item));
    generate_items(items, n);
    
    // 生成详细物品信息文件
    FILE *fp_items = fopen("knapsack_items_1000.csv", "w");
    if (!fp_items) {
        printf("无法创建文件\n");
        return 1;
    }
    
    fprintf(fp_items, "物品编号,物品重量,物品价值\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp_items, "%d,%d,%.2f\n", items[i].index, items[i].weight, items[i].value);
    }
    fclose(fp_items);
    printf("已生成 knapsack_items_1000.csv - 包含1000个物品的详细信息\n");
    
    // 计算统计信息
    int total_weight = 0;
    double total_value = 0.0;
    int min_weight = 1000, max_weight = 0;
    double min_value = 10000.0, max_value = 0.0;
    int count_weight_range[10] = {0};
    int count_value_range[10] = {0};
    
    for (int i = 0; i < n; i++) {
        total_weight += items[i].weight;
        total_value += items[i].value;
        
        if (items[i].weight < min_weight) min_weight = items[i].weight;
        if (items[i].weight > max_weight) max_weight = items[i].weight;
        if (items[i].value < min_value) min_value = items[i].value;
        if (items[i].value > max_value) max_value = items[i].value;
        
        // 统计重量分布
        int weight_bin = items[i].weight / 10;
        if (weight_bin >= 10) weight_bin = 9;
        count_weight_range[weight_bin]++;
        
        // 统计价值分布
        int value_bin = (int)(items[i].value / 100);
        if (value_bin >= 10) value_bin = 9;
        count_value_range[value_bin]++;
    }
    
    // 生成统计报告
    FILE *fp_report = fopen("knapsack_statistics_1000.txt", "w");
    if (!fp_report) {
        printf("无法创建统计报告文件\n");
        return 1;
    }
    
    fprintf(fp_report, "========================================\n");
    fprintf(fp_report, "     0-1背包问题 1000个物品统计报告       \n");
    fprintf(fp_report, "========================================\n\n");
    
    fprintf(fp_report, "【基本统计信息】\n");
    fprintf(fp_report, "----------------------------------------\n");
    fprintf(fp_report, "物品总数: %d\n", n);
    fprintf(fp_report, "总重量: %d\n", total_weight);
    fprintf(fp_report, "总价值: %.2f\n", total_value);
    fprintf(fp_report, "\n");
    fprintf(fp_report, "重量范围: %d ~ %d\n", min_weight, max_weight);
    fprintf(fp_report, "价值范围: %.2f ~ %.2f\n", min_value, max_value);
    fprintf(fp_report, "\n");
    fprintf(fp_report, "平均重量: %.2f\n", (double)total_weight / n);
    fprintf(fp_report, "平均价值: %.2f\n", total_value / n);
    fprintf(fp_report, "\n");
    
    fprintf(fp_report, "【重量分布统计】\n");
    fprintf(fp_report, "----------------------------------------\n");
    fprintf(fp_report, "重量区间\t物品数量\t占比\n");
    fprintf(fp_report, "----------------------------------------\n");
    for (int i = 0; i < 10; i++) {
        int start = i * 10 + 1;
        int end = (i + 1) * 10;
        if (end > 100) end = 100;
        double percentage = (double)count_weight_range[i] / n * 100;
        fprintf(fp_report, "%2d-%2d\t\t%d\t\t%.1f%%\n", start, end, count_weight_range[i], percentage);
    }
    fprintf(fp_report, "\n");
    
    fprintf(fp_report, "【价值分布统计】\n");
    fprintf(fp_report, "----------------------------------------\n");
    fprintf(fp_report, "价值区间\t物品数量\t占比\n");
    fprintf(fp_report, "----------------------------------------\n");
    for (int i = 0; i < 10; i++) {
        int start = i * 100 + 100;
        int end = (i + 1) * 100;
        if (end > 1000) end = 1000;
        double percentage = (double)count_value_range[i] / n * 100;
        fprintf(fp_report, "%3d-%3d\t\t%d\t\t%.1f%%\n", start, end, count_value_range[i], percentage);
    }
    fprintf(fp_report, "\n");
    
    fprintf(fp_report, "【前20个物品详情】\n");
    fprintf(fp_report, "----------------------------------------\n");
    fprintf(fp_report, "编号\t重量\t价值\t价值密度\n");
    fprintf(fp_report, "----------------------------------------\n");
    for (int i = 0; i < 20 && i < n; i++) {
        double density = items[i].value / items[i].weight;
        fprintf(fp_report, "%d\t%d\t%.2f\t%.4f\n", items[i].index, items[i].weight, items[i].value, density);
    }
    
    fclose(fp_report);
    printf("已生成 knapsack_statistics_1000.txt - 统计报告\n");
    
    // 输出到控制台（表格形式）
    printf("\n========================================\n");
    printf("     0-1背包问题 1000个物品统计报告       \n");
    printf("========================================\n\n");
    printf("【基本统计信息】\n");
    printf("物品总数: %d\n", n);
    printf("总重量: %d\n", total_weight);
    printf("总价值: %.2f\n", total_value);
    printf("\n【表格1: 前10个物品信息】\n");
    printf("+----------+----------+----------+\n");
    printf("| 物品编号 | 物品重量 | 物品价值 |\n");
    printf("+----------+----------+----------+\n");
    for (int i = 0; i < 10 && i < n; i++) {
        printf("| %8d | %8d | %8.2f |\n", items[i].index, items[i].weight, items[i].value);
    }
    printf("+----------+----------+----------+\n");
    printf("\n完整数据请查看文件: knapsack_items_1000.csv\n");
    printf("统计报告请查看文件: knapsack_statistics_1000.txt\n");
    
    free(items);
    return 0;
}