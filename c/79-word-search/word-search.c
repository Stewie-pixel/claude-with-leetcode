bool traversal(char **board, int row, int col, int rowSize, int colSize, char *word, int index) {

    if (row < 0 || row >= rowSize || col < 0 || col >= colSize || board[row][col] == '#')
        return false;

    if (board[row][col] != word[index])
        return false;

    if (word[index + 1] == '\0')
        return true;

    char temp = board[row][col];
    board[row][col] = '#';

    bool found = traversal(board, row + 1, col, rowSize, colSize, word, index + 1) ||
                 traversal(board, row, col + 1, rowSize, colSize, word, index + 1) ||
                 traversal(board, row - 1, col, rowSize, colSize, word, index + 1) ||
                 traversal(board, row, col - 1, rowSize, colSize, word, index + 1);

    board[row][col] = temp;

    return found;
}

bool exist(char** board, int boardSize, int* boardColSize, char* word) {
    int rows = boardSize;
    int cols = boardColSize[0];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (traversal(board, i, j, rows, cols, word, 0)) {
                return true;
            }
        }
    }
    return false;
}
