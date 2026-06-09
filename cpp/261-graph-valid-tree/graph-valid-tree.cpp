// -----------------------------------------------------------------------------
// Problem: Graph Valid Tree
// Source : https://leetcode.com/problems/graph-valid-tree/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- IDEA ----------------------------------
//
// 1. A graph is a valid tree if and only if:
//      - It has exactly (n - 1) edges, and
//      - It is fully connected (only one connected component).
//
// 2. Check the first condition directly.
// 3. Build adjacency list and perform DFS to count connected components.
// 4. If there is exactly one connected component, it’s a valid tree.
//
// Time Complexity  : O(n + e)
// Space Complexity : O(n + e)
// -----------------------------------------------------------------------------

#include <vector>
using namespace std;

class Solution {
public:
    // Depth-first search to mark connected nodes
    void dfs(const vector<vector<int>>& adj, int u, vector<int>& visited) {
        visited[u] = 1;
        for (auto& node : adj[u]) {
            if (!visited[node]) {
                dfs(adj, node, visited);
            }
        }
    }

    // Count number of connected components
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

    // Main function: determine if graph is a valid tree
    bool validTree(int n, vector<vector<int>>& edges) {
        // A tree must have exactly (n - 1) edges
        if (edges.size() != n - 1) {
            return false;
        }

        // Build adjacency list
        vector<vector<int>> adj(n);
        for (auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }

        // Graph is a tree if it has only one connected component
        return countCC(adj, n) == 1;
    }
};
