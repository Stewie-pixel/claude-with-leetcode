void backtracking(char **result, int n, int *returnSize, int open, int close, char *combination, int position){
    if(open == close && open == n){
        result[(*returnSize)] = (char *)malloc(sizeof(char) * (2 * n + 1));
        combination[position] = '\0';
        strcpy(result[(*returnSize)], combination);
        (*returnSize)++;
        return;
    }
    if(open < n){
        combination[position] = '(';
        backtracking(result, n, returnSize, open + 1, close, combination, position + 1);
    }
    if(close < open){
        combination[position] = ')';
        backtracking(result, n, returnSize, open, close + 1, combination, position + 1);
    }
}

char** generateParenthesis(int n, int* returnSize) {
    enum { MAX_RESULTS = 1430 }; // Max combinations for n<=8 (Catalan(8))
    char **result = (char **)malloc(sizeof(char *) * MAX_RESULTS);
    (*returnSize) = 0;
    int position = 0;
    char *combination = (char *)malloc(sizeof(char) * (2 * n + 1));
    backtracking(result, n, returnSize, 0, 0, combination, position);
    result = (char **)realloc(result, sizeof(char *) * (*returnSize));
    free(combination);
    return result;
}
