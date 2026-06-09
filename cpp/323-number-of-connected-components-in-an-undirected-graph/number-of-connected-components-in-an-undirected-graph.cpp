// -----------------------------------------------------------------------------
// Problem: Number of Connected Components in an Undirected Graph
// Source : https://leetcode.com/problems/number-of-connected-components-in-an-undirected-graph/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- IDEA ----------------------------------
//
// 1. The problem asks for the number of connected components in an undirected graph.
//
// 2. Approach:
//      - Build adjacency list from the edge list.
//      - Use DFS (or BFS) to traverse each component.
//      - Each time we start a DFS from an unvisited node, we found a new component.
//
// 3. Count how many times DFS starts -> that’s the number of components.
//
// Time Complexity  : O(n + e)
// Space Complexity : O(n + e)
// -----------------------------------------------------------------------------

#include <vector>
using namespace std;

class Solution {
public:
    // Depth-first search to visit all nodes in the same component
    void dfs(const vector<vector<int>>& adj, int u, vector<bool>& visited) {
        visited[u] = true;
        for (const auto &node : adj[u]) {
            if (!visited[node]) {
                dfs(adj, node, visited);
            }
        }
    }

    // Count the number of connected components in the graph
    int countComponents(int n, const vector<vector<int>> &edges) {
        vector<vector<int>> adj(n);
        vector<bool> visited(n, false);

        // Build adjacency list
        for (const auto &edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }

        int count = 0;

        // Start DFS for each unvisited node
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                count++;
                dfs(adj, i, visited);
            }
        }

        return count;
    }
};
