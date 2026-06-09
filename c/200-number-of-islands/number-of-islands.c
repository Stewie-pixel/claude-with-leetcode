// -----------------------------------------------------------------------------
// Problem: Number of Islands
// Source : https://leetcode.com/problems/number-of-islands/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- IDEA ----------------------------------
//
// 1. Traverse the grid cell by cell.
// 2. When encountering a land cell ('1'), start a DFS to mark all connected
//    land cells as water ('0') to avoid recounting them.
// 3. Each DFS traversal corresponds to one distinct island.
// 4. Return the total number of islands found.
//
// Time Complexity  : O(m * n)   — each cell visited once
// Space Complexity : O(m * n)   — recursion stack in the worst case
// -----------------------------------------------------------------------------

void dfs(char **grid, int i, int j, int rows, int cols) {
    // Out of bounds or water cell
    if (i < 0 || j < 0 || i >= rows || j >= cols || grid[i][j] == '0')
        return;

    // Mark current land cell as visited
    grid[i][j] = '0';

    // Explore 4 directions (right, left, up, down)
    dfs(grid, i, j + 1, rows, cols);
    dfs(grid, i, j - 1, rows, cols);
    dfs(grid, i - 1, j, rows, cols);
    dfs(grid, i + 1, j, rows, cols);
}

// Main function: count number of islands in the grid
int numIslands(char** grid, int gridSize, int* gridColSize) {
    int rows = gridSize;
    int cols = *gridColSize;
    int count = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == '1') {
                count++;
                dfs(grid, i, j, rows, cols);
            }
        }
    }

    return count;
}
