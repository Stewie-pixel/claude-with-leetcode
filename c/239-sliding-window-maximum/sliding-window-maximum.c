// -----------------------------------------------------------------------------
// Problem: Sliding Window Maximum
// Source : https://leetcode.com/problems/sliding-window-maximum/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- IDEA ----------------------------------
//
// 1. Use a deque to store indexes of useful elements in current window.
// 2. Maintain deque in decreasing order of corresponding values in `nums`.
// 3. The front of deque always represents the index of the maximum element
//    in the current window.
// 4. For each new element, remove indices that are out of the current window
//    or smaller than the current element.
// 5. Append the maximum (nums[dqueue[front]]) to the result after the first
//    window is formed.
//
// Time Complexity  : O(n)
// Space Complexity : O(n)
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

int* maxSlidingWindow(int* nums, int numsSize, int k, int* returnSize) {
    int *result = (int *)malloc(sizeof(int) * (numsSize - k + 1));
    if (!result) {
        *returnSize = 0;
        return NULL;
    }

    int size = 0;
    int *dqueue = (int *)malloc(sizeof(int) * numsSize); // store indexes
    int front = 0, rear = -1;
    int i = 0;

    // 1. Process the first k elements (initial window)
    while (i < k) {
        while (front <= rear && nums[dqueue[rear]] <= nums[i]) {
            rear--;
        }
        dqueue[++rear] = i;
        i++;
    }

    // 2. First window's maximum
    result[size++] = nums[dqueue[front]];

    // 3. Process remaining elements
    while (i < numsSize) {
        // Remove elements out of window range
        while (front <= rear && dqueue[front] < i - k + 1) {
            front++;
        }

        // Remove all elements smaller than current
        while (front <= rear && nums[dqueue[rear]] <= nums[i]) {
            rear--;
        }

        // Add current element index
        dqueue[++rear] = i;

        // Append current maximum to result
        result[size++] = nums[dqueue[front]];

        i++;
    }

    free(dqueue);
    *returnSize = size;
    return result;
}
