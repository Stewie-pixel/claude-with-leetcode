# 2D Dynamic Programming

## Video Solution

For more details about **Longest Common Subsequence**, watch the walkthrough at [https://www.youtube.com/watch?v=Ua0GhsJSlWM](https://www.youtube.com/watch?v=Ua0GhsJSlWM)

## Concept

Dynamic Programming solves problems by breaking them into overlapping sub‑problems and storing the results so each sub‑problem is solved only once.  
For the Longest Common Subsequence (LCS) we compare two strings character‑by‑character.  
If the current characters match, they can be part of the LCS, so we add 1 to the best answer for the prefixes *without* those characters.  
If they don’t match, we skip one character from either string and keep the better of the two possibilities.

A real‑world analogy: imagine two sentences and you want to find the longest phrase that appears in both in the same order (not necessarily consecutively). You scan the sentences left‑to‑right, and whenever you see the same word you can extend the phrase; otherwise you decide which sentence to advance in.

## When to Use It

Use 2D DP (specifically the LCS pattern) when you see:
- Two sequences (strings, arrays) and you need the longest/shortest common **subsequence** (order matters, not necessarily contiguous).
- Problems that ask for edit distance, longest common subsequence, shortest common supersequence, or similar alignment‑type tasks.
- The input sizes are up to a few thousand (O(n·m) time and space is acceptable).

## Template

```python
def lcs(text1: str, text2: str) -> int:
    n, m = len(text1), len(text2)
    # dp[i][j] = LCS length of text1[:i] and text2[:j]
    dp = [[0] * (m + 1) for _ in range(n + 1)]

    for i in range(1, n + 1):
        for j in range(1, m + 1):
            if text1[i - 1] == text2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1          # match → extend
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]) # skip one char

    return dp[n][m]
```

## LeetCode Problem Walkthrough

### Problem: 1143. Longest Common Subsequence

https://leetcode.com/problems/longest-common-subsequence/

---

### Approach 1: Brute Force (Recursion)

**Algorithm**
- Try every possible subsequence of `text1` and check if it appears in `text2`.
- Recursively, at each pair of indices `(i, j)`:
  - If either string is exhausted → return 0.
  - If `text1[i] == text2[j]` → we can take this character and solve for `(i+1, j+1)`, adding 1.
  - Otherwise we have two choices: skip `text1[i]` → `(i+1, j)` or skip `text2[j]` → `(i, j+1)`. Return the max of the two.

**Implementation**

```python
class Solution:
    def longestCommonSubsequence(self, text1: str, text2: str) -> int:
        def dfs(i: int, j: int) -> int:
            if i == len(text1) or j == len(text2):
                return 0
            if text1[i] == text2[j]:
                return 1 + dfs(i + 1, j + 1)
            else:
                return max(dfs(i + 1, j), dfs(i, j + 1))

        return dfs(0, 0)
```

**Complexity Analysis**
- Time complexity: O(2^(n+m)) — each call branches into two when characters differ, leading to exponential calls.
- Space complexity: O(n+m) — recursion stack depth at most the length of the longer string.

---

### Approach 2: Top‑Down Memoization

**Intuition**
The brute force recursion recomputes the same `(i, j)` state many times. By caching results we turn the exponential recursion into a polynomial DP.

**Algorithm**
- Use a memo dictionary (or 2D array) initialized to -1.
- The recursive function returns the cached value if present; otherwise compute as in brute force and store it.

**Implementation**

```python
class Solution:
    def longestCommonSubsequence(self, text1: str, text2: str) -> int:
        n, m = len(text1), len(text2)
        memo = [[-1] * m for _ in range(n)]

        def dfs(i: int, j: int) -> int:
            if i == n or j == m:
                return 0
            if memo[i][j] != -1:
                return memo[i][j]

            if text1[i] == text2[j]:
                memo[i][j] = 1 + dfs(i + 1, j + 1)
            else:
                memo[i][j] = max(dfs(i + 1, j), dfs(i, j + 1))
            return memo[i][j]

        return dfs(0, 0)
```

**Complexity Analysis**
- Time complexity: O(n·m) — each state `(i, j)` is computed once.
- Space complexity: O(n·m) — memo table plus recursion stack O(max(n,m)).

---

### Approach 3: Bottom‑Up DP (Iterative)

**Intuition**
Instead of recursion we fill a table iteratively from the base cases (empty prefixes) outward. The relation is the same: if characters match we extend the diagonal cell; otherwise we take the max of the top or left cell.

**Algorithm**
- Create a `(n+1) × (m+1)` table initialized with 0 (empty prefix vs anything gives 0).
- Iterate `i` from 1..n and `j` from 1..m:
  - If `text1[i-1] == text2[j-1]`: `dp[i][j] = dp[i-1][j-1] + 1`.
  - Else: `dp[i][j] = max(dp[i-1][j], dp[i][j-1])`.
- Answer is `dp[n][m]`.

**Implementation**

```python
class Solution:
    def longestCommonSubsequence(self, text1: str, text2: str) -> int:
        n, m = len(text1), len(text2)
        dp = [[0] * (m + 1) for _ in range(n + 1)]

        for i in range(1, n + 1):
            for j in range(1, m + 1):
                if text1[i - 1] == text2[j - 1]:
                    dp[i][j] = dp[i - 1][j - 1] + 1
                else:
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])

        return dp[n][m]
```

**Complexity Analysis**
- Time complexity: O(n·m) — two nested loops.
- Space complexity: O(n·m) — the DP table. (Can be reduced to O(min(n,m)) with rolling arrays, but not required for clarity.)

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the bottom‑up DP on the example `text1 = "abcde"`, `text2 = "ace"`.

#### Dry Run

| i\j | 0 (∅) | 1 (a) | 2 (c) | 3 (e) |
|-----|-------|-------|-------|-------|
| 0   | 0     | 0     | 0     | 0     |
| 1 (a) | 0   | 1 (a↔a) | 1     | 1     |
| 2 (b) | 0   | 1     | 1     | 1     |
| 3 (c) | 0   | 1     | 2 (c↔c) | 2     |
| 4 (d) | 0   | 1     | 2     | 2     |
| 5 (e) | 0   | 1     | 2     | 3 (e↔e) |

**Explanation of a few cells**
- `dp[1][1]`: `text1[0]=='a'`, `text2[0]=='a'` → match → `dp[0][0]+1 = 1`.
- `dp[3][2]`: `text1[2]=='c'`, `text2[1]=='c'` → match → `dp[2][1]+1 = 1+1 = 2`.
- Final cell `dp[5][3] = 3` → length of `"ace"`.

---

### Summary

- Start with the naïve exponential recursion to understand the overlapping sub‑problems.
- Apply memoization to obtain O(n·m) time and space.
- Implement the clean bottom‑up table, which is the most common interview solution.
- Always state both time and space complexity for each approach.