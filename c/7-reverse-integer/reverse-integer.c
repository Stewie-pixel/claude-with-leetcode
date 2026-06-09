// -----------------------------------------------------------------------------
// Problem: Reverse Integer (7)
// Source : https://leetcode.com/problems/reverse-integer/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// Pop digits from x and push into result r while checking overflow conditions.
// Use 64-bit temp or careful boundary checks to detect overflow and return 0.
//
// Time Complexity  : O(log10(|x|))
// Space Complexity : O(1)
// -----------------------------------------------------------------------------

#define LIM1 (-2147483648)
#define LIM2 2147483647

int reverse(int x){
    int res = 0;
    while(x != 0){
        if(res < (LIM1 / 10) || res > (LIM2 / 10)){
            return 0;
        }
        res = res * 10 + x % 10;
        x /= 10;
    }
    return res;
}