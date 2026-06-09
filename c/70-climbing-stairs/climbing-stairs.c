int climbStairs(int n) {
    if(n == 1) {
        return 1;
    }
    int dp0, dp1, result;
    dp0 = 0;
    dp1 = 1;
    for(int i = 0; i < n; i++) {
        result = dp0 + dp1;
        dp0 = dp1;
        dp1 = result;
    }
    return result;
}
