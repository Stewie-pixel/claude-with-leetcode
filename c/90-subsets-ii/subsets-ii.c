int compare(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

void dfs(int* nums, int index, int numsSize, int *temp, int *count,
         int **result, int* rows, int** cols) {

    if (index == numsSize) {
        result[(*rows)] = malloc(sizeof(int) * (*count));
        for (int j = 0; j < (*count); j++) {
            result[(*rows)][j] = temp[j];
        }
        (*cols)[(*rows)] = (*count);
        (*rows)++;
        return;
    }

    temp[(*count)++] = nums[index];
    dfs(nums, index + 1, numsSize, temp, count, result, rows, cols);

    while (index + 1 < numsSize && nums[index + 1] == nums[index]) {
        index++;
    }

    (*count)--;
    dfs(nums, index + 1, numsSize, temp, count, result, rows, cols);
}

int** subsetsWithDup(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {

    int sizeTemp = 1 << numsSize;

    int **result = (int **)malloc(sizeof(int *) * sizeTemp);
    *returnColumnSizes = (int *)malloc(sizeof(int) * sizeTemp);
    *returnSize = 0;

    int *temp = (int *)malloc(sizeof(int) * numsSize);
    int count = 0;

    qsort(nums, numsSize, sizeof(int), compare);

    dfs(nums, 0, numsSize, temp, &count, result, returnSize, returnColumnSizes);

    result = (int **)realloc(result, sizeof(int *) * (*returnSize));
    *returnColumnSizes = (int *)realloc((*returnColumnSizes), sizeof(int) * (*returnSize));

    free(temp);
    return result;
}
