class Solution {
public:
    int minCost(string source, string target, vector<vector<string>>& rules, vector<int>& costs) {
        int n = source.size();
        if ((int)target.size() != n) return -1;

        vector<vector<pair<int, int>>> edgesByEnd(n + 1);

        for (int idx = 0; idx < (int)rules.size(); idx++) {
            const string& pattern = rules[idx][0];
            const string& replacement = rules[idx][1];
            int L = pattern.size();
            if (L > n) continue;
            int wildcards = count(pattern.begin(), pattern.end(), '*');
            int costI = costs[idx] + wildcards;

            for (int l = 0; l + L <= n; l++) {
                bool ok = true;
                for (int j = 0; j < L; j++) {
                    char c = pattern[j];
                    if (c != '*' && c != source[l + j]) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) continue;
                for (int j = 0; j < L; j++) {
                    if (target[l + j] != replacement[j]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) edgesByEnd[l + L].push_back({l, costI});
            }
        }

        const long long INF = LLONG_MAX / 2;
        vector<long long> dp(n + 1, INF);
        dp[0] = 0;

        for (int i = 1; i <= n; i++) {
            long long best = INF;
            if (source[i-1] == target[i-1] && dp[i-1] < best) best = dp[i-1];
            for (auto& [start, cost] : edgesByEnd[i]) {
                if (dp[start] + cost < best) best = dp[start] + cost;
            }
            dp[i] = best;
        }
        return dp[n] >= INF ? -1 : (int)dp[n];
    }
};