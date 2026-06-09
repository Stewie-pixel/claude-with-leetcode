void dfs(int **grid, int i, int j, int m, int n, int time){

    if(i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == 0){
        return;
    }

    if(grid[i][j] == 1 || grid[i][j] >= time){
        grid[i][j] = time;
        dfs(grid, i + 1, j, m, n, time + 1);
        dfs(grid, i, j + 1, m, n, time + 1);
        dfs(grid, i - 1, j, m, n, time + 1);
        dfs(grid, i, j - 1, m, n, time + 1);
    }
}

int orangesRotting(int** grid, int gridSize, int* gridColSize) {

    int m = gridSize;
    int n = gridColSize[0];

    int time = 2, res = 2;

    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(grid[i][j] == 2){
                dfs(grid, i, j, m, n, time);
            }
        }
    }

    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(grid[i][j] == 1){

                return -1;
            }
            else{

                res = res < grid[i][j] ? grid[i][j] : res;
            }
        }
    }

    return res - 2;
}
