// -----------------------------------------------------------------------------
// Problem: Min Cost Climbing Stairs
// Source : https://leetcode.com/problems/min-cost-climbing-stairs/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Task: Given cost per step, compute minimum cost to reach top (beyond last).
// 2. Approach: DP where dp[i] is min cost to reach step i; dp[i] = cost[i] + min(dp[i-1], dp[i-2]).
//
// Time Complexity  : O(n)
// Space Complexity : O(n) (can be optimized to O(1))
// -----------------------------------------------------------------------------

int min(int a, int b){
    return a < b ? a : b;
}

int minCostClimbingStairs(int* cost, int costSize) {
    int *dp = (int *)calloc(costSize + 1, sizeof(int));
    for(int i = 0; i < costSize; i++){
        dp[i] = cost[i];
    }
    for(int i = 2; i <= costSize; i++){
        dp[i] += min(dp[i - 1], dp[i - 2]);
    }
    int res = min(dp[costSize - 1], dp[costSize]);
    free(dp);
    return res;
}