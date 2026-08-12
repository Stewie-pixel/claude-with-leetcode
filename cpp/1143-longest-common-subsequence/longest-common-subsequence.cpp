class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        vector<int> dp(text1.length(), 0);
        int longest = 0;

        for (char c : text2) {
            int cur = 0;
            for (int i = 0; i < dp.size(); i++) {
                if (cur < dp[i]) {
                    cur =  dp[i];
                } else if (c == text1[i]) {
                    dp[i] = cur + 1;
                    longest = max(longest, cur + 1);
                }
            }
        }
        return longest;
    }
};