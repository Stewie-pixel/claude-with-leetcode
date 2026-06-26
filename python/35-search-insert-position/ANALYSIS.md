# Binary Search

## Video Solution

For more details about **Search Insert Position**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=K-RYzDZkzCI).

## Concept

Binary search repeatedly halves the search space by comparing the target with the middle element of a sorted array. If the middle element equals the target, we return its index. If the target is smaller, we continue searching the left half; otherwise we search the right half. When the search space is exhausted, the `low` pointer indicates the insertion position.

## When to Use It

Use binary search when you see:
- A sorted array or list
- Need to find an element or its insertion point
- Required runtime better than O(n) (typically O(log n))
- The problem hints at “search”, “find”, “minimum/maximum valid value”, or “guess the number”

## Template

```python
def binary_search(nums, target):
    """
    Returns index of target if found, otherwise the index where it should be inserted.
    Assumes nums is sorted in ascending order.
    """
    low, high = 0, len(nums) - 1          # inclusive bounds
    while low <= high:                    # while there is still a search space
        mid = (low + high) // 2           # middle index
        if nums[mid] == target:           # found target
            return mid
        elif nums[mid] < target:          # target is in right half
            low = mid + 1
        else:                             # target is in left half
            high = mid - 1
    # loop ended → target not present; low is the insertion point
    return low
```

## LeetCode Problem Walkthrough

### Problem: 35. Search Insert Position

https://leetcode.com/problems/search-insert-position/

### Approach 1: Brute Force (Linear Scan)

**Algorithm**
Scan the array from left to right. Return the first index where the element is >= target. If no such element exists, return the length of the array.

**Implementation**

```python
class Solution:
    def searchInsert(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: int
        """
        for i, num in enumerate(nums):
            if num >= target:          # first position that is not less than target
                return i
        return len(nums)               # target is greater than all elements
```

**Complexity Analysis**

- Time complexity: O(n) — we may examine every element once.
- Space complexity: O(1) — only a few integer variables are used.

### Approach 2: Binary Search (Iterative)

**Intuition**
Because the array is sorted, we can discard half of the remaining candidates at each step by comparing the target to the middle element. This reduces the search space exponentially, giving O(log n) time.

**Algorithm**
Initialize `low` = 0 and `high` = len(nums)‑1. While `low` ≤ `high`, compute `mid`. If `nums[mid]` equals the target, return `mid`. If `nums[mid]` < target, move `low` to `mid + 1` (search right half). Otherwise move `high` to `mid‑1` (search left half). When the loop ends, `low` is the smallest index where the target could be inserted.

**Implementation**

```python
class Solution:
    def searchInsert(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: int
        """
        low, high = 0, len(nums) - 1
        while low <= high:
            mid = (low + high) // 2
            if nums[mid] == target:
                return mid
            elif nums[mid] < target:
                low = mid + 1
            else:
                high = mid - 1
        return low
```

**Complexity Analysis**

- Time complexity: O(log n) — each iteration halves the search space.
- Space complexity: O(1) — only constant extra space for pointers.

### Approach 3: Using Python’s `bisect_left`

**Intuition**
Python’s `bisect` module already implements binary search for insertion points. `bisect_left` returns the index where `target` should be inserted to keep the list sorted, which is exactly what we need.

**Algorithm**
Import `bisect_left` from the `bisect` module and call it on `nums` with `target`. Return the result directly.

**Implementation**

```python
from bisect import bisect_left

class Solution:
    def searchInsert(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: int
        """
        return bisect_left(nums, target)
```

**Complexity Analysis**

- Time complexity: O(log n) — `bisect_left` performs binary search internally.
- Space complexity: O(1) — no additional space beyond the input.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: nums = [1, 3, 5, 6], target = 2

| Step | low | high | mid | nums[mid] | Comparison                | Action                     |
|------|-----|------|-----|-----------|---------------------------|----------------------------|
| 1    | 0   | 3 1   | 3         | nums[mid] > target     | high = mid  => |
| 2    |  0  |    | nums[mid] <  > 1 < target    |  low = mid + 1 => low = 1 |
| 3    | 1   | 0  | (loop ends) | –   | –                           | return low = 1            |

The algorithm finishes with `low = 1`, which is the correct insertion index for target `2`.