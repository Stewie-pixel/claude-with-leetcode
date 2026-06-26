class Solution {
public:
    long long countMajoritySubarrays(vector<int>& nums, int target) {
        int n = nums.size();
        vector<int> P(n + 1, 0);
        for (int i = 0; i < n; i++)
            P[i + 1] = P[i] + (nums[i] == target ? 1 : -1);

        vector<int> sortedVals = P;
        sort(sortedVals.begin(), sortedVals.end());
        sortedVals.erase(unique(sortedVals.begin(), sortedVals.end()), sortedVals.end());

        int size = sortedVals.size();
        vector<int> bit(size + 1, 0);

        auto update = [&](int i) {
            for (; i <= size; i += i & (-i)) bit[i]++;
        };
        auto query = [&](int i) {
            long long s = 0;
            for (; i > 0; i -= i & (-i)) s += bit[i];
            return s;
        };

        long long ans = 0;
        for (int j = 0; j <= n; j++) {
            int r = lower_bound(sortedVals.begin(), sortedVals.end(), P[j]) - sortedVals.begin() + 1;
            ans += query(r - 1);
            update(r);
        }
        return ans;
    }
};