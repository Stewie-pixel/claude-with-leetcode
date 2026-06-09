// -----------------------------------------------------------------------------
// Problem : Subsets
// Source  : https://leetcode.com/problems/subsets/
// Author  : Ngoc-Tien To
// -----------------------------------------------------------------------------


#define N 10000   // Maximum number of subsets (2^numsSize, upper bound)
#define M 1000    // Maximum possible length of nums


// ---------------------------------- DFS HELPER FUNCTION ----------------------------------

/**
 * @brief Generate all subsets using DFS (Depth-First Search)
 * 
 * @param nums      Input array
 * @param numsSize  Size of input array
 * @param index     Current position in nums
 * @param temp      Temporary array to hold the current subset
 * @param count     Current length of temp
 * @param result    2D array to store all subsets
 * @param rows      Current number of generated subsets
 * @param cols      Array to store the length of each subset
 */
void dfs(int *nums, int numsSize, int index, int *temp, int *count, int **result, int *rows, int **cols) {
    // Base case: reached the end of the array
    if (index >= numsSize) {
        // If no elements are chosen → subset = []
        if ((*count) == 0) {
            result[(*rows)] = NULL;           // Empty subset
            (*cols)[(*rows)] = (*count);      // Column size = 0
            (*rows)++;
            return;
        }

        // Otherwise, allocate and store the current subset
        result[(*rows)] = (int *)malloc(sizeof(int) * (*count));
        for (int j = 0; j < (*count); j++) {
            result[(*rows)][j] = temp[j];     // Copy elements
        }
        (*cols)[(*rows)] = (*count);          // Store subset length
        (*rows)++;
        return;
    }

    // -------------------------------------------------------------------------
    // Decision 1: Include nums[index] in current subset
    // -------------------------------------------------------------------------
    temp[(*count)++] = nums[index];
    dfs(nums, numsSize, index + 1, temp, count, result, rows, cols);

    // -------------------------------------------------------------------------
    // Decision 2: Exclude nums[index] and move on
    // -------------------------------------------------------------------------
    (*count)--;  // Backtrack (remove the last element)
    dfs(nums, numsSize, index + 1, temp, count, result, rows, cols);
}


// ---------------------------------- MAIN FUNCTION ----------------------------------

/**
 * @brief Main function to generate all subsets
 * 
 * @param nums               Input array
 * @param numsSize           Size of input array
 * @param returnSize         Pointer to number of subsets
 * @param returnColumnSizes  Pointer to array storing the size of each subset
 * @return int**             2D array of all subsets
 */
int** subsets(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
    // Allocate memory for result storage
    int **result = (int **)malloc(sizeof(int *) * N);
    (*returnColumnSizes) = (int *)malloc(sizeof(int) * N);

    *returnSize = 0;   // Initialize count of subsets
    int temp[M], count = 0;

    // Perform DFS to generate subsets
    dfs(nums, numsSize, 0, temp, &count, result, returnSize, returnColumnSizes);

    // Reallocate memory to fit exact result size
    result = (int **)realloc(result, sizeof(int *) * (*returnSize));
    (*returnColumnSizes) = (int *)realloc((*returnColumnSizes), sizeof(int) * (*returnSize));

    return result;
}
