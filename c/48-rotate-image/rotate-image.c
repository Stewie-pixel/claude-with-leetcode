// -----------------------------------------------------------------------------
// Problem: Rotate Image (48)
// Source : https://leetcode.com/problems/rotate-image/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// Rotate matrix by 90 degrees clockwise in-place. Typical approach:
// 1) Transpose the matrix (swap symmetric elements)
// 2) Reverse each row
//
// Time Complexity  : O(n^2)
// Space Complexity : O(1)
// -----------------------------------------------------------------------------

#include <stdlib.h>

// utility: swap two ints
void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

// reverse array of length n in place
void reverse(int *a, int n){
    int l = 0, r = n - 1;
    while(l < r){
        swap(&a[l], &a[r]);
        l++;
        r--;
    }
}

// rotate matrix 90 degrees clockwise in-place
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