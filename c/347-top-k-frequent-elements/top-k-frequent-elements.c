// Source: https://leetcode.com/problems/top-k-frequent-elements/
// Author: Ngoc-Tien To

// Idea:
// 1. Use a hash map to count the frequency of each element.
// 2. Store each (element, frequency) pair in an array/struct.
// 3. Sort the array by frequency in descending order.
// 4. Take the first k elements and add their values to the result array.

#define N 1e4

struct Helper{
    int nums;
    int fre;
};

// Utility to sort elements's frequency in no-increasing order
int compare(const void *a, const void *b){
    struct Helper *x = (struct Helper *)a;
    struct Helper *y = (struct Helper *)b;
    return y->fre - x->fre;
}


// Add the number and its frequency to helper array
struct Helper *createHelperArray(int *fre, int numsSize, int *count){
    struct Helper *f = (struct Helper *)malloc(sizeof(struct Helper) * numsSize);
    for(int i = 0; i <= 2 * N; i++){
        if(fre[i] != 0){
            f[(*count)].nums = i - N;
            f[(*count)].fre = fre[i];
            (*count)++;
        }
    }
    f = (struct Helper *)realloc(f, sizeof(struct Helper) * (*count));
    return f;
}

int* topKFrequent(int* nums, int numsSize, int k, int* returnSize) {
    if(numsSize <= 0){
        return NULL;
    }

    int *result = (int *)malloc(sizeof(int) * k);
    int count = 0;
    
    int *fre = (int *)calloc(20001, sizeof(int));// Count the frequency of each element
    for(int i = 0; i < numsSize; i++){
        int index = nums[i] + N;
        fre[index]++;
    }
    
    struct Helper *helper = createHelperArray(fre, numsSize, &count);
    qsort(helper, count, sizeof(struct Helper), compare);

    // Add k needed elements to result array
    for(int i = 0; i < k; i++){
        result[i] = helper[i].nums;
    }
    (*returnSize) = k;
    
    free(helper);
    return result;
}