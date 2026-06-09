#define N 1e9

bool containsDuplicate(int* nums, int numsSize) {
    int *count = (int *)calloc(2 * N + 1, sizeof(int));
    for(int i = 0; i < numsSize; i++){
        int index = nums[i] + N;
        if(count[index]){
            free(count);
            return true;
        }
        count[index] = 1;
    }
    free(count);
    return false;
}
