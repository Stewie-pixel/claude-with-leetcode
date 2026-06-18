class Solution {
public:
    bool isPossible(int n, vector<vector<int>>& edges) {
        vector<unordered_set<int>> adj(n + 1);
        for (const auto& edge : edges) {
            adj[edge[0]].insert(edge[1]);
            adj[edge[1]].insert(edge[0]);
        }

        vector<int> odd;
        for (int i = 1; i <= n; ++i) {
            if (adj[i].size() % 2 != 0) {
                odd.push_back(i);
            }
        }

        if (odd.size() == 0) {
            return true;
        }

        if (odd.size() == 2) {
            int a = odd[0], b = odd[1];
            if (adj[a].find(b) == adj[a].end()) {
                return true;
            }
            for (int c = 1; c <= n; ++c) {
                if (c != a && c != b && adj[a].find(c) == adj[a].end() && adj[b].find(c) == adj[b].end()) {
                    return true;
                }
            }
            return false;
        }

        if (odd.size() == 4) {
            int a = odd[0], b = odd[1], c = odd[2], d = odd[3];
            if ((adj[a].find(b) == adj[a].end() && adj[c].find(d) == adj[c].end()) ||
                (adj[a].find(c) == adj[a].end() && adj[b].find(d) == adj[b].end()) ||
                (adj[a].find(d) == adj[a].end() && adj[b].find(c) == adj[b].end())) {
                return true;
            }
            return false;
        }

        return false;
    }
};