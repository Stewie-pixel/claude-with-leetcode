class Solution {
public:
    int findMaxPathScore(vector<vector<int>>& edges, vector<bool>& online, long long k) {
        int n = online.size();
        vector<vector<pair<int,long long>>> adj(n);
        vector<int> indeg(n, 0);
        set<long long> costSet;
        for (auto& e : edges) {
            int u = e[0], v = e[1];
            long long c = e[2];
            adj[u].push_back({v, c});
            indeg[v]++;
            costSet.insert(c);
        }

        if (costSet.empty()) return -1;

        vector<int> deg = indeg;
        queue<int> dq;
        for (int i = 0; i < n; i++) if (deg[i] == 0) dq.push(i);
        vector<int> topo;
        topo.reserve(n);
        while (!dq.empty()) {
            int u = dq.front(); dq.pop();
            topo.push_back(u);
            for (auto& [v, c] : adj[u]) {
                if (--deg[v] == 0) dq.push(v);
            }
        }

        vector<long long> sortedCosts(costSet.begin(), costSet.end());
        const long long INF = LLONG_MAX / 2;

        auto feasible = [&](long long T) -> bool {
            vector<long long> dist(n, INF);
            dist[0] = 0;
            for (int u : topo) {
                if (dist[u] >= INF) continue;
                long long du = dist[u];
                for (auto& [v, c] : adj[u]) {
                    if (c >= T && online[v]) {
                        long long nd = du + c;
                        if (nd < dist[v]) dist[v] = nd;
                    }
                }
            }
            return dist[n - 1] <= k;
        };

        int lo = 0, hi = (int)sortedCosts.size() - 1;
        long long ans = -1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (feasible(sortedCosts[mid])) {
                ans = sortedCosts[mid];
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        return (int)ans;
    }
};