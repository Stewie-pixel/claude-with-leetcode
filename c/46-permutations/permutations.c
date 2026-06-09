void dfsPermute(int* nums, int numsSize, int *temp, int *count,
                int **result, int *rows, int **cols, int *visited) {

    if ((*count) == numsSize) {
        result[(*rows)] = (int *)malloc(sizeof(int) * numsSize);
        for (int k = 0; k < numsSize; k++) {
            result[(*rows)][k] = temp[k];
        }
        (*cols)[(*rows)] = numsSize;
        (*rows)++;
        return;
    }

    for (int j = 0; j < numsSize; j++) {
        if (!visited[j]) {
            visited[j] = 1;
            temp[(*count)++] = nums[j];

            dfsPermute(nums, numsSize, temp, count, result, rows, cols, visited);

            (*count)--;
            visited[j] = 0;
        }
    }
}

int** permute(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {

    int factorial = 1;
    for (int i = 1; i <= numsSize; i++) {
        factorial *= i;
    }

    int **result = (int **)malloc(sizeof(int *) * factorial);
    *returnColumnSizes = (int *)malloc(sizeof(int) * factorial);
    *returnSize = 0;

    int *temp = (int *)malloc(sizeof(int) * numsSize);
    int count = 0;
    int *visited = (int *)calloc(numsSize, sizeof(int));

    dfsPermute(nums, numsSize, temp, &count, result, returnSize, returnColumnSizes, visited);

    free(temp);
    free(visited);

    return result;
}
