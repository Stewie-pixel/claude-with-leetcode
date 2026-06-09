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
