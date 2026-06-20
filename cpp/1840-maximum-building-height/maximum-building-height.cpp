class Solution {
public:
    int maxBuilding(int n, vector<vector<int>>& restrictions) {
        restrictions.push_back({1, 0});
        sort(restrictions.begin(), restrictions.end());
        
        int m = restrictions.size();
        for (int i = 1; i < m; ++i) {
            restrictions[i][1] = min(restrictions[i][1], restrictions[i - 1][1] + (restrictions[i][0] - restrictions[i - 1][0]));
        }
        for (int i = m - 2; i >= 0; --i) {
            restrictions[i][1] = min(restrictions[i][1], restrictions[i + 1][1] + (restrictions[i + 1][0] - restrictions[i][0]));
        }
        
        int max_height = restrictions.back()[1] + (n - restrictions.back()[0]);
        for (int i = 0; i < m - 1; ++i) {
            long long x1 = restrictions[i][0], h1 = restrictions[i][1];
            long long x2 = restrictions[i + 1][0], h2 = restrictions[i + 1][1];
            long long current_max = (h1 + h2 + (x2 - x1)) / 2;
            max_height = max(max_height, (int)current_max);
        }
        
        return max_height;
    }
};