// Source: https://leetcode.com/problems/product-of-array-except-self/
// Author: Ngoc-Tien To

// Idea:
// 1. Prefix variable: stores the product of all elements before index i in the array.
// 2. Suffix variable: stores the product of all elements after index i in the array.
// 3. Result at index i: is the product of the prefix and suffix at that position.

int* productExceptSelf(int* nums, int numsSize, int* returnSize) {
    int *answer = (int *)malloc(sizeof(int) * numsSize);
    (*returnSize) = numsSize;
    int prefix = 1;
    for(int i = 0; i < numsSize; i++){
        answer[i] = prefix;
        prefix *= nums[i];
    }
    int suffix = 1;
    for(int i = numsSize - 1; i >= 0; i--){
        answer[i] *= suffix;
        suffix *= nums[i];
    }
    return answer;
}
