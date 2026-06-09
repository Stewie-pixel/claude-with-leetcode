#include <stdlib.h>

typedef struct {
    int k;
    int *minHeap;
    int size;
} KthLargest;

int compare(const void *a, const void *b) {
    return *(int *)b - *(int *)a;
}

KthLargest* kthLargestCreate(int k, int* nums, int numsSize) {
    KthLargest *obj = (KthLargest *)malloc(sizeof(KthLargest));
    obj->k = k;
    obj->minHeap = (int *)malloc(sizeof(int) * numsSize);
    obj->size = numsSize;
    for(int i = 0; i < numsSize; i++) {
        obj->minHeap[i] = nums[i];
    }
    qsort(obj->minHeap, numsSize, sizeof(int), compare);
    return obj;
}

int kthLargestAdd(KthLargest* obj, int val) {
    obj->minHeap = (int *)realloc(obj->minHeap, sizeof(int) * (obj->k + 1));
    int i;
    for(i = obj->k - 1; i >= 0; i--) {
        if(val > obj->minHeap[i]) {
            obj->minHeap[i + 1] = obj->minHeap[i];
        }
        else {
            break;
        }
    }
    obj->minHeap[i + 1] = val;
    return obj->minHeap[obj->k - 1];
}

void kthLargestFree(KthLargest* obj) {
    free(obj->minHeap);
    free(obj);
}
