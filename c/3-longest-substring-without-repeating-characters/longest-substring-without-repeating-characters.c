int max(int a, int b) {
    return a > b ? a : b;
}

int lengthOfLongestSubstring(char* s) {
    int index[256];
    memset(index, -1, sizeof(index));
    int maxLength = 0;
    int left = 0;

    for (int right = 0; right < strlen(s); right++) {
        char ch = s[right];

        if (index[(unsigned char)ch] >= left) {
            left = index[(unsigned char)ch] + 1;
        }

        index[(unsigned char)ch] = right;

        int currentLength = right - left + 1;
        maxLength = max(maxLength, currentLength);
    }

    return maxLength;
}
