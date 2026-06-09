// -----------------------------------------------------------------------------
// Problem: Maximum Product Subarray
// Source : https://leetcode.com/problems/maximum-product-subarray/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Task: Find contiguous subarray with maximum product.
// 2. Approach: Track both max and min product ending at current position since
//    a negative number can swap them. Update global max accordingly.
//
// Time Complexity  : O(n)
// Space Complexity : O(1)
// -----------------------------------------------------------------------------

#include <stdlib.h>

int maxNum(int a, int b){
    return a > b ? a : b;
}

int minNum(int a, int b){
    return a > b ? b : a;
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int maxProduct(int* nums, int numsSize) {
    int minPro = 1;
    int maxPro = 1;
    int result = -1e9;
    for(int i = 0; i < numsSize; i++){
        if(nums[i] < 0){
            swap(&minPro, &maxPro);
        }
        maxPro = maxNum(nums[i], nums[i] * maxPro);
        minPro = minNum(nums[i], nums[i] * minPro);
        result = maxNum(result, maxPro);
    }
    return result;
}