#include <stdlib.h>

void setZeroes(int** matrix, int matrixSize, int* matrixColSize) {
    int m = matrixSize;
    int n = matrixColSize[0];

    int *row = (int *)calloc(m, sizeof(int));
    int *col = (int *)calloc(n, sizeof(int));

    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(matrix[i][j] == 0){

                row[i] = 1;
                col[j] = 1;
            }
        }
    }

    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){

            if(row[i] || col[j]){
                matrix[i][j] = 0;
            }
        }
    }
}
