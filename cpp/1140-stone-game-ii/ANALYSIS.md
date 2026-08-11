# 2D Dynamic Programming

## Video Solution

For more details about **Stone Game II**, watch the walkthrough at [https://www.youtube.com/watch?v=Ik-wBw3sOfo](https://www.youtube.com/watch?v=Ik-wBw3sOfo).

## Concept

Stone Game II is a turn‑based game where each player can take a variable number of piles from the front of the row. The number of piles you may take on a turn depends on a parameter `M`, which starts at 1 and is updated to `max(M, X)` after taking `X` piles. Both players play optimally, trying to maximize their own total stones.

The key observation is that the future of the game depends only on two pieces of information:
1. The index `i` of the first remaining pile.
2. The current value of `M`.

If we know `(i, M)`, the optimal result for the current player can be expressed in terms of the results for the states that follow after taking `X` piles (`1 ≤ X ≤ 2M`). This naturally leads to a DP over the two‑dimensional state `(i, M)`.

To avoid repeatedly summing piles, we pre‑compute a **suffix sum array** `suffix[i] = sum(piles[i:])`. Then the stones remaining from index `i` onward are obtained in O(1) time.

## When to Use It

Use 2D DP when you see:
- A turn‑based game where each move changes a parameter that influences future move limits (here `M`).
- The decision depends on both a position in an array/list and an auxiliary variable that can vary up to O(n).
- The goal is to maximize/minimize a score assuming optimal play from both sides.

Typical clues: “take X piles where 1 ≤ X ≤ 2M”, “M = max(M, X)", “return the maximum stones the first player can get”.

## Template

```python
from functools import lru_cache

def solve_game(piles):
    n = len(piles)
    # suffix[i] = sum(piles[i:])
    suffix = [0] * (n + 1)
    for i in range(n - 1, -1, -1):
        suffix[i] = suffix[i + 1] + piles[i]

    @lru_cache(None)
    def dp(i, M):
        # If we can take all remaining piles, take them.
        if i + 2 * M >= n:
            return suffix[i]

        best = 0          # stones the current player can guarantee from state (i, M)
        # Try every legal X
        for X in range(1, 2 * M + 1):
            # Opponent's optimal gain from the resulting state
            opponent = dp(i + X, max(M, X))
            # Current player gets everything left minus what the opponent will get
            current = suffix[i] - opponent
            if current > best:
                best = current
        return best

    return dp(0, 1)
```

## LeetCode Problem Walkthrough

### Problem: 1140. Stone Game II

https://leetcode.com/problems/stone-game-ii/

### Approach 1: Brute Force (Plain Recursion)

**Algorithm**
- Recursively simulate every possible choice of `X` (1 … 2M) for the current player.
- After taking `X` piles, the game continues from index `i+X` with new `M' = max(M, X)`.
- When no piles remain, return 0.
- The current player’s score is the sum of the taken piles plus whatever they can obtain from the subsequent state (since the opponent will then play optimally, we subtract the opponent’s best gain from the remaining stones).
- Because we explore every branch without memoization, the same state `(i, M)` may be recomputed many times.

**Implementation**

```python
class Solution:
    def stoneGameII(self, piles: List[int]) -> int:
        n = len(piles)

        # helper to compute sum of piles[l:r] quickly (prefix sum)
        prefix = [0] * (n + 1)
        for i in range(n):
            prefix[i + 1] = prefix[i] + piles[i]

        def range_sum(l, r):   # inclusive l, exclusive r
            return prefix[r] - prefix[l]

        def dfs(i, M):
            # if we can take all remaining piles, just take them
            if i + 2 * M >= n:
                return range_sum(i, n)

            best = 0
            for X in range(1, 2 * M + 1):
                taken = range_sum(i, i + X)
                opponent = dfs(i + X, max(M, X))
                # after our move, the opponent will get `opponent` from the rest
                # we keep the rest of the stones: total remaining - opponent
                remaining = range_sum(i, n)
                current = taken + (remaining - taken - opponent)  # = remaining - opponent
                if current > best:
                    best = current
            return best

        return dfs(0, 1)
```

**Complexity Analysis**
- Time complexity: O(2^n) — each call branches up to 2M ≤ 2n times, and depth can be n.
- Space complexity: O(n) — recursion stack depth (no memo storage).

### Approach 2: Memoized Top‑Down DP

**Intuition**
The brute force solution recomputes the result for the same `(i, M)` many times. By caching these results we ensure each state is processed once. The suffix sum lets us obtain the total stones remaining in O(1), turning the inner loop into a simple maximization.

**Algorithm**
- Build a suffix sum array `suffix[i] = sum(piles[i:])`.
- Define `dp(i, M)` as the maximum stones the current player can obtain starting from pile `i` with parameter `M`.
- Base case: if `i + 2M ≥ n`, the current player can take all remaining stones → return `suffix[i]`.
- Otherwise, try every legal `X` (1 … 2M). After taking `X` piles, the opponent will obtain `dp(i+X, max(M, X))`. The current player gets `suffix[i] - opponent`.
- Return the maximum over all `X`.
- Memoize results with `@lru_cache` (or a dictionary).

**Implementation**

```python
from functools import lru_cache
from typing import List

class Solution:
    def stoneGameII(self, piles: List[int]) -> int:
        n = len(piles)

        # suffix[i] = sum(piles[i:])
        suffix = [0] * (n + 1)
        for i in range(n - 1, -1, -1):
            suffix[i] = suffix[i + 1] + piles[i]

        @lru_cache(maxsize=None)
        def dp(i: int, M: int) -> int:
            # If we can take all remaining piles, do it.
            if i + 2 * M >= n:
                return suffix[i]

            best = 0
            for X in range(1, 2 * M + 1):
                opponent = dp(i + X, max(M, X))
                current = suffix[i] - opponent   # stones we keep
                if current > best:
                    best = current
            return best

        return dp(0, 1)
```

**Complexity Analysis**
- Time complexity: O(n³) — there are O(n²) distinct states (`i` in [0, n), `M` in [1, n]) and each state iterates up to 2M ≤ O(n).
- Space complexity: O(n²) for memoization plus O(n) for the suffix array → O(n²).

### Approach 3: Bottom‑Up DP (Iterative)

**Intuition**
Instead of recursion we can fill a DP table from the end of the array towards the start. The relation is identical: `dp[i][m] = max_{1≤X≤2m} (suffix[i] - dp[i+X][max(m, X)])`. Computing it iteratively avoids recursion overhead and makes the order of evaluation explicit.

**Algorithm**
- Compute suffix sums as before.
- Create a 2D array `dp[i][m]` where `i` ranges 0…n and `m` ranges 1…n (we can cap `m` at n because `M` never exceeds n).
- Initialize `dp[i][m] = suffix[i]` for all `i` where `i + 2m ≥ n` (base case).
- Fill the table for `i` from n‑1 down to 0 and for `m` from 1 up to n:
    - For each allowed `X`, compute candidate = `suffix[i] - dp[i+X][max(m, X)]`.
    - Keep the maximum candidate.
- Answer is `dp[0][1]`.

**Implementation**

```python
from typing import List

class Solution:
    def stoneGameII(self, piles: List[int]) -> int:
        n = len(piles)

        # suffix[i] = sum(piles[i:])
        suffix = [0] * (n + 1)
        for i in range(n - 1, -1, -1):
            suffix[i] = suffix[i + 1] + piles[i]

        # dp[i][m] = max stones current player can get from position i with M=m
        # We only need m up to n because M never exceeds n.
        dp = [[0] * (n + 1) for _ in range(n + 1)]

        # Base cases: if we can take all remaining piles, take them.
        for i in range(n):
            for m in range(1, n + 1):
                if i + 2 * m >= n:
                    dp[i][m] = suffix[i]

        # Fill table backwards
        for i in range(n - 1, -1, -1):
            for m in range(1, n + 1):
                if i + 2 * m >= n:   # already set by base case, can skip
                    continue
                best = 0
                for X in range(1, 2 * m + 1):
                    nxt_i = i + X
                    nxt_m = max(m, X)
                    candidate = suffix[i] - dp[nxt_i][nxt_m]
                    if candidate > best:
                        best = candidate
                dp[i][m] = best

        return dp[0][1]
```

**Complexity Analysis**
- Time complexity: O(n³) — three nested loops (`i`, `m`, `X`) each up to O(n).
- Space complexity: O(n²) for the DP table plus O(n) for suffix → O(n²).

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the memoized solution on the example `piles = [2, 7, 9, 4, 4]` (n = 5).

**Suffix sums**

```
index: 0 1 2 3 4 5
piles: 2 7 9 4 4
suffix:26 24 17 8 4 0
```

**Dry run of dp(i, M)** (showing only states that are actually visited)

```
dp(0,1):
  i+2M = 0+2 = 2 < 5 → not base case
  try X=1:
     opponent = dp(1, max(1,1)=1)
  try X=2:
     opponent = dp(2, max(1,2)=2)
  ... (we need values of those substates)
```

We compute the needed substates:

- `dp(4,1)`: i+2M = 4+2=6 ≥5 → base → suffix[4] = 4
- `dp(3,1)`: 3+2=5 ≥5 → base → suffix[3] = 8
- `dp(3,2)`: 3+4=7 ≥5 → base → suffix[3] = 8
- `dp(2,1)`: 2+2=4 <5 → try X=1,2
    * X=1 → opponent = dp(3,1)=8 → current = suffix[2] - 8 = 17-8=9
    * X=2 → opponent = dp(4,2)=? (4+4≥5 → base) suffix[4]=4 → current = 17-4=13 → best=13
    → dp(2,1)=13
- `dp(2,2)`: 2+4=6 ≥5 → base → suffix[2]=17
- `dp(1,1)`: 1+2=3 <5 → try X=1,2
    * X=1 → opponent = dp(2,1)=13 → current = suffix[1]-13 = 24-13=11
    * X=2 → opponent = dp(3,2)=8 → current = 24-8=16 → best=16
    → dp(1,1)=16
- `dp(1,2)`: 1+4=5 ≥5 → base → suffix[1]=24
- `dp(0,1)` continuation:
    * X=1 → opponent = dp(1,1)=16 → current = suffix[0]-16 = 26-16=10
    * X=2 → opponent = dp(2,2)=17 → current = 26-17=9
    * X>2 not allowed (2M=2)
    → best = max(10,9) = 10

Thus `dp(0,1) = 10`, matching the expected answer.

The table shows how the algorithm reuses previously computed states (`dp(2,1)`, `dp(1,1)`) instead of recomputing them, turning an exponential search into a polynomial‑time solution.