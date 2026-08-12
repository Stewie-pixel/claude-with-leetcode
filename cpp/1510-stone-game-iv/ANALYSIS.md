# 1D Dynamic Programming

## Video Solution

For more details about **Stone Game IV**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=8eF-3SoczxA).

## Concept

Stone Game IV is a two‑player turn‑based game where each player can remove any positive square number of stones. The game ends when a player cannot move (i.e., zero stones remain) and that player loses.  
We want to know whether the first player (Alice) has a winning strategy assuming optimal play.

This is a classic **impartial combinatorial game** that can be solved with DP by defining the state `dp[i]` = *True* if the player whose turn it is with `i` stones can force a win, otherwise *False*.  
The transition looks at all possible moves (removing `k²` stones) and checks if any move leaves the opponent in a losing state.

## When to Use It

Use 1D DP when:

* The problem involves a single integer parameter that defines the game/state (e.g., remaining stones, current index, sum).
* Transitions depend only on smaller values of that parameter.
* You need to decide win/lose, min/max, or count possibilities for each prefix/sub‑problem.
* The recurrence can be expressed as `dp[i] = f(dp[i - move])` where `move` ranges over a set of allowed actions.

## Template

```python
def solve(n):
    # dp[i] = True if the player to move with i stones can win
    dp = [False] * (n + 1)   # dp[0] = False  (no stones → lose)

    for i in range(1, n + 1):
        # try every square number <= i
        k = 1
        while k * k <= i:
            if not dp[i - k * k]:      # opponent loses after this move
                dp[i] = True
                break
            k += 1
    return dp[n]
```

## LeetCode Problem Walkthrough

### Problem: 1510. Stone Game IV

https://leetcode.com/problems/stone-game-iv/

### Approach 1: Brute Force (Recursion without memo)

**Algorithm**  
Define a recursive function `win(stones)` that returns *True* if the current player can win from `stones`.  
For each square `s ≤ stones`, recursively call `win(stones - s)`. If any recursive call returns *False* (meaning the opponent loses), the current player wins.  
Base case: `stones == 0` → *False* (no move → lose).  
No memoization → exponential time.

**Implementation**

```python
class Solution:
    def winnerSquareGame(self, n: int) -> bool:
        def can_win(stones: int) -> bool:
            if stones == 0:
                return False
            k = 1
            while k * k <= stones:
                if not can_win(stones - k * k):
                    return True
                k += 1
            return False
        return can_win(n)
```

**Complexity Analysis**

- Time complexity: O(ⁿ) – each state branches into up to √n subcalls, leading to exponential blow‑up.
- Space complexity: O(n) – recursion depth at most n (call stack).

### Approach 2: Top‑Down DP (Memoization)

**Intuition**  
The brute‑force recursion recomputes the same sub‑problems many times. By caching the result of `win(stones)` we turn the exponential recursion into a linear‑ish DP.

**Algorithm**  
Use a memo array/dict initialized with `-1` (unknown).  
`win(stones)` returns the cached value if known; otherwise computes it as in the brute force, stores the result, and returns it.

**Implementation**

```python
class Solution:
    def winnerSquareGame(self, n: int) -> bool:
        from functools import lru_cache

        @lru_cache(None)
        def win(stones: int) -> bool:
            if stones == 0:
                return False
            k = 1
            while k * k <= stones:
                if not win(stones - k * k):
                    return True
                k += 1
            return False

        return win(n)
```

**Complexity Analysis**

- Time complexity: O(n·√n) – each `stones` value (0…n) is processed once, and for each we iterate over all squares ≤ stones.
- Space complexity: O(n) – memoization table + recursion stack.

### Approach 3: Bottom‑Down DP (Iterative)

**Intuition**  
Instead of recursion we fill the DP table from small to large `i`.  
`dp[i]` depends only on `dp[i - k²]` where `k² < i`, which are already computed when we iterate `i` increasingly.

**Algorithm**  
Initialize `dp[0] = False`. For each `i` from 1 to n, try every square `k² ≤ i`. If any move leads to a losing state for the opponent (`dp[i - k²] == False`), set `dp[i] = True` and break (no need to check further).  
Finally return `dp[n]`.

**Implementation**

```python
class Solution:
    def winnerSquareGame(self, n: int) -> bool:
        dp = [False] * (n + 1)          # dp[0] = False already
        for i in range(1, n + 1):
            k = 1
            while k * k <= i:
                if not dp[i - k * k]:
                    dp[i] = True
                    break
                k += 1
        return dp[n]
```

**Complexity Analysis**

- Time complexity: O(n·√n) – outer loop n, inner loop up to √i.
- Space complexity: O(n) – the DP array.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Let's trace the algorithm for `n = 4`.

| i (stones) | k² tried | dp[i - k²] | dp[i] (win?) | Reason |
|------------|----------|------------|--------------|--------|
| 0          | –        | –          | False        | base case |
| 1          | 1        | dp[0] = False | True   | move to 0 → opponent loses |
| 2          | 1        | dp[1] = True  | False  | only move leaves opponent winning |
| 3          | 1        | dp[2] = False | True   | move 1 → opponent loses |
| 4          | 1        | dp[3] = True  | continue | |
|            | 4        | dp[0] = False | True   | move 4 → opponent loses |

Thus `dp[4] = True` → Alice wins, matching the example.

--- 

*This lecture follows the 1D Dynamic Programming pattern and can be reused for similar take‑away games.*