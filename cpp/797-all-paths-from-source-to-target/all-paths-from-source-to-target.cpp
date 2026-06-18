class Solution {
public:
    vector<vector<int>> ans;
    vector<int> path;
    
    void dfs(int node, vector<vector<int>>& graph, int target) {
        path.push_back(node);

        if (node == target) {
            ans.push_back(path);
        } else {
            for (int next: graph[node]) {
                dfs(next, graph, target);
            }
        }
        path.pop_back();
    }

    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        int n = graph.size();
        dfs(0, graph, n - 1);
        return ans;
    }
};