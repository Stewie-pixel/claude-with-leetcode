class Solution {
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();
        int perimeter = 0;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] == 1) {
                    perimeter += 4;

                    if (r > 0 && grid[r-1][c] == 1) perimeter--;
                    if (r < rows-1 && grid[r+1][c] == 1) perimeter--;
                    if (c > 0 && grid[r][c-1] == 1) perimeter--;
                    if (c < cols-1 && grid[r][c+1] == 1) perimeter--;
                }
            }
        }

        return perimeter;
    }
};