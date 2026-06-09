// -----------------------------------------------------------------------------
// Problem: Climbing Stairs
// Source : https://leetcode.com/problems/climbing-stairs/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Task: Count number of distinct ways to climb n stairs taking 1 or 2 steps.
// 2. Approach: simple DP — the recurrence is ways[n] = ways[n-1] + ways[n-2],
//    which is Fibonacci-like. Use two rolling variables to achieve O(1) space.
//
// Time Complexity  : O(n)
// Space Complexity : O(1)
// -----------------------------------------------------------------------------

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