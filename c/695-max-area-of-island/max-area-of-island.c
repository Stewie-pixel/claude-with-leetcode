// -----------------------------------------------------------------------------
// Problem: Max Area of Island
// Source : https://leetcode.com/problems/max-area-of-island/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. We need to find the largest connected area of 1s (island) in a binary grid.
// 2. Approach: DFS flood-fill — when we find a land cell (1), recursively visit
//    all connected land cells, mark them visited (set to 0), and accumulate area.
// 3. Iterate all cells and track the maximum area found.
//
// Time Complexity  : O(n * m) — each cell visited at most once
// Space Complexity : O(n * m) in worst-case recursion stack
// -----------------------------------------------------------------------------

// helper to flood-fill and count area starting from (i, j)
// grid  : pointer to the grid (array of int pointers)
// i, j  : start coordinates
// n, m  : grid dimensions
// area  : pointer to accumulator for this island
void getArea(int **grid, int i, int j, int n, int m, int *area){
    if(i >= n || j >= m || j < 0 || i < 0 || !grid[i][j]){
        return;
    }
    // mark cell visited by setting to 0 (prevents revisits)
    grid[i][j] = 0;
    // increment the current island area
    (*area)++;
    getArea(grid, i + 1, j, n, m, area);
    getArea(grid, i, j + 1, n, m, area);
    getArea(grid, i - 1, j, n, m, area);
    getArea(grid, i, j - 1, n, m, area);
}

int maxAreaOfIsland(int** grid, int gridSize, int* gridColSize) {
    // track maximum island area found
    int maxArea = 0;
    int n = gridSize;
    int m = *gridColSize;
    // scan all cells; launch flood-fill when a land cell (1) is discovered
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            // if the current cell is land (non-zero), flood-fill to compute its area
            if(grid[i][j]){
                int area = 0;
                // start DFS flood-fill from (i, j)
                getArea(grid, i, j, n, m, &area);
                // update maximum if this island is larger
                maxArea = area > maxArea ? area : maxArea;
            }
        }
    }
    return maxArea;
}