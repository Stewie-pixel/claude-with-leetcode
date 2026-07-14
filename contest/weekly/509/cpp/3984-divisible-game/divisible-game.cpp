class Solution {
public:
    int divisibleGame(vector<int>& nums) {
        int MOD = 1e9 + 7;
        int n = nums.size();
        set<long long> candidates;
        candidates.insert(2);

        for (int num : nums) {
            for (long long d = 1; d * d <= num; d++) {
                if (num % d == 0) {
                    if (d > 1) candidates.insert(d);
                    long long other = num / d;
                    if (other > 1) candidates.insert(other);
                }
            }
        }

        long long bestDiff = LLONG_MIN;
        long long bestK = 2;

        for (long long k : candidates) {
            long long curr = 0, maxSum = LLONG_MIN;
            for (int i = 0; i < n; i++) {
                long long v = (nums[i] % k == 0) ? nums[i] : -nums[i];
                curr = max(v, curr + v);
                maxSum = max(maxSum, curr);
            }
            if (maxSum > bestDiff) {
                bestDiff = maxSum;
                bestK = k;
            }
        }

        long long diffMod = ((bestDiff % MOD) + MOD) % MOD;
        long long kMod = bestK % MOD;
        long long ans = (diffMod * kMod) % MOD;
        return (int)ans;
    }
};