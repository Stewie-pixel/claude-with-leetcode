# 2D Dynamic Programming

## Video Solution

For more details about **Interleaving String**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=3Rw3p9LrgvE).

## Concept

We want to know whether `s3` can be formed by interleaving `s1` and `s2` while preserving the relative order of characters from each string.  
Define `dp[i][j]` as **True** if the first `i` characters of `s1` and the first `j` characters of `s2` can interleave to form the first `i + j` characters of `s3`.  
The answer we need is `dp[len(s1)][len(s2)]`.

Transition:  
- If we take the last character from `s1` (`s1[i‑1]`) and it matches `s3[i+j‑1]`, then `dp[i][j]` is true when `dp[i‑1][j]` was true.  
- Similarly, if we take the last character from `s2` (`s2[j‑1]`) and it matches `s3[i+j‑1]`, then `dp[i][j]` is true when `dp[i][j‑1]` was true.

Initialize `dp[0][0] = True` (empty strings interleave to an empty string). Fill the table row‑by‑row (or column‑by‑column).

## When to Use It

Use 2D DP when:
- You need to preserve order of two (or more) sequences while building a target sequence.
- The problem statement mentions “interleaving”, “merge”, “shuffle”, or “forming a string by picking from two strings”.
- Lengths of inputs are modest (≤ 100‑200) allowing O(n*m) memory/time.

## Template

```python
def interleaving_dp(s1: str, s2: str, s3: str) -> bool:
    # Quick length check
    if len(s1) + len(s2) != len(s3):
        return False

    # dp[i][j] == True  <=>  s1[:i] and s2[:j] interleave to s3[:i+j]
    dp = [[False] * (len(s2) + 1) for _ in range(len(s1) + 1)]
    dp[0][0] = True

    # Fill first row (only s2 contributes)
    for j in range(1, len(s2) + 1):
        dp[0][j] = dp[0][j-1] and s2[j-1] == s3[j-1]

    # Fill first column (only s1 contributes)
    for i in range(1, len(s1) + 1):
        dp[i][0] = dp[i-1][0] and s1[i-1] == s3[i-1]

    # Fill the rest
    for i in range(1, len(s1) + 1):
        for j in range(1, len(s2) + 1):
            take_from_s1 = dp[i-1][j] and s1[i-1] == s3[i+j-1]
            take_from_s2 = dp[i][j-1] and s2[j-1] == s3[i+j-1]
            dp[i][j] = take_from_s1 or take_from_s2

    return dp[len(s1)][len(s2)]
```

## LeetCode Problem Walkthrough

### Problem: 97. Interleaving String

https://leetcode.com/problems/interleaving-string/

### Approach 1: Brute Force Recursion

**Algorithm**  
Try every possible way to build `s3` by recursively picking the next character either from `s1` or from `s2`, as long as it matches the current position in `s3`.  
If we reach the end of `s3` successfully, return `True`.  
If both choices fail, backtrack.

**Implementation**

```python
def isInterleave_bruteforce(s1: str, s2: str, s3: str) -> bool:
    if len(s1) + len(s2) != len(s3):
        return False

    def dfs(i: int, j: int) -> bool:
        # i chars taken from s1, j chars taken from s2
        if i == len(s1) and j == len(s2):
            return True
        # try taking from s1
        if i < len(s1) and s1[i] == s3[i+j] and dfs(i+1, j):
            return True
        # try taking from s2
        if j < len(s2) and s2[j] == s3[i+j] and dfs(i, j+1):
            return True
        return False

    return dfs(0, 0)
```

**Complexity Analysis**

- Time complexity: O(2^(n+m)) — each step branches into two choices in the worst case.
- Space complexity: O(n+m) — recursion stack depth equals length of `s3`.

### Approach 2: Memoization (Top‑Down DP)

**Intuition**  
The brute force explores many overlapping subproblems (same `(i, j)` pair reached via different paths).  
Cache the result of each `(i, j)` state to avoid recomputation.

**Algorithm**  
Same recursion as above, but store computed results in a dictionary/memo table.  
Return `True` if either choice leads to a solution; otherwise store and return `False`.

**Implementation**

```python
def isInterleave_memo(s1: str, s2: str, s3: str) -> bool:
    if len(s1) + len(s2) != len(s3):
        return False

    memo = {}

    def dfs(i: int, j: int) -> bool:
        if (i, j) in memo:
            return memo[(i, j)]
        if i == len(s1) and j == len(s2):
            memo[(i, j)] = True
            return True

        res = False
        if i < len(s1) and s1[i] == s3[i+j]:
            res = dfs(i+1, j)
        if not res and j < len(s2) and s2[j] == s3[i+j]:
            res = dfs(i, j+1)

        memo[(i, j)] = res
        return res

    return dfs(0, 0)
```

**Complexity Analysis**

- Time complexity: O(n*m) — each state `(i, j)` computed once.
- Space complexity: O(n*m) — memo table plus recursion stack.

### Approach 3: Bottom‑Up DP (2D Table)

**Intuition**  
Instead of recursion, fill a table iteratively using the same transition logic.  
This eliminates recursion overhead and makes the DP order explicit.

**Algorithm**  
(See the Template section above.)

**Implementation**

```python
def isInterleave_dp(s1: str, s2: str, s3: str) -> bool:
    if len(s1) + len(s2) != len(s3):
        return False

    n, m = len(s1), len(s2)
    dp = [[False] * (m + 1) for _ in range(n + 1)]
    dp[0][0] = True

    # first row
    for j in range(1, m + 1):
        dp[0][j] = dp[0][j-1] and s2[j-1] == s3[j-1]

    # first column
    for i in range(1, n + 1):
        dp[i][0] = dp[i-1][0] and s1[i-1] == s3[i-1]

    # rest of the table
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            from_s1 = dp[i-1][j] and s1[i-1] == s3[i+j-1]
            from_s2 = dp[i][j-1] and s2[j-1] == s3[i+j-1]
            dp[i][j] = from_s1 or from_s2

    return dp[n][m]
```

**Complexity Analysis**

- Time complexity: O(n*m) — each cell filled once.
- Space complexity: O(n*m) — the DP table.

*(Optional follow‑up: space can be reduced to O(m) by keeping only the current row.)*

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

### Dry Run

We trace the bottom‑up DP on the example:

```
s1 = "aabcc"
s2 = "dbbca"
s3 = "aadbbcbcac"
```

Lengths: n = 5, m = 5, n+m = 10 = len(s3) → proceed.

We show the table after initialization and after each row; only the relevant cells are displayed.

```
dp[i][j]  (i = chars taken from s1, j = chars taken from s2)
```

**Initialization**

- dp[0][0] = True  
- First row (i = 0):
  - j=1: s2[0]='d' vs s3[0]='a' → False → dp[0][1]=False  
  - Since dp[0][1] is False, the rest of the row stays False.

- First column (j = 0):
  - i=1: s1[0]='a' vs s3[0]='a' → True → dp[1][0]=True  
  - i=2: s1[1]='a' vs s3[1]='a' → True → dp[2][0]=True  
  - i=3: s1[2]='b' vs s3[2]='d' → False → dp[3][0]=False (and remaining False)

So after borders we have:

```
j  0 1 2 3 4 5
i0 T F F F F F
i1 T . . . . .
i2 T . . . . .
i3 F . . . . .
i4 F . . . . .
i5 F . . . . .
```

**Filling the table**

We compute cell by cell (row‑major order).

| i | j | s1[i-1] | s2[j-1] | s3[i+j-1] | from_s1 | from_s2 | dp[i][j] |
|---|---|---------|---------|-----------|---------|---------|----------|
|1|1|'a'|'d'|s3[1]='a'|dp[0][1]&'a'=='a' → F|dp[1][0]&'d'=='a' → T&F→F|F|
|1|2|'a'|'b'|s3[2]='d'|dp[0][2]&'a'=='d'→F|dp[1][1]&'b'=='d'→F|F|
|1|3|'a'|'b'|s3[3]='b'|dp[0][3]&'a'=='b'→F|dp[1][2]&'b'=='b'→F|F|
|1|4|'a'|'c'|s3[4]='b'|dp[0][4]&'a'=='b'→F|dp[1][3]&'c'=='b'→F|F|
|1|5|'a'|'a'|s3[5]='c'|dp[0][5]&'a'=='c'→F|dp[1][4]&'a'=='c'→F|F|

Row 1 stays all False.

Proceed similarly; the only path that yields True follows the interleaving:

```
(0,0) → (1,0) → (2,0) → (2,1) → (2,2) → (3,2) → (3,3) → (4,3) → (4,4) → (5,4) → (5,5)
```

At the end `dp[5][5]` becomes True.

**Result**

The algorithm returns `True`, matching the expected output.
