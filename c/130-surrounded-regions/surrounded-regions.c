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

void solve(char** board, int boardSize, int* boardColSize) {
    int m = boardSize;
    int n = boardColSize[0];

    for(int i = 0; i < m; i++){
        if(board[i][0] == 'O'){
            dfs(board, i, 0, m, n);
        }
        if(board[i][n - 1] == 'O'){
            dfs(board, i, n - 1, m, n);
        }
    }

    for(int i = 0; i < n; i++){
        if(board[0][i] == 'O'){
            dfs(board, 0, i, m, n);
        }
        if(board[m - 1][i] == 'O'){
            dfs(board, m - 1, i, m, n);
        }
    }

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
