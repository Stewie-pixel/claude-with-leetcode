// -----------------------------------------------------------------------------
// Problem : Combination Sum
// Source  : https://leetcode.com/problems/combination-sum/
// Author  : Ngoc-Tien To
// -----------------------------------------------------------------------------


#define N 200  // Maximum number of combinations (enough for LeetCode input limits)


// ---------------------------------- DFS BACKTRACKING ----------------------------------

/**
 * @brief Recursive DFS function to generate all valid combinations that sum to the target.
 * 
 * @param nums      Array of candidate numbers
 * @param index     Current index in the array
 * @param size      Total size of nums
 * @param temp      Temporary array holding the current combination
 * @param count     Number of elements currently in temp
 * @param target    Remaining target value
 * @param result    2D array storing all valid combinations
 * @param rows      Current number of combinations found (acts as row index)
 * @param cols      Array storing the length of each combination
 */
void dfs(int* nums, int index, int size, int *temp, int *count, int target, int **result, int* rows, int** cols) {
    // Base case: if target == 0, a valid combination is found
    if (target == 0) {
        result[(*rows)] = (int *)malloc(sizeof(int) * (*count));
        for (int j = 0; j < (*count); j++) {
            result[(*rows)][j] = temp[j];
        }
        (*cols)[(*rows)] = (*count);
        (*rows)++;
        return;
    }

    // Stop if index is out of range or target becomes negative
    if (index >= size || target < 0) {
        return;
    }

    // ----------------------------------------------------
    // 1️. Choose the current number (can be reused)
    // ----------------------------------------------------
    temp[(*count)++] = nums[index];
    dfs(nums, index, size, temp, count, target - nums[index], result, rows, cols);

    // ----------------------------------------------------
    // 2️. Skip the current number and move to the next one
    // ----------------------------------------------------
    (*count)--;  // Backtrack (remove last added element)
    dfs(nums, index + 1, size, temp, count, target, result, rows, cols);
}


// ---------------------------------- MAIN FUNCTION ----------------------------------

/**
 * @brief Returns all unique combinations of numbers that sum up to the given target.
 * 
 * @param candidates            Input array of distinct integers
 * @param candidatesSize        Size of the input array
 * @param target                Target sum
 * @param returnSize            Output pointer for the number of combinations found
 * @param returnColumnSizes     Output pointer for the length of each combination
 * @return int**                2D array containing all valid combinations
 */

int** combinationSum(int* candidates, int candidatesSize, int target, int* returnSize, int** returnColumnSizes) {
    // Allocate initial memory for result storage
    int **result = (int **)malloc(sizeof(int *) * N);
    (*returnColumnSizes) = (int *)malloc(sizeof(int) * N);
    *returnSize = 0;

    int index = 0;
    int *temp = (int *)malloc(sizeof(int) * N);  // Temporary array to hold current combination
    int count = 0;

    // Start the recursive DFS
    dfs(candidates, index, candidatesSize, temp, &count, target, result, returnSize, returnColumnSizes);

    // Resize memory to fit the actual number of results
    result = (int **)realloc(result, sizeof(int *) * (*returnSize));
    (*returnColumnSizes) = (int *)realloc((*returnColumnSizes), sizeof(int) * (*returnSize));

    free(temp);  // Free temporary array
    return result;
}
