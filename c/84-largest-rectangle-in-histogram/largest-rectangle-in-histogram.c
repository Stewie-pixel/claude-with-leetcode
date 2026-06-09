// Largest Rectangle in Histogram
// Source: https://leetcode.com/problems/largest-rectangle-in-histogram/
// Author: Ngoc-Tien To

// Description:
// Given an array of integers heights representing the histogram's bar height 
// where the width of each bar is 1, return the area of the largest rectangle in the histogram.

// Idea:
// 1. Use a Monotonic Stack to store indices of bars in increasing order
// 2. When encoutering a bar shorter than the bar at the top of stack -> pop the top:
//      - The top of the stack is the highest bar that the rectangle can widen
//      - Width = (stack is empty ? i : i - stack[cap - 1] - 1)
// 3. Update maxArea after each pop operator
// 4. Add a dummy bar with height 0 at the end of heights[] to flush the stack


int max(int a, int b) {
    return a < b ? b : a;
}

int largestRectangleArea(int* heights, int heightsSize) {
    int n = heightsSize;
    int *stack = (int *)malloc(sizeof(int) * (n + 1));
    int cap = 0;
    int maxArea = 0;
    for(int i = 0; i <= n; i++) {
        int currentHeight = (i == n ? 0 : heights[i]);
        while(cap > 0 && heights[stack[cap - 1]] > currentHeight) {
            int top = stack[--cap];
            int width = (cap == 0 ? i : i - stack[cap - 1] - 1);
            maxArea = max(maxArea, heights[top] * width);
        }
        stack[cap++] = i;
    }
    free(stack);
    return maxArea;
}