# Binary Search

## Video Solution

For more details about **Search Insert Position**, watch the walkthrough at [https://www.youtube.com/watch?v=K-RYzDZkzCI](https://www.youtube.com/watch?v=K-RYzDZkzCI)

## Concept

Binary search repeatedly halves the search space by comparing the target with the middle element of a sorted array. If the middle element equals the target, we have found it. If the middle element is larger than the target, the target can only lie in the left half; otherwise it lies in the right half. This yields **O(log n)** time.

A real‑world analogy: looking up a word in a dictionary. You open the book roughly in the middle, see whether the word comes before or after that page, and discard half of the remaining pages each time.

## When to Use It

Use binary search when you see:
- The input array (or search space) is **sorted**.
- You need to find an exact value or the **first position where a condition becomes true** (e.g., insert position, lower bound).
- The problem asks for **O(log n)** runtime.

## Template

```python
def binary_search(nums, target):
    """
    Returns the index of target if present, otherwise the insertion point.
    Works for sorted, distinct ascending list.
    """
    low, high = 0, len(nums)          # search interval [low, high)
    while low < high:                 # while there is still a range to examine
        mid = (low + high) // 2       # middle index
        if nums[mid] < target:        # target must be right of mid
            low = mid + 1
        else:                         # nums[mid] >= target -> answer is at mid or left
            high = mid
    return low                         # low == high is the insertion index
```

## LeetCode Problem Walkthrough

### Problem: 35. Search Insert Position

https://leetcode.com/problems/search-insert-position/

### Approach 1: Brute Force (Linear Scan)

**Algorithm**
Scan the array from left to right. The first index where `nums[i] >= target` is the answer; if no such index exists, return `len(nums)`.

**Implementation**

```python
class Solution:
    def searchInsert(self, nums: List[int], target: int) -> int:
        for i, val in enumerate(nums):
            if val >= target:          # found place to insert or exact match
                return i
        return len(nums)               # target larger than all elements
```

**Complexity Analysis**
- Time complexity: O(n) — we may examine every element once.
- Space complexity: O(1) — only a few integer variables are used.

### Approach 2: Binary Search (Classic Lower‑Bound)

**Intuition**
Because the array is sorted, we can discard half of the remaining candidates each step by comparing the middle element with the target. The loop invariant is that the answer lies in `[low, high)`. When `nums[mid] < target` we know the answer must be right of `mid`; otherwise it is at `mid` or left.

**Algorithm**
1. Initialise `low = 0`, `high = len(nums)`.
2. While `low < high`:
   - Compute `mid = (low + high) // 2`.
   - If `nums[mid] < target`, set `low = mid + 1`.
   - Else (`nums[mid] >= target`), set `high = mid`.
3. When the loop ends, `low` is the smallest index with `nums[low] >= target` (or `len(nums)` if none).

**Implementation**

```python
class Solution:
    def searchInsert(self, nums: List[int], target: int) -> int:
        low, high = 0, len(nums)
        while low < high:
            mid = (low + high) // 2
            if nums[mid] < target:
                low = mid + 1
            else:
                high = mid
        return low
```

**Complexity Analysis**
- Time complexity: O(log n) — each iteration halves the search range.
- Space complexity: O(1) — only pointers and a middle index are stored.

### Approach 3: Binary Search Using `bisect_left` (Standard Library)

**Intuition**
Python’s `bisect` module already implements the lower‑bound binary search. Using it makes the intent explicit and leverages a well‑tested implementation.

**Algorithm**
Call `bisect_left(nums, target)`, which returns the insertion point for `target` in a sorted list.

**Implementation**

```python
from bisect import bisect_left

class Solution:
    def searchInsert(self, nums: List[int], target: int) -> int:
        return bisect_left(nums, target)
```

**Complexity Analysis**
- Time complexity: O(log n) — `bisect_left` performs binary search.
- Space complexity: O(1) — no extra space beyond the input.

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

### Dry Run

Input: `nums = [1, 3, 5, 6]`, `target = 5`

| Step | low | high | mid | nums[mid] | Comparison (`nums[mid] < target?`) | Action                     |
|------|-----|------|-----|-----------|-----------------------------------|----------------------------|
| 1    | 0   | 4    | 2   | 5         | False (5 < 5 is no)               | `high = mid = 2`           |
| 2    | 0   | 2    | 1   | 3         | True (3 < 5)                      | `low = mid + 1 = 2`        |
| 3    | 2   | 2    | —   | —         | Loop ends (`low == high`)         | Return `low = 2`           |

The algorithm correctly returns index `2`, where `5` is located. If the target were `2`, the trace would show the final `low` as `1`, the correct insertion position.