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
