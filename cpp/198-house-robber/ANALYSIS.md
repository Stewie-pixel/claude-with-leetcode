# House Robber

## Video Solution

For more details about **House Robber**, watch the walkthrough at [https://www.youtube.com/watch?v=73r3KWiEvyk](https://www.youtube.com/watch?v=73r3KWiEvyk).

## Concept

House Robber is a classic 1‑dimensional dynamic programming problem.  
Imagine you walk down a street and at each house you decide **take** the money (and skip the next house) or **skip** it (and move to the next house). The best amount you can have at house *i* depends only on the best amounts at houses *i‑1* and *i‑2*. This overlapping‑substructure lets us build the answer iteratively.

## When to Use It

Use this pattern when you see:
- A sequence (array/list) where you cannot pick two adjacent elements.
- The goal is to maximize (or minimize) a sum/value under that adjacency constraint.
- The problem exhibits optimal substructure: the solution for prefix `[0..i]` depends only on solutions for `[0..i-1]` and `[0..i-2]`.

## Template

```python
def rob(nums):
    """
    Returns the maximum amount of money that can be robbed without
    robbing two adjacent houses.
    """
    # prev2 = best up to i-2, prev1 = best up to i-1
    prev2, prev1 = 0, 0
    for money in nums:
        # If we take this house, we add its money to prev2;
        # if we skip it, we keep prev1.
        curr = max(prev2 + money, prev1)
        # Shift the window forward.
        prev2, prev1 = prev1, curr
    return prev1
```

The template keeps only two states, achieving O(1) space.

---

# LeetCode Problem Walkthrough

### Problem: 198. House Robber
https://leetcode.com/problems/house-robber/

### Approach 1: Brute Force (Recursion)

**Algorithm**
- Try every possible subset of houses that respects the “no adjacent” rule.
- At each index `i` we have two choices:
  1. Rob house `i` → add `nums[i]` and move to `i+2`.
  2. Skip house `i` → add `0` and move to `i+1`.
- Return the maximum of the two choices.
- This explores all possibilities, leading to exponential time.

**Implementation**

```python
class Solution:
    def rob(self, nums):
        def dfs(i):
            if i >= len(nums):
                return 0
            # Choice 1: rob this house
            take = nums[i] + dfs(i + 2)
            # Choice 2: skip this house
            skip = dfs(i + 1)
            return max(take, skip)

        return dfs(0)
```

**Complexity Analysis**
- Time complexity: O(2^n) — each house spawns two recursive calls.
- Space complexity: O(n) — recursion depth up to n.

---

### Approach 2: Top‑Down DP with Memoization

**Intuition**
The recursive solution recomputes the same subproblems many times (e.g., `dfs(i)` is called from both `i-1` and `i-2`). By caching results we turn the exponential recursion into linear time.

**Algorithm**
- Use a memo array/dictionary where `memo[i]` stores the best amount obtainable from house `i` to the end.
- The recurrence is the same as in brute force: `memo[i] = max(nums[i] + memo[i+2], memo[i+1])`.
- Fill memo lazily via recursion or iteratively from the end.

**Implementation**

```python
class Solution:
    def rob(self, nums):
        from functools import lru_cache

        @lru_cache(maxsize=None)
        def dfs(i):
            if i >= len(nums):
                return 0
            take = nums[i] + dfs(i + 2)
            skip = dfs(i + 1)
            return max(take, skip)

        return dfs(0)
```

**Complexity Analysis**
- Time complexity: O(n) — each index `i` is computed once.
- Space complexity: O(n) — memoization cache + recursion stack.

---

### Approach 3: Bottom‑Up DP (O(1) Space)

**Intuition**
Since `dp[i]` depends only on `dp[i-1]` and `dp[i-2]`, we do not need the full array; two variables are enough. We iterate forward, updating the pair `(prev2, prev1)` to represent `dp[i-2]` and `dp[i-1]`.

**Algorithm**
- Initialize `prev2 = 0` (best up to i‑2) and `prev1 = 0` (best up to i‑1).
- For each house value `money`:
  - `curr = max(prev2 + money, prev1)` → best up to current house.
  - Shift: `prev2 = prev1`, `prev1 = curr`.
- After processing all houses, `prev1` holds the answer.

**Implementation**

```python
class Solution:
    def rob(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        prev2, prev1 = 0, 0          # dp[i-2], dp[i-1]
        for money in nums:
            curr = max(prev2 + money, prev1)  # dp[i]
            prev2, prev1 = prev1, curr       # shift window
        return prev1
```

**Complexity Analysis**
- Time complexity: O(n) — single pass through the array.
- Space complexity: O(1) — only two integer variables.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: `nums = [2, 7, 9, 3, 1]`

```
| i | money | prev2 (i-2) | prev1 (i-1) | take = prev2+money | skip = prev1 | curr = max(take,skip) | new prev2 | new prev1 |
|---|-------|-------------|-------------|--------------------|--------------|-----------------------|-----------|-----------|
| 0 |   2   |      0      |      0      |        0+2=2       |      0       |          2            |     0     |     2     |
| 1 |   7   |      0      |      2      |        0+7=7       |      2       |          7            |     2     |     7     |
| 2 |   9   |      2      |      7      |        2+9=11      |      7       |         11            |     7     |    11     |
| 3 |   3   |      7      |     11      |        7+3=10      |     11       |         11            |    11     |    11     |
| 4 |   1   |     11      |     11      |       11+1=12      |     11       |         12            |    11     |    12     |
```

Result: `prev1 = 12`, which matches the expected output.