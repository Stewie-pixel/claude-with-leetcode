class Solution {
public:
    int maxConsistentColumns(vector<vector<int>>& grid, int limit) {
        int m = grid.size();
        int n = grid[0].size();
        
        vector<int> dp(n, 1);
        int ans = 1;
        
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < j; i++) {
                bool ok = true;
                for (int r = 0; r < m; r++) {
                    if (abs(grid[r][j] - grid[r][i]) > limit) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    dp[j] = max(dp[j], dp[i] + 1);
                }
            }
            ans = max(ans, dp[j]);
        }
        
        return ans;
    }
};