// Binary Search
// Source: https://leetcode.com/problems/binary-search/
// Author: Ngoc-Tien To

// Description:
// Given an array of integers nums which is sorted in ascending order,
// and an integer target, write a function to search target in nums. 
// If target exists, then return its index. Otherwise, return -1.
// You must write an algorithm with O(log n) runtime complexity.

// Idea: use binary search algorithm

int search(int* nums, int numsSize, int target) {
    int left = 0, right = numsSize - 1;
    while(left <= right){
        int mid = (left + right) / 2;
        if(nums[mid] == target){
            return mid;
        }
        else if(target < nums[mid]){
            right = mid - 1;
        }
        else{
            left = mid + 1;
        }
    }
    return -1;
}