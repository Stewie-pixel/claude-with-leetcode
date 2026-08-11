# 1D Dynamic Programming

## Video Solution

For more details about **Unique Paths**, watch the walkthrough at [https://www.youtube.com/watch?v=IlEsdxuD4lY](https://www.youtube.com/watch?v=IlEsdxuD4lY)

## Concept

Dynamic Programming (DP) solves problems by breaking them into overlapping sub‑problems and storing the results so each sub‑problem is solved only once.  
In **1‑D DP** we keep a single array (or a few variables) that represents the DP state for the current row/column, allowing us to compute the answer in linear space.

**Real‑world analogy:** Imagine you are counting the number of ways to reach each floor of a building if you can only come from the floor directly below or from the floor directly left. As you sweep across the building, you only need to know the counts for the current row and the row above — you never need the whole building at once.

## When to Use It

Use 1‑D DP when you notice:

- The problem involves a grid or matrix where each cell depends only on the cell above and/or the cell to the left (or similar directional dependence).
- The recurrence relation is of the form `dp[i] = f(dp[i-1], dp[i-2], ...)` with a fixed‑size window.
- You can iterate row‑by‑row (or column‑by‑column) and reuse the same array for the previous row.

Typical signals: “unique paths”, “minimum path sum”, “decode ways”, “climbing stairs”, etc.

## Template

```python
# 1‑D DP template for grid problems where dp[j] depends on dp[j] (left) and previous dp[j] (up)
def solve(m, n):
    # dp[j] will hold the answer for the current row at column j
    dp = [0] * n
    # initialize first row (or first column) as needed
    for j in range(n):
        dp[j] = 1          # base case: only one way to reach any cell in the first row
    # process remaining rows
    for i in range(1, m):
        for j in range(1, n):
            # dp[j] (up) + dp[j-1] (left)
            dp[j] = dp[j] + dp[j-1]
        # first column stays 1 (only one way from above)
        dp[0] = 1
    return dp[-1]
```

## LeetCode Problem Walkthrough

### Problem: 62. Unique Paths  
https://leetcode.com/problems/unique-paths/

---

### Approach 1: Brute Force (Recursive Exploration)

**Algorithm**  
From the start cell (0,0) recursively try every possible move: either go down or go right. When we reach the bottom‑right cell (m‑1, n‑1) we have found one valid path; return 1. Otherwise return the sum of paths from the two possible moves. Use the grid bounds to stop recursion.

**Implementation**

```python
class Solution:
    def uniquePaths(self, m: int, n: int) -> int:
        def dfs(r, c):
            # out of bounds → no path
            if r >= m or c >= n:
                return 0
            # reached destination
            if r == m - 1 and c == n - 1:
                return 1
            # explore down and right
            return dfs(r + 1, c) + dfs(r, c + 1)

        return dfs(0, 0)
```

**Complexity Analysis**

- Time complexity: O(2^(m+n)) — each call branches into two, leading to exponential calls.
- Space complexity: O(m+n) — recursion stack depth equals the length of a path.

---

### Approach 2: Top‑Down DP with Memoization

**Intuition**  
The brute force solution recomputes the same sub‑problems many times (e.g., the number of ways to reach cell (i,j) is needed from both its top and left neighbor). By caching the result of each cell after the first computation, we turn the exponential recursion into a polynomial‑time algorithm.

**Algorithm**  
Use a dictionary or 2‑D list `memo` to store the number of ways to reach each cell. The recursive function now checks `memo` before computing; if the value is present, return it directly. Otherwise compute as `memo[r][c] = dfs(r+1,c) + dfs(r,c+1)` and store it.

**Implementation**

```python
class Solution:
    def uniquePaths(self, m: int, n: int) -> int:
        memo = [[-1] * n for _ in range(m)]

        def dfs(r, c):
            # out of bounds
            if r >= m or c >= n:
                return 0
            # destination
            if r == m - 1 and c == n - 1:
                return 1
            # cached result
            if memo[r][c] != -1:
                return memo[r][c]
            # compute and cache
            memo[r][c] = dfs(r + 1, c) + dfs(r, c + 1)
            return memo[r][c]

        return dfs(0, 0)
```

**Complexity Analysis**

- Time complexity: O(m * n) — each cell is computed once.
- Space complexity: O(m * n) — memo table plus recursion stack (O(m+n)).

---

### Approach 3: Bottom‑Up 1‑D DP (Most Optimal)

**Intuition**  
Observing the recurrence `dp[i][j] = dp[i-1][j] + dp[i][j-1]` we see that to compute the current row we only need the values from the same column in the previous row (`dp[i-1][j]`) and the value from the left cell in the current row (`dp[i][j-1]`). Therefore we can reuse a single 1‑D array where `dp[j]` always holds the number of ways to reach cell `(current_row, j)`. As we iterate left‑to‑right, `dp[j]` is updated to `dp[j] (old, i‑1,j) + dp[j-1] (current row, j‑1)`.

**Algorithm**  
1. Initialize a 1‑D array `dp` of length `n` with all 1’s – the first row has exactly one way to reach each cell (only right moves).  
2. For each subsequent row `i` from 1 to `m-1`:  
   - The first column stays 1 (only down moves).  
   - For each column `j` from 1 to `n-1`: update `dp[j] = dp[j] + dp[j-1]`.  
3. After processing all rows, `dp[n-1]` holds the answer.

**Implementation**

```python
class Solution:
    def uniquePaths(self, m: int, n: int) -> int:
        # dp[j] = number of ways to reach cell in current row, column j
        dp = [1] * n                     # first row: all 1s

        for i in range(1, m):            # process remaining rows
            for j in range(1, n):        # left to right update
                dp[j] = dp[j] + dp[j-1]  # up (old dp[j]) + left (dp[j-1])
            # dp[0] remains 1 (only one way down the first column)

        return dp[-1]
```

**Complexity Analysis**

- Time complexity: O(m * n) — one constant‑time update per cell.
- Space complexity: O(n) — only a single row of size n is stored.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Let's trace the algorithm for `m = 3, n = 7` (answer = 28).

We start with `dp = [1,1,1,1,1,1,1]` (first row).

**Row 1 (i = 1, second actual row):**

| j | dp[j] before | dp[j-1] | dp[j] after (=up+left) |
|---|--------------|---------|------------------------|
| 0 | 1            | –       | 1 (unchanged)          |
| 1 | 1            | 1       | 2                      |
| 2 | 1            | 2       | 3                      |
| 3 | 1            | 3       | 4                      |
| 4 | 1            | 4       | 5                      |
| 5 | 1            | 5       | 6                      |
| 6 | 1            | 6       | 7                      |

Result after row 1: `dp = [1,2,3,4,5,6,7]`

**Row 2 (i = 2, third actual row):**

| j | dp[j] before | dp[j-1] | dp[j] after |
|---|--------------|---------|-------------|
| 0 | 1            | –       | 1           |
| 1 | 2            | 1       | 3           |
| 2 | 3            | 3       | 6           |
| 3 | 4            | 6       | 10          |
| 4 | 5            | 10      | 15          |
| 5 | 6            | 15      | 21          |
| 6 | 7            | 21      | 28          |

Final `dp[6] = 28`, which matches the expected output.

--- 

*End of lecture.*