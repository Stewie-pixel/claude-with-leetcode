class Solution {
public:
    long long maxTotal(vector<int>& nums, string s) {
    vector<int> tokens;
    for (int i = 0; i < (int)nums.size(); i++)
        if (s[i] == '1') tokens.push_back(i);

    if (tokens.empty()) return 0;

    long long dp0 = nums[tokens[0]];
    optional<long long> dp1 = (tokens[0] > 0)
        ? optional<long long>(nums[tokens[0] - 1])
        : nullopt;

    for (int j = 1; j < (int)tokens.size(); j++) {
        int t  = tokens[j];
        int tp = tokens[j - 1];

        long long best_prev = dp1 ? max(dp0, *dp1) : dp0;
        long long new_dp0 = best_prev + nums[t];

        optional<long long> new_dp1 = nullopt;
        if (t > 0) {
            long long val = (t - 1 == tp) ? dp0 : dp0 + nums[t - 1];
            if (dp1) val = max(val, *dp1 + nums[t - 1]);
            new_dp1 = val;
        }

        dp0 = new_dp0;
        dp1 = new_dp1;
    }

    return dp1 ? max(dp0, *dp1) : dp0;
    }
};
