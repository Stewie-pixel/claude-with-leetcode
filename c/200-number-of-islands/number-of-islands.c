void dfs(char **grid, int i, int j, int rows, int cols) {

    if (i < 0 || j < 0 || i >= rows || j >= cols || grid[i][j] == '0')
        return;

    grid[i][j] = '0';

    dfs(grid, i, j + 1, rows, cols);
    dfs(grid, i, j - 1, rows, cols);
    dfs(grid, i - 1, j, rows, cols);
    dfs(grid, i + 1, j, rows, cols);
}

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
