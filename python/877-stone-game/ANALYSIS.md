# 2D Dynamic Programming

## Video Solution

For more details about **Stone Game**, watch the walkthrough at [https://www.youtube.com/watch?v=8jvt2vw7j_Y](https://www.youtube.com/watch?v=8jvt2vw7j_Y).

## Concept

Stone Game is a two‑player turn‑based game where each player can take a pile from either the left or the right end of the row. Both players play optimally, meaning each tries to maximize **their own total stones** (or equivalently, maximize the difference between their score and the opponent's).  

Because the total number of stones is odd, there are no ties – the player with the higher total wins.  
We can model the game as a zero‑sum contest: on each turn the current player gains the value of the pile they pick, while the opponent’s future gain is subtracted from the current player’s perspective.  
Thus we define `dp[i][j]` as the **maximum net advantage** (current player’s stones minus opponent’s stones) the player whose turn it is can achieve from the sub‑array `piles[i..j]`.  

The recurrence follows directly from the two choices:

* Take the left pile → gain `piles[i]`, then the opponent faces sub‑array `[i+1..j]` and can achieve a net advantage of `dp[i+1][j]` from *their* perspective. From the current player’s view this contributes `-dp[i+1][j]`.
* Take the right pile → gain `piles[j]`, then the opponent faces `[i..j-1]` → contributes `-dp[i][j-1]`.

Hence  

```
dp[i][j] = max(piles[i] - dp[i+1][j], piles[j] - dp[i][j-1])
```

The base case is a single pile: `dp[i][i] = piles[i]` (the current player takes it and the opponent gets nothing).  
Alice starts first, so she wins iff `dp[0][n-1] > 0`.

## When to Use It

Use this interval‑DP pattern when you see:

* A row/array of items and two players alternately taking from the **ends**.
* Both players play **optimally** (maximizing their own score).
* The goal is to decide who wins or the maximum score difference.
* The total sum has a known parity (odd/even) that guarantees no ties.

Typical clues: “take from beginning or end”, “optimal play”, “return true if first player wins”, even number of elements, odd total sum.

## Template

```python
def stone_game(piles):
    n = len(piles)
    # dp[i][j] = max net advantage for the player whose turn it is on piles[i..j]
    dp = [[0] * n for _ in range(n)]

    # base case: one pile
    for i in range(n):
        dp[i][i] = piles[i]

    # fill table for increasing lengths
    for length in range(2, n + 1):          # current sub‑array length
        for i in range(n - length + 1):
            j = i + length - 1
            dp[i][j] = max(piles[i] - dp[i+1][j],
                           piles[j] - dp[i][j-1])

    # Alice's net advantage > 0 means she wins
    return dp[0][n-1] > 0
```

## LeetCode Problem Walkthrough

### Problem: 877. Stone Game  
https://leetcode.com/problems/stone-game/

### Approach 1: Brute Force (Recursion)

**Algorithm**  
Define a recursive function `score(i, j)` that returns the maximum net advantage the current player can obtain from `piles[i..j]`.  
* If `i == j`: return `piles[i]`.  
* Otherwise, the player can choose left or right:  
  * left  → `piles[i] - score(i+1, j)`  
  * right → `piles[j] - score(i, j-1)`  
Return the maximum of the two choices.  
Alice wins if `score(0, n-1) > 0`.

**Implementation**

```python
from functools import lru_cache

class Solution:
    def stoneGame(self, piles):
        n = len(piles)

        @lru_cache(None)
        def score(i, j):
            if i == j:
                return piles[i]
            left  = piles[i] - score(i+1, j)
            right = piles[j] - score(i, j-1)
            return max(left, right)

        return score(0, n-1) > 0
```

**Complexity Analysis**  
- Time complexity: O(2ⁿ) without memoization – each call branches into two, leading to exponential calls.  
- Space complexity: O(n) recursion depth (call stack) → O(n).  
*With `lru_cache` the time drops to O(n²) because each `(i,j)` pair is computed once; space becomes O(n²) for the cache.*  
We present the raw recursion to illustrate the exponential baseline; the memoized version is Approach 2.

### Approach 2: Memoized Top‑Down DP

**Intuition**  
The recursive solution recomputes the same sub‑problems many times (overlapping sub‑problems). By caching results for each interval `[i, j]` we turn the exponential recursion into a polynomial DP.

**Algorithm**  
Same recurrence as above, but we store results in a 2‑D table `memo[i][j]` (or use `lru_cache`). Fill entries on demand.

**Implementation**

```python
class Solution:
    def stoneGame(self, piles):
        n = len(piles)
        memo = [[None] * n for _ in range(n)]

        def score(i, j):
            if i == j:
                return piles[i]
            if memo[i][j] is not None:
                return memo[i][j]
            left  = piles[i] - score(i+1, j)
            right = piles[j] - score(i, j-1)
            memo[i][j] = max(left, right)
            return memo[i][j]

        return score(0, n-1) > 0
```

**Complexity Analysis**  
- Time complexity: O(n²) – each of the n² intervals is solved once.  
- Space complexity: O(n²) – the memo table plus recursion stack O(n).

### Approach 3: Bottom‑Up DP (Iterative)

**Intuition**  
Instead of recursion we fill the DP table iteratively from smaller intervals to larger ones. This removes recursion overhead and makes the order of evaluation explicit.

**Algorithm**  
* Initialize `dp[i][i] = piles[i]`.  
* For each possible length `len` from 2 to n:  
  * For each start index `i`, compute `j = i + len - 1`.  
  * Apply the recurrence `dp[i][j] = max(piles[i] - dp[i+1][j], piles[j] - dp[i][j-1])`.  
* The answer is `dp[0][n-1] > 0`.

**Implementation** (the code you already wrote)

```python
class Solution:
    def stoneGame(self, piles):
        n = len(piles)
        dp = [[0] * n for _ in range(n)]

        for i in range(n):
            dp[i][i] = piles[i]

        for length in range(2, n + 1):
            for i in range(n - length + 1):
                j = i + length - 1
                dp[i][j] = max(piles[i] - dp[i+1][j],
                               piles[j] - dp[i][j-1])

        return dp[0][n-1] > 0
```

**Complexity Analysis**  
- Time complexity: O(n²) – two nested loops over lengths and start indices.  
- Space complexity: O(n²) – the DP table.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the bottom‑up DP on the example `piles = [5, 3, 4, 5]` (n = 4).

#### Dry Run

| Step | i | j | length | piles[i] | piles[j] | dp[i+1][j] | dp[i][j-1] | dp[i][j] = max(piles[i] - dp[i+1][j], piles[j] - dp[i][j-1]) |
|------|---|---|--------|----------|----------|------------|------------|---------------------------------------------------------------|
| Init | 0 | 0 | 1      | 5        | 5        | –          | –          | **5** (base) |
| Init | 1 | 1 | 1      | 3        | 3        | –          | –          | **3** |
| Init | 2 | 2 | 1      | 4        | 4        | –          | –          | **4** |
| Init | 3 | 3 | 1      | 5        | 5        | –          | –          | **5** |
| L=2  | 0 | 1 | 2      | 5        | 3        | dp[1][1]=3 | dp[0][0]=5 | max(5‑3=2, 3‑5=-2) → **2** |
| L=2  | 1 | 2 | 2      | 3        | 4        | dp[2][2]=4 | dp[1][1]=3 | max(3‑4=-1, 4‑3=1) → **1** |
| L=2  | 2 | 3 | 2      | 4        | 5        | dp[3][3]=5 | dp[2][2]=4 | max(4‑5=-1, 5‑4=1) → **1** |
| L=3  | 0 | 2 | 3      | 5        | 4        | dp[1][2]=1 | dp[0][1]=2 | max(5‑1=4, 4‑2=2) → **4** |
| L=3  | 1 | 3 | 3      | 3        | 5        | dp[2][3]=1 | dp[1][2]=1 | max(3‑1=2, 5‑1=4) → **4** |
| L=4  | 0 | 3 | 4      | 5        | 5        | dp[1][3]=4 | dp[0][2]=4 | max(5‑4=1, 5‑4=1) → **1** |

Final `dp[0][3] = 1 > 0` → Alice’s net advantage is 1 stone, so she wins → return **true**.

The table shows how each interval’s optimal net advantage builds from smaller intervals, confirming the correctness of the bottom‑up DP.