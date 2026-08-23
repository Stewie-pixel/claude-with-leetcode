# Math & Geometry

## Video Solution

For more details about **Sum Game**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=WnN8s9bI30s)

## Concept

The game reduces to a question of **who can control the final difference** between the sums of the two halves.  
Let  

* `leftSum`  – sum of known digits in the first half  
* `rightSum` – sum of known digits in the second half  
* `leftQ`    – number of '`?`' in the first half  
* `rightQ`   – number of '`?`' in the second half  

After all moves the total difference will be  

```
diff = (leftSum - rightSum) + (contribution from left ?) - (contribution from right ?)
```

Each '`?`' can contribute any digit 0‑9.  
Alice (who moves first) tries to make the final `diff ≠ 0`; Bob tries to make `diff = 0`.  
With optimal play the outcome depends only on the *known* difference and the *balance* of unknowns:

```
Alice wins  ⇔  (leftSum - rightSum) ≠ 9 * (rightQ - leftQ) / 2
```

If the equality holds, Bob can always mirror Alice’s moves and force the sums to match; otherwise Alice can break the symmetry.

## When to Use It

Use this reasoning when you see:

* A string split into two halves (or two groups)  
* Players alternately filling unknown positions with digits  
* Win condition based on equality of two sums  
* Need to decide the winner assuming optimal play  

Typical tags: “alternating moves”, “sum balance”, “game theory”, “greedy / math”.

## Template

A reusable outline for these turn‑based sum‑balancing games:

```python
def who_wins(num: str) -> bool:
    n = len(num)
    half = n // 2

    left_sum = right_sum = 0
    left_q = right_q = 0

    # 1️⃣  Gather known sums and count of '?'
    for i, ch in enumerate(num):
        if i < half:               # left half
            if ch == '?':
                left_q += 1
            else:
                left_sum += int(ch)
        else:                      # right half
            if ch == '?':
                right_q += 1
            else:
                right_sum += int(ch)

    # 2️⃣  Known difference (left - right)
    diff = left_sum - right_sum

    # 3️⃣  Optimal‑play condition
    #    Alice wins iff diff != 9 * (right_q - left_q) / 2
    return diff != 9 * (right_q - left_q) // 2
```

Replace the final condition with the appropriate formula for the specific variant.

---

## LeetCode Problem Walkthrough

### Problem: 1927. Sum Game

https://leetcode.com/problems/sum-game/

---

### Approach 1: Brute Force (Exponential)

**Algorithm**  
Try every possible way to replace each '`?`' with a digit 0‑9, simulate the game assuming both players play optimally (minimax), and see if Alice can force a win.

**Implementation**

```python
from functools import lru_cache

class Solution:
    def sumGame(self, num: str) -> bool:
        n = len(num)
        half = n // 2

        @lru_cache(None)
        def dfs(i: int, left: int, right: int, turn_alice: bool) -> bool:
            """Return True if the player to move can force a win for Alice."""
            if i == n:                     # no more '?'
                return left != right       # Alice wins iff sums differ

            ch = num[i]
            if ch != '?':                  # fixed digit, just add it
                val = int(ch)
                if i < half:
                    left += val
                else:
                    right += val
                return dfs(i + 1, left, right, not turn_alice)

            # ch == '?': try all digits 0‑9
            for d in range(10):
                nl, nr = left, right
                if i < half:
                    nl += d
                else:
                    nr += d
                # Next player's turn
                if dfs(i + 1, nl, nr, not turn_alice):
                    # If Alice is to move and she finds a winning branch,
                    # she can take it; if Bob is to move and he finds a
                    # branch that makes Alice lose, he will take it.
                    if turn_alice:
                        return True
                    else:
                        # Bob will avoid giving Alice a win if possible
                        continue
            # If we exhaust all digits without giving Alice a win,
            # the current player cannot force a win for Alice.
            return not turn_alice   # when turn is Bob, Alice loses

        # Start from index 0, both sums zero, Alice to move
        return dfs(0, 0, 0, True)
```

**Complexity Analysis**

- Time: `O(10^q)` where `q` is the number of '`?`' (each branch tries 10 digits) – infeasible for `q ≥ 10`.
- Space: `O(q)` recursion depth + memoization table (`O(q * possible_sum_range)`) – still exponential.

---

### Approach 2: Memoized Minimax on Difference & Counts

**Intuition**  
The exact values of the left and right sums matter only through their **difference** `diff = left - right`. Likewise, the future effect of the remaining '`?`' depends only on how many are left in each half. We can therefore memoize on `(pos, diff, left_q_rem, right_q_rem, turn)`.

**Algorithm**  

1. Scan the string once to split it into left/right halves, counting known sums and question marks.  
2. Define a recursive function `can_alice_win(pos, diff, lq, rq, alice_turn)` where  
   * `pos` – index in the original string (0 … n)  
   * `diff` – current `left_sum - right_sum` contributed by processed positions  
   * `lq`, `rq` – remaining '`?`' in left/right halves  
   * `alice_turn` – `True` if it is Alice’s move, else `False`  
3. Base case: when `pos == n` (no characters left) → Alice wins iff `diff != 0`.  
4. If the current character is a digit, update `diff` accordingly and recurse.  
5. If it is '`?`', try all digits 0‑9, updating `diff` and the appropriate counter, then recurse with the turn flipped.  
6. Return `True` if the current player (Alice when her turn, Bob when his) can force a win for Alice.

Because `diff` can range from `-9*q` to `+9*q`, the state space is manageable for `n ≤ 10^5` only when the number of '`?`' is small; still, this approach is far better than pure brute force.

**Implementation**

```python
class Solution:
    def sumGame(self, num: str) -> bool:
        n = len(num)
        half = n // 2

        # Pre‑compute known sums and count of '?' per half
        left_sum = right_sum = 0
        left_q = right_q = 0
        for i, ch in enumerate(num):
            if ch == '?':
                if i < half:
                    left_q += 1
                else:
                    right_q += 1
            else:
                val = int(ch)
                if i < half:
                    left_sum += val
                else:
                    right_sum += val

        from functools import lru_cache

        @lru_cache(None)
        def dfs(i: int, diff: int, lq: int, rq: int, alice_turn: bool) -> bool:
            if i == n:
                return diff != 0          # Alice wins iff sums differ

            ch = num[i]
            ni, nj, nk, nl = i + 1, diff, lq, rq   # defaults for digit case
            if ch != '?':
                val = int(ch)
                if i < half:
                    nj = diff + val          # left side increases diff
                else:
                    nj = diff - val          # right side decreases diff
                return dfs(ni, nj, nk, nl, not alice_turn)

            # ch == '?': try digits 0‑9
            for d in range(10):
                ndiff = diff
                nlq, nrq = lq, rq
                if i < half:
                    ndiff += d               # left ?
                    nlq -= 1
                else:
                    ndiff -= d               # right ?
                    nrq -= 1
                # Recurse with turn flipped
                if dfs(i + 1, ndiff, nlq, nrq, not alice_turn):
                    if alice_turn:
                        return True          # Alice found a winning move
                    # else Bob will avoid this branch if possible
            # No digit leads to Alice win → current player cannot force Alice win
            return not alice_turn

        return dfs(0, left_sum - right_sum, left_q, right_q, True)
```

**Complexity Analysis**

- Time: `O(n * D * LQ * RQ)` where `D` is the range of possible `diff` values (≈ `9 * total_q`). In the worst case (`q ≈ n/5`) this is still large, but far better than exponential and passes the given constraints because the number of distinct states is bounded by `O(n * 9 * q)`.  
- Space: Memoization table of the same size.

---

### Approach 3: Mathematical Optimal‑Play Formula (O(n) time, O(1) space)

**Intuition**  
With optimal play each player will try to **balance** the contribution of the unknowns.  
Consider the game from the perspective of the *difference* `diff = leftSum - rightSum`.  

* Each '`?`' in the left half can increase `diff` by any amount `0…9`.  
* Each '`?`' in the right half can decrease `diff` by any amount `0…9`.  

If Alice moves first, she can think of the game as her trying to push `diff` away from zero, while Bob tries to push it toward zero.  
The crucial observation: the **optimal** move for each player is always to pick either `0` or `9` (the extremes), because any intermediate value can be mirrored or countered more effectively by the opponent.  

After all moves, the total contribution from the left '`?`' will be some integer `L` in `[0, 9*left_q]` and from the right '`?`' some integer `R` in `[0, 9*right_q]`. The final difference is  

```
final = diff + L - R
```

Bob can force `final = 0` **iff** he can always respond to Alice’s choice so that the sum of the left contributions equals the sum of the right contributions plus the initial offset. This reduces to the simple equality:

```
diff == 9 * (right_q - left_q) / 2
```

If the equality holds, Bob mirrors each of Alice’s moves (choosing `9` when she picks `0` on the opposite half, etc.) and keeps the ability to zero out the difference.  
If it does **not** hold, Alice can break the symmetry on her first move and guarantee a non‑zero final difference.

**Algorithm**  

1. Scan the string once, computing  
   * `left_sum`, `right_sum`  
   * `left_q`, `right_q`  
2. Compute `diff = left_sum - right_sum`.  
3. Return `True` (Alice wins) iff `diff != 9 * (right_q - left_q) // 2`.  
   (The division is exact because the numerator is always even when the equality can hold; using integer division works for the inequality test.)

**Implementation**

```python
class Solution:
    def sumGame(self, num: str) -> bool:
        n = len(num)
        half = n // 2

        left_sum = right_sum = 0
        left_q = right_q = 0

        for i, ch in enumerate(num):
            if ch == '?':
                if i < half:
                    left_q += 1
                else:
                    right_q += 1
            else:
                d = ord(ch) - 48          # faster than int(ch)
                if i < half:
                    left_sum += d
                else:
                    right_sum += d

        diff = left_sum - right_sum
        # Alice wins if the balancing condition is NOT met
        return diff != 9 * (right_q - left_q) // 2
```

**Complexity Analysis**

- Time: `O(n)` – single pass over the string.  
- Space: `O(1)` – only a few integer variables.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimal‑play condition on Example 2: `num = "25??"`.

| Step | i | ch | half? | left_sum | right_sum | left_q | right_q | diff = L‑R |
|------|---|----|-------|----------|-----------|--------|---------|------------|
| 0    | 0 | '2'| left  | 2        | 0         | 0      | 0       | 2          |
| 1    | 1 | '5'| left  | 7        | 0         | 0      | 0       | 7          |
| 2    | 2 | '?'| right | 7        | 0         | 0      | 1       | 7          |
| 3    | 3 | '?'| right | 7        | 0         | 0      | 2       | 7          |

Final values:  

* `diff = 7`  
* `left_q = 0`  
* `right_q = 2`  

Check the condition:

```
9 * (right_q - left_q) // 2 = 9 * (2 - 0) // 2 = 9
```

Since `diff (7) != 9`, the condition fails → **Alice wins**.  

Indeed, Alice can place a `9` in the first '`?`' (making left sum 16) and Bob can never compensate enough to equalize the halves.

---

**Summary**

* Recognize the problem as a *sum‑balancing turn‑based game*.  
* Reduce the state to the known difference and the counts of '`?`' in each half.  
* The optimal‑play outcome follows a simple equality test, yielding an O(n) / O(1) solution.  
* This pattern appears in many “alternating moves, target equality” problems; mastering it lets you solve them instantly.