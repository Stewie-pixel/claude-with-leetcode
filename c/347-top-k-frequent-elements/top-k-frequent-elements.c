#define N 1e4

struct Helper{
    int nums;
    int fre;
};

int compare(const void *a, const void *b){
    struct Helper *x = (struct Helper *)a;
    struct Helper *y = (struct Helper *)b;
    return y->fre - x->fre;
}

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

    int *fre = (int *)calloc(20001, sizeof(int));
    for(int i = 0; i < numsSize; i++){
        int index = nums[i] + N;
        fre[index]++;
    }

    struct Helper *helper = createHelperArray(fre, numsSize, &count);
    qsort(helper, count, sizeof(struct Helper), compare);

    for(int i = 0; i < k; i++){
        result[i] = helper[i].nums;
    }
    (*returnSize) = k;

    free(helper);
    return result;
}
