# Sliding Window

## Video Solution

For more details about the Longest Subarray of 1's After Deleting One Element, see at [resources](https://www.youtube.com/results?search_query=leetcode+solution)

## Concept

The sliding window technique maintains a contiguous segment (window) of the array that satisfies a certain condition. By expanding the right end of the window and, when needed, shrinking the left end, we can examine all valid windows in linear time. For this problem the condition is “the window contains at most one zero”. The length of such a window tells us how many elements we could keep after deleting at most one element inside it.

## When to Use It

Use sliding window when you see:
- Requests for the longest/shortest subarray, substring, or contiguous segment.
- A condition that can be checked incrementally (e.g., count of zeros, sum, frequency).
- The need to find an optimal window without examining every possible start‑end pair (which would be O(n²)).

## Template

```python
def sliding_window_at_most_k(nums, k, condition):
    """
    Generic sliding window: longest subarray where `condition` holds
    and we are allowed at most `k` violations of the condition.
    """
    left = 0
        # state that tracks the number of violations inside the window
    violation_count = 0
    best = 0

    for right in range(len(nums)):
        # include nums[right] into the window
        if condition(nums[right]):          # e.g., nums[right] == 0 for zero‑count
            violation_count += 1

        # shrink left side while we exceed the allowed violations
        while violation_count > k:
            if condition(nums[left]):
                violation_count -= 1
            left += 1

        # window [left, right] is now valid
        best = max(best, right - left + 1)

    return best
```

*Replace `condition` with the specific test (e.g., `lambda x: x == 0` for counting zeros).*

## LeetCode Problem Walkthrough

### Problem: 1493. Longest Subarray of 1's After Deleting One Element

https://leetcode.com/problems/longest-subarray-of-1s-after-deleting-one-element/

### Approach 1: Brute Force

**Algorithm**
Enumerate every possible subarray `[i, j]`. Count how many zeros it contains. If the count is ≤ 1, the subarray can become all 1s after deleting one element (the zero if present, otherwise any 1). Compute the resulting length and keep the maximum.

**Implementation**

```python
from typing import List

class Solution:
    def longestSubarray(self, nums: List[int]) -> int:
        n = len(nums)
        best = 0
        for i in range(n):
            zeros = 0
            for j in range(i, n):
                if nums[j] == 0:
                    zeros += 1
                if zeros > 1:               # more than one zero -> cannot fix with one deletion
                    break
                length = j - i + 1
                # after deleting one element:
                if zeros == 0:              # all ones, we must delete a 1
                    cand = length - 1
                else:                       # exactly one zero, delete that zero
                    cand = length
                best = max(best, cand)
        return best
```

**Complexity Analysis**
- Time complexity: **O(n²)** — two nested loops over all start‑end pairs.
- Space complexity: **O(1)** — only a few integer variables.

### Approach 2: Sliding Window (At Most One Zero)

**Intuition**
If we keep a window that contains **no more than one zero**, we can turn that window into all 1s by deleting the zero (if it exists). When the window has no zero we still must delete one element (any 1), so the answer becomes `window_len − 1`. Scanning the array with such a window yields the optimum in linear time.

**Algorithm**
1. Use two pointers `left` and `right` to represent the current window.
2. Expand `right` step‑by‑step, counting zeros inside the window.
3. When the zero count exceeds 1, move `left` forward until the window is valid again.
4. After each expansion, update the maximum window length seen.
5. At the end, if the best window never contained a zero (i.e., its length equals the whole array), subtract one; otherwise return the length as is.

**Implementation**

```python
from typing import List

class Solution:
    def longestSubarray(self, nums: List[int]) -> int:
        left = 0
        zero_cnt = 0
        max_len = 0

        for right in range(len(nums)):
            if nums[right] == 0:
                zero_cnt += 1

            # shrink until we have at most one zero
            while zero_cnt > 1:
                if nums[left] == 0:
                    zero_cnt -= 1
                left += 1

            # window [left, right] is valid
            max_len = max(max_len, right - left + 1)

        # If the best window had no zero, we must delete a 1
        return max_len - 1 if max_len == len(nums) else max_len
```

**Complexity Analysis**
- Time complexity: **O(n)** — each index is visited at most twice (once by `right`, once by `left`).
- Space complexity: **O(1)** — only a handful of integer variables.

### Approach 3: Prefix‑Suffix Count

**Intuition**
For any zero at position `i`, deleting that zero lets us join the block of consecutive 1s ending at `i‑1` with the block starting at `i+1`. The length after deletion equals `leftOnes[i‑1] + rightOnes[i+1]`. By pre‑computing the lengths of consecutive 1s to the left and to the right of each index we can evaluate every possible deletion in O(n) time. If the array contains no zero, we must delete a 1, giving `n‑1`.

**Algorithm**
1. Build `left[i]` = number of consecutive 1s ending at `i` (including `i` if it is 1).
2. Build `right[i]` = number of consecutive 1s starting at `i` (including `i` if it is 1).
3. For each index `i` where `nums[i]==0`, compute candidate = `left[i‑1] + right[i+1]` (treat missing sides as 0).
4. Track the maximum candidate.
5. If no zero was found, return `len(nums)-1`.

**Implementation**

```python
from typing import List

class Solution:
    def longestSubarray(self, nums: List[int]) -> int:
        n = len(nums)
        left = [0] * n
        right = [0] * n

        # consecutive 1s to the left
        for i in range(n):
            if nums[i] == 1:
                left[i] = left[i-1] + 1 if i > 0 else 1

        # consecutive 1s to the right
        for i in range(n-1, -1, -1):
            if nums[i] == 1:
                right[i] = right[i+1] + 1 if i < n-1 else 1

        ans = 0
        for i in range(n):
            if nums[i] == 0:
                l = left[i-1] if i > 0 else 0
                r = right[i+1] if i < n-1 else 0
                ans = max(ans, l + r)

        # all ones case
        if ans == 0:          # no zero encountered
            return n - 1
        return ans
```

**Complexity Analysis**
- Time complexity: **O(n)** — three linear passes over the array.
- Space complexity: **O(n)** — two auxiliary arrays of size `n`.