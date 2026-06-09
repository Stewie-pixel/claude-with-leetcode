// -----------------------------------------------------------------------------
// Problem : Permutations
// Source  : https://leetcode.com/problems/permutations/
// Author  : Ngoc-Tien To
// -----------------------------------------------------------------------------


// --------------------------- BACKTRACKING HELPER -----------------------------

/**
 * @brief Recursively generate all permutations using DFS (Depth-First Search)
 * 
 * @param nums     Input array
 * @param numsSize Length of input array
 * @param temp     Temporary array to store current permutation
 * @param count    Current length of temp
 * @param result   2D array to store all generated permutations
 * @param rows     Pointer to count how many permutations found so far
 * @param cols     Array storing the length of each permutation
 * @param visited  Boolean array marking which elements have been used
 */
void dfsPermute(int* nums, int numsSize, int *temp, int *count, 
                int **result, int *rows, int **cols, int *visited) {
    // Base case: if we used all numbers, store the permutation
    if ((*count) == numsSize) {
        result[(*rows)] = (int *)malloc(sizeof(int) * numsSize);
        for (int k = 0; k < numsSize; k++) {
            result[(*rows)][k] = temp[k];
        }
        (*cols)[(*rows)] = numsSize;
        (*rows)++;
        return;
    }

    // Try placing each unused number into the current position
    for (int j = 0; j < numsSize; j++) {
        if (!visited[j]) {
            visited[j] = 1;                        // Mark as used
            temp[(*count)++] = nums[j];            // Add to current path

            dfsPermute(nums, numsSize, temp, count, result, rows, cols, visited);

            (*count)--;                            // Backtrack
            visited[j] = 0;                        // Unmark for next iteration
        }
    }
}


// ------------------------------ MAIN FUNCTION -------------------------------

/**
 * @brief Generate all permutations of the input array.
 * 
 * @param nums               Input array
 * @param numsSize           Size of input array
 * @param returnSize         Pointer to store number of permutations
 * @param returnColumnSizes  Pointer to array storing length of each permutation
 * @return int**             2D array containing all permutations
 */
int** permute(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
    // Compute maximum number of permutations (numsSize!)
    int factorial = 1;
    for (int i = 1; i <= numsSize; i++) {
        factorial *= i;
    }

    // Allocate memory for results
    int **result = (int **)malloc(sizeof(int *) * factorial);
    *returnColumnSizes = (int *)malloc(sizeof(int) * factorial);
    *returnSize = 0;

    // Temporary storage
    int *temp = (int *)malloc(sizeof(int) * numsSize);
    int count = 0;
    int *visited = (int *)calloc(numsSize, sizeof(int));  // Initialize with zeros

    // Generate permutations
    dfsPermute(nums, numsSize, temp, &count, result, returnSize, returnColumnSizes, visited);

    // Cleanup temporary arrays
    free(temp);
    free(visited);

    return result;
}
