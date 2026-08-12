# 1D Dynamic Programming

## Video Solution

For more details about **N-th Tribonacci Number**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=3lpNp5Ojvrw)

## Concept

Dynamic Programming (DP) solves problems by breaking them into overlapping sub‑problems and storing the results of those sub‑problems to avoid redundant work.  
For the Tribonacci sequence each term depends on the three preceding terms, so we can compute `T[n]` iteratively while keeping only the last three values.

## When to Use It

Use 1D DP when you see:
- A sequence defined by a recurrence that looks back a fixed number of steps (e.g., `f(n) = f(n‑1) + f(n‑2) + f(n‑3)`).
- The problem asks for the *n*th value and `n` is moderate (here ≤ 37).
- Overlapping sub‑problems would cause exponential time in a naïve recursion.

## Template

```python
def solve_1d_dp(n, base, combine):
    """
    Generic 1‑D DP template.
    :param n: index we want
    :param base: list of known values for indices 0 … len(base)-1
    :param combine: function that receives the dp list and current index i and returns dp[i]
    :return: dp[n]
    """
    if n < len(base):
        return base[n]

    dp = [0] * (n + 1)          # table for sub‑solutions
    for i, v in enumerate(base):
        dp[i] = v               # seed the base cases

    for i in range(len(base), n + 1):
        dp[i] = combine(dp, i)  # compute current state from earlier ones

    return dp[n]
```

For Tribonacci we can specialize `combine` to `lambda dp, i: dp[i-1] + dp[i-2] + dp[i-3]`.

---

## LeetCode Problem Walkthrough

### Problem: 1137. N-th Tribonacci Number
https://leetcode.com/problems/n-th-tribonacci-number/

### Approach 1: Brute Force (Plain Recursion)

**Algorithm**
- Directly follow the definition:
  - If `n == 0` → 0
  - If `n == 1 or n == 2` → 1
  - Otherwise return `tribonacci(n‑1) + tribonacci(n‑2) + tribonacci(n‑3)`.

**Implementation**

```python
class Solution:
    def tribonacci(self, n: int) -> int:
        if n == 0:
            return 0
        if n == 1 or n == 2:
            return 1
        return self.tribonacci(n-1) + self.tribonacci(n-2) + self.tribonacci(n-3)
```

**Complexity Analysis**
- Time complexity: O(3ⁿ) — each call spawns three new calls, leading to an exponential tree.
- Space complexity: O(n) — recursion depth at most `n`.

---

### Approach 2: Top‑Down DP with Memoization

**Intuition**
The recursive solution recomputes the same sub‑problems many times. By caching each result the first time we compute it, we turn the exponential algorithm into a linear one.

**Algorithm**
- Use a dictionary or list `memo` to store already‑computed values.
- The recursive function checks `memo[n]` before recursing; if missing, compute it and store.

**Implementation**

```python
class Solution:
    def tribonacci(self, n: int) -> int:
        memo = {0: 0, 1: 1, 2: 1}

        def helper(k):
            if k in memo:
                return memo[k]
            memo[k] = helper(k-1) + helper(k-2) + helper(k-3)
            return memo[k]

        return helper(n)
```

**Complexity Analysis**
- Time complexity: O(n) — each `tribonacci(i)` is computed once.
- Space complexity: O(n) — memo table plus recursion stack.

---

### Approach 3: Bottom‑Up DP with O(1) Space

**Intuition**
To compute `T[i]` we only ever need `T[i‑1]`, `T[i‑2]`, and `T[i‑3]`. We can keep three rolling variables instead of a full table, achieving constant extra space.

**Algorithm**
- Handle the base cases `n = 0,1,2` directly.
- Initialize three variables `a = T0`, `b = T1`, `c = T2`.
- Iterate from `3` to `n`:
  - `next_val = a + b + c`
  - Shift the window: `a ← b`, `b ← c`, `c ← next_val`.
- After the loop, `c` holds `T[n]`.

**Implementation**

```python
class Solution:
    def tribonacci(self, n: int) -> int:
        if n == 0:
            return 0
        if n == 1 or n == 2:
            return 1

        a, b, c = 0, 1, 1          # T0, T1, T2
        for _ in range(3, n + 1):
            nxt = a + b + c
            a, b, c = b, c, nxt    # slide the window
        return c
```

**Complexity Analysis**
- Time complexity: O(n) — one pass from 3 to n.
- Space complexity: O(1) — only three integer variables.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the O(1) space solution for `n = 4`.

Input: n = 4
Initial: a = 0 (T0), b = 1 (T1), c = 1 (T2)

| Step | i | a (T{i-3}) | b (T{i-2}) | c (T{i-1}) | nxt = a+b+c | New (a,b,c)            |
|------|---|------------|------------|------------|------------|------------------------|
| 1    | 3 | 0          | 1          | 1          | 0+1+1 = 2  | (1, 1, 2)  -> T3 = 2   |
| 2    | 4 | 1          | 1          | 2          | 1+1+2 = 4  | (1, 2, 4)  -> T4 = 4   |

After the loop `c = 4`, which matches the expected output.
