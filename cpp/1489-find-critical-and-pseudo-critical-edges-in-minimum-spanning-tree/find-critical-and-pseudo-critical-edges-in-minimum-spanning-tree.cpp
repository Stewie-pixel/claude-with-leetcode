class Solution {
public:
    vector<int> parent, rank_;
    
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
        return true;
    }
    
    int store(int n, vector<vector<int>>& edges, int skipIdx, int forceIdx) {
        parent.resize(n);
        rank_.assign(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
        
        int weight = 0, count = 0;
        
        if (forceIdx != -1) {
            unite(edges[forceIdx][0], edges[forceIdx][1]);
            weight += edges[forceIdx][2];
            count++;
        }
        
        for (int i = 0; i < (int)edges.size(); i++) {
            if (i == skipIdx || i == forceIdx) continue;
            if (unite(edges[i][0], edges[i][1])) {
                weight += edges[i][2];
                count++;
            }
        }
        
        if (count != n - 1) return INT_MAX;
        return weight;
    }
    
    vector<vector<int>> findCriticalAndPseudoCriticalEdges(int n, vector<vector<int>>& edges) {
        int m = edges.size();
        vector<vector<int>> indexed(m);
        for (int i = 0; i < m; i++) {
            indexed[i] = {edges[i][0], edges[i][1], edges[i][2], i};
        }
        
        sort(indexed.begin(), indexed.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[2] < b[2];
        });
        
        int baseline = store(n, indexed, -1, -1);
        
        vector<int> critical, pseudo;
        
        for (int i = 0; i < m; i++) {
            int withoutWeight = store(n, indexed, i, -1);
            if (withoutWeight > baseline) {
                critical.push_back(indexed[i][3]);
            } else {
                int withWeight = store(n, indexed, -1, i);
                if (withWeight == baseline) {
                    pseudo.push_back(indexed[i][3]);
                }
            }
        }
        
        return {critical, pseudo};
    }
};