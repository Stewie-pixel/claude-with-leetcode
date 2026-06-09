// -----------------------------------------------------------------------------
// Problem: Rotting Oranges
// Source : https://leetcode.com/problems/rotting-oranges/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Each minute, fresh oranges adjacent to rotten ones become rotten.
// 2. Approach: run DFS/BFS from rotten oranges and record infection time; the
//    maximum time needed to rot all reachable fresh oranges is the answer.
// 3. If any fresh orange remains unreachable, return -1.
//
// Time Complexity  : O(m * n)
// Space Complexity : O(m * n)
// -----------------------------------------------------------------------------

// DFS propagates rot to adjacent cells and records time
// grid : pointer to grid matrix
// i, j : current coordinates
// m, n : grid dimensions
// time : the moment (as an int marker) when this cell becomes rotten
void dfs(int **grid, int i, int j, int m, int n, int time){
    // boundary check + empty cell (0) means nothing to do
    if(i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == 0){
        return;
    }
    // if this cell is fresh (1) or can be updated with an earlier time,
    // mark it rotten at `time` and propagate to neighbors with time+1
    if(grid[i][j] == 1 || grid[i][j] >= time){
        grid[i][j] = time;
        dfs(grid, i + 1, j, m, n, time + 1);
        dfs(grid, i, j + 1, m, n, time + 1);
        dfs(grid, i - 1, j, m, n, time + 1);
        dfs(grid, i, j - 1, m, n, time + 1);
    }
}

int orangesRotting(int** grid, int gridSize, int* gridColSize) {
    // m = number of rows, n = number of columns
    int m = gridSize;
    int n = gridColSize[0];
    // time marker starts at 2 (grid uses 0 empty, 1 fresh, 2 rotten) and increments
    int time = 2, res = 2;

    // start DFS from all initially rotten oranges (value 2)
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(grid[i][j] == 2){
                dfs(grid, i, j, m, n, time);
            }
        }
    }
    // scan grid to detect any remaining fresh orange. Also compute maximum
    // recorded time value (the largest time marker found indicates finishing time)
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(grid[i][j] == 1){
                // a fresh orange remains -> impossible to rot all
                return -1;
            }
            else{
                // track the latest time marker used by any rotten cell
                res = res < grid[i][j] ? grid[i][j] : res;
            }
        }
    }
    // subtract the starting marker to get elapsed minutes
    return res - 2;
}