// -----------------------------------------------------------------------------
// Problem: Longest Palindromic Substring
// Source : https://leetcode.com/problems/longest-palindromic-substring/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Task: Return the longest palindromic substring in `s`.
// 2. Approach: Expand around each center (odd/even centers), track max length
//    and starting index. Copy result into a newly allocated buffer.
//
// Time Complexity  : O(n^2) in worst-case
// Space Complexity : O(n) for output buffer
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>

char* longestPalindrome(char* s) {
    int n = strlen(s);

    int index, maxLen = 0;
    int l, r;
    for(int i = 0; i < n; i++){
        l = r = i;
        while(r + 1 < n && s[r + 1] == s[r]){
            r++;
        }

        while(l >= 0 && r < n && s[l] == s[r]){
            if(r - l + 1 > maxLen){
                maxLen = r - l + 1;
                index = l;
            }
            l--;
            r++;
        }
    }

    char *result = (char *)malloc(sizeof(char) * (maxLen + 1));
    strncpy(result, s + index, maxLen);
    result[maxLen] = '\0';
    return result;
}