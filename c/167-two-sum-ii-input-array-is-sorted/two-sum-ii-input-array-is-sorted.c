// Source: https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/
// Author: Ngoc-Tien To

// Idea:
// 1. Initialize two pointers: i at the beginning, j at the end of the array.
// 2. While i < j:
//      2.1 Compute sum = nums[i] + nums[j].
//      2.2 If sum > target, move j backward to reduce the sum.
//      2.3 If sum < target, move i forward to increase the sum.
//      2.4 If sum == target, store the pair of indices (i, j) as the answer.

int* twoSum(int* numbers, int numbersSize, int target, int* returnSize) {
    int *result = (int *)malloc(sizeof(int) * 2);
    (*returnSize) = 2;
    int i = 0, j = numbersSize - 1;
    while(i < j){
        int sum = numbers[i] + numbers[j];
        if(sum > target){
            j--;
        }
        else if(sum < target){
            i++;
        }
        else{
            result[0] = i + 1;
            result[1] = j + 1;
            break;
        }
    }
    return result;
}