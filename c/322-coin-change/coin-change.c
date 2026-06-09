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
