#include <stdlib.h>

int* spiralOrder(int** matrix, int matrixSize, int* matrixColSize, int* returnSize) {

    int n = matrixSize;
    int m = matrixColSize[0];
    int *ans = (int *)malloc(sizeof(int) * n * m);
    *returnSize = n * m;

    int top = 0, bottom = n - 1;
    int left = 0, right = m - 1;
    int i = 0;

    while(top <= bottom && left <= right){

        for(int j = left; j <= right; j++){
            ans[i++] = matrix[top][j];
        }
        top++;

        for(int j = top; j <= bottom; j++){
            ans[i++] = matrix[j][right];
        }
        right--;

        if(top <= bottom){

            for(int j = right; j >= left; j--){
                ans[i++] = matrix[bottom][j];
            }
            bottom--;
        }
        if(left <= right){

            for(int j = bottom; j >= top; j--){
                ans[i++] = matrix[j][left];
            }
            left++;
        }
    }

    return ans;
}
