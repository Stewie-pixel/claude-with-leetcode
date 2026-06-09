// -----------------------------------------------------------------------------
// Problem: Single Number (136)
// Source : https://leetcode.com/problems/single-number/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// Use XOR accumulation: paired numbers cancel and the unique element remains.
//
// Time Complexity  : O(n)
// Space Complexity : O(1)
// -----------------------------------------------------------------------------

int singleNumber(int* nums, int numsSize) {
    int check = 0;
    // XOR accumulation cancels paired numbers; leftover is the single one
    for(int i = 0; i < numsSize; i++){
        check ^= nums[i];
    }
    return check;
}