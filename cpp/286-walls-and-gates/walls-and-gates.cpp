// -----------------------------------------------------------------------------
// Problem: Walls and Gates
// Source : https://leetcode.com/problems/walls-and-gates/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- IDEA ----------------------------------
//
// 1. Each gate is represented by a cell with value 0.
// 2. Each empty room has value INF (2147483647).
// 3. Perform DFS from every gate (cell == 0):
//      - Propagate distance outward to adjacent rooms.
//      - Only update a room if the current distance is smaller than its value.
// 4. DFS ensures that each room gets the shortest distance to the nearest gate.
//
// Note: This solution uses DFS. A more efficient BFS version also exists.
//
// Time Complexity  : O(m * n)
// Space Complexity : O(m * n)  — recursion stack in worst case
// -----------------------------------------------------------------------------

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
        // Out of bounds or cell already has a shorter distance
        if (i < 0 || i >= rows || j < 0 || j >= cols || grid[i][j] < distance)
            return;

        // Update current cell with the new distance
        grid[i][j] = distance;

        // Explore all 4 directions
        dfs(grid, i + 1, j, rows, cols, distance + 1);
        dfs(grid, i, j + 1, rows, cols, distance + 1);
        dfs(grid, i - 1, j, rows, cols, distance + 1);
        dfs(grid, i, j - 1, rows, cols, distance + 1);
    }
};
