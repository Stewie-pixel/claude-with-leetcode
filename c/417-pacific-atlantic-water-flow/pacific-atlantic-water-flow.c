void dfs(int **h, int i, int j, int m, int n, int prev, int **visited){

    if(i < 0 || j < 0 || i >= m || j >= n){
        return;
    }

    if(visited[i][j] || h[i][j] < prev){
        return;
    }

    visited[i][j] = 1;

    dfs(h, i - 1, j, m, n, h[i][j], visited);
    dfs(h, i + 1, j, m, n, h[i][j], visited);
    dfs(h, i, j - 1, m, n, h[i][j], visited);
    dfs(h, i, j + 1, m, n, h[i][j], visited);
}

int** pacificAtlantic(int** heights, int heightsSize, int* heightsColSize, int* returnSize, int** returnColumnSizes) {

    int m = heightsSize;
    int n = heightsColSize[0];

    int **pacific = (int **)malloc(sizeof(int *) * m);
    int **atlantic = (int **)malloc(sizeof(int *) * m);
    for(int i = 0; i < m; i++){

        pacific[i] = (int *)calloc(n, sizeof(int));
        atlantic[i] = (int *)calloc(n, sizeof(int));
    }

    for(int i = 0; i < m; i++){
        dfs(heights, i, 0, m, n, -1, pacific);
    }
    for(int i = 0; i < n; i++){
        dfs(heights, 0, i, m, n, -1, pacific);
    }

    for(int i = 0; i < m; i++){
        dfs(heights, i, n - 1, m, n, -1, atlantic);
    }
    for(int i = 0; i < n; i++){
        dfs(heights, m - 1, i, m, n, -1, atlantic);
    }

    int **res = (int **)malloc(sizeof(int *) * m * n);
    *returnColumnSizes = (int *)malloc(sizeof(int) * m * n);
    *returnSize = 0;
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){

            if(pacific[i][j]&& atlantic[i][j]){
                res[(*returnSize)] = (int *)malloc(sizeof(int) * 2);
                res[(*returnSize)][0] = i;
                res[(*returnSize)][1] = j;
                (*returnColumnSizes)[(*returnSize)++] = 2;
            }
        }
    }

    res = (int **)realloc(res, sizeof(int *) * (*returnSize));
    *returnColumnSizes = (int *)realloc(*returnColumnSizes, sizeof(int) * (*returnSize));
    return res;
}
