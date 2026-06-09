// -----------------------------------------------------------------------------
// Problem: Kth Largest Element in a Stream
// Source : https://leetcode.com/problems/kth-largest-element-in-a-stream/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Task: Design a structure that returns the kth largest element in a stream
//    of integers after each insertion.
// 2. Approach: Maintain a min-heap (or sorted buffer) of size k. The root/top
//    of the min-heap is the kth largest. This file uses a simple sorted buffer
//    approach for clarity (qsort/shift), though a proper heap is preferable.
//
// Time Complexity  : O(k)–O(n) per add for buffer approach (O(log k) with heap)
// Space Complexity : O(k)
// -----------------------------------------------------------------------------

#include <stdlib.h>

typedef struct {
    int k;
    int *minHeap; // sorted descending buffer used as min-heap alternative
    int size;     // current size of buffer
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