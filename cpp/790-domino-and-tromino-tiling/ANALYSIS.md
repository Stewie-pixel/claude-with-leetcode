# 2D Dynamic Programing

## Video Solution

For more details about **Domino and Tromino Tiling**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=CecjOo4Zo-g).

## Concept

We need to count the number of ways to completely fill a `2 × n` board using `2×1` dominoes (which can be placed vertically or horizontally) and L‑shaped trominoes (which can be rotated).  
Because each column only interacts with its immediate neighbours, the problem exhibits optimal substructure: the number of tilings for width `n` depends only on tilings for smaller widths. This makes it a classic **1‑dimensional DP** problem.

We define `dp[i]` as the number of ways to tile a `2 × i` board. The answer we want is `dp[n]`.

## When to Use It

Use this DP pattern when you see:
- A board or strip that is built column‑by‑column (or row‑by‑row).
- Tiles that have a fixed height (here 2) and can extend at most a few columns forward.
- The need to count configurations modulo a large prime.
- Overlapping subproblems: the same width appears many times in different tilings.

In short, **tiling / filling problems with constant height** → 1D DP.

## Template

```python
MOD = 10**9 + 7

def count_tilings(n: int) -> int:
    # base cases for small n
    if n == 0: return 1          # empty board
    if n == 1: return 1
    if n == 2: return 2
    if n == 3: return 5

    # dp[i] = number of ways to fill 2 x i board
    dp0, dp1, dp2, dp3 = 1, 1, 2, 5   # dp[i-4], dp[i-3], dp[i-2], dp[i-1]
    for i in range(4, n + 1):
        # recurrence: dp[i] = 2*dp[i-1] + dp[i-3]
        cur = (2 * dp3 + dp0) % MOD
        # shift window
        dp0, dp1, dp2, dp3 = dp1, dp2, dp3, cur
    return dp3
```

## LeetCode Problem Walkthrough

### Problem: 790. Domino and Tromino Tiling

https://leetcode.com/problems/domino-and-tromino-tiling/

### Approach 1: Brute Force (Backtracking)

**Algorithm**
- Try to place a tile at the first empty cell (scan left‑to‑right, top‑to‑bottom).
- For each possible tile (vertical domino, two horizontal dominoes, four tromino orientations) that fits, place it and recursively count tilings of the remaining board.
- Sum the counts; use memoization on the board state to avoid recomputation (state can be represented as a bitmask of two rows).

**Implementation**

```python
from functools import lru_cache

class Solution:
    def numTilings(self, n: int) -> int:
        MOD = 10**9 + 7
        full = (1 << n) - 1          # all cells filled in a row

        @lru_cache(None)
        def dfs(mask_top: int, mask_bottom: int) -> int:
            # if both rows are completely filled -> one valid tiling
            if mask_top == full and mask_bottom == full:
                return 1
            # find first empty cell (scanning top row then bottom)
            if mask_top != full:
                # first empty in top row
                pos = (mask_top ^ full).bit_length() - 1   # index of highest 0? simpler: find low bit
                # easier: scan from left
                for i in range(n):
                    if not (mask_top >> i) & 1:
                        pos = i
                        break
            else:
                # top row full, look at bottom row
                for i in range(n):
                    if not (mask_bottom >> i) & 1:
                        pos = i
                        break

            ans = 0
            # try vertical domino (covers both rows at pos)
            if not (mask_top >> pos) & 1 and not (mask_bottom >> pos) & 1:
                ans += dfs(mask_top | (1 << pos), mask_bottom | (1 << pos))
            # try two horizontal dominoes (top+bottom at pos and pos+1)
            if pos + 1 < n:
                if not ((mask_top >> pos) & 1) and not ((mask_top >> (pos+1)) & 1):
                    ans += dfs(mask_top | (1 << pos) | (1 << (pos+1)), mask_bottom)
                if not ((mask_bottom >> pos) & 1) and not ((mask_bottom >> (pos+1)) & 1):
                    ans += dfs(mask_top, mask_bottom | (1 << pos) | (1 << (pos+1)))
            # try tromino shapes (4 orientations)
            # shape covering (top,pos),(top,pos+1),(bottom,pos)
            if pos + 1 < n:
                if not ((mask_top >> pos) & 1) and not ((mask_top >> (pos+1)) & 1) and not ((mask_bottom >> pos) & 1):
                    ans += dfs(mask_top | (1 << pos) | (1 << (pos+1)), mask_bottom | (1 << pos))
            # shape covering (top,pos),(bottom,pos),(bottom,pos+1)
            if pos + 1 < n:
                if not ((mask_top >> pos) & 1) and not ((mask_bottom >> pos) & 1) and not ((mask_bottom >> (pos+1)) & 1):
                    ans += dfs(mask_top | (1 << pos), mask_bottom | (1 << pos) | (1 << (pos+1)))
            # shape covering (bottom,pos),(bottom,pos+1),(top,pos)
            if pos + 1 < n:
                if not ((mask_bottom >> pos) & 1) and not ((mask_bottom >> (pos+1)) & 1) and not ((mask_top >> pos) & 1):
                    ans += dfs(mask_top | (1 << pos), mask_bottom | (1 << pos) | (1 << (pos+1)))
            # shape covering (bottom,pos),(top,pos),(top,pos+1)
            if pos + 1 < n:
                if not ((mask_bottom >> pos) & 1) and not ((mask_top >> pos) & 1) and not ((mask_top >> (pos+1)) & 1):
                    ans += dfs(mask_top | (1 << pos) | (1 << (pos+1)), mask_bottom | (1 << pos))

            return ans % MOD

        return dfs(0, 0)
```

**Complexity Analysis**
- Time complexity: O(3^n) in the worst case (each column spawns up to 3 recursive calls) — exponential.
- Space complexity: O(n) recursion depth + O(2^n × 2^n) memoization table (impractical for n=1000).

### Approach 2: DP with Linear Recurrence

**Intuition**
Look at the leftmost column that is not yet completely filled. There are limited ways to start filling it:
1. Place a vertical domino → reduces problem to `2 × (n‑1)`.
2. Place two horizontal dominoes (stacked) → reduces to `2 × (n‑2)`.
3. Place an L‑tromino together with a matching tile to fill the gap → this creates a configuration that looks like a `2 × (n‑3)` board plus a filled “corner”. The net effect adds `dp[n‑3]` ways, but because the corner can be on the top or bottom we get `2 * dp[n‑3]`.  
Summing these gives the recurrence `dp[n] = 2·dp[n‑1] + dp[n‑3]`.

**Algorithm**
- Initialize `dp[0]=1, dp[1]=1, dp[2]=2, dp[3]=5`.
- For `i` from 4 to `n`: `dp[i] = (2*dp[i‑1] + dp[i‑3]) % MOD`.
- Return `dp[n]`.

**Implementation**

```python
class Solution:
    def numTilings(self, n: int) -> int:
        MOD = 10**9 + 7
        if n == 0: return 1
        if n == 1: return 1
        if n == 2: return 2
        if n == 3: return 5

        dp = [0] * (n + 1)
        dp[0], dp[1], dp[2], dp[3] = 1, 1, 2, 5
        for i in range(4, n + 1):
            dp[i] = (2 * dp[i-1] + dp[i-3]) % MOD
        return dp[n]
```

**Complexity Analysis**
- Time complexity: O(n) — one loop from 4 to n.
- Space complexity: O(n) for the DP table (can be reduced to O(1)).

### Approach 3: O(1) Space DP (Rolling Variables)

**Intuition**
The recurrence only depends on the three previous values (`dp[i‑1]`, `dp[i‑2]`, `dp[i‑3]`). We can keep just those three variables in a sliding window, updating them as we iterate.

**Algorithm**
- Handle base cases `n ≤ 3` directly.
- Keep `a = dp[i-3]`, `b = dp[i-2]`, `c = dp[i-1]`.
- For each `i` from 4 to `n`: compute `cur = (2*c + a) % MOD`, then shift: `a, b, b = b, c, cur`.
- Return `c` after the loop.

**Implementation**

```python
class Solution:
    def numTilings(self, n: int) -> int:
        MOD = 10**9 + 7
        if n == 0: return 1
        if n == 1: return 1
        if n == 2: return 2
        if n == 3: return 5

        a, b, c = 1, 2, 5   # dp[1], dp[2], dp[3] (we already handled dp[0]=1 implicitly)
        for _ in range(4, n + 1):
            cur = (2 * c + a) % MOD
            a, b, c = b, c, cur
        return c
```

**Complexity Analysis**
- Time complexity: O(n) — single pass.
- Space complexity: O(1) — only a few integer variables.

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimized O(1) DP for `n = 5`.

```
MOD = 1e9+7 (large enough that values stay small for this example)

Initialize:
a = dp[1] = 1
b = dp[2] = 2
c = dp[3] = 5

i = 4:
cur = 2*c + a = 2*5 + 1 = 11   -> dp[4] = 11
shift: a=b=2, b=c=5, c=cur=11

i = 5:
cur = 2*c + a = 2*11 + 2 = 24   -> dp[5] = 24
shift: a=b=5, b=c=11, c=cur=24

Result: dp[5] = 24
```

Thus there are **24** ways to tile a `2 × 5` board with dominoes and trominoes (mod 1e9+7).

--- 

This lecture covers the tiling problem, shows the progression from brute force to an optimal O(n) time / O(1) space DP solution, and provides a concrete dry‑run to solidify understanding.
