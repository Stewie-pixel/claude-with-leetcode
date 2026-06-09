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
