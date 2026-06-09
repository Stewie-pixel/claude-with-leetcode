// -----------------------------------------------------------------------------
// Problem : Subsets II
// Source  : https://leetcode.com/problems/subsets-ii/
// Author  : Ngoc-Tien To
// -----------------------------------------------------------------------------


// ------------------------------ HELPER FUNCTION ------------------------------

/**
 * @brief Comparator for qsort() to sort integers in ascending order
 */
int compare(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}


/**
 * @brief Depth-First Search (DFS) to generate all unique subsets.
 *
 * @param nums      Sorted input array (to handle duplicates)
 * @param index     Current index in nums
 * @param numsSize  Size of input array
 * @param temp      Temporary array to store current subset
 * @param count     Current subset length
 * @param result    2D array storing all subsets
 * @param rows      Number of subsets generated so far
 * @param cols      Array storing the length of each subset
 */
void dfs(int* nums, int index, int numsSize, int *temp, int *count,
         int **result, int* rows, int** cols) {
    
    // Base case: reached the end → store current subset
    if (index == numsSize) {
        result[(*rows)] = malloc(sizeof(int) * (*count));
        for (int j = 0; j < (*count); j++) {
            result[(*rows)][j] = temp[j];
        }
        (*cols)[(*rows)] = (*count);
        (*rows)++;
        return;
    }

    // -------------------------------------------------------------------------
    // Option 1: Include nums[index]
    // -------------------------------------------------------------------------
    temp[(*count)++] = nums[index];
    dfs(nums, index + 1, numsSize, temp, count, result, rows, cols);

    // -------------------------------------------------------------------------
    // Skip duplicates to ensure uniqueness
    // -------------------------------------------------------------------------
    while (index + 1 < numsSize && nums[index + 1] == nums[index]) {
        index++;
    }

    // -------------------------------------------------------------------------
    // Option 2: Exclude nums[index]
    // -------------------------------------------------------------------------
    (*count)--;  // Backtrack
    dfs(nums, index + 1, numsSize, temp, count, result, rows, cols);
}


// ------------------------------- MAIN FUNCTION -------------------------------

/**
 * @brief Generate all unique subsets of nums (handling duplicates)
 * 
 * @param nums               Input array
 * @param numsSize           Size of input array
 * @param returnSize         Pointer to total number of subsets
 * @param returnColumnSizes  Pointer to array storing each subset’s size
 * @return int**             2D array containing all subsets
 */
int** subsetsWithDup(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
    // Maximum possible subsets = 2^numsSize
    int sizeTemp = 1 << numsSize;

    // Allocate result containers
    int **result = (int **)malloc(sizeof(int *) * sizeTemp);
    *returnColumnSizes = (int *)malloc(sizeof(int) * sizeTemp);
    *returnSize = 0;

    // Temporary array for DFS path
    int *temp = (int *)malloc(sizeof(int) * numsSize);
    int count = 0;

    // Sort to handle duplicates properly
    qsort(nums, numsSize, sizeof(int), compare);

    // Generate all subsets using DFS
    dfs(nums, 0, numsSize, temp, &count, result, returnSize, returnColumnSizes);

    // Shrink memory to actual result size
    result = (int **)realloc(result, sizeof(int *) * (*returnSize));
    *returnColumnSizes = (int *)realloc((*returnColumnSizes), sizeof(int) * (*returnSize));

    free(temp);
    return result;
}
