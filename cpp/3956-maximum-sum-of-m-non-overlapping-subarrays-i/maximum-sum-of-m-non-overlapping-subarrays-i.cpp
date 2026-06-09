class Solution {
public:
    long long maximumSum(vector<int>& nums, int m, int l, int r) {
        int n = nums.size();
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++)
            prefix[i + 1] = prefix[i] + nums[i];

        vector<optional<long long>> prev(n + 1, 0LL), curr(n + 1, nullopt);
        optional<long long> ans = nullopt;

        for (int j = 1; j <= m; j++) {
            fill(curr.begin(), curr.end(), nullopt);
            deque<int> dq;

            for (int i = 1; i <= n; i++) {
                int k = i - l;
                if (k >= 0 && prev[k]) {
                    long long val = *prev[k] - prefix[k];
                    while (!dq.empty() && *prev[dq.back()] - prefix[dq.back()] <= val)
                        dq.pop_back();
                    dq.push_back(k);
                }

                while (!dq.empty() && dq.front() < i - r)
                    dq.pop_front();

                curr[i] = curr[i - 1];
                if (!dq.empty()) {
                    long long candidate = prefix[i] + *prev[dq.front()] - prefix[dq.front()];
                    curr[i] = curr[i] ? max(*curr[i], candidate) : candidate;
                }
            }

            if (curr[n])
                ans = ans ? max(*ans, *curr[n]) : curr[n];
            swap(prev, curr);
        }

        return *ans;
    }
};
