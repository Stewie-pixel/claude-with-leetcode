// -----------------------------------------------------------------------------
// Problem: Plus One (66)
// Source : https://leetcode.com/problems/plus-one/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// Simulate decimal addition from least-significant digit upwards, handling
// carry and, if needed, enlarge the array when the input is all 9s.
//
// Time Complexity  : O(n)
// Space Complexity : O(n) for the possible resized output
// -----------------------------------------------------------------------------

#include <stdlib.h>

// plusOne: add one to a decimal integer represented by array of digits
int* plusOne(int* digits, int digitsSize, int* returnSize) {
    int n = digitsSize;
    int *ans = (int *)malloc(sizeof(int) * (n + 1));

    for(int i = 0; i < n; i++){
        ans[i] = digits[i];
    }
    // add one with carry propagation
    for(int i = n - 1; i >= 0; i--){
        if(ans[i] == 9){
            ans[i] = 0;
        }
        else{
            ans[i]++;
            *returnSize = n;
            return ans;
        }
    }

    // all digits were 9 -> expand array with leading 1 and trailing zeros
    ans[0] = 1;
    for(int i = 1; i <= n; i++){
        ans[i] = 0;
    }
    *returnSize = n + 1;

    return ans;
}