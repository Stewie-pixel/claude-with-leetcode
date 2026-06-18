class Solution {
public:
    int countCompleteComponents(int n, vector<vector<int>>& edges) {
        vector<vector<int>> adj(n);
        for (auto &e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        vector<bool> visited(n, false);
        int completeCount = 0;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                queue<int> q;
                q.push(i);
                visited[i] = true;

                vector<int> nodes;
                int edgeCount = 0;

                while (!q.empty()) {
                    int u = q.front();
                    q.pop();
                    nodes.push_back(u);

                    edgeCount += adj[u].size();

                    for (int v : adj[u]) {
                        if (!visited[v]) {
                            visited[v] = true;
                            q.push(v);
                        }
                    }
                }

                int m = nodes.size();
                edgeCount /= 2;

                if (edgeCount == m * (m - 1) / 2)
                    completeCount++;
            }
        }

        return completeCount;
    }
};