# Prefix Sum

## Video Solution

For more details about **Left and Right Sum Differences**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=1K3JxUe9c8I).

## Concept

Prefix sums store the cumulative sum of elements up to each index, allowing us to compute the sum of any sub‑array in O(1) time.  
For this problem we need the sum of elements **left** of `i` and the sum **right** of `i`.  
- `leftSum[i]` = sum of `nums[0 … i‑1]` → a classic prefix sum.  
- `rightSum[i]` = sum of `nums[i+1 … n‑1]` → can be obtained from the total sum minus the prefix up to `i` and the current element.

A real‑world analogy: Imagine you walk along a hallway with numbered doors. At each door you want to know how many people are standing before you and after you. If you keep a running count of people you’ve passed, you instantly know the “before” count; the “after” count is the total minus those you’ve passed and the person at the current door.

## When to Use It

Use prefix/suffix sums when you see:
- Requests for sum of elements to the left/right of each index.
- Problems that ask for sub‑array sums, equilibrium indices, or “split array” conditions.
- Any situation where recomputing a sum from scratch for each position would lead to O(n²) work.

## Template

```python
def prefix_sum(nums):
    """Returns an array where pref[i] = sum(nums[0..i])"""
    n = len(nums)
    pref = [0] * n
    running = 0
    for i in range(n):
        running += nums[i]
        pref[i] = running
    return pref
```

---

## LeetCode Problem Walkthrough

### Problem: 2190. Left and Right Sum Differences

https://leetcode.com/problems/left-and-right-sum-differences/

### Approach 1: Brute Force

**Algorithm**
For each index `i`:
1. Compute `leftSum` by iterating from `0` to `i‑1` and adding the elements.
2. Compute `rightSum` by iterating from `i+1` to `n‑1` and adding the elements.
3. Store `abs(leftSum - rightSum)` in the answer.

**Implementation**

```python
class Solution:
    def leftRightDifference(self, nums: List[int]) -> List[int]:
        n = len(nums)
        ans = [0] * n
        for i in range(n):
            left_sum = 0
            for j in range(i):
                left_sum += nums[j]
            right_sum = 0
            for j in range(i + 1, n):
                right_sum += nums[j]
            ans[i] = abs(left_sum - right_sum)
        return ans
```

**Complexity Analysis**
- Time complexity: O(n²) — two nested loops over the array.
- Space complexity: O(1) — only a few integer variables besides the output.

### Approach 2: Prefix and Suffix Arrays

**Intuition**
If we pre‑compute the prefix sum (`leftSum`) and suffix sum (`rightSum`) for every index, each answer becomes a simple absolute difference lookup — O(1) per index after O(n) preprocessing.

**Algorithm**
1. Build `leftSum[i]` = sum of elements before `i` (prefix sum, `leftSum[0] = 0`).
2. Build `rightSum[i]` = sum of elements after `i` (suffix sum, `rightSum[n‑1] = 0`).
3. For each `i`, answer[i] = `abs(leftSum[i] - rightSum[i])`.

**Implementation**

```python
class Solution:
    def leftRightDifference(self, nums: List[int]) -> List[int]:
        n = len(nums)
        left = [0] * n
        right = [0] * n

        # prefix sums for left side
        for i in range(1, n):
            left[i] = left[i - 1] + nums[i - 1]

        # suffix sums for right side
        for i in range(n - 2, -1, -1):
            right[i] = right[i + 1] + nums[i + 1]

        # absolute differences
        ans = [abs(left[i] - right[i]) for i in range(n)]
        return ans
```

**Complexity Analysis**
- Time complexity: O(n) — three linear passes.
- Space complexity: O(n) — two auxiliary arrays of size n.

### Approach 3: Total Sum Trick (O(1) Extra Space)

**Intuition**
The right sum for index `i` can be derived from the total sum of the array:
`rightSum[i] = totalSum - leftSum[i] - nums[i]`.  
Thus we only need a single running left sum while iterating once.

**Algorithm**
1. Compute `total = sum(nums)`.
2. Initialize `left = 0`.
3. For each index `i`:
   - `right = total - left - nums[i]`
   - `ans[i] = abs(left - right)`
   - Update `left += nums[i]` for the next iteration.

**Implementation**

```python
class Solution:
    def leftRightDifference(self, nums: List[int]) -> List[int]:
        total = sum(nums)
        left = 0
        ans = []
        for x in nums:
            right = total - left - x
            ans.append(abs(left - right))
            left += x
        return ans
```

**Complexity Analysis**
- Time complexity: O(n) — one pass to compute total, one pass to build answer.
- Space complexity: O(1) extra (output array not counted).

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Input: `nums = [10, 4, 8, 3]`

| Step | i | nums[i] | left before | right = total - left - nums[i] | |left - right| | Action (update left) |
| ---- | - | ------- | ----------- | ------------------------------ | ------------- | -------------------- |
| 0    | 0 | 10      | 0           | 25 - 0 - 10 = 15               | 15            | left = 0 + 10 = 10   |
| 1    | 1 | 4       | 10          | 25 - 10 - 4 = 11               | 1             | left = 10 + 4 = 14   |
| 2    | 2 | 8       | 14          | 25 - 14 - 8 = 3                | 11            | left = 14 + 8 = 22   |
| 3    | 3 | 3       | 22          | 25 - 22 - 3 = 0                | 22            | left = 22 + 3 = 25   |

Result: `[15, 1, 11, 22]` matches the expected output.