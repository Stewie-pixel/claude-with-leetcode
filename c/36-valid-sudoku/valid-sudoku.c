// Source: https://leetcode.com/problems/valid-sudoku/
// Author: Ngoc-Tien To

// Idea: For each non-empty cell, check 
// if the current value already exists in the same row, column, or 3x3 sub-box.
// If a duplicate is found, return false; otherwise, continue. 
// If no violations are found, return true.

bool isValidSudoku(char** board, int boardSize, int* boardColSize) {
    for(int i = 0; i < boardSize; i++){
        for(int j = 0; j < boardSize; j++){
            if(board[i][j] != '.'){ // Check in whole box
                for(int k = 0; k < boardSize; k++){
                    if((k != j && board[i][k] == board[i][j]) ||
                    (k != i && board[k][j] == board[i][j])){
                        return false;
                    }
                }
                    
                for(int x = 0; x < 3; x++){ // Check in sub-box
                    for(int y = 0; y < 3; y++){
                        int index1 = (i / 3) * 3 + x;
                        int index2 = (j / 3) * 3 + y;
                        if(index1 != i && index2 != j && board[i][j] == board[index1][index2]){
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}