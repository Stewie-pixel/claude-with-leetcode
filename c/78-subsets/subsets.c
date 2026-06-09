#define N 10000
#define M 1000

void dfs(int *nums, int numsSize, int index, int *temp, int *count, int **result, int *rows, int **cols) {

    if (index >= numsSize) {

        if ((*count) == 0) {
            result[(*rows)] = NULL;
            (*cols)[(*rows)] = (*count);
            (*rows)++;
            return;
        }

        result[(*rows)] = (int *)malloc(sizeof(int) * (*count));
        for (int j = 0; j < (*count); j++) {
            result[(*rows)][j] = temp[j];
        }
        (*cols)[(*rows)] = (*count);
        (*rows)++;
        return;
    }

    temp[(*count)++] = nums[index];
    dfs(nums, numsSize, index + 1, temp, count, result, rows, cols);

    (*count)--;
    dfs(nums, numsSize, index + 1, temp, count, result, rows, cols);
}

int** subsets(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {

    int **result = (int **)malloc(sizeof(int *) * N);
    (*returnColumnSizes) = (int *)malloc(sizeof(int) * N);

    *returnSize = 0;
    int temp[M], count = 0;

    dfs(nums, numsSize, 0, temp, &count, result, returnSize, returnColumnSizes);

    result = (int **)realloc(result, sizeof(int *) * (*returnSize));
    (*returnColumnSizes) = (int *)realloc((*returnColumnSizes), sizeof(int) * (*returnSize));

    return result;
}
