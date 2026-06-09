// -----------------------------------------------------------------------------
// Problem: Longest Substring Without Repeating Characters
// Source : https://leetcode.com/problems/longest-substring-without-repeating-characters/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------


// ---------------------------------- IDEA -------------------------------------
//
// 1. Use the Sliding Window technique with two pointers (left, right).
// 2. Maintain a window [left, right] that always has unique characters.
// 3. Use an array `index[256]` to store the last seen position of each character.
// 4. When we encounter a repeated character s[right],
//      → move the left pointer just after its previous position.
// 5. Update the last seen position of s[right] to the current index.
// 6. Compute the current window length and update the maximum length found.
//
//  Time Complexity  : O(n)
//  Space Complexity : O(1)   (only 256 ASCII characters)
//
// -----------------------------------------------------------------------------


int max(int a, int b) {
    return a > b ? a : b;
}

int lengthOfLongestSubstring(char* s) {
    int index[256];                          // 1. Store the last seen index of each character
    memset(index, -1, sizeof(index));        // 2. Initialize all positions to -1 (not seen yet)
    int maxLength = 0;
    int left = 0;                            // 3. Start of the current window

    for (int right = 0; right < strlen(s); right++) {
        char ch = s[right];

        // 4. If character already seen within current window → move left pointer
        if (index[(unsigned char)ch] >= left) {
            left = index[(unsigned char)ch] + 1;
        }

        // 5. Update the last seen position of current character
        index[(unsigned char)ch] = right;

        // 6. Calculate the current window length and update maxLength
        int currentLength = right - left + 1;
        maxLength = max(maxLength, currentLength);
    }

    return maxLength;
}
