class Solution {
public:
    vector<string> createGrid(int m, int n, int k) {
        int a = -1, b = -1;
        
        if (k == 1) {
            a = 1; b = 1;
        } else if (k == 2) {
            if (m >= 2 && n >= 2) { a = 2; b = 2; }
        } else if (k == 3) {
            if (n >= 3 && m >= 2) { a = 2; b = 3; }
            else if (m >= 3 && n >= 2) { a = 3; b = 2; }
        } else if (k == 4) {
            if (m >= 2 && n >= 4) { a = 2; b = 4; }
            else if (m >= 4 && n >= 2) { a = 4; b = 2; }
            else if (m == 3 && n == 3) {
                vector<string> grid(m, string(n, '.'));
                grid[0][n - 1] = '#';
                grid[m - 1][0] = '#';
                return grid;
            }
        }
        
        if (a == -1) return {};
        
        vector<string> grid(m, string(n, '#'));
        
        for (int i = 0; i < a; i++) {
            for (int j = 0; j < b; j++) {
                grid[i][j] = '.';
            }
        }
        
        for (int i = a; i < m; i++) {
            grid[i][b - 1] = '.';
        }
        
        for (int j = b - 1; j < n; j++) {
            grid[m - 1][j] = '.';
        }
        
        return grid;
    }
};