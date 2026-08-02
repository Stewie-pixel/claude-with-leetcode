# 2D Dynamic Programming

## Video Solution

For more details about **Stone Game**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=uhgdXOlGYqE).

## Concept

2D Dynamic Programming (interval DP) solves problems where the state depends on a contiguous sub‑array or sub‑string defined by two indices `i` (start) and `j` (end).  
We fill a table `dp[i][j]` that stores the optimal result for the sub‑problem `piles[i..j]`.  
The table is built increasing by length so that when we compute `dp[i][j]`, the needed smaller sub‑problems (`dp[i+1][j]`, `dp[i][j-1]`, etc.) are already known.

In Stone Game we let `dp[i][j]` be the **maximum net score difference** (current player’s score minus opponent’s score) that the player whose turn it is can achieve from the sub‑array `piles[i..j]`.  
If it is Alice’s turn, she wants to maximize this difference; if it is Bob’s turn, he wants to minimize it (equivalently, Alice wants to maximize the negative of Bob’s gain).  
By checking the parity of the remaining moves we know whose turn it is.

## When to Use It

Use interval DP when you see:
- A game where players take from the **ends** of a line/array.
- The goal is to compute the best possible outcome assuming **optimal play**.
- The problem can be broken into smaller **contiguous** sub‑problems.
- The total number of elements is moderate (≤ 500) so an O(n²) table fits.

## Template

```python
def interval_dp(piles):
    n = len(piles)
    # dp[i][j] = net score difference (current player - opponent) for piles[i..j]
    dp = [[0] * n for _ in range(n)]

    # consider sub‑arrays of increasing length
    for length in range(1, n + 1):
        for i in range(n - length + 1):
            j = i + length - 1
            # whose turn? (parity of remaining moves)
            # remaining piles = length, total piles = n
            # if (n - length) is even -> same player as start (Alice), else Bob
            # we can compute turn parity as (n - length) % 2
            if (n - length) % 2 == 0:          # Alice's turn -> maximize
                dp[i][j] = max(
                    piles[i] + dp[i + 1][j],   # take left
                    piles[j] + dp[i][j - 1]    # take right
                )
            else:                              # Bob's turn -> minimize Alice's net gain
                dp[i][j] = min(
                    -piles[i] + dp[i + 1][j],  # Bob takes left -> subtract his gain
                    -piles[j] + dp[i][j - 1]   # Bob takes right
                )
    # Alice wins if her net gain > 0
    return dp[0][n - 1] > 0
```

## LeetCode Problem Walkthrough

### Problem: 877. Stone Game

https://leetcode.com/problems/stone-game/

### Approach 1: Brute Force (Recursion)

**Algorithm**
- Recursively simulate both choices (take left or take right).
- Return `True` if the current player can force a win.
- Base case: no piles left → compare scores.

**Implementation**

```python
from functools import lru_cache

class Solution:
    def stoneGame(self, piles):
        @lru_cache(None)
        def win(i, j, alice_turn):
            if i > j:
                return 0   # no stones left, net gain = 0
            if alice_turn:
                # Alice tries to maximize her net gain
                return max(
                    piles[i] + win(i + 1, j, False),
                    piles[j] + win(i, j - 1, False)
                )
            else:
                # Bob tries to minimize Alice's net gain (maximize his own)
                return min(
                    -piles[i] + win(i + 1, j, True),
                    -piles[j] + win(i, j - 1, True)
                )
        return win(0, len(piles) - 1, True) > 0
```

**Complexity Analysis**
- Time complexity: O(2ⁿ) without memoization — each call branches into two.
- Space complexity: O(n) recursion depth (call stack) plus O(2ⁿ) for memoization if added.

### Approach 2: Memoized Top‑Down DP

**Intuition**
The recursive solution repeats the same sub‑problems many times. By caching results for each `(i, j, turn)` we avoid recomputation, turning the exponential recursion into a polynomial solution.

**Algorithm**
- Same recursion as above, but store results in a dictionary or `lru_cache`.
- The state includes whose turn it is; however we can drop the turn dimension by encoding it via parity of remaining moves (as in the bottom‑up version).

**Implementation**

```python
class Solution:
    def stoneGame(self, piles):
        n = len(piles)
        from functools import lru_cache

        @lru_cache(None)
        def dp(i, j):
            # net score difference for the player whose turn it is
            if i > j:
                return 0
            # whose turn? (parity of remaining piles)
            if (n - (j - i + 1)) % 2 == 0:   # Alice's turn
                return max(piles[i] + dp(i + 1, j),
                           piles[j] + dp(i, j - 1))
            else:                            # Bob's turn
                return min(-piles[i] + dp(i + 1, j),
                           -piles[j] + dp(i, j - 1))

        return dp(0, n - 1) > 0
```

**Complexity Analysis**
- Time complexity: O(n²) — each pair `(i, j)` computed once.
- Space complexity: O(n²) for the memo table + O(n) recursion stack.

### Approach 3: Bottom‑Up DP (Iterative)

**Intuition**
Instead of recursion we fill the DP table iteratively by increasing sub‑array length.  
The turn parity can be derived from how many elements have already been removed:  
`(n - length) % 2 == 0` → Alice's turn (maximize), else Bob's turn (minimize).  
This removes the need for an explicit turn dimension and yields a clean O(n²) solution.

**Algorithm**
1. Create `dp[n][n]` initialized to 0.
2. For each `length` from 1 to `n`:
   - For each start `i`, compute `j = i + length - 1`.
   - Determine whose turn it is via parity.
   - If Alice's turn: `dp[i][j] = max(piles[i] + dp[i+1][j], piles[j] + dp[i][j-1])`.
   - If Bob's turn: `dp[i][j] = min(-piles[i] + dp[i+1][j], -piles[j] + dp[i][j-1])`.
3. Alice wins if `dp[0][n-1] > 0`.

**Implementation**

```python
class Solution:
    def stoneGame(self, piles):
        n = len(piles)
        dp = [[0] * n for _ in range(n)]

        for length in range(1, n + 1):
            for i in range(n - length + 1):
                j = i + length - 1
                # whose turn? (parity of remaining piles)
                if (n - length) % 2 == 0:          # Alice's turn -> maximize
                    dp[i][j] = max(
                        piles[i] + dp[i + 1][j],
                        piles[j] + dp[i][j - 1]
                    )
                else:                              # Bob's turn -> minimize
                    dp[i][j] = min(
                        -piles[i] + dp[i + 1][j],
                        -piles[j] + dp[i][j - 1]
                    )
        return dp[0][n - 1] > 0
```

**Complexity Analysis**
- Time complexity: O(n²) — two nested loops over `length` and `i`.
- Space complexity: O(n²) — the DP table.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the bottom‑up DP on the example `piles = [5, 3, 4, 5]` (n = 4).

| length | i | j | sub‑array | turn (len parity) | dp[i][j] calculation | dp[i][j] |
|--------|---|---|-----------|-------------------|----------------------|----------|
| 1      | 0 | 0 | [5]       | (4‑1)%2=1 → Bob   | min(-5+0, -5+0) = -5 | -5 |
| 1      | 1 | 1 | [3]       | Bob               | -3                   | -3 |
| 1      | 2 | 2 | [4]       | Bob               | -4                   | -4 |
| 1      | 3 | 3 | [5]       | Bob               | -5                   | -5 |
| 2      | 0 | 1 | [5,3]     | (4‑2)%2=0 → Alice | max(5+dp[1][1], 3+dp[0][0]) = max(5-3, 3-5) = 2 | 2 |
| 2      | 1 | 2 | [3,4]     | Alice             | max(3+dp[2][2], 4+dp[1][1]) = max(3-4, 4-3) = 1 | 1 |
| 2      | 2 | 3 | [4,5]     | Alice             | max(4+dp[3][3], 5+dp[2][2]) = max(4-5, 5-4) = 1 | 1 |
| 3      | 0 | 2 | [5,3,4]   | (4‑3)%2=1 → Bob   | min(-5+dp[1][2], -4+dp[0][1]) = min(-5+1, -4+2) = min(-4, -2) = -4 | -4 |
| 3      | 1 | 3 | [3,4,5]   | Bob               | min(-3+dp[2][3], -5+dp[1][2]) = min(-3+1, -5+1) = min(-2, -4) = -4 | -4 |
| 4      | 0 | 3 | [5,3,4,5] | (4‑4)%2=0 → Alice | max(5+dp[1][3], 5+dp[0][2]) = max(5-4, 5-4) = 1 | 1 |

Final `dp[0][3] = 1 > 0`, so Alice wins → **true**.

--- 

*End of lecture.*