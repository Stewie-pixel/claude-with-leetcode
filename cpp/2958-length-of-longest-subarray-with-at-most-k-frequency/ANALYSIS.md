# Sliding Window

## Video Solution

For more details about **Length of Longest Subarray With at Most K Frequency**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=W_KYZGp2QzU).

## Concept

A sliding window maintains a contiguous segment `[left, right]` of the array while we expand `right` one step at a time.  
We keep a frequency map of the elements inside the window. If adding `nums[right]` makes any element’s frequency exceed `k`, the window is no longer “good”. We then shrink the window from the left until the condition is restored.  
During the process we record the maximum window size that satisfied the condition.

**Real‑world analogy:** Imagine a buffet line where you can take at most `k` plates of each dish. You keep walking forward (expanding the window) and whenever you would exceed the limit for a dish, you step back (moving `left` forward) until the buffet rule is satisfied again. The longest stretch you walked while obeying the rule is the answer.

## When to Use It

Use sliding window when you see:
- A request for the **longest** (or shortest) **contiguous** subarray/substring.
- A condition that can be checked incrementally as the window expands (e.g., frequency ≤ k, sum ≤ target, all characters unique, etc.).
- The condition is **monotonic**: if a window is invalid, any larger window that contains it remains invalid; thus we can safely move the left bound forward.

## Template

```python
def longest_subarray(nums, k):
    freq = {}          # frequency of elements inside the window
    left = 0           # left bound of the window
    best = 0           # answer

    for right, val in enumerate(nums):
        freq[val] = freq.get(val, 0) + 1          # expand window

        # shrink while window is invalid
        while freq[val] > k:                      # only the newly added val can break the rule
            left_val = nums[left]
            freq[left_val] -= 1
            if freq[left_val] == 0:
                del freq[left_val]                # optional cleanup
            left += 1

        # window [left, right] is now valid
        best = max(best, right - left + 1)

    return best
```

## LeetCode Problem Walkthrough

### Problem: 2958. Length of Longest Subarray With at Most K Frequency

https://leetcode.com/problems/length-of-longest-subarray-with-at-most-k-frequency/

### Approach 1: Brute Force

**Algorithm**  
Check every possible subarray `[i, j]`. For each subarray build a frequency map and verify that no element appears more than `k` times. Keep track of the maximum length that satisfies the condition.

**Implementation**

```python
class Solution:
    def maxSubarrayLength(self, nums: List[int], k: int) -> int:
        n = len(nums)
        ans = 0
        for i in range(n):
            freq = {}
            for j in range(i, n):
                x = nums[j]
                freq[x] = freq.get(x, 0) + 1
                if freq[x] > k:          # window became invalid, no need to extend further
                    break
                ans = max(ans, j - i + 1)
        return ans
```

**Complexity Analysis**

- Time complexity: O(n²) — two nested loops over the array.
- Space complexity: O(m) — at most the number of distinct values in the current subarray (worst‑case O(n)).

### Approach 2: Sliding Window (Optimized)

**Intuition**  
When we extend the window to the right, only the newly added element can cause a frequency violation. If it does, we move the left bound rightward until that element’s frequency drops back to ≤ k. Because the left bound only moves forward, each array element is processed at most twice (once when `right` passes it, once when `left` passes it), giving linear time.

**Algorithm**

1. Initialise an empty frequency dictionary, `left = 0`, and `best = 0`.
2. Iterate `right` from `0` to `n‑1`:
   - Increment the count of `nums[right]`.
   - While the count of `nums[right]` exceeds `k`, decrement the count of `nums[left]` and increment `left`.
   - Update `best` with the current window size `right - left + 1`.
3. Return `best`.

**Implementation**

```python
class Solution:
    def maxSubarrayLength(self, nums: List[int], k: int) -> int:
        freq = {}
        left = 0
        best = 0

        for right, val in enumerate(nums):
            freq[val] = freq.get(val, 0) + 1

            # shrink window until the newly added value respects the limit
            while freq[val] > k:
                left_val = nums[left]
                freq[left_val] -= 1
                if freq[left_val] == 0:
                    del freq[left_val]   # keep the map tidy
                left += 1

            best = max(best, right - left + 1)

        return best
```

**Complexity Analysis**

- Time complexity: O(n) — each index moves at most once as `right` and once as `left`.
- Space complexity: O(m) — at most the number of distinct elements in the current window (≤ n).

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Input: `nums = [1,2,3,1,2,3,1,2]`, `k = 2`

| Step | right | val | freq after add                     | left before shrink | while triggered? | left after shrink | window size | best |
|------|-------|-----|------------------------------------|--------------------|------------------|-------------------|-------------|------|
| 0    | 0     | 1   | {1:1}                              | 0                  | No               | 0                 | 1           | 1    |
| 1    | 1     | 2   | {1:1, 2:1}                         | 0                  | No               | 0                 | 2           | 2    |
| 2    | 2     | 3   | {1:1,2:1,3:1}                      | 0                  | No               | 0                 | 3           | 3    |
| 3    | 3     | 1   | {1:2,2:1,3:1}                      | 0                  | No               | 0                 | 4           | 4    |
| 4    | 4     | 2   | {1:2,2:2,3:1}                      | 0                  | No               | 0                 | 5           | 5    |
| 5    | 5     | 3   | {1:2,2:2,3:2}                      | 0                  | No               | 0                 | 6           | 6    |
| 6    | 6     | 1   | {1:3,2:2,3:2}                      | 0                  | Yes (1>2)        | 1                 | 5           | 6    |
| 7    | 7     | 2   | {1:2,2:3,3:2} (after add)          | 1                  | Yes (2>2)        | 2                 | 5           | 6    |

The maximum window size observed is `6`, which matches the expected answer.
