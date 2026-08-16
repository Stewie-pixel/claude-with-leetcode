# Math & Geometry

## Video Solution

For more details about **Stone Game IX**, watch the walkthrough at [https://www.youtube.com/watch?v=7r34JD5ud-c](https://www.youtube.com/watch?v=7r34JD5ud-c)

## Concept

Stone Game IX is a turn‑based game where each move removes a stone and the loser is the player who makes the total sum of removed stones divisible by 3.  
Only the remainder of each stone’s value modulo 3 matters because adding 3 does not change divisibility by 3.  
Thus we reduce every stone to one of three classes:  

* `cnt[0]` – stones ≡ 0 (mod 3)  
* `cnt[1]` – stones ≡ 1 (mod 3)  
* `cnt[2]` – stones ≡ 2 (mod 3)

The game then becomes a question of how these counts interact with the parity of turns. By analyzing who is forced to pick a stone that makes the running sum ≡ 0 (mod 3), we can derive a simple win condition.

## When to Use It

Use this modulo‑counting technique when you see:

* A game where players alternately pick items.
* The losing condition depends on a **sum modulo a small constant** (here 3).
* The actual values are large but only their remainder matters.
* The solution reduces to counting how many items fall into each remainder class.

## Template

```python
def solve_mod_game(values, mod):
    # Count how many numbers give each remainder
    cnt = [0] * mod
    for v in values:
        cnt[v % mod] += 1

    # ---- Insert game‑specific logic here ----
    # Example placeholder: return True if first player wins
    return False
```

Replace the placeholder with the reasoning derived from the particular modulo condition.

---

# LeetCode Problem Walkthrough

### Problem: 2029. Stone Game IX  
https://leetcode.com/problems/stone-game-ix/

## Approach 1: Brute Force (Backtracking)

**Algorithm**  
Try every possible order of removals using recursion/backtracking.  
At each step keep the current sum modulo 3 and whose turn it is.  
If a player makes the sum ≡ 0 (mod 3) they lose immediately.  
If all stones are removed without hitting a losing sum, Bob wins by rule.

**Implementation**

```python
from functools import lru_cache

class Solution:
    def stoneGameIX(self, stones):
        n = len(stones)

        @lru_cache(None)
        def dfs(mask, turn, cur_mod):
            # mask: bitset of remaining stones
            # turn: 0 = Alice, 1 = Bob
            # cur_mod: sum of removed stones % 3
            if mask == 0:               # no stones left → Bob wins
                return turn == 0        # Alice loses if it's her turn
            # try removing each remaining stone
            for i in range(n):
                if mask & (1 << i):
                    nxt = (cur_mod + stones[i]) % 3
                    # if this move makes sum %3 == 0, current player loses
                    if nxt == 0:
                        # current player loses → opponent wins
                        return turn == 1   # Alice wins only if Bob just lost
                    # otherwise continue
                    if not dfs(mask ^ (1 << i), turn ^ 1, nxt):
                        return True      # found a winning move
            return False                 # no winning move

        full_mask = (1 << n) - 1
        return dfs(full_mask, 0, 0)      # start with Alice's turn
```

**Complexity Analysis**

- Time complexity: O(2ⁿ · n) — each state (mask, turn, cur_mod) is visited once, and we iterate over up to n choices.
- Space complexity: O(2ⁿ · 2 · 3) for memoization → O(2ⁿ).

*This approach is exponential and only works for tiny n (≤ 15).*

## Approach 2: DP on Counts (Intermediate Optimization)

**Intuition**  
Only the remainders modulo 3 matter, and the order of identical‑remainder stones does not affect the outcome.  
We can therefore DP on the triple `(c0, c1, c2, turn, cur_mod)` where `ci` is the count of remaining stones with remainder i.  
The state space is bounded by the counts (≤ n) and is far smaller than 2ⁿ.

**Algorithm**  
Use memoized recursion on the counts.  
Base case: if all counts are zero → Bob wins (return `turn == 0`).  
For each remainder class with a positive count, simulate removing one stone:  

* new sum modulo 3 = `(cur_mod + r) % 3`  
* if the new sum ≡ 0 → current player loses → opponent wins.  
* otherwise recurse with the count of that class decreased and turn flipped.

If any move leads to a state where the opponent loses, the current player can win.

**Implementation**

```python
from functools import lru_cache

class Solution:
    def stoneGameIX(self, stones):
        cnt = [0, 0, 0]
        for x in stones:
            cnt[x % 3] += 1
        c0, c1, c2 = cnt

        @lru_cache(None)
        def win(c0, c1, c2, turn, cur):
            # turn: 0 = Alice, 1 = Bob
            if c0 == c1 == c2 == 0:
                return turn == 0          # Bob wins → Alice loses if it's her turn
            # try removing a stone of each remainder
            for r, (nc0, nc1, nc2) in enumerate([
                (c0-1, c1, c2),   # r = 0
                (c0, c1-1, c2),   # r = 1
                (c0, c1, c2-1)    # r = 2
            ]):
                if r == 0 and c0 == 0: continue
                if r == 1 and c1 == 0: continue
                if r == 2 and c2 == 0: continue
                nxt = (cur + r) % 3
                if nxt == 0:          # current player loses
                    return turn == 1   # Alice wins only if Bob just lost
                if not win(nc0, nc1, nc2, turn ^ 1, nxt):
                    return True
            return False

        return win(c0, c1, c2, 0, 0)
```

**Complexity Analysis**

- Time complexity: O(c0 · c1 · c2 · 2 · 3) ≤ O(n³) in the worst case, but because each count ≤ n and the product is limited by the total n, it is effectively O(n²) for typical constraints; with n ≤ 10⁵ this is still too large, yet it shows the principle.
- Space complexity: O(c0 · c1 · c2 · 2 · 3) for memoization.

*While better than brute force, this is still not fast enough for the maximum input size, prompting the final O(1) solution.*

## Approach 3: Optimal O(1) Counting Solution

**Intuition**  
After reducing to remainder counts, the game outcome depends only on:

* The parity of `cnt[0]` (how many zero‑remainder stones there are).  
* The difference between `cnt[1]` and `cnt[2]`.

Zero‑remainder stones do not change the running sum modulo 3, so they merely **skip turns**.  
If there is an even number of them, the turn order after all zeros are exhausted is the same as the start (Alice to move).  
If odd, the turn order flips (Bob to move after zeros are gone).

The non‑zero stones shift the sum by ±1 each move.  
Alice can force a win unless the non‑zero stones are so imbalanced that the player who must move when the sum is about to hit a multiple of 3 has no safe move.

The derived condition is:

* If `cnt[0]` is even → Alice wins iff both `cnt[1]` and `cnt[2]` are > 0 (she can start with a 1 and Bob must later face a 2, etc.).
* If `cnt[0]` is odd → Alice wins iff `abs(cnt[1] - cnt[2]) >= 3` (she has enough excess of one residue to survive the turn flip).

**Implementation**

```python
class Solution:
    def stoneGameIX(self, stones):
        cnt = [0, 0, 0]
        for x in stones:
            cnt[x % 3] += 1
        c0, c1, c2 = cnt

        if c0 % 2 == 0:
            # even number of zeros → turn order unchanged
            return c1 > 0 and c2 > 0
        else:
            # odd number of zeros → turn order flips
            return abs(c1 - c2) >= 3
```

**Complexity Analysis**

- Time complexity: O(n) — one pass to compute the three counts.
- Space complexity: O(1) — only a few integer variables.

---

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

### Dry Run

We trace the optimal solution on the example `stones = [5,1,2,4,3]`.

| Step | Stone value | value % 3 | Running sum % 3 before move | Action                               | Running sum % 3 after |
|------|-------------|----------|-----------------------------|--------------------------------------|-----------------------|
| Start| –           | –        | 0                           | –                                    | 0                     |
| 1    | 1 (Alice)   | 1        | 0                           | Alice removes 1                      | (0+1)%3 = 1           |
| 2    | 3 (Bob)     | 0        | 1                           | Bob removes 3 (zero)                 | (1+0)%3 = 1           |
| 3    | 4 (Alice)   | 1        | 1                           | Alice removes 4                      | (1+1)%3 = 2           |
| 4    | 2 (Bob)     | 2        | 2                           | Bob removes 2 → sum becomes 4 → (2+2)%3 = 1? Wait miscalc → actually (2+2)%3 = 1 (not zero) – but in the example Bob wins later. |
| 5    | 5 (Alice)   | 2        | 1                           | Alice removes 5 → (1+2)%3 = 0 → Alice loses. |

The table shows that after Alice’s final move the sum becomes divisible by 3, so she loses. The counting solution predicts this:

* `cnt[0] = 1` (only the stone `3`), `cnt[1] = 1` (stone `1`), `cnt[2] = 3` (stones `5,2,4` → actually 5%3=2, 2%3=2, 4%3=1? Let's recompute: 5%3=2, 1%3=1, 2%3=2, 4%3=1, 3%3=0 → `cnt[0]=1, cnt[1]=2, cnt[2]=2`).  
* `c0` is odd → check `abs(c1-c2) = |2-2| = 0 < 3` → Alice **cannot** win → returns `False`.  

This matches the traced outcome.

---