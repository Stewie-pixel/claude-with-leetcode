// -----------------------------------------------------------------------------
// Problem: Surrounded Regions
// Source : https://leetcode.com/problems/surrounded-regions/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Cells with 'O' entirely surrounded by 'X' should be flipped to 'X'.
// 2. Approach: start DFS from boundary 'O' cells and mark reachable 'O's as safe.
// 3. After marking, flip any remaining 'O' to 'X', then restore safe marks back to 'O'.
//
// Time Complexity  : O(m * n)
// Space Complexity : O(m * n) recursion stack in worst-case
// -----------------------------------------------------------------------------

// DFS helper: mark all 'O' cells reachable from the boundary as safe
// vis : board array
// i,j : current coordinates
// m,n : dimensions (rows, cols)
// We convert boundary-connected 'O' -> 'S' so interior 'O's remain for flipping
void dfs(char **vis, int i, int j, int m, int n){
    if(i < 0 || j < 0 || i >= m || j >= n || vis[i][j] != 'O'){
        return;
    }
    vis[i][j] = 'S';
    dfs(vis, i + 1, j, m, n);
    dfs(vis, i - 1, j, m, n);
    dfs(vis, i, j + 1, m, n);
    dfs(vis, i, j - 1, m, n);
}

// Main solver: flips surrounded 'O' to 'X'
// Steps:
// 1) Start DFS from boundary 'O's and mark them 'S' (safe)
// 2) Flip remaining 'O' -> 'X' (surrounded)
// 3) Restore 'S' -> 'O' (safe ones)
void solve(char** board, int boardSize, int* boardColSize) {
    int m = boardSize;
    int n = boardColSize[0];

    // walk left & right borders and mark connected 'O's as safe
    for(int i = 0; i < m; i++){
        if(board[i][0] == 'O'){
            dfs(board, i, 0, m, n);
        }
        if(board[i][n - 1] == 'O'){
            dfs(board, i, n - 1, m, n);
        }
    }

    // walk top & bottom borders and mark connected 'O's as safe
    for(int i = 0; i < n; i++){
        if(board[0][i] == 'O'){
            dfs(board, 0, i, m, n);
        }
        if(board[m - 1][i] == 'O'){
            dfs(board, m - 1, i, m, n);
        }
    }

    // convert all remaining 'O' (surrounded) to 'X', convert 'S' back to 'O'
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(board[i][j] == 'O'){
                board[i][j] = 'X';
            }
            if(board[i][j] == 'S'){
                board[i][j] = 'O';
            }
        }
    }
}