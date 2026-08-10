class Solution {
    int dfs(int i, int m, vector<int>& piles, unordered_map<int, int>& memo) {
        int n = piles.size();
        if (i + m * 2 >= n)
            return piles[i];

        int key = (i << 8) | m;
        if (memo.count(key))
            return memo[key];

        int res = 2000000000;
        for (int k = 1; k <= m * 2; k++)
            res = min(res, dfs(i + k, max(m, k), piles, memo));

        memo[key] = piles[i] - res;

        return memo[key];
    }

public:
    int stoneGameII(vector<int>& piles) {
        int n = piles.size();
        for (int i = n - 2; i >= 0; i--)
            piles[i] += piles[i + 1];

        unordered_map<int, int> memo;

        return dfs(0, 1, piles, memo);
    }
};