// Source: https://leetcode.com/problems/3sum/
// Author: Ngoc-Tien To

// Idea: Find all unique triplets (i, j, k) such that nums[i] + nums[j] + nums[k] == 0
// 1. Sort the array in non-decreasing order to apply two-pointer technique easily
// 2. For each index i from 0 to n-3:
//      2.1 Skip duplicate nums[i] to avoid repeated triplets
//      2.2 Initialize two pointers: j = i + 1, k = n - 1
//      2.3 While j < k:
//            - Compute sum = nums[i] + nums[j] + nums[k]
//            - If sum == 0: store the triplet, move j++, k--, and skip duplicates
//            - If sum < 0: move j++ to increase sum
//            - If sum > 0: move k-- to decrease sum


int compare(const void *a, const void *b){
    return *(int *)a - *(int *)b;
}

int** threeSum(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
    int capacity = 10;// Inital size for result array
    int **result = (int **)malloc(capacity * sizeof(int *));
    int *columnSizes = (int *)malloc(capacity * sizeof(int));

    qsort(nums, numsSize, sizeof(int), compare);

    int count = 0;// Count of valid triplets found

    for(int i = 0; i < numsSize - 2; i++){
        if(i > 0 && nums[i] == nums[i - 1]){// Skip duplicate elements to avoid repeated triplets
            continue;
        }

        int j = i + 1, k = numsSize - 1;
        while(j < k){
            int sum = nums[i] + nums[j] + nums[k];
            if(sum == 0){
                // Found a valid triplet
                int *temp = (int *)malloc(3 * sizeof(int));
                temp[0] = nums[i];
                temp[1] = nums[j];
                temp[2] = nums[k];

                if(count >= capacity){ // Resize the result array if capacity is exceeded
                    capacity *= 2;
                    result = (int **)realloc(result, capacity * sizeof(int *));
                    columnSizes = (int *)realloc(columnSizes, capacity * sizeof(int));
                }

                // Store the triplet in the result array
                result[count] = temp;
                columnSizes[count] = 3;
                count++;
                
                // Move both pointers to find next triplet
                j++;
                k--;

                // Skip duplicate elements on both sides
                while(j < k && nums[j] == nums[j - 1]){
                    j++; 
                }   
                while(j < k && nums[k] == nums[k + 1]){
                    k--;
                }
            }
            else if(sum < 0){
                j++;
            }
            else{
                k--;
            }
        }
    }

    // Reallocate to save the memory
    *returnSize = count;
    *returnColumnSizes = columnSizes;
    result = (int **)realloc(result, count * sizeof(int *));

    return result;
}