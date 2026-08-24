#include <stdlib.h>

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void reverse(int *a, int n){
    int l = 0, r = n - 1;
    while(l < r){
        swap(&a[l], &a[r]);
        l++;
        r--;
    }
}

void rotate(int** matrix, int matrixSize, int* matrixColSize) {
    int n = matrixSize;
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            swap(&matrix[i][j], &matrix[j][i]);
        }
    }
    for(int i = 0; i < n; i++){
        reverse(matrix[i], n);
    }
}
