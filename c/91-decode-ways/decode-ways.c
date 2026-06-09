// -----------------------------------------------------------------------------
// Problem: Decode Ways
// Source : https://leetcode.com/problems/decode-ways/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Task: Count the number of ways to decode a digit string where '1'->'A',
//    '2'->'B', ..., '26'->'Z'.
// 2. Approach: DP over prefix length. dp[i] = ways to decode first i chars.
//    Transition: if single digit valid add dp[i-1], if two-digit valid add dp[i-2].
//
// Time Complexity  : O(n)
// Space Complexity : O(n) (can be reduced to O(1))
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>

int numDecodings(char* s) {
    int n = strlen(s);
    if(n == 0 || s[0] == '0'){
        return 0;
    }

    int *dp = (int *)calloc(n + 1, sizeof(int));
    dp[0] = dp[1] = 1;
    for(int i = 2; i <= n; i++){
        int num1 = s[i - 1] - 48;
        int num2 = (s[i - 2] - 48) * 10 + s[i - 1] - 48;

        if(num1 != 0){
            dp[i] += dp[i - 1];
        }
        if(num2 >= 10 && num2 <= 26){
            dp[i] += dp[i - 2];
        }
    }

    int count = dp[n];
    free(dp);
    return count;
}