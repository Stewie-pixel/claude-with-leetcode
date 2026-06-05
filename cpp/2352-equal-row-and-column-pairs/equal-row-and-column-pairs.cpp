class Solution {
public:
    int equalPairs(vector<vector<int>>& grid) {
        int n = grid.size();
        unordered_map<string, int> map;

        for (int i = 0; i < n; i++) {
            string key;
            for (int j = 0; j < n; j++) {
                key += to_string(grid[i][j]) + ",";
            }
            map[key]++;
        }

        int res = 0;
        for (int col = 0; col < n; col++) {
            string key;
            for (int row = 0; row < n; row++) {
                key += to_string(grid[row][col]) + ",";
            }
            if (map.count(key)) {
                res += map[key];
            }
        }
        return res;
    }
};