// -----------------------------------------------------------------------------
// Problem: Spiral Matrix (54)
// Source : https://leetcode.com/problems/spiral-matrix/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// Traverse the 2D matrix boundaries (top, right, bottom, left) moving inward
// until all elements are visited. Maintain four pointers (top, bottom, left,
// right) and update them after traversing each side.
//
// Time Complexity  : O(n * m)
// Space Complexity : O(n * m) for the returned result
// -----------------------------------------------------------------------------

#include <stdlib.h>

// spiralOrder(matrix, matrixSize, matrixColSize, returnSize)
// - matrix: array of row pointers (2D matrix)
// - matrixSize: number of rows
// - matrixColSize: pointer to number of columns per row
// - returnSize: out param to receive length of returned array
int* spiralOrder(int** matrix, int matrixSize, int* matrixColSize, int* returnSize) {
    // required include for malloc/calloc/realloc used by other files
    // (kept local comment only — no change to executable lines above)
    int n = matrixSize;
    int m = matrixColSize[0];
    int *ans = (int *)malloc(sizeof(int) * n * m);
    *returnSize = n * m;

    // boundaries of the area still to traverse
    // top..bottom are row bounds, left..right are column bounds
    int top = 0, bottom = n - 1;
    int left = 0, right = m - 1;
    int i = 0;
    // traverse the current rectangle bounded by top/bottom/left/right
    while(top <= bottom && left <= right){
        // move right across the top row (fixed row: top, columns left..right)
        for(int j = left; j <= right; j++){
            ans[i++] = matrix[top][j];
        }
        top++;

        // move down the right column (fixed column: right, rows top..bottom)
        for(int j = top; j <= bottom; j++){
            ans[i++] = matrix[j][right];
        }
        right--;

        if(top <= bottom){
            // move left across the bottom row (fixed row: bottom, columns right..left)
            for(int j = right; j >= left; j--){
                ans[i++] = matrix[bottom][j];
            }
            bottom--;
        }
        if(left <= right){
            // move up the left column (fixed column: left, rows bottom..top)
            for(int j = bottom; j >= top; j--){
                ans[i++] = matrix[j][left];
            }
            left++;
        }
    }
    
    return ans;
}  