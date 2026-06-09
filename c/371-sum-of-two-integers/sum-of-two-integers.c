// -----------------------------------------------------------------------------
// Problem: Sum of Two Integers (371)
// Source : https://leetcode.com/problems/sum-of-two-integers/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// Compute sum without '+' using bitwise ops:
// - partial sum without carry: a ^ b
// - carry bits: (a & b) << 1
// Iterate until carry is zero.
//
// Time Complexity  : O(1) bounded by word size
// Space Complexity : O(1)
// -----------------------------------------------------------------------------

int getSum(int a, int b) {
    // loop until carry is zero
    while(b != 0){
        unsigned carry = (unsigned)(a & b);
        a ^= b;             // sum without carry
        b = carry << 1;     // propagate carry
    }
    return a;
}