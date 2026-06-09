// Generate Parentheses 
// Source: https://leetcode.com/problems/generate-parentheses/
// Author: Ngoc-Tien To

// Decription:
// Given n pairs of parentheses, write a function to generate all combinations of well-formed parentheses.

// Idea: Draw the recursive tree and think about the conditions when an opening/closing bracket can be appended
// 1. Use backtracking to generate all valid combinations of parentheses
// 2. Define a recursive function `backtracking(result, n, returnSize, open, close, combination, position)`
//      - `open`: number of '(' used so far
//      - `close`: number of ')' used so far
//      - `combination`: current string being built
//      - `position`: current index in the combination
// 3. Base condition:
//      3.1 If open == close == n:
//          a. Current combination is complete and valid
//          b. Copy the combination into result array
//          c. Increase the returnSize counter
// 4. Recursive exploration:
//      4.1 If open < n:
//          a. Add '(' at current position
//          b. Recurse with open + 1
//      4.2 If close < open:
//          a. Add ')' at current position
//          b. Recurse with close + 1
// 5. Continue exploring until all valid combinations are generated
// 6. After recursion completes, return the result array containing all valid parentheses strings


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
    char **result = (char **)malloc(sizeof(char *) * 2000);
    (*returnSize) = 0;
    int position = 0;
    char *combination = (char *)malloc(sizeof(char) * (2 * n + 1));
    backtracking(result, n, returnSize, 0, 0, combination, position);
    result = (char **)realloc(result, sizeof(char *) * (*returnSize));
    free(combination);
    return result;
}