# 1D Dynamic Programming

## Video Solution

For more details about **Stone Game III**, watch the walkthrough at [https://www.youtube.com/watch?v=HsLG5QW9CFQ](https://www.youtube.com/watch?v=HsLG5QW9CFQ)

## Concept

1D DP solves problems where the optimal solution for a prefix (or suffix) depends only on a fixed number of previous states. We store results in a 1‑dimensional array (or a few variables) and build the answer iteratively.

**Analogy:** Imagine climbing a staircase where you can take 1, 2, or 3 steps at a time, and each step has a reward (or cost). The best total reward you can achieve from step *i* to the top depends only on the best rewards from the next 1‑3 steps.

## When to Use It

Use 1D DP when you see:
- The problem asks for an optimal value (max/min, win/lose) over a sequence.
- The decision at position *i* only depends on a constant‑size window of future (or previous) positions (e.g., take 1‑3 stones).
- Overlapping subproblems appear when you try a brute‑force recursion.

## Template

```python
# dp[i] = best outcome we can achieve from position i to the end
dp = [0] * (n + 1)          # extra slot for base case
for i in range(n - 1, -1, -1):
    # compute dp[i] using dp[i+1], dp[i+2], ... up to a fixed window
    dp[i] = ...             # transition
return dp[0]                # answer from the start
```

If only the last *k* values are needed, we can replace the array with a rolling window of size *k*.

## LeetCode Problem Walkthrough

### Problem: 1406. Stone Game III

https://leetcode.com/problems/stone-game-iii/

### Approach 1: Brute Force (Recursion)

**Algorithm**
- Try every possible move (take 1, 2, or 3 stones) from the current index.
- The current player's gain is the sum of taken stones minus the opponent's best result from the remaining position.
- Return the maximum gain achievable.
- Base case: when index == n, no stones left → gain 0.

**Implementation**

```python
from functools import lru_cache

class Solution:
    def stoneGameIII(self, stoneValue: List[int]) -> str:
        n = len(stoneValue)

        @lru_cache(None)
        def dfs(i: int) -> int:
            if i >= n:
                return 0
            best = -10**9
            take = 0
            for k in range(1, 4):
                if i + k > n:
                    break
                take += stoneValue[i + k - 1]
                best = max(best, take - dfs(i + k))
            return best

        diff = dfs(0)
        if diff > 0:
            return "Alice"
        elif diff < 0:
            return "Bob"
        else:
            return "Tie"
```

**Complexity Analysis**
- Time complexity: O(3^n) without memoization; with memoization it becomes O(n) but we treat this as the pure brute force version → exponential.
- Space complexity: O(n) recursion stack + O(n) cache → O(n).

*(Note: The above code already uses memoization; the pure brute force would omit @lru_cache and be exponential.)*

### Approach 2: Top‑Down DP with Memoization

**Intuition**
The recursion repeatedly solves the same sub‑problem: “what is the net advantage (current player score – opponent score) starting from index *i*?” Storing that result avoids recomputation.

**Algorithm**
- Same recursion as above but cache the result for each index *i*.
- Iterate *i* from *n‑1* down to 0 (or rely on recursion with memo).
- The transition: dp[i] = max_{k=1..3} (sum_{t=0}^{k-1} stoneValue[i+t] - dp[i+k]).
- dp[n] = 0 (no stones left).

**Implementation**

```python
class Solution:
    def stoneGameIII(self, stoneValue: List[int]) -> str:
        n = len(stoneValue)
        dp = [float('-inf')] * (n + 1)
        dp[n] = 0                         # base case

        for i in range(n - 1, -1, -1):
            take = 0
            best = float('-inf')
            for k in range(1, 4):
                if i + k > n:
                    break
                take += stoneValue[i + k - 1]
                best = max(best, take - dp[i + k])
            dp[i] = best

        diff = dp[0]
        if diff > 0:
            return "Alice"
        elif diff < 0:
            return "Bob"
        else:
            return "Tie"
```

**Complexity Analysis**
- Time complexity: O(n) — each state *i* computes at most 3 transitions.
- Space complexity: O(n) — the dp array of size *n+1*.

### Approach 3: Bottom‑Up DP with O(1) Space

**Intuition**
Since dp[i] only depends on dp[i+1], dp[i+2], and dp[i+3], we never need the full array; a rolling window of three values suffices.

**Algorithm**
- Initialize three variables representing dp[i+1], dp[i+2], dp[i+3] (starting with zeros for beyond the end).
- Iterate *i* from *n‑1* down to 0:
    - Compute take sums for k=1,2,3.
    - Compute candidate = take - future_dp[k] where future_dp[1] = dp[i+1], etc.
    - dp_i = max(candidate1, candidate2, candidate3).
    - Shift the window: dp[i+3] <- dp[i+2]; dp[i+2] <- dp[i+1]; dp[i+1] <- dp_i.
- After loop, dp_i holds dp[0] (the advantage for Alice).

**Implementation**

```python
class Solution:
    def stoneGameIII(self, stoneValue: List[int]) -> str:
        n = len(stoneValue)
        # dp_i_plus_1, dp_i_plus_2, dp_i_plus_3 correspond to dp[i+1], dp[i+2], dp[i+3]
        dp_i_plus_1 = dp_i_plus_2 = dp_i_plus_3 = 0

        for i in range(n - 1, -1, -1):
            take = 0
            best = -10**9
            # k = 1
            take += stoneValue[i]
            best = max(best, take - dp_i_plus_1)
            # k = 2
            if i + 1 < n:
                take += stoneValue[i + 1]
                best = max(best, take - dp_i_plus_2)
            # k = 3
            if i + 2 < n:
                take += stoneValue[i + 2]
                best = max(best, take - dp_i_plus_3)
            # shift window
            dp_i_plus_3 = dp_i_plus_2
            dp_i_plus_2 = dp_i_plus_1
            dp_i_plus_1 = best

        diff = dp_i_plus_1   # after last shift, dp_i_plus_1 holds dp[0]
        if diff > 0:
            return "Alice"
        elif diff < 0:
            return "Bob"
        else:
            return "Tie"
```

**Complexity Analysis**
- Time complexity: O(n) — one pass, constant work per index.
- Space complexity: O(1) — only a few integer variables.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: stoneValue = [1,2,3,7]

We'll trace the O(1) space version.

Initialize: dp1 = dp2 = dp3 = 0 (represent dp[4], dp[5], dp[6] – all beyond array).

i = 3 (value 7)
- k1: take = 7 → cand = 7 - dp1(0) = 7
- k2: i+1=4 out of bounds → stop
- best = 7
Shift: dp3=dp2(0), dp2=dp1(0), dp1=best(7) → dp1=7, dp2=0, dp3=0

i = 2 (value 3)
- k1: take = 3 → cand = 3 - dp1(7) = -4
- k2: take = 3+7=10 → cand = 10 - dp2(0) = 10
- k3: i+2=4 out → stop
- best = max(-4,10) = 10
Shift: dp3=dp2(0), dp2=dp1(7), dp1=10 → dp1=10, dp2=7, dp3=0

i = 1 (value 2)
- k1: take=2 → cand = 2 - dp1(10) = -8
- k2: take=2+3=5 → cand = 5 - dp2(7) = -2
- k3: take=2+3+7=12 → cand = 12 - dp3(0) = 12
- best = max(-8,-2,12)=12
Shift: dp3=dp2(7), dp2=dp1(10), dp1=12 → dp1=12, dp2=10, dp3=7

i = 0 (value 1)
- k1: take=1 → cand = 1 - dp1(12) = -11
- k2: take=1+2=3 → cand = 3 - dp2(10) = -7
- k3: take=1+2+3=6 → cand = 6 - dp3(7) = -1
- best = max(-11,-7,-1) = -1
Shift: dp3=dp2(10), dp2=dp1(12), dp1=-1 → dp1=-1, dp2=12, dp3=10

After loop, diff = dp1 = -1 → negative → Bob wins. Matches example.