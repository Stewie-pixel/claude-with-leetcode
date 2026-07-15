class Solution {
public:
    vector<int> leftmostBuildingQueries(vector<int>& heights, vector<vector<int>>& queries) {
        int n = heights.size();
        int q = queries.size();
        vector<int> ans(q, -1);

        vector<vector<pair<int,int>>> bucket(n);

        for (int i = 0; i < q; i++) {
            int a = queries[i][0], b = queries[i][1];
            int lo = min(a, b), hi = max(a, b);
            if (lo == hi) {
                ans[i] = lo;
            } else if (heights[lo] < heights[hi]) {
                ans[i] = hi;
            } else {
                bucket[hi].push_back({heights[lo], i});
            }
        }

        vector<pair<int,int>> stk;

        for (int idx = n - 1; idx >= 0; idx--) {
            for (auto& [target, qIdx] : bucket[idx]) {
                int left = 0, right = (int)stk.size() - 1, resPos = -1;
                while (left <= right) {
                    int mid = (left + right) / 2;
                    if (stk[mid].first > target) {
                        resPos = mid;
                        left = mid + 1;
                    } else {
                        right = mid - 1;
                    }
                }
                if (resPos != -1) ans[qIdx] = stk[resPos].second;
            }

            while (!stk.empty() && stk.back().first <= heights[idx]) {
                stk.pop_back();
            }
            stk.push_back({heights[idx], idx});
        }

        return ans;
    }
};