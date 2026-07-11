class Solution {
public:
    vector<int> pathExistenceQueries(int n, vector<int>& nums, int maxDiff, vector<vector<int>>& queries) {
        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b){ return nums[a] < nums[b]; });

        vector<int> sortedVal(n), pos(n);
        for (int k = 0; k < n; k++) {
            sortedVal[k] = nums[order[k]];
            pos[order[k]] = k;
        }

        vector<int> comp(n, 0);
        for (int k = 1; k < n; k++)
            comp[k] = (sortedVal[k] - sortedVal[k-1] <= maxDiff) ? comp[k-1] : comp[k-1] + 1;

        vector<int> right(n);
        int r = 0;
        for (int k = 0; k < n; k++) {
            if (r < k) r = k;
            while (r + 1 < n && sortedVal[r+1] - sortedVal[k] <= maxDiff) r++;
            right[k] = r;
        }

        int LOG = max(1, (int)ceil(log2(max(2, n))) + 1);
        vector<vector<int>> up(LOG, vector<int>(n));
        up[0] = right;
        for (int p = 1; p < LOG; p++)
            for (int k = 0; k < n; k++)
                up[p][k] = up[p-1][up[p-1][k]];

        auto minHops = [&](int i, int j) -> int {
            if (i >= j) return 0;
            int R = i, dist = 0;
            for (int p = LOG - 1; p >= 0; p--) {
                if (up[p][R] < j) {
                    R = up[p][R];
                    dist += (1 << p);
                }
            }
            return dist + 1;
        };

        vector<int> ans;
        ans.reserve(queries.size());
        for (auto& q : queries) {
            int u = q[0], v = q[1];
            if (u == v) { ans.push_back(0); continue; }
            int pu = pos[u], pv = pos[v];
            if (comp[pu] != comp[pv]) { ans.push_back(-1); continue; }
            int i = min(pu, pv), j = max(pu, pv);
            ans.push_back(minHops(i, j));
        }
        return ans;
    }
};