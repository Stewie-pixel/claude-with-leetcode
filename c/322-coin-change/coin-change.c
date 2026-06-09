// -----------------------------------------------------------------------------
// Problem: Coin Change
// Source : https://leetcode.com/problems/coin-change/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Task: Given coin denominations, compute minimum number of coins to make
//    a target amount.
// 2. Approach: DP over amounts — dp[x] = min(dp[x], dp[x - coin] + 1). Initialize
//    dp[0] = 0 and others to a large sentinel (amount+1).
// 3. Implementation notes: sort coins first (optional) and iterate coins
//    to update dp table.
//
// Time Complexity  : O(n * amount)
// Space Complexity : O(amount)
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>

int cmp(const void *a, const void *b){
    return *(int *)a - *(int *)b;
}

int coinChange(int* coins, int coinsSize, int amount) {
    int n = coinsSize;
    int *dp = (int *)malloc(sizeof(int) * (amount + 1));
    for(int i = 0; i <= amount; i++){
        dp[i] = amount + 1;
    }
    dp[0] = 0;
    qsort(coins, n, sizeof(int), cmp);
    for(int i = 0; i < n; i++){
        for(int j = 0; j <= amount; j++){
            if(j >= coins[i]){
                dp[j] = dp[j] < dp[j - coins[i]] + 1 ? dp[j] : dp[j - coins[i]] + 1;
            }
        }
    }
    return dp[amount] == amount + 1 ? -1 : dp[amount];
}