// -----------------------------------------------------------------------------
// Problem: Number of 1 Bits (191)
// Source : https://leetcode.com/problems/number-of-1-bits/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// Count set bits by repeatedly clearing the lowest-set bit: n &= (n - 1).
// Each loop iteration removes one set bit until n becomes 0.
//
// Time Complexity  : O(k) where k is number of set bits (<= 32)
// Space Complexity : O(1)
// -----------------------------------------------------------------------------

int hammingWeight(int n) {
    int count = 0;
    // clear the lowest-set bit repeatedly until number becomes zero
    while(n){
        n &= (n - 1);
        count++;
    }
    return count;
}