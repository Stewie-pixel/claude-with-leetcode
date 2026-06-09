#include <stdio.h>
#include <stdlib.h>

int* maxSlidingWindow(int* nums, int numsSize, int k, int* returnSize) {
    int *result = (int *)malloc(sizeof(int) * (numsSize - k + 1));
    if (!result) {
        *returnSize = 0;
        return NULL;
    }

    int size = 0;
    int *dqueue = (int *)malloc(sizeof(int) * numsSize);
    int front = 0, rear = -1;
    int i = 0;

    while (i < k) {
        while (front <= rear && nums[dqueue[rear]] <= nums[i]) {
            rear--;
        }
        dqueue[++rear] = i;
        i++;
    }

    result[size++] = nums[dqueue[front]];

    while (i < numsSize) {

        while (front <= rear && dqueue[front] < i - k + 1) {
            front++;
        }

        while (front <= rear && nums[dqueue[rear]] <= nums[i]) {
            rear--;
        }

        dqueue[++rear] = i;

        result[size++] = nums[dqueue[front]];

        i++;
    }

    free(dqueue);
    *returnSize = size;
    return result;
}
