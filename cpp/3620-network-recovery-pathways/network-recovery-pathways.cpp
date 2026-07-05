class Solution {
public:
    int maxMinPath(vector<vector<int>>& edges, vector<bool>& online, long long k) {
        int n = (int)online.size();
        vector<vector<pair<int, int>>> adj(n);
        vector<int> indeg(n, 0);
        vector<int> costs;
        costs.reserve(edges.size());

        for (auto& e : edges) {
            int u = e[0], v = e[1], w = e[2];
            adj[u].push_back({v, w});
            indeg[v]++;
            costs.push_back(w);
        }

        if (costs.empty()) return -1;

        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) q.push(i);
        }

        vector<int> topo;
        topo.reserve(n);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            topo.push_back(u);
            for (auto& [v, w] : adj[u]) {
                if (--indeg[v] == 0) q.push(v);
            }
        }

        sort(costs.begin(), costs.end());
        costs.erase(unique(costs.begin(), costs.end()), costs.end());

        auto feasible = [&](int threshold) {
            const long long INF = LLONG_MAX / 4;
            vector<long long> dist(n, INF);
            dist[0] = 0;

            for (int u : topo) {
                if (dist[u] == INF) continue;
                long long du = dist[u];
                for (auto& [v, w] : adj[u]) {
                    if (w < threshold) continue;
                    if (!online[v] && v != n - 1) continue;
                    long long nd = du + w;
                    if (nd < dist[v]) dist[v] = nd;
                }
            }

            return dist[n - 1] <= k;
        };

        int lo = 0, hi = (int)costs.size() - 1;
        int ans = -1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (feasible(costs[mid])) {
                ans = costs[mid];
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        return ans;
    }
};