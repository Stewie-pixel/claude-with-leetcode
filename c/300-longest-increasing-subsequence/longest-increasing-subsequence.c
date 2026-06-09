// -----------------------------------------------------------------------------
// Problem: Longest Increasing Subsequence
// Source : https://leetcode.com/problems/longest-increasing-subsequence/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Task: Find length of LIS in an array of integers.
// 2. Approach: patience sorting technique — maintain an array `tails` where
//    tails[i] = smallest tail value of an increasing subsequence of length i+1.
//    Use binary search to place each number in tails; result is number of piles.
//
// Time Complexity  : O(n log n)
// Space Complexity : O(n)
// -----------------------------------------------------------------------------

#include <stdlib.h>

void upperBound(int *result, int left, int right, int target){
    while(left + 1 < right){
        int mid = (left + right) / 2;
        if(result[mid] < target){
            left = mid;
        }
        else{
            right = mid;
        }
    }
    result[right] = target;
}

int lengthOfLIS(int* nums, int numsSize) {
    if(numsSize <= 0){
        return 0;
    }
    int *result = (int *)malloc(sizeof(int) * numsSize);
    int index = 0;
    for(int i = 0; i < numsSize; i++){
        if(index == 0 || nums[i] > result[index - 1]){
            result[index++] = nums[i];
        }
        else{
            if(nums[i] > result[0]){
                upperBound(result, 0, index - 1, nums[i]);
            }
            else{
                result[0] = nums[i];
            }
        }
    }
    free(result);
    return index;
}