// -----------------------------------------------------------------------------
// Problem: Pow(x, n) (50)
// Source : https://leetcode.com/problems/powx-n/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// Use fast exponentiation (divide and conquer): compute x^(n/2) recursively
// and square it. Handle negative exponent by taking reciprocal.
//
// Time Complexity  : O(log n)
// Space Complexity : O(log n) recursion depth
// -----------------------------------------------------------------------------

// no special headers required; this file implements fast exponentiation
// getPow: helper for myPow
double getPow(double x, int n){
    if(n == 0){
        return (double)1;
    }
    double temp = getPow(x, n / 2);
    if(n % 2 == 1){
        return x * temp * temp;
    }
    return temp * temp;
}

double myPow(double x, int n) {
    if(n >= 0){
        return getPow(x, n);
    }
    return (double)1 / (getPow(x, -n - 1) * getPow(x, 1));
}