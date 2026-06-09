#include <vector>
using namespace std;

class Solution {
public:

    void dfs(const vector<vector<int>>& adj, int u, vector<int>& visited) {
        visited[u] = 1;
        for (auto& node : adj[u]) {
            if (!visited[node]) {
                dfs(adj, node, visited);
            }
        }
    }

    int countCC(const vector<vector<int>>& adj, int n) {
        int count = 0;
        vector<int> visited(n, 0);
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                count++;
                dfs(adj, i, visited);
            }
        }
        return count;
    }

    bool validTree(int n, vector<vector<int>>& edges) {

        if (edges.size() != n - 1) {
            return false;
        }

        vector<vector<int>> adj(n);
        for (auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }

        return countCC(adj, n) == 1;
    }
};
