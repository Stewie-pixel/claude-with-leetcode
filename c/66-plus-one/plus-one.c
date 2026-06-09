#include <stdlib.h>

int* plusOne(int* digits, int digitsSize, int* returnSize) {
    int n = digitsSize;
    int *ans = (int *)malloc(sizeof(int) * (n + 1));

    for(int i = 0; i < n; i++){
        ans[i] = digits[i];
    }

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

    ans[0] = 1;
    for(int i = 1; i <= n; i++){
        ans[i] = 0;
    }
    *returnSize = n + 1;

    return ans;
}
