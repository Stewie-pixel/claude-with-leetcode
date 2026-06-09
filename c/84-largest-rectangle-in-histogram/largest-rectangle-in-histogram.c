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
