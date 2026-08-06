int max(int a, int b){
    return a > b ? a : b;
}

int min(int a, int b){
    return a < b ? a : b;
}

int trap1(int* height, int heightSize) {
    int capacity = 0;
    int *maxLeft = (int *)malloc(sizeof(int) * heightSize);
    int *maxRight = (int *)malloc(sizeof(int) * heightSize);

    maxLeft[0] = height[0];
    for(int i = 1; i < heightSize; i++){
        maxLeft[i] = max(maxLeft[i - 1], height[i]);
    }
    maxRight[heightSize - 1] = height[heightSize - 1];
    for(int i = heightSize - 2; i >= 0; i--){
        maxRight[i] = max(maxRight[i + 1], height[i]);
    }

    for(int i = 0; i < heightSize; i++){
        capacity += min(maxLeft[i], maxRight[i]) - height[i];
    }

    free(maxLeft);
    free(maxRight);

    return capacity;
}

int trap(int* height, int heightSize) {
    int capacity = 0;
    int left = 0, right = heightSize - 1;
    int maxLeft = height[left], maxRight = height[right];

    while(left < right){
        if(height[left] > height[right]){
            right--;
            maxRight = max(maxRight, height[right]);
            capacity += maxRight - height[right];
        }
        else{
            left++;
            maxLeft = max(maxLeft, height[left]);
            capacity += maxLeft - height[left];
        }
    }

    return capacity;
}
