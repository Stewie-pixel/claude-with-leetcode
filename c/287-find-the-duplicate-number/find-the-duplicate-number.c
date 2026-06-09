// -----------------------------------------------------------------------------
// Problem: Find the Duplicate Number
// Source : https://leetcode.com/problems/find-the-duplicate-number/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- DESCRIPTION ----------------------------------
//
// Given an array of integers nums containing n + 1 integers where each integer 
// is in the range [1, n] inclusive.
//
// There is only one repeated number in nums, return this repeated number.
//
// Example:
//   Input: nums = [1,3,4,2,2]
//   Output: 2
//
//   Input: nums = [3,1,3,4,2]
//   Output: 3
//
// Constraints:
//   - You must not modify the array (read-only).
//   - You must use only constant extra space.
//   - Time complexity must be less than O(n²).
//
// ----------------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Treat the array as a linked list where each index points to nums[index].
// 2. Since one number appears twice, there will be a cycle in this "linked list".
// 3. Use Floyd’s Tortoise and Hare algorithm to detect the cycle.
// 4. First, find the intersection point of slow and fast pointers.
// 5. Then, reset one pointer to the start and move both one step at a time.
// 6. The node where they meet again is the duplicate number.
//
//  Time Complexity  : O(n)
//  Space Complexity : O(1)
// -----------------------------------------------------------------------------


int findDuplicate(int* nums, int numsSize) {

    // 1. Initialize two pointers (slow and fast)
    int slow = 0, fast = 0;

    // 2. Phase 1: Find the intersection point in the cycle
    while(1){
        slow = nums[slow];           // move slow by one step
        fast = nums[nums[fast]];     // move fast by two steps
        if(slow == fast){            // if they meet, cycle detected
            break;
        }
    }

    // 3. Phase 2: Find the entrance to the cycle (duplicate number)
    int slow2 = 0;
    while(1){
        slow2 = nums[slow2];         // move from start
        slow = nums[slow];           // move from meeting point
        if(slow2 == slow){           // when they meet → duplicate found
            break;
        }
    }

    // 4. Return the duplicate number
    return slow;
}
