// -----------------------------------------------------------------------------
// Problem: Counting Bits (338)
// -----------------------------------------------------------------------------
//
// IDEA
// Use DP: ans[i] = ans[i & (i - 1)] + 1 which removes the lowest set bit.
// Alternatively ans[i] = ans[i >> 1] + (i & 1) — both are valid.
//
// Time Complexity  : O(n)
// Space Complexity : O(n)
// -----------------------------------------------------------------------------

#include <stdlib.h>

int* countBits(int n, int* returnSize) {
    int *ans = (int *)malloc(sizeof(int) * (n + 1));
    *returnSize = n + 1;
    ans[0] = 0;
    for(int i = 1; i <= n; i++){
        ans[i] = (i % 2) + ans[i >> 1];
    }
    return ans;
}