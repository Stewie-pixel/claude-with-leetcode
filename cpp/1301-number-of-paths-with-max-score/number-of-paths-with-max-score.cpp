class Solution {
public:
    vector<int> pathsWithMaxScore(vector<string>& board) {
        int n = board.size();
        const int MOD = 1e9 + 7;
        vector<vector<long long>> dp(n, vector<long long>(n, -1));
        vector<vector<long long>> cnt(n, vector<long long>(n, 0));

        dp[n-1][n-1] = 0;
        cnt[n-1][n-1] = 1;

        for (int i = n - 1; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                if (i == n-1 && j == n-1) continue;
                if (board[i][j] == 'X') continue;

                long long best = -1, ways = 0;

                int di[3] = {1, 0, 1};
                int dj[3] = {0, 1, 1};

                for (int d = 0; d < 3; d++) {
                    int pi = i + di[d], pj = j + dj[d];
                    if (pi >= n || pj >= n) continue;
                    if (dp[pi][pj] == -1) continue;

                    if (dp[pi][pj] > best) {
                        best = dp[pi][pj];
                        ways = cnt[pi][pj];
                    } else if (dp[pi][pj] == best) {
                        ways = (ways + cnt[pi][pj]) % MOD;
                    }
                }

                if (best == -1) continue;

                int val = (board[i][j] == 'E') ? 0 : (board[i][j] - '0');
                dp[i][j] = best + val;
                cnt[i][j] = ways;
            }
        }

        if (dp[0][0] == -1) return {0, 0};
        return {(int)dp[0][0], (int)cnt[0][0]};
    }
};