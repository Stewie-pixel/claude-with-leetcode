class Solution {
public:
    int findTheCity(int n, vector<vector<int>>& edges, int distanceThreshold) {
        vector<vector<pair<int, int>>> adj(n);
        for (auto &e : edges) {
            adj[e[0]].push_back({e[1], e[2]});
            adj[e[1]].push_back({e[0], e[2]});
        }

        auto dijkstra = [&](int src) {
            const int INF = 1e9;
            vector<int> dist(n, INF);
            priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

            dist[src] = 0;
            pq.push({0, src});

            while (!pq.empty()) {
                auto [d, u] = pq.top();
                pq.pop();
                if (d > dist[u]) continue;

                for (auto &p : adj[u]) {
                    int v = p.first;
                    int w = p.second;
                    if (dist[u] + w < dist[v]) {
                        dist[v] = dist[u] + w;
                        pq.push({dist[v], v});
                    }
                }
            }
            return dist;
        };

        int bestCity = -1;
        int bestCount = INT_MAX;

        for (int i = 0; i < n; i++) {
            vector<int> dist = dijkstra(i);

            int count = 0;
            for (int j = 0; j < n; j++) {
                if (i != j && dist[j] <= distanceThreshold)
                    count++;
            }

            if (count < bestCount || (count == bestCount && i > bestCity)) {
                bestCount = count;
                bestCity = i;
            }
        }

        return bestCity;
    }
};