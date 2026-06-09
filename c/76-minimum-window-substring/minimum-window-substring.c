// -----------------------------------------------------------------------------
// Problem: Minimum Window Substring
// Source : https://leetcode.com/problems/minimum-window-substring/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- IDEA ----------------------------------
//
// 1. Use the sliding window technique with two pointers (left and right).
// 2. Maintain a frequency count of required characters from t.
// 3. Expand the right pointer to include characters until the window 
//    contains all required characters.
// 4. Then, contract the left pointer to minimize the window while still 
//    containing all required characters.
// 5. Keep track of the smallest window length and starting position.
// 6. Return the substring corresponding to that minimum window.
//
// Time Complexity  : O(|s| + |t|)
// Space Complexity : O(1) — fixed alphabet size (256 possible characters)
// -----------------------------------------------------------------------------

#define MAX 256
#define MAX_INT 1e9

char* minWindow(char* s, char* t) {
    int lenS = strlen(s);
    int lenT = strlen(t);
    if(lenT > lenS) {
        char *empty = (char *)malloc(1);
        empty[0] = '\0';
        return empty;
    }

    int count[MAX] = {0};
    for(int i = 0; i < lenT; i++) {
        count[t[i]]++;
    }
    
    int start = 0;
    int minLength = MAX_INT, left = 0, right = 0, need = lenT;
    while(right < lenS) {
        if(count[s[right]] > 0) {
            need--;
        }
        count[s[right]]--;
        right++;

        while(need == 0) {
            if(right - left < minLength) {
                minLength = right - left;
                start = left;
            }
            count[s[left]]++;
            if(count[s[left]] > 0) {
                need++;
            }
            left++;
        }
    }

    if(minLength == MAX_INT) {
        char *empty = (char *)malloc(1);
        empty[0] = '\0';
        return empty;
    }

    char *result = (char *)malloc(sizeof(char) * (minLength + 1));
    strncpy(result, s + start, minLength);
    result[minLength] = '\0';
    return result;
}
