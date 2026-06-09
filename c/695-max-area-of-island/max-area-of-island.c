void getArea(int **grid, int i, int j, int n, int m, int *area){
    if(i >= n || j >= m || j < 0 || i < 0 || !grid[i][j]){
        return;
    }

    grid[i][j] = 0;

    (*area)++;
    getArea(grid, i + 1, j, n, m, area);
    getArea(grid, i, j + 1, n, m, area);
    getArea(grid, i - 1, j, n, m, area);
    getArea(grid, i, j - 1, n, m, area);
}

int maxAreaOfIsland(int** grid, int gridSize, int* gridColSize) {

    int maxArea = 0;
    int n = gridSize;
    int m = *gridColSize;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){

            if(grid[i][j]){
                int area = 0;

                getArea(grid, i, j, n, m, &area);

                maxArea = area > maxArea ? area : maxArea;
            }
        }
    }
    return maxArea;
}
