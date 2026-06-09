class Solution {
public:
    vector<string> generateValidStrings(int n, int k) {
        vector<string> res;
        string cur(n, '0');

        function<void(int, int, bool)> backtrack = [&](int pos, int cost, bool prevOne) {
            if (cost > k) return;
            if (pos == n) {
                res.push_back(cur);
                return;
            }
            cur[pos] = '0';
            backtrack(pos + 1, cost, false);

            if (!prevOne) {
                cur[pos] = '1';
                backtrack(pos + 1, cost + pos, true);
            }
        };

        backtrack(0, 0, false);
        return res;
    }
};
