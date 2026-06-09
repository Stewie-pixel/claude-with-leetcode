// Search a 2D Matrix
// Source: https://leetcode.com/problems/search-a-2d-matrix/
// Author: Ngoc-Tien To

// Description:
/*You are given an m x n integer matrix matrix with the following two properties:
Each row is sorted in non-decreasing order.
The first integer of each row is greater than the last integer of the previous row.
Given an integer target, return true if target is in matrix or false otherwise.
You must write a solution in O(log(m * n)) time complexity.*/

// Idea:
// 1. Start searching from the top-right corner of the matrix.
// 2. Since each row and column is sorted in ascending order:
//      - Moving left → values decrease
//      - Moving down → values increase
// 3. At each step:
//      3.1 If target == matrix[i][j] → found
//      3.2 If target > matrix[i][j] → move down
//      3.3 If target < matrix[i][j] → move left
// 4. Continue until indices go out of bounds.
// 5. Return true if target was found, otherwise false.


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