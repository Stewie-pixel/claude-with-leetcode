#include <vector>
using namespace std;

#define INF 2147483647

class Solution {
public:
    void islandsAndTreasure(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == 0) {
                    dfs(grid, i, j, rows, cols, 0);
                }
            }
        }
    }

private:
    void dfs(vector<vector<int>>& grid, int i, int j, int rows, int cols, int distance) {

        if (i < 0 || i >= rows || j < 0 || j >= cols || grid[i][j] < distance)
            return;

        grid[i][j] = distance;

        dfs(grid, i + 1, j, rows, cols, distance + 1);
        dfs(grid, i, j + 1, rows, cols, distance + 1);
        dfs(grid, i - 1, j, rows, cols, distance + 1);
        dfs(grid, i, j - 1, rows, cols, distance + 1);
    }
};
