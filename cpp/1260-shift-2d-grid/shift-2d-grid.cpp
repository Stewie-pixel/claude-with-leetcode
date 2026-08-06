class Solution {
public:
    vector<vector<int>> shiftGrid(vector<vector<int>>& grid, int k) {
        int m = grid.size();
        int n = grid[0].size();

        k %= (m * n);

        while (k--) {
            int last = grid[m - 1][n - 1];

            for (int i = m - 1; i >= 0; i--) {
                for (int j = n - 1; j >= 0; j--) {
                    if (i == 0 && j == 0)
                        continue;

                    if (j == 0)
                        grid[i][j] = grid[i - 1][n - 1];
                    else
                        grid[i][j] = grid[i][j - 1];
                }
            }

            grid[0][0] = last;
        }

        return grid;
    }
};