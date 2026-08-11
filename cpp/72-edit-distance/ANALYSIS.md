# 2D Dynamic Programming

## Video Solution

For more details about **Edit Distance**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=XYi2-LPrwm4).

## Concept

2D Dynamic Programming solves problems where the answer depends on two varying dimensions (e.g., indices of two strings). We build a table `dp[i][j]` that stores the optimal solution for the subproblem consisting of the first `i` characters of one string and the first `j` characters of another. The final answer lives in `dp[m][n]`.

**Real‑world analogy:** Imagine you have two drafts of a document and you want to make them identical using the fewest edits. You compare the drafts word‑by‑word, keeping track of the best cost for every prefix pair in a grid. Each cell tells you the cheapest way to align those prefixes.

## When to Use It

Use 2D DP when you see:
- Two sequences (strings, arrays) and you need to compare prefixes/subsequences.
- The problem asks for minimum/maximum cost, count, or length involving both sequences.
- Typical clues: “edit distance”, “longest common subsequence”, “minimum alignment cost”, “interleaving strings”.

## Template

```python
def solve_2d_dp(s1: str, s2: str) -> int:
    m, n = len(s1), len(s2)
    # dp[i][j] = answer for s1[:i] and s2[:j]
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    # base cases: one string empty
    for i in range(m + 1):
        dp[i][0] = i          # delete all chars from s1[:i]
    for j in range(n + 1):
        dp[0][j] = j          # insert all chars of s2[:j]

    # fill the table
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if s1[i - 1] == s2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1]          # no extra cost
            else:
                dp[i][j] = 1 + min(
                    dp[i - 1][j - 1],   # replace
                    dp[i - 1][j],       # delete from s1
                    dp[i][j - 1]        # insert into s1
                )
    return dp[m][n]
```

## LeetCode Problem Walkthrough

### Problem: 72. Edit Distance

https://leetcode.com/problems/edit-distance/

### Approach 1: Brute Force (Recursion)

**Algorithm**
- Define a recursive function `f(i, j)` that returns the minimum operations to convert `word1[i:]` to `word2[j:]`.
- If either index reaches the end, the answer is the length of the remaining suffix (all inserts or deletes).
- If current characters match, move both pointers forward with no cost.
- Otherwise, try the three operations (replace, delete, insert) and take the minimum, adding 1 for the operation.

**Implementation**

```python
class Solution:
    def minDistance(self, word1: str, word2: str) -> int:
        def dfs(i: int, j: int) -> int:
            if i == len(word1):
                return len(word2) - j          # insert rest of word2
            if j == len(word2):
                return len(word1) - i          # delete rest of word1
            if word1[i] == word2[j]:
                return dfs(i + 1, j + 1)       # characters match
            # try replace, delete, insert
            replace = 1 + dfs(i + 1, j + 1)
            delete  = 1 + dfs(i + 1, j)
            insert  = 1 + dfs(i, j + 1)
            return min(replace, delete, insert)

        return dfs(0, 0)
```

**Complexity Analysis**
- Time complexity: O(3^(m+n)) — each state branches into three calls, exponential in string lengths.
- Space complexity: O(m+n) — recursion depth at most m+n (call stack).

### Approach 2: Memoization (Top‑Down DP)

**Intuition**
The brute force recursion recomputes the same `(i, j)` pairs many times. By caching results we turn the exponential recursion into polynomial time.

**Algorithm**
- Same recursion as before, but store each computed `f(i, j)` in a memo table (2D list or dict).
- Before recursing, check if the value is already memoized.

**Implementation**

```python
class Solution:
    def minDistance(self, word1: str, word2: str) -> int:
        m, n = len(word1), len(word2)
        memo = [[-1] * (n + 1) for _ in range(m + 1)]

        def dfs(i: int, j: int) -> int:
            if i == m:
                return n - j
            if j == n:
                return m - i
            if memo[i][j] != -1:
                return memo[i][j]
            if word1[i] == word2[j]:
                memo[i][j] = dfs(i + 1, j + 1)
            else:
                memo[i][j] = 1 + min(
                    dfs(i + 1, j + 1),   # replace
                    dfs(i + 1, j),       # delete
                    dfs(i, j + 1)        # insert
                )
            return memo[i][j]

        return dfs(0, 0)
```

**Complexity Analysis**
- Time complexity: O(m*n) — each of the (m+1)*(n+1) states computed once.
- Space complexity: O(m*n) — memo table plus recursion stack O(m+n) → dominated by O(m*n).

### Approach 3: Bottom‑Up DP (Iterative)

**Intuition**
Instead of recursion we fill the DP table iteratively from the base cases (empty prefixes) outward. This removes recursion overhead and is often easier to reason about.

**Algorithm**
- Create a `(m+1) x (n+1)` table.
- Initialize first row/column: converting to/from an empty string needs inserts/deletes.
- For each cell, if characters match, copy the diagonal value; otherwise take 1 + min of left (insert), top (delete), diagonal (replace).

**Implementation**

```python
class Solution:
    def minDistance(self, word1: str, word2: str) -> int:
        m, n = len(word1), len(word2)
        dp = [[0] * (n + 1) for _ in range(m + 1)]

        for i in range(m + 1):
            dp[i][0] = i          # delete i chars from word1
        for j in range(n + 1):
            dp[0][j] = j          # insert j chars to match word2

        for i in range(1, m + 1):
            for j in range(1, n + 1):
                if word1[i - 1] == word2[j - 1]:
                    dp[i][j] = dp[i - 1][j - 1]
                else:
                    dp[i][j] = 1 + min(
                        dp[i - 1][j - 1],   # replace
                        dp[i - 1][j],       # delete
                        dp[i][j - 1]        # insert
                    )
        return dp[m][n]
```

**Complexity Analysis**
- Time complexity: O(m*n) — two nested loops over the DP table.
- Space complexity: O(m*n) — the DP table. (Can be optimized to O(min(m,n)) with rolling arrays, but not required.)

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the bottom‑up DP on the example `word1 = "horse"`, `word2 = "ros"`.

#### Dry Run

Input: word1 = "horse", word2 = "ros"

| i\j | 0 ( ) | 1 (r) | 2 (o) | 3 (s) |
|-----|-------|-------|-------|-------|
| 0 ( ) | 0 | 1 | 2 | 3 |
| 1 (h) | 1 | 1 (replace) | 2 | 3 |
| 2 (ho)| 2 | 2 | 1 (match) | 2 |
| 3 (hor)| 3 | 2 | 2 | 2 |
| 4 (hors)| 4 | 3 | 3 | 2 |
| 5 (horse)| 5 | 4 | 4 | 3 |

Explanation of a few cells:
- `dp[1][1]`: 'h' vs 'r' → different → 1 + min(dp[0][0]=0, dp[0][1]=1, dp[1][0]=1) = 1.
- `dp[2][2]`: 'o' vs 'o' → same → dp[1][1] = 1.
- Final answer `dp[5][3] = 3`, matching the expected output.

--- 

*End of lecture.*