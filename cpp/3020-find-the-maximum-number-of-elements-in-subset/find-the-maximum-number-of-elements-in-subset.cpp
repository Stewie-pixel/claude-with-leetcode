class Solution {
public:
    int maximumLength(vector<int>& nums) {
        unordered_map<long long, int> count;
        for (int x : nums) count[x]++;

        int ans = 1;

        if (count.count(1)) {
            int c = count[1];
            ans = (c % 2 == 1) ? c : c - 1;
        }

        for (auto& [x, _] : count) {
            if (x == 1) continue;

            int pairs = 0;
            long long cur = x;

            while (count.count(cur) && count[cur] >= 2) {
                pairs++;
                cur = cur * cur;
            }

            if (count.count(cur) && count[cur] >= 1)
                ans = max(ans, 2 * pairs + 1);
            else if (pairs > 0)
                ans = max(ans, 2 * pairs - 1);
        }

        return ans;
    }
};