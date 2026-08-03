class Solution {
public:
    bool predictTheWinner(vector<int>& A) {
        int size = A.size();
        vector<int> dp(size);

        for (int i = size - 1; i >= 0; i--) {
            dp[i] = A[i];
            for (int j = i + 1; j < size; j++)
                dp[j] = max(A[i] - dp[j], A[j] - dp[j - 1]);
        }

        return dp[size - 1] >= 0;
    }
};