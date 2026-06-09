#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

void dfs(char *s, int index, int len, char **temp, int *count,
         char ***result, int *rows, int **cols) {

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

    for (int j = index; j < len; j++) {
        if (isPalindrome(s, index, j)) {

            temp[(*count)] = (char *)malloc(sizeof(char) * (j - index + 2));
            strncpy(temp[(*count)], s + index, j - index + 1);
            temp[(*count)][j - index + 1] = '\0';
            (*count)++;

            dfs(s, j + 1, len, temp, count, result, rows, cols);

            (*count)--;
        }
    }
}

char*** partition(char* s, int* returnSize, int** returnColumnSizes) {
    int len = strlen(s);
    int sizeTemp = 1 << len;

    char ***result = (char ***)malloc(sizeof(char **) * sizeTemp);
    *returnColumnSizes = (int *)malloc(sizeof(int) * sizeTemp);
    *returnSize = 0;

    char **temp = (char **)malloc(sizeof(char *) * len);
    int count = 0;

    dfs(s, 0, len, temp, &count, result, returnSize, returnColumnSizes);

    result = (char ***)realloc(result, sizeof(char **) * (*returnSize));
    *returnColumnSizes = (int *)realloc(*returnColumnSizes, sizeof(int) * (*returnSize));

    for (int i = 0; i < len; i++) {
        free(temp[i]);
    }
    free(temp);

    return result;
}
