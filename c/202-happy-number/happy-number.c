// -----------------------------------------------------------------------------
// Problem: Happy Number (202)
// Source : https://leetcode.com/problems/happy-number/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// Repeatedly replace n by the sum of the squares of its digits. If the
// process reaches 1, n is happy. If it reaches a known cycle (e.g., 89),
// it's not happy. This implementation checks for known cycle values.
//
// Time Complexity  : O(k) where k is steps until cycle detection
// Space Complexity : O(1)
// -----------------------------------------------------------------------------

#include <stdbool.h>

int square(int a){
    return a * a;
}

// sum of squares of digits
int sumDigits(int n){
    int res = 0;
    while(n){
        res += square(n % 10);
        n /= 10;
    }
    return res;
}



// main predicate: detect happy number
bool isHappy(int n) {
    if(n == 1 || n == 7){
        return true;
    }
    else if(n < 10){
        return false;
    }
    while(1){
        if(n == 89){
            return false;
        }
        else if(n == 1){
            return true;
        }
        n = sumDigits(n);
    }
    return n == 1 || n == 7;
}