// -----------------------------------------------------------------------------
// Problem: Set Matrix Zeroes (73)
// Source : https://leetcode.com/problems/set-matrix-zeroes/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// Detect rows and columns that must be zeroed (because they contain a 0),
// then set those rows and columns to 0. Use O(m + n) extra space for flags.
//
// Time Complexity  : O(m * n)
// Space Complexity : O(m + n)
// -----------------------------------------------------------------------------

#include <stdlib.h>

// setZeroes(matrix, matrixSize, matrixColSize)
// - matrix: 2D array where zeroing must be applied
// - matrixSize: number of rows
// - matrixColSize: pointer (cols per row)
void setZeroes(int** matrix, int matrixSize, int* matrixColSize) {
    int m = matrixSize;
    int n = matrixColSize[0];
    // allocate flags: row[i] == 1 means row i should be zeroed, col[j] == 1 means column j should be zeroed
    int *row = (int *)calloc(m, sizeof(int));
    int *col = (int *)calloc(n, sizeof(int));
    // first pass: detect rows and columns that contain zero
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(matrix[i][j] == 0){
                // mark this row and column for zeroing later
                row[i] = 1;
                col[j] = 1;
            }
        }
    }
    // second pass: set matrix cells to zero if their row or column marked
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            // if either the row or column was marked, set this cell to zero
            if(row[i] || col[j]){
                matrix[i][j] = 0;
            }
        }
    }
}