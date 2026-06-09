// -----------------------------------------------------------------------------
// Problem: House Robber II
// Source : https://leetcode.com/problems/house-robber-ii/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Task: Houses form a circle. Cannot rob adjacent houses; first and last
//    are adjacent. Solve by reducing to two linear cases (exclude first or last).
// 2. Approach: reuse linear helper `rob1` and take max of two ranges.
//
// Time Complexity  : O(n)
// Space Complexity : O(1)
// -----------------------------------------------------------------------------

int max(int a, int b){
    return a > b ? a : b;
}

int rob1(int* nums, int numsSize) {
    if(numsSize == 0){
        return 0;
    }
    if(numsSize == 1){
        return nums[0];
    }

    int rob1, rob2;
    rob1 = rob2 = 0;
    for(int i = 0; i < numsSize; i++){
        int temp = max(rob1 + nums[i], rob2);
        rob1 = rob2;
        rob2 = temp;
    }

    return rob2;
}

int rob(int* nums, int numsSize) {
    return max(nums[0], max(rob1(nums + 1, numsSize - 1), rob1(nums, numsSize - 1)));
}