// -----------------------------------------------------------------------------
// Problem: Multiply Strings (43)
// Source : https://leetcode.com/problems/multiply-strings/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// Multiply two non-negative numeric strings by simulating long multiplication
// using an integer array to store intermediate digit sums, then convert to string.
//
// Time Complexity  : O(n1 * n2)
// Space Complexity : O(n1 + n2)
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// multiply: multiply two decimal numeric strings and return product string
char* multiply(char* num1, char* num2) {
    int n1 = strlen(num1);
    int n2 = strlen(num2);
    char *ans = (char *)malloc(sizeof(char) * 2);
    if((n1 == 1 && num1[0] == '0') || (n2 == 1 && num2[0] == '0')){
        ans[0] = '0';
        ans[1] = '\0';
        return ans;
    }

    int *res = (int *)calloc(n1 + n2, sizeof(int));
    // multiply digits (long multiplication), store results in res[]
    for(int i = n1 - 1; i >= 0; i--){
        for(int j = n2 - 1; j >= 0; j--){
            int mul = (num1[i] - '0') * (num2[j] - '0');
            int sum = mul + res[i + j + 1];
            res[i + j + 1] = sum % 10;
            res[i + j] += sum / 10;
        }
        // debug: print intermediate row (left as-is)
        for(int j = 0; j < n1 + n2; j++){
            printf("%d ", res[j]);
        }
        printf("\n");
    }

    int i = 0;
    // skip leading zeros in the result array
    while(i < n1 + n2 && res[i] == 0){
        i++;
    }
    
    // allocate enough chars for the result string and null terminator
    ans = (char *)realloc(ans, sizeof(char) * (n2 + n1 - i + 2));
    int size = 0;
    while(i < n1 + n2){
        ans[size++] = res[i++] + '0';
    }
    ans[size] = '\0';
    free(res);

    return ans;
}