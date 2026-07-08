#define N 200

void dfs(int* nums, int index, int size, int *temp, int *count, int target, int **result, int* rows, int** cols) {

    if (target == 0) {
        result[(*rows)] = (int *)malloc(sizeof(int) * (*count));
        for (int j = 0; j < (*count); j++) {
            result[(*rows)][j] = temp[j];
        }
        (*cols)[(*rows)] = (*count);
        (*rows)++;
        return;
    }

    if (index >= size || target < 0) {
        return;
    }

    temp[(*count)++] = nums[index];
    dfs(nums, index, size, temp, count, target - nums[index], result, rows, cols);

    (*count)--;
    dfs(nums, index + 1, size, temp, count, target, result, rows, cols);
}

int** combinationSum(int* candidates, int candidatesSize, int target, int* returnSize, int** returnColumnSizes) {

    int **result = (int **)malloc(sizeof(int *) * N);
    (*returnColumnSizes) = (int *)malloc(sizeof(int) * N);
    *returnSize = 0;

    int index = 0;
    int *temp = (int *)malloc(sizeof(int) * N);
    int count = 0;

    dfs(candidates, index, candidatesSize, temp, &count, target, result, returnSize, returnColumnSizes);

    result = (int **)realloc(result, sizeof(int *) * (*returnSize));
    (*returnColumnSizes) = (int *)realloc((*returnColumnSizes), sizeof(int) * (*returnSize));

    free(temp);
    return result;
}
