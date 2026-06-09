// -----------------------------------------------------------------------------
// Problem: Palindromic Substrings
// Source : https://leetcode.com/problems/palindromic-substrings/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Task: Count all palindromic substrings in a given string.
// 2. Approach: Expand around each center for odd and even length palindromes
//    and count matches.
//
// Time Complexity  : O(n^2)
// Space Complexity : O(1)
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>

int countSubstrings(char* s) {
    int n = strlen(s);
    int result = 0;
    int l, r;
    for(int i = 0; i < n; i++){
        l = r = i;
        while(l >= 0 && r < n && s[l] == s[r]){
            result++;
            l--;
            r++;
        }

        l = i; r = i + 1;
        while(l >= 0 && r < n && s[l] == s[r]){
            result++;
            l--;
            r++;
        }
    }
    
    return result;
}