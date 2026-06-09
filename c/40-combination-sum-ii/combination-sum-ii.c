// -----------------------------------------------------------------------------
// Problem : Combination Sum II
// Source  : https://leetcode.com/problems/combination-sum-ii/
// Author  : Ngoc-Tien To
// -----------------------------------------------------------------------------


#define N 200   // Maximum possible number of combinations


// ---------------------------------- HELPER FUNCTIONS ----------------------------------

/**
 * @brief Compare function for qsort (ascending order)
 */
int cmp(const void *a, const void *b){
    return *(int *)a - *(int *)b;
}

/**
 * @brief Perform DFS (backtracking) to find unique combinations that sum to target
 * 
 * @param nums   Input array (sorted)
 * @param index  Current index in nums
 * @param size   Size of input array
 * @param temp   Temporary array holding the current combination
 * @param count  Current size of temp
 * @param target Remaining target value
 * @param result 2D array to store all valid combinations
 * @param rows   Number of valid combinations found so far
 * @param cols   Array storing the length of each combination
 */
void dfs(int* nums, int index, int size, int *temp, int *count, int target, int **result, int* rows, int** cols){
    // Base case: found a valid combination
    if(target == 0){
        result[(*rows)] = (int *)malloc(sizeof(int) * (*count));
        for(int j = 0; j < (*count); j++){
            result[(*rows)][j] = temp[j];
        }
        (*cols)[(*rows)] = (*count);
        (*rows)++;
        return;
    }

    // If out of range or target becomes negative, stop recursion
    if(index >= size || target < 0){
        return;
    }

    // --------------------------------------------------------------
    // Decision 1: Include current number nums[index]
    // --------------------------------------------------------------
    temp[(*count)++] = nums[index];
    dfs(nums, index + 1, size, temp, count, target - nums[index], result, rows, cols);

    // --------------------------------------------------------------
    // Decision 2: Exclude current number and skip duplicates
    // --------------------------------------------------------------
    (*count)--;  // Backtrack
    while(index + 1 < size && nums[index + 1] == nums[index]){
        index++;
    }
    dfs(nums, index + 1, size, temp, count, target, result, rows, cols);
}


// ---------------------------------- MAIN FUNCTION ----------------------------------

/**
 * @brief Return all unique combinations where candidates' sum equals target
 * 
 * @param candidates          Input array
 * @param candidatesSize      Length of the input array
 * @param target              Target sum
 * @param returnSize          Pointer to store number of valid combinations
 * @param returnColumnSizes   Pointer to array storing each combination's length
 * @return int**              2D array of combinations
 */
int** combinationSum2(int* candidates, int candidatesSize, int target, int* returnSize, int** returnColumnSizes) {
    // Allocate memory for results
    int **result = (int **)malloc(sizeof(int *) * N);
    (*returnColumnSizes) = (int *)malloc(sizeof(int) * N);
    *returnSize = 0;

    // Temporary storage for current combination
    int *temp = (int *)malloc(sizeof(int) * N);
    int count = 0;

    // Sort candidates to handle duplicates easily
    qsort(candidates, candidatesSize, sizeof(int), cmp);

    // Perform backtracking search
    dfs(candidates, 0, candidatesSize, temp, &count, target, result, returnSize, returnColumnSizes);

    // Resize memory to actual used size
    result = (int **)realloc(result, sizeof(int *) * (*returnSize));
    (*returnColumnSizes) = (int *)realloc((*returnColumnSizes), sizeof(int) * (*returnSize));

    free(temp);
    return result;
}
