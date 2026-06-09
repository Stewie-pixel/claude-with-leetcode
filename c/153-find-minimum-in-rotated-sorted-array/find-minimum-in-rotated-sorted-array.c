// Find Minimum in Rotated Sorted Array
// Source: https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/
// Author: Ngoc-Tien To

// ----------------------------------DESCRIPTION----------------------------------

// Suppose an array of length n sorted in ascending order is rotated between 1 and n times. 
// For example, the array nums = [0,1,2,4,5,6,7] might become:

// [4,5,6,7,0,1,2] if it was rotated 4 times.
// [0,1,2,4,5,6,7] if it was rotated 7 times.
// Notice that rotating an array [a[0], a[1], a[2], ..., a[n-1]] 1 time results in the array [a[n-1], a[0], a[1], a[2], ..., a[n-2]].

// Given the sorted rotated array nums of unique elements, return the minimum element of this array.

// You must write an algorithm that runs in O(log n) time.


// ----------------------------------IDEA----------------------------------
// This problem is a variation of the standard binary search.
// The key observation is that the array is sorted but rotated at some "break point".
// We need to determine which side of the array (left or right) contains the minimum.
// By comparing the middle element with the boundary values, 
// we can eliminate one half of the array each time until we find the smallest element.


int findMin(int* nums, int numsSize) {
    int n = numsSize;
    int left = 0, right = n - 1;
    while(left < right){
        int mid = left + (right - left) / 2;
        if(mid > 0 && nums[mid] < nums[mid - 1]){
            return nums[mid];
        }
        if(nums[left] <= nums[mid] && nums[mid] > nums[right]){
            left = mid + 1;
        }
        else{
            right = mid - 1;
        }
    }
    return nums[left];
}