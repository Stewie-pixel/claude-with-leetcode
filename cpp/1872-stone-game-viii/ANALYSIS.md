# 1D Dynamic Programming

## Video Solution

For more details about **Stone Game VIII**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=bpDsg8OGcs0)

## Concept

1‑D DP solves problems where the optimal solution for a prefix (or suffix) of the input depends only on a *single* earlier state.  
Think of climbing a staircase where each step’s best score only depends on the best score of the previous step (or a few previous steps). By storing those intermediate results we avoid recomputing the same sub‑problems over and over.

In game‑theory problems like Stone Game VIII the “state” is the current position in the array, and the value we store is the **maximum score difference** the current player can achieve from that position to the end, assuming both play optimally.

## When to Use It

Use 1‑D DP when you notice:

* The problem asks for an optimal value (max/min, win/lose, score difference) over a sequence.
* The decision at index *i* only depends on a limited set of earlier indices (usually *i‑1*, *i‑2*, … or a suffix/prefix aggregate).
* You can express the recurrence as  
  `dp[i] = f( dp[i‑1], dp[i‑2], …, some function of input[0..i] )`.
* Brute force would try all possibilities leading to exponential time, but overlapping sub‑problems appear.

Typical tags: **game theory**, **prefix/suffix sums**, **alternating turns**, **optimal substructure**.

## Template

```python
def solve_1d_dp(arr):
    n = len(arr)
    # dp[i] will hold the answer for the sub‑array starting at i
    dp = [0] * (n + 1)          # extra cell for base case (empty suffix)
    # base case: dp[n] = 0  (no stones → score difference 0)
    for i in range(n - 1, -1, -1):
        # transition depends on the problem; placeholder:
        # dp[i] = best choice using arr[i] and dp[i+1] ...
        dp[i] = ...              # <-- replace with actual recurrence
    return dp[0]                # answer for the whole array
```

Replace the transition (`...`) with the specific relation for the problem at hand.

---

## LeetCode Problem Walkthrough

### Problem: 1872. Stone Game VIII

https://leetcode.com/problems/stone-game-viii/

### Approach 1: Brute Force (Recursion)

**Algorithm**  
Explore every legal move `x > 1`.  
When a player removes the leftmost `x` stones, they gain `sum(stones[0:x])` points and a new stone with that sum is placed at the front.  
The game then continues on the new array.  
We compute the score difference as  

```
score_diff(current_player) = max over x ( gained - score_diff(next_player) )
```

The recursion stops when only one stone remains (no further move → difference 0).

**Implementation**

```python
from itertools import accumulate
from functools import lru_cache

class Solution:
    def stoneGameVIII(self, stones):
        n = len(stones)
        pref = list(accumulate(stones))          # prefix sums for O(1) range sum

        @lru_cache(None)
        def dfs(start):                          # start index of current virtual array
            # only one stone left → game over
            if start == n - 1:
                return 0
            best = -float('inf')
            # try every possible x (>1) that fits in the remaining length
            for x in range(2, n - start + 1):
                taken = pref[start + x - 1] - (pref[start - 1] if start > 0 else 0)
                # after the move the new array begins with the summed stone,
                # which is equivalent to starting at start+1 but with its value changed.
                # Instead of rebuilding the array we notice the summed stone will be
                # part of all future prefix sums, so we can just treat the state as
                # starting at start+1 and subtract the opponent's best result.
                best = max(best, taken - dfs(start + 1))
            return best

        return dfs(0)
```

**Complexity Analysis**

- Time complexity: `O(2^n)` – each call branches into up to `O(n)` choices and depth is `O(n)`; memoization would cut this down, but without it we explore an exponential game tree.  
- Space complexity: `O(n)` – recursion depth plus memoization table (if used).

---

### Approach 2: Top‑Down Memoized DP (Optimized Recursion)

**Intuition**  
The brute‑force recursion recomputes the same state many times because the game only cares about the *starting index* of the current array, not the exact contents (the added stone’s value is always the sum of the removed prefix).  
Thus we can memoize the result for each start index.

**Algorithm**  
Same recurrence as above, but we cache `dfs(start)` so each index is processed once.  
The transition simplifies because after taking a prefix of length `x`, the new first stone’s value equals the prefix sum, which will be added to all future prefix sums. This lets us rewrite the recurrence purely in terms of prefix sums:

```
dfs(i) = max_{j>i} ( prefix[j] - dfs(j+1) )
```

where `prefix[j]` is the sum of stones[0..j] (inclusive) and we treat the array as 0‑based.

**Implementation**

```python
from itertools import accumulate
from functools import lru_cache

class Solution:
    def stoneGameVIII(self, stones):
        n = len(stones)
        pref = list(accumulate(stones))

        @lru_cache(None)
        def dp(i):                     # best score diff for suffix starting at i
            if i >= n - 1:             # 0 or 1 stone left → no move
                return 0
            best = -float('inf')
            # j is the last index of the taken prefix (inclusive)
            for j in range(i + 1, n):
                taken = pref[j] - (pref[i - 1] if i > 0 else 0)
                best = max(best, taken - dp(j + 1))
            return best

        return dp(0)
```

**Complexity Analysis**

- Time complexity: `O(n^2)` – we iterate over all pairs `(i, j)` with `i < j`.  
- Space complexity: `O(n)` – memoization table + recursion stack.

---

### Approach 3: Bottom‑Up O(1) Space DP (Optimal)

**Intuition**  
Observe that the recurrence  

```
dp[i] = max_{j>i} ( prefix[j] - dp[j+1] )
```

only needs the *best* value of `prefix[j] - dp[j+1]` for any `j > i`.  
If we scan `i` from right to left we can keep a running maximum of that expression, eliminating the inner loop.

Let `best` denote `max_{j>i} ( prefix[j] - dp[j+1] )`.  
When we move from `i+1` to `i`, the candidate `j = i+1` becomes available, so we update:

```
best = max(best, prefix[i+1] - dp[i+2])
dp[i] = best
```

Since `dp[i+2]` is just the previously computed `best` from two steps ahead, we can keep only two variables. The given solution does exactly this, ending with a single variable `ans`.

**Algorithm**

1. Compute prefix sums `pref`.
2. Initialise `ans = pref[-1]` (taking the whole array in the first move).
3. Iterate `i` from `n-2` down to `1`:
   - `ans = max(ans, pref[i] - ans)`
4. Return `ans`.

**Implementation**

```python
from itertools import accumulate

class Solution:
    def stoneGameVIII(self, stones):
        pref = list(accumulate(stones))
        ans = pref[-1]                     # take all stones at once
        for i in range(len(stones) - 2, 0, -1):
            ans = max(ans, pref[i] - ans) # either stop here or continue
        return ans
```

**Complexity Analysis**

- Time complexity: `O(n)` – one pass for prefix sums, one pass for the DP loop.  
- Space complexity: `O(1)` extra (apart from the input‑derived prefix array, which can be reused in‑place if desired).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the optimal O(1) solution on the first example:

**Input:** `stones = [-1, 2, -3, 4, -5]`

Prefix sums: `[-1, 1, -2, 2, -3]`

We iterate from right to left, keeping `ans`.

| Step | i | pref[i] | ans before | ans = max(ans, pref[i] - ans) | ans after |
|------|---|---------|------------|------------------------------|-----------|
| init | – | –       | –          | `ans = pref[-1] = -3`        | -3        |
| 1    | 3 | 2       | -3         | max(-3, 2 - (-3) = 5)        | 5         |
| 2    | 2 | -2      | 5          | max(5, -2 - 5 = -7)          | 5         |
| 3    | 1 | 1       | 5          | max(5, 1 - 5 = -4)           | 5         |
| 4    | 0 | -1      | (stop – loop ends at i>0) | – | 5 |

Final `ans = 5`, which matches the expected output (Alice’s score minus Bob’s).

---

**Summary**  
We explored the problem from exponential brute force to a linear‑time, constant‑space DP solution by recognizing the optimal substructure and using prefix sums to collapse the state. This exemplifies how 1‑D DP turns a seemingly complex game into a simple right‑to‑left scan.