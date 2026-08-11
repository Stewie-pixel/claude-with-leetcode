class Solution {
public:
    int kConcatenationMaxSum(vector<int>& arr, int k) {
        const long long MOD = 1000000007;

        auto kadane = [&](const vector<int>& a) {
            long long cur = 0, best = 0;
            for (long long x : a) {
                cur = max(0LL, cur + x);
                best = max(best, cur);
            }
            return best;
        };

        long long total = 0;
        for (int x : arr) total += x;

        long long kad1 = kadane(arr);

        long long prefix = 0, cur = 0;
        for (int x : arr) {
            cur += x;
            prefix = max(prefix, cur);
        }

        long long suffix = 0;
        cur = 0;
        for (int i = arr.size() - 1; i >= 0; i--) {
            cur += arr[i];
            suffix = max(suffix, cur);
        }

        if (k == 1) return kad1 % MOD;
        if (total <= 0) return max(kad1, prefix + suffix) % MOD;

        return (prefix + suffix + (k - 2) * total) % MOD;
    }
};