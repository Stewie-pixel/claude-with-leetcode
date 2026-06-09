// -----------------------------------------------------------------------------
// Problem: Longest Repeating Character Replacement
// Source : https://leetcode.com/problems/longest-repeating-character-replacement/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Use the Sliding Window technique.
// 2. Maintain a window [left, right] representing the current substring.
// 3. Track the count of each character within the window.
// 4. Keep track of the most frequent character’s count (`maxFrequency`).
// 5. If (window length - maxFrequency) > k → too many replacements → shrink from left.
// 6. Otherwise, expand the window by moving `right`.
// 7. Keep track of the maximum valid window size.
//
//  Time Complexity  : O(n)
//  Space Complexity : O(1)   (only 26 letters)
// -----------------------------------------------------------------------------


int max(int a, int b){
    return a > b ? a : b;
}

int characterReplacement(char* s, int k) {
    int maxFrequency = 0;                    // 1. Track highest frequency character in current window
    int n = strlen(s);
    int result = 0;                          // 2. Store maximum valid window length
    int *count = (int *)calloc('Z' - 'A' + 1, sizeof(int));  // 3. Frequency count for 26 uppercase letters
    int left = 0;

    // 4. Expand window by moving right pointer
    for(int right = 0; right < n; right++){
        count[(unsigned char)(s[right] - 'A')]++;
        maxFrequency = max(maxFrequency, count[(unsigned char)(s[right] - 'A')]);

        // 5. If window needs more than k changes → shrink from the left
        while((right - left + 1) - maxFrequency > k){
            count[(unsigned char)(s[left] - 'A')]--;
            left++;
        }

        // 6. Update the result with the maximum valid window length
        result = max(result, right - left + 1);
    }

    // 7. Cleanup and return
    free(count);
    return result;
}
