// -----------------------------------------------------------------------------
// Problem: Reverse Bits (190)
// Source : https://leetcode.com/problems/reverse-bits/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// Reverse bit order by iterating over 32 bits, shifting result left and
// appending the least-significant bit of input.
//
// Time Complexity  : O(32) = O(1)
// Space Complexity : O(1)
// -----------------------------------------------------------------------------

int reverseBits(int n) {
    int res = 0;
    // accumulate reversed bits into res
    for(int i = 0; i < 32; i++) {
        // shift accumulated result left
        res <<= 1;
        // append current LSB of n
        res |= (n & 1);
        // move next bit into LSB position
        n >>= 1;
    }
    return res;
}