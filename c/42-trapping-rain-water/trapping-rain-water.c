// Trapping rain water
// Source: https://leetcode.com/problems/trapping-rain-water/
// Author: Ngoc-Tien To

// Description:
//   Given an elevation map, compute how much water it can trap after raining.
//   Two solutions are provided:
//     1. Dynamic Programming with precomputed arrays (O(n) time, O(n) space)
//     2. Two-pointer optimized version (O(n) time, O(1) space)

int max(int a, int b){
    return a > b ? a : b;
}

int min(int a, int b){
    return a < b ? a : b;
}

// Idea:
// 1. The amount of water trapped at each index depends on the height of the tallest
//    bar to its left and the tallest bar to its right.
//    → water[i] = min(maxLeft[i], maxRight[i]) - height[i]
//
// 2. To avoid repeatedly scanning left and right for every index (which would be O(n²)),
//    precompute two arrays:
//       - maxLeft[i]: the maximum height from the start up to index i
//       - maxRight[i]: the maximum height from the end up to index i
//
// 3. Build both arrays in O(n):
//       - From left to right → fill maxLeft
//       - From right to left → fill maxRight
//
// 4. Traverse the array once more:
//       - For each index i, compute the trapped water:
//             capacity += min(maxLeft[i], maxRight[i]) - height[i]
//
// 5. The total 'capacity' after the loop is the total trapped water.

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

// An updated solution

// Idea:
// 1. Instead of using two arrays (maxLeft[] and maxRight[]), keep only two variables
//    maxLeft and maxRight to track the tallest bars on both sides.
//    → This reduces space complexity from O(n) to O(1).
//
// 2. Use two pointers: 'left' starting from the beginning, and 'right' from the end.
//    - Compare height[left] and height[right]:
//
//        a. If height[left] <= height[right]:
//             → The left side is lower, so the water trapped there depends only on maxLeft.
//             → Move left++ and update maxLeft = max(maxLeft, height[left]).
//             → Add trapped water: capacity += maxLeft - height[left].
//
//        b. Otherwise (height[left] > height[right]):
//             → The right side is lower, so the water trapped there depends only on maxRight.
//             → Move right-- and update maxRight = max(maxRight, height[right]).
//             → Add trapped water: capacity += maxRight - height[right].
//
// 3. Continue until left and right meet.
// 4. The total 'capacity' is the sum of all trapped water.


int trap2(int* height, int heightSize) {
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