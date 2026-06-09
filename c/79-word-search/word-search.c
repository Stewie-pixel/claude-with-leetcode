// -----------------------------------------------------------------------------
// Problem : Word Search
// Source  : https://leetcode.com/problems/word-search/
// Author  : Ngoc-Tien To
// -----------------------------------------------------------------------------


/**
 * @brief Depth-First Search helper to explore all possible paths
 * 
 * @param board   2D character grid
 * @param row     Current row index
 * @param col     Current column index
 * @param rowSize Number of rows in the board
 * @param colSize Number of columns in the board
 * @param word    Target word to search
 * @param index   Current character index in word
 * @return true   If path matches the word
 * @return false  Otherwise
 */
bool traversal(char **board, int row, int col, int rowSize, int colSize, char *word, int index) {
    // Out of bounds or already visited
    if (row < 0 || row >= rowSize || col < 0 || col >= colSize || board[row][col] == '#')
        return false;

    // Character mismatch
    if (board[row][col] != word[index])
        return false;

    // Word fully matched
    if (word[index + 1] == '\0')
        return true;

    // Temporarily mark current cell as visited
    char temp = board[row][col];
    board[row][col] = '#';

    // Explore all 4 directions (down, right, up, left)
    bool found = traversal(board, row + 1, col, rowSize, colSize, word, index + 1) ||
                 traversal(board, row, col + 1, rowSize, colSize, word, index + 1) ||
                 traversal(board, row - 1, col, rowSize, colSize, word, index + 1) ||
                 traversal(board, row, col - 1, rowSize, colSize, word, index + 1);

    // Restore cell after backtracking
    board[row][col] = temp;

    return found;
}


/**
 * @brief Main function to determine if the word exists in the grid
 * 
 * @param board          2D character grid
 * @param boardSize      Number of rows
 * @param boardColSize   Array of column counts for each row
 * @param word           Word to search
 * @return true          If word is found in the grid
 * @return false         Otherwise
 */
bool exist(char** board, int boardSize, int* boardColSize, char* word) {
    int rows = boardSize;
    int cols = boardColSize[0];

    // Try starting DFS from every cell
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (traversal(board, i, j, rows, cols, word, 0)) {
                return true;
            }
        }
    }
    return false;
}
