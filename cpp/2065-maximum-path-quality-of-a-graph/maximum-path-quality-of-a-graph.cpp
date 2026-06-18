class Solution {
private:
    int max_quality = 0;
    
    void dfs(int node, int current_time, int current_value, int maxTime,
             const vector<int>& values, const vector<vector<pair<int, int>>>& adj,
             vector<int>& visited) {
        
        if (node == 0) {
            max_quality = max(max_quality, current_value);
        }
        
        for (const auto& edge : adj[node]) {
            int neighbor = edge.first;
            int time_needed = edge.second;
            
            if (current_time + time_needed <= maxTime) {
                int next_value = current_value;
                if (visited[neighbor] == 0) {
                    next_value += values[neighbor];
                }
                
                visited[neighbor]++;
                dfs(neighbor, current_time + time_needed, next_value, maxTime, values, adj, visited);
                visited[neighbor]--;
            }
        }
    }

public:
    int maximalPathQuality(vector<int>& values, vector<vector<int>>& edges, int maxTime) {
        int n = values.size();
        vector<vector<pair<int, int>>> adj(n);
        for (const auto& edge : edges) {
            adj[edge[0]].push_back({edge[1], edge[2]});
            adj[edge[1]].push_back({edge[0], edge[2]});
        }
        
        vector<int> visited(n, 0);
        visited[0] = 1;
        
        dfs(0, 0, values[0], maxTime, values, adj, visited);
        
        return max_quality;
    }
};