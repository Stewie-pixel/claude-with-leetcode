// Source: https://leetcode.com/problems/container-with-most-water/
// Author: Ngoc-Tien To

// Idea: Think of its as finding the largest rectangle formed by two lines.
// Initialize two pointers (left, right) at the beginning and at the end.
// The width decreases as we move any pointer,
// so to maximum the area, we must look for a taller line,
// by moving the pointer at shorter line inward

int maxArea(int* height, int heightSize) {
    int maxValue = 0;
    int left = 0, right = heightSize - 1;

    while(left < right){
        int minHeight = (height[left] < height[right]) ? height[left] : height[right];
        int area = (right - left) * minHeight;

        maxValue = (area > maxValue) ? area : maxValue;

        if(height[left] < height[right]){
            left++;
        }
        else{
            right--;
        }
    }    

    return maxValue;
}