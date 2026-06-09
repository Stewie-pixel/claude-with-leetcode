class Solution {
public:
    long long maximumSum(vector<int>& nums, int m, int l, int r) {
        int n = nums.size();
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++)
            prefix[i + 1] = prefix[i] + nums[i];

        auto solve = [&](long long c) -> pair<long long, long long> {
            vector<long long> dp(n + 1, 0), count(n + 1, 0);
            deque<int> dq;

            for (int i = 1; i <= n; i++) {
                int k = i - l;
                if (k >= 0) {
                    long long vk = dp[k] - prefix[k];
                    while (!dq.empty()) {
                        int b = dq.back();
                        long long vb = dp[b] - prefix[b];
                        if (vb < vk || (vb == vk && count[b] < count[k]))
                            dq.pop_back();
                        else break;
                    }
                    dq.push_back(k);
                }

                while (!dq.empty() && dq.front() < i - r)
                    dq.pop_front();

                dp[i] = dp[i - 1];
                count[i] = count[i - 1];

                if (!dq.empty()) {
                    int f = dq.front();
                    long long cand = prefix[i] - c + dp[f] - prefix[f];
                    long long cand_count = count[f] + 1;
                    if (cand > dp[i] || (cand == dp[i] && cand_count > count[i])) {
                        dp[i] = cand;
                        count[i] = cand_count;
                    }
                }
            }
            return {dp[n], count[n]};
        };

        auto [g0, k0] = solve(0);

        if (k0 == 0) {
            long long best = LLONG_MIN;
            deque<int> dq;
            for (int i = l; i <= n; i++) {
                int j = i - l;
                while (!dq.empty() && prefix[dq.back()] > prefix[j])
                    dq.pop_back();
                dq.push_back(j);
                while (!dq.empty() && dq.front() < i - r) dq.pop_front();
                best = max(best, prefix[i] - prefix[dq.front()]);
            }
            return best;
        }

        if (k0 <= m) return g0;

        long long low = 0, high = (long long)2e10;
        while (low < high) {
            long long mid = low + (high - low + 1) / 2;
            if (solve(mid).second >= m) low = mid;
            else high = mid - 1;
        }

        auto [g, k] = solve(low);
        return g + low * m;
    }
};
