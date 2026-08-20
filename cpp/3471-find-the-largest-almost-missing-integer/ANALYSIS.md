# Sliding Window

## Video Solution

For more details about **Find the Largest Almost Missing Integer**, watch the walkthrough at [https://www.youtube.com/watch?v=LJVcZSHCrio](https://www.youtube.com/watch?v=LJVcZSHCrio).

## Concept

A sliding window is a technique where we maintain a contiguous segment (window) of an array and move it from left to right, updating information incrementally instead of recomputing from scratch.  
In this problem we need to know, for each distinct value, how many windows of size `k` contain that value at least once. By sliding the window we can update the set of values present in the current window and update a global counter for each value.

## When to Use It

Use sliding window when you see:
- Contiguous subarrays or substrings of a fixed size `k` (or variable size with monotonic property).
- A need to compute some aggregate over all windows efficiently (e.g., counts, sums, minima/maxima).
- The problem constraints allow O(n) or O(n · α) solutions where α is the cost of updating the window.

## Template

```python
def sliding_window(nums, k):
    """
    Generic sliding window template.
    Returns a list of results for each window (if needed).
    """
    from collections import defaultdict

    freq = defaultdict(int)      # frequency of elements inside the current window
    present = set()              # elements that appear at least once in the window
    result = []                  # optional: store per‑window info

    # initialise first window
    for i in range(k):
        freq[nums[i]] += 1
        if freq[nums[i]] == 1:
            present.add(nums[i])

    # process first window
    # ... (use `present` as needed)

    # slide the window
    for i in range(k, len(nums)):
        # remove leftmost element
        left = nums[i - k]
        freq[left] -= 1
        if freq[left] == 0:
            present.remove(left)

        # add new rightmost element
        right = nums[i]
        freq[right] += 1
        if freq[right] == 1:
            present.add(right)

        # process current window
        # ... (use `present` as needed)

    return result
```

## LeetCode Problem Walkthrough

### Problem: 3471. Find the Largest Almost Missing Integer

https://leetcode.com/problems/find-the-largest-almost-missing-integer/

### Approach 1: Brute Force (Check every window)

**Algorithm**
1. For each distinct value `x` in `nums`, initialise a counter `cnt[x] = 0`.
2. Enumerate every possible start index `s` of a window of size `k` (`0 … n‑k`).
3. For each window, iterate through its elements and mark which distinct values appear (using a temporary set to avoid double‑counting the same value inside one window).
4. For every value that appears in the window, increment its counter.
5. After processing all windows, scan the counters and pick the largest value with `cnt[x] == 1`. Return `-1` if none.

**Implementation**
```python
from typing import List

class Solution:
    def largestInteger(self, nums: List[int], k: int) -> int:
        n = len(nums)
        # count how many windows of size k contain each value
        win_cnt = {}

        for start in range(n - k + 1):
            seen_in_window = set()
            for i in range(start, start + k):
                seen_in_window.add(nums[i])
            for val in seen_in_window:
                win_cnt[val] = win_cnt.get(val, 0) + 1

        answer = -1
        for val, c in win_cnt.items():
            if c == 1 and val > answer:
                answer = val
        return answer
```

**Complexity Analysis**
- Time complexity: O((n‑k+1)·k) ≤ O(n²) — we examine each window and each element inside it.
- Space complexity: O(u) — where `u` is the number of distinct values (at most n+1).

---

### Approach 2: Contribution Formula (Mathematical Optimization)

**Intuition**
Instead of scanning every window, we can compute directly how many windows of size `k` include a particular index `i`.  
A window start `s` must satisfy `max(0, i‑k+1) ≤ s ≤ min(i, n‑k)`.  
The number of such starts is `min(i, n‑k) - max(0, i‑k+1) + 1` (if positive).  
If a value appears multiple times, we sum the contributions of all its occurrences.  
Thus we can compute the total window count for each distinct value in O(n) time.

**Algorithm**
1. Create a dictionary `total` to accumulate window counts per value.
2. For each index `i` with value `v = nums[i]`:
   - Compute `left = max(0, i - k + 1)`.
   - Compute `right = min(i, n - k)`.
   - If `left ≤ right`, the number of windows covering `i` is `right - left + 1`; add this to `total[v]`.
3. After processing all indices, find the largest value whose total equals `1`.

**Implementation**
```python
from typing import List

class Solution:
    def largestInteger(self, nums: List[int], k: int) -> int:
        n = len(nums)
        total = {}

        for i, v in enumerate(nums):
            left = max(0, i - k + 1)
            right = min(i, n - k)
            if left <= right:
                contrib = right - left + 1
                total[v] = total.get(v, 0) + contrib

        ans = -1
        for val, cnt in total.items():
            if cnt == 1 and val > ans:
                ans = val
        return ans
```

**Complexity Analysis**
- Time complexity: O(n) — single pass over the array.
- Space complexity: O(u) — dictionary of distinct values.

---

### Approach 3: Sliding Window with Presence Set (Optimized Update)

**Intuition**
We can keep a sliding window and maintain a set `present` of values that occur at least once in the current window.  
When the window moves by one position, we update the frequencies of the outgoing and incoming elements, adjusting `present` only when a frequency drops to zero or rises from zero.  
For each window position, we increment a global counter for every value in `present`.  
Because each element enters and leaves the window exactly once, the total work is O(n · α) where α is the cost of iterating over `present`. In the worst case α can be O(k), but with the given constraints (n ≤ 50) this is still fast and illustrates the sliding‑window mindset.

**Algorithm**
1. Initialize frequency map `freq` and an empty set `present`.
2. Populate the first window (indices `0 … k‑1`), updating `freq` and `present`.
3. For each value in `present`, increment its global window counter.
4. Slide the window from left to right:
   - Decrease frequency of the outgoing element; if it becomes zero, remove it from `present`.
   - Increase frequency of the incoming element; if it becomes one, add it to `present`.
   - After the update, increment the global counter for each value in `present`.
5. After processing all windows, return the largest value with counter = 1.

**Implementation**
```python
from typing import List
from collections import defaultdict

class Solution:
    def largestInteger(self, nums: List[int], k: int) -> int:
        n = len(nums)
        freq = defaultdict(int)
        present = set()
        win_cnt = defaultdict(int)

        # first window
        for i in range(k):
            v = nums[i]
            freq[v] += 1
            if freq[v] == 1:
                present.add(v)
        for v in present:
            win_cnt[v] += 1

        # slide the window
        for i in range(k, n):
            # remove left element
            left = nums[i - k]
            freq[left] -= 1
            if freq[left] == 0:
                present.remove(left)

            # add right element
            right = nums[i]
            freq[right] += 1
            if freq[right] == 1:
                present.add(right)

            # count current window
            for v in present:
                win_cnt[v] += 1

        ans = -1
        for val, c in win_cnt.items():
            if c == 1 and val > ans:
                ans = val
        return ans
```

**Complexity Analysis**
- Time complexity: O(n · p) where `p` is the average size of `present`. In the worst case `p = O(k)`, giving O(n·k) ≤ O(n²) (still fine for n ≤ 50).  
- Space complexity: O(u + k) for the frequency map, the presence set, and the counters.

---

### Dry Run

**Input:** `nums = [3,9,2,1,7]`, `k = 3`

We'll dry‑run the **Contribution Formula** (Approach 2) because it shows the core idea clearly.

| Index i | nums[i] | left = max(0,i‑k+1) | right = min(i,n‑k) | windows covering i | contribution |
|--------|---------|----------------------|--------------------|--------------------|--------------|
| 0      | 3       | max(0,‑2)=0          | min(0,2)=0         | [0,0] → 1          | +1 for 3    |
| 1      | 9       | max(0,‑1)=0          | min(1,2)=1         | [0,1] → 2          | +2 for 9    |
| 2      | 2       | max(0,0)=0           | min(2,2)=2         | [0,2] → 3          | +3 for 2    |
| 3      | 1       | max(0,1)=1           | min(3,2)=2         | [1,2] → 2          | +2 for 1    |
| 4      | 7       | max(0,2)=2           | min(4,2)=2         | [2,2] → 1          | +1 for 7    |

Accumulated totals:
- 3: 1
- 9: 2
- 2: 3
- 1: 2
- 7: 1

Values with total = 1 are `{3,7}`; the largest is **7**, matching the expected output.

---

**End of Lecture**. Commit this file as `lectures/2025-09-26-sliding-window.md`.