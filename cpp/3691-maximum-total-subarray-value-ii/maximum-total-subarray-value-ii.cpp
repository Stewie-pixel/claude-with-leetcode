class Solution {
public:
    long long maxTotalValue(vector<int>& nums, int k) {
        int n = nums.size();

        vector<vector<int>> max_sp = {nums}, min_sp = {nums};
        for (int j = 1; (1 << j) <= n; j++) {
            int half = 1 << (j - 1), size = n - (1 << j) + 1;
            max_sp.push_back(vector<int>(size));
            min_sp.push_back(vector<int>(size));
            for (int i = 0; i < size; i++) {
                max_sp[j][i] = max(max_sp[j-1][i], max_sp[j-1][i + half]);
                min_sp[j][i] = min(min_sp[j-1][i], min_sp[j-1][i + half]);
            }
        }

        auto query = [&](int l, int r) -> int {
            if (l == r) return 0;
            int p = 31 - __builtin_clz(r - l + 1);
            int i2 = r - (1 << p) + 1;
            return max(max_sp[p][l], max_sp[p][i2]) - min(min_sp[p][l], min_sp[p][i2]);
        };

        using T = tuple<int, int, int>;
        priority_queue<T> heap;
        unordered_set<long long> seen;

        heap.emplace(query(0, n - 1), 0, n - 1);
        seen.insert(n - 1);

        long long total = 0;
        while (k--) {
            auto [v, l, r] = heap.top();
            heap.pop();
            total += v;

            if (l < r) {
                for (auto [nl, nr] : vector<pair<int,int>>{{l, r-1}, {l+1, r}}) {
                    long long key = (long long)nl * n + nr;
                    if (!seen.count(key)) {
                        seen.insert(key);
                        heap.emplace(query(nl, nr), nl, nr);
                    }
                }
            }
        }

        return total;
    }
};