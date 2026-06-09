// -----------------------------------------------------------------------------
// Problem: Pacific Atlantic Water Flow
// Source : https://leetcode.com/problems/pacific-atlantic-water-flow/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. A cell can reach the Pacific if there exists a non-decreasing path from
//    that cell to any cell on the top or left border. Similarly, it can reach
//    the Atlantic if a non-decreasing path exists to any cell on the bottom
//    or right border.
// 2. Approach: run DFS from all border cells (Pacific-facing borders and
//    Atlantic-facing borders) to mark which cells can reach each ocean. The
//    intersection of the two visited sets are cells that can reach both.
// 3. Complexity: time O(m*n) — each cell visited a constant number of times; 
//    space O(m*n) for visited matrices plus recursion stack.
// -----------------------------------------------------------------------------

// DFS helper: explores reachable cells with non-decreasing heights
// h       : grid of heights
// i,j     : current coordinates
// m,n     : grid dimensions (m rows, n cols)
// prev    : previous cell height used to ensure non-decreasing path
// visited : matrix recording visited/reachable cells for a given ocean
void dfs(int **h, int i, int j, int m, int n, int prev, int **visited){
    // if out-of-bounds, stop
    if(i < 0 || j < 0 || i >= m || j >= n){
        return;
    }
    // if already visited for this ocean or height decreases, cannot proceed
    if(visited[i][j] || h[i][j] < prev){
        return;
    }
    // mark current cell reachable for this ocean
    visited[i][j] = 1;
    // explore neighbors (up, down, left, right) maintaining non-decreasing height
    dfs(h, i - 1, j, m, n, h[i][j], visited);
    dfs(h, i + 1, j, m, n, h[i][j], visited);
    dfs(h, i, j - 1, m, n, h[i][j], visited);
    dfs(h, i, j + 1, m, n, h[i][j], visited);
}

// Main entry: compute cells that can reach both oceans
// heights: grid input; heightsSize = number of rows; heightsColSize = pointer to number of cols
int** pacificAtlantic(int** heights, int heightsSize, int* heightsColSize, int* returnSize, int** returnColumnSizes) {
    // dimensions
    int m = heightsSize;
    int n = heightsColSize[0];

    // boolean-style visited matrices for Pacific and Atlantic reachability
    int **pacific = (int **)malloc(sizeof(int *) * m);
    int **atlantic = (int **)malloc(sizeof(int *) * m);
    for(int i = 0; i < m; i++){
        // initialize visited rows with zeros
        pacific[i] = (int *)calloc(n, sizeof(int));
        atlantic[i] = (int *)calloc(n, sizeof(int));
    }

    // run DFS from Pacific borders (top and left edges)
    for(int i = 0; i < m; i++){
        dfs(heights, i, 0, m, n, -1, pacific);
    }
    for(int i = 0; i < n; i++){
        dfs(heights, 0, i, m, n, -1, pacific);
    }
    // run DFS from Atlantic borders (bottom and right edges)
    for(int i = 0; i < m; i++){
        dfs(heights, i, n - 1, m, n, -1, atlantic);
    }
    for(int i = 0; i < n; i++){
        dfs(heights, m - 1, i, m, n, -1, atlantic);
    }

    // collect intersection cells that can reach both oceans
    int **res = (int **)malloc(sizeof(int *) * m * n);
    *returnColumnSizes = (int *)malloc(sizeof(int) * m * n);
    *returnSize = 0;
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            // if both matrices mark cell reachable, add to results
            if(pacific[i][j]&& atlantic[i][j]){
                res[(*returnSize)] = (int *)malloc(sizeof(int) * 2);
                res[(*returnSize)][0] = i;
                res[(*returnSize)][1] = j;
                (*returnColumnSizes)[(*returnSize)++] = 2;
            }
        }
    }
    // trim allocations to the actual number of results
    res = (int **)realloc(res, sizeof(int *) * (*returnSize));
    *returnColumnSizes = (int *)realloc(*returnColumnSizes, sizeof(int) * (*returnSize));
    return res;
}