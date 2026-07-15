class Solution {
public:
    long long getSum(vector<int>& nums) {
        int n = nums.size();
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) prefix[i + 1] = prefix[i] + nums[i];

        vector<int> t(2 * n + 1, -1);
        for (int i = 0; i < n; i++) t[2 * i + 1] = nums[i];

        int m = t.size();
        vector<int> rad(m, 0);
        int center = 0, right = 0;
        for (int i = 0; i < m; i++) {
            if (i < right) rad[i] = min(right - i, rad[2 * center - i]);
            while (i - rad[i] - 1 >= 0 && i + rad[i] + 1 < m &&
                   t[i - rad[i] - 1] == t[i + rad[i] + 1]) {
                rad[i]++;
            }
            if (i + rad[i] > right) { center = i; right = i + rad[i]; }
        }

        long long best = LLONG_MIN;
        for (int i = 0; i < m; i++) {
            int len = rad[i];
            int l = i - len, r = i + len;
            if (l % 2 == 0) { l++; len--; r--; }
            if (len < 0) continue;
            int origL = l / 2, origR = r / 2;
            if (origL > origR) continue;
            long long sum = prefix[origR + 1] - prefix[origL];
            best = max(best, sum);
        }

        return best;
    }
};