bool searchMatrix(int** matrix, int matrixSize, int* matrixColSize, int target) {
    int rows = matrixSize;
    int cols = matrixColSize[0];
    int i = 0, j = cols - 1;
    while(i < rows && j >= 0){
        if(target > matrix[i][j]){
            i++;
        }
        else if(target < matrix[i][j]){
            j--;
        }
        else{
            return true;
        }
    }
    return false;
}
