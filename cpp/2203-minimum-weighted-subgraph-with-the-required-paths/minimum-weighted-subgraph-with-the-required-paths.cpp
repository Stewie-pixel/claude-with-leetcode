class Solution {
public:
    vector<long long> dijkstra(int n, vector<vector<pair<int,int>>>& adj, int src) {
        vector<long long> dist(n, LLONG_MAX);
        dist[src] = 0;
        priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
        pq.push({0, src});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (auto& [v, w] : adj[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }

    long long minimumWeight(int n, vector<vector<int>>& edges, int src1, int src2, int dest) {
        vector<vector<pair<int,int>>> adj(n), radj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back({e[1], e[2]});
            radj[e[1]].push_back({e[0], e[2]});
        }

        vector<long long> d1 = dijkstra(n, adj, src1);
        vector<long long> d2 = dijkstra(n, adj, src2);
        vector<long long> d3 = dijkstra(n, radj, dest);

        long long ans = LLONG_MAX;
        for (int i = 0; i < n; i++) {
            if (d1[i] == LLONG_MAX || d2[i] == LLONG_MAX || d3[i] == LLONG_MAX) continue;
            ans = min(ans, d1[i] + d2[i] + d3[i]);
        }

        return ans == LLONG_MAX ? -1 : ans;
    }
};