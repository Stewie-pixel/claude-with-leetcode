class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        int S = accumulate(nums.begin(), nums.end(), 0);
        if ((S + target) % 2 != 0 || S < abs(target)) {
            return 0;
        }

        int P = (S + target) / 2;

        vector<int> dp(P + 1, 0);
        dp[0] = 1;

        for (int n : nums) {
            for (int x = P; x >= n; x--) {
                dp[x] += dp[x - n];
            }
        }
        return dp[P];
    }
};