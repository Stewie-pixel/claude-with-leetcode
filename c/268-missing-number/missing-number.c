// -----------------------------------------------------------------------------
// Problem: Missing Number (268)
// -----------------------------------------------------------------------------
//
// IDEA
// XOR all indices 0..n and all numbers in nums. Pairs cancel and XOR leaves
// the missing number.
//
// Time Complexity  : O(n)
// Space Complexity : O(1)
// -----------------------------------------------------------------------------

int missingNumber(int* nums, int numsSize) {
    int x = 0;
    for (int i = 0; i < numsSize; ++i) {
        x ^= nums[i] ^ i;
    }
    x ^= numsSize; // account for the final index
    return x;
}