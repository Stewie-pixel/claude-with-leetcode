# Math & Geometry

## Video Solution

For more details about **Nim Game**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=I2oKHaimKxE).

## Concept

In the Nim Game each player can remove 1, 2, or 3 stones from a single pile. The player who takes the last stone wins.  
Because both players play optimally, the outcome depends only on whether the starting pile size is a *losing* position.  
A losing position is one where **no matter how many stones you take (1‑3), you leave the opponent in a winning position**.  
By analyzing small values we see a pattern: pile sizes that are multiples of 4 are losing for the player whose turn it is.  
Thus the first player wins iff `n % 4 != 0`.

## When to Use It

Use this reasoning when you encounter:
- Turn‑based take‑away games with a fixed maximum removal per turn (e.g., 1‑k stones).
- Both players play optimally and you only need to know *who* wins, not the exact moves.
- The solution reduces to a simple modulo or periodic pattern after computing a few base cases.

## Template

A common way to solve impartial combinatorial games is dynamic programming where `dp[i]` tells if the player to move with `i` stones can force a win.

```python
def can_win_nim(n: int, max_take: int = 3) -> bool:
    """
    DP template for take‑away games.
    dp[i] = True if the current player can win with i stones.
    Transition: dp[i] = any(not dp[i - move]) for move in 1..max_take if i >= move
    Base case: dp[0] = False (no stones → current player loses)
    """
    dp = [False] * (n + 1)          # dp[0] already False
    for i in range(1, n + 1):
        # try every legal move
        for move in range(1, max_take + 1):
            if i - move >= 0 and not dp[i - move]:
                dp[i] = True
                break               # found a winning move
    return dp[n]
```

For Nim Game we can specialize `max_take = 3`.

## LeetCode Problem Walkthrough

### Problem: 292. Nim Game

https://leetcode.com/problems/nim-game/

### Approach 1: Brute Force (Recursive Simulation)

**Algorithm**
- Recursively simulate both players’ moves.
- The function `win(stones)` returns `True` if the player to move can force a win.
- Try removing 1, 2, or 3 stones; if any move leaves the opponent in a losing position (`win(stones - move) == False`), the current player wins.
- Memoize results to avoid exponential recomputation.

**Implementation**

```python
from functools import lru_cache

class Solution:
    def canWinNim(self, n: int) -> bool:
        @lru_cache(maxsize=None)
        def win(stones: int) -> bool:
            if stones == 0:
                return False                     # no stones → lose
            # try each possible removal
            for take in (1, 2, 3):
                if stones - take >= 0 and not win(stones - take):
                    return True                  # found a move that makes opponent lose
            return False                         # all moves lead to opponent win
        return win(n)
```

**Complexity Analysis**
- Time complexity: O(n) — each state `stones` is computed once due to memoization, and each state checks up to 3 moves.
- Space complexity: O(n) — recursion stack + memoization table of size n+1.

### Approach 2: Dynamic Programming (Iterative)

**Intuition**
- The recursive solution shows that the outcome for `i` stones depends only on the outcomes for `i-1`, `i-2`, and `i-3`.
- We can fill a DP table bottom‑up, which eliminates recursion overhead and is easier to reason about.

**Algorithm**
- Initialize `dp[0] = False` (losing).
- For each `i` from 1 to `n`, set `dp[i] = True` if any of `dp[i-1]`, `dp[i-2]`, `dp[i-3]` is `False` (i.e., there exists a move that leaves the opponent in a losing state).
- Return `dp[n]`.

**Implementation**

```python
class Solution:
    def canWinNim(self, n: int) -> bool:
        # dp[i] = True if the player to move with i stones can win
        dp = [False] * (n + 1)   # dp[0] = False by definition
        for i in range(1, n + 1):
            # check the three possible previous states
            if (i - 1 >= 0 and not dp[i - 1]) or \
               (i - 2 >= 0 and not dp[i - 2]) or \
               (i - 3 >= 0 and not dp[i - 3]):
                dp[i] = True
            # else dp[i] stays False
        return dp[n]
```

**Complexity Analysis**
- Time complexity: O(n) — one pass over 1…n, constant work per i.
- Space complexity: O(n) — the DP table. (Can be reduced to O(1) with sliding window, but kept simple for clarity.)

### Approach 3: Mathematical Optimization (Optimal)

**Intuition**
- By computing the first few values manually we observe a repeating pattern:
  - n = 1 → win (True)
  - n = 2 → win (True)
  - n = 3 → win (True)
  - n = 4 → lose (False)
  - n = 5 → win (True)  (take 1, leave 4)
  - n = 6 → win (True)  (take 2, leave 4)
  - n = 7 → win (True)  (take 3, leave 4)
  - n = 8 → lose (False) (any move leaves 5‑7, all winning for opponent)
- The pattern repeats every 4 stones: losing positions are exactly multiples of 4.
- Hence the first player wins iff `n` is **not** divisible by 4.

**Algorithm**
- Return `n % 4 != 0`.

**Implementation**

```python
class Solution:
    def canWinNim(self, n: int) -> bool:
        return n % 4 != 0
```

**Complexity Analysis**
- Time complexity: O(1) — single modulo operation.
- Space complexity: O(1) — only a few variables.

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the DP / mathematical solution for `n = 6`.

### Dry Run

| n | n % 4 | Win? (True/False) | Reasoning |
|---|-------|-------------------|-----------|
| 0 | 0     | False             | base case – no stones |
| 1 | 1     | True              | 1 % 4 ≠ 0 → can take 1 and win |
| 2 | 2     | True              | 2 % 4 ≠ 0 → can take 2 and win |
| 3 | 3     | True              | 3 % 4 ≠ 0 → can take 3 and win |
| 4 | 0     | False             | any move leaves 1‑3 (all winning for opponent) |
| 5 | 1     | True              | take 1 → leave 4 (losing for opponent) |
| 6 | 2     | True              | take 2 → leave 4 (losing for opponent) |

Thus for `n = 6` the function returns `True`, meaning the first player can force a win.