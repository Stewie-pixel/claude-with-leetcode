#include <vector>
using namespace std;

class Solution {
public:

    void dfs(const vector<vector<int>>& adj, int u, vector<bool>& visited) {
        visited[u] = true;
        for (const auto &node : adj[u]) {
            if (!visited[node]) {
                dfs(adj, node, visited);
            }
        }
    }

    int countComponents(int n, const vector<vector<int>> &edges) {
        vector<vector<int>> adj(n);
        vector<bool> visited(n, false);

        for (const auto &edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }

        int count = 0;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                count++;
                dfs(adj, i, visited);
            }
        }

        return count;
    }
};
