// -----------------------------------------------------------------------------
// Problem : Palindrome Partitioning
// Source  : https://leetcode.com/problems/palindrome-partitioning/
// Author  : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// Approach: DFS + Backtracking
//   - Explore all possible ways to split the string into substrings.
//   - For each start index `i`, extend to every end index `j ≥ i`.
//   - If substring s[i..j] is a palindrome, recursively continue from j+1.
//   - When the recursion reaches the end (index == len), a valid partition is found.
//
// Time Complexity: O(2^n * n)
// Space Complexity: O(n^2)
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Check if substring s[left..right] is a palindrome
bool isPalindrome(char *s, int left, int right) {
    while (left < right) {
        if (s[left] != s[right]) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

// DFS helper to generate all palindrome partitions
void dfs(char *s, int index, int len, char **temp, int *count, 
         char ***result, int *rows, int **cols) {

    // Base case: reached the end of the string → store current partition
    if (index == len) {
        result[(*rows)] = (char **)malloc(sizeof(char *) * (*count));
        for (int j = 0; j < (*count); j++) {
            result[(*rows)][j] = (char *)malloc(sizeof(char) * (strlen(temp[j]) + 1));
            strcpy(result[(*rows)][j], temp[j]);
        }
        (*cols)[(*rows)] = (*count);
        (*rows)++;
        return;
    }

    // Explore all possible end indices for the next palindrome substring
    for (int j = index; j < len; j++) {
        if (isPalindrome(s, index, j)) {
            // Choose substring s[index..j]
            temp[(*count)] = (char *)malloc(sizeof(char) * (j - index + 2));
            strncpy(temp[(*count)], s + index, j - index + 1);
            temp[(*count)][j - index + 1] = '\0';
            (*count)++;

            // Recurse for the remaining substring
            dfs(s, j + 1, len, temp, count, result, rows, cols);

            // Backtrack
            (*count)--;
        }
    }
}

// Main function
char*** partition(char* s, int* returnSize, int** returnColumnSizes) {
    int len = strlen(s);
    int sizeTemp = 1 << len; // upper bound on number of partitions

    char ***result = (char ***)malloc(sizeof(char **) * sizeTemp);
    *returnColumnSizes = (int *)malloc(sizeof(int) * sizeTemp);
    *returnSize = 0;

    char **temp = (char **)malloc(sizeof(char *) * len);
    int count = 0;

    dfs(s, 0, len, temp, &count, result, returnSize, returnColumnSizes);

    // Reallocate to fit the actual number of results
    result = (char ***)realloc(result, sizeof(char **) * (*returnSize));
    *returnColumnSizes = (int *)realloc(*returnColumnSizes, sizeof(int) * (*returnSize));

    for (int i = 0; i < len; i++) {
        free(temp[i]);
    }
    free(temp);

    return result;
}
