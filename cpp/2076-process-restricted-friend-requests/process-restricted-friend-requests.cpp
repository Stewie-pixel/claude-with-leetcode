class Solution {
public:
    vector<int> parent, rnk;

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        if (rnk[px] < rnk[py]) swap(px, py);
        parent[py] = px;
        if (rnk[px] == rnk[py]) rnk[px]++;
    }

    vector<bool> friendRequests(int n, vector<vector<int>>& restrictions, vector<vector<int>>& requests) {
        parent.resize(n);
        rnk.resize(n, 0);
        iota(parent.begin(), parent.end(), 0);

        vector<bool> result;

        for (auto& req : requests) {
            int pu = find(req[0]), pv = find(req[1]);

            if (pu == pv) {
                result.push_back(true);
                continue;
            }

            bool blocked = false;
            for (auto& res : restrictions) {
                int rx = find(res[0]), ry = find(res[1]);
                if ((rx == pu && ry == pv) || (rx == pv && ry == pu)) {
                    blocked = true;
                    break;
                }
            }

            if (!blocked) {
                unite(pu, pv);
                result.push_back(true);
            } else {
                result.push_back(false);
            }
        }

        return result;
    }
};