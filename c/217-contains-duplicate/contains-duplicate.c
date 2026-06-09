// Source: https://leetcode.com/problems/contains-duplicate/
// Author: Ngoc-Tien To

// Idea: Use a large hash table (size 2 * 10^9 + 1) to mark each number, including negatives.
// If a number is already marked when encountered, return true (duplicate found).

#define N 1e9

bool containsDuplicate(int* nums, int numsSize) {
    int *count = (int *)calloc(2 * N + 1, sizeof(int));
    for(int i = 0; i < numsSize; i++){
        int index = nums[i] + N;
        if(count[index]){
            free(count);
            return true;
        }
        count[index] = 1;
    }
    free(count);
    return false;
}