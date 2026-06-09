int compare(const void *a, const void *b){
    return *(int *)a - *(int *)b;
}

int** threeSum(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
    int capacity = 10;
    int **result = (int **)malloc(capacity * sizeof(int *));
    int *columnSizes = (int *)malloc(capacity * sizeof(int));

    qsort(nums, numsSize, sizeof(int), compare);

    int count = 0;

    for(int i = 0; i < numsSize - 2; i++){
        if(i > 0 && nums[i] == nums[i - 1]){
            continue;
        }

        int j = i + 1, k = numsSize - 1;
        while(j < k){
            int sum = nums[i] + nums[j] + nums[k];
            if(sum == 0){

                int *temp = (int *)malloc(3 * sizeof(int));
                temp[0] = nums[i];
                temp[1] = nums[j];
                temp[2] = nums[k];

                if(count >= capacity){
                    capacity *= 2;
                    result = (int **)realloc(result, capacity * sizeof(int *));
                    columnSizes = (int *)realloc(columnSizes, capacity * sizeof(int));
                }

                result[count] = temp;
                columnSizes[count] = 3;
                count++;

                j++;
                k--;

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

    *returnSize = count;
    *returnColumnSizes = columnSizes;
    result = (int **)realloc(result, count * sizeof(int *));

    return result;
}
