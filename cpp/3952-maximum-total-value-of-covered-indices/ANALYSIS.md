# Greedy

## Video Solution

For more details about **Maximum Total Value of Covered Indices**, watch the walkthrough at [https://www.youtube.com/watch?v=Bu343sVk9cY](https://www.youtube.com/watch?v=Bu343sVk9cY)

## Concept

Each token can either stay at its current index `i` or move left to `i‑1` (if `i>0`). The decision for one token does **not** affect the choices available to any other token because multiple tokens may occupy the same index after moving. Therefore the problem decomposes into independent sub‑problems: for every token we simply pick the larger of `nums[i]` (stay) and `nums[i‑1]` (move left). Summing these local maxima yields the global optimum.

## When to Use It

- You see a problem where each element makes an independent binary choice (stay/move, take/skip, assign A/B, …).  
- The total objective is a sum (or other additive function) of the individual contributions.  
- No capacity constraints or conflicts exist between choices (overlap is allowed).  
- In such cases a greedy per‑element selection of the locally optimal option is provably optimal.

## Template

```python
def greedy_sum(nums: List[int], s: str) -> int:
    total = 0
    for i, ch in enumerate(s):
        if ch == '1':
            stay = nums[i]
            move = nums[i-1] if i > 0 else 0   # moving left is impossible at i==0
            total += max(stay, move)
    return total
```

---

## LeetCode Problem Walkthrough

### Problem: 2740. Maximum Total Value of Covered Indices

https://leetcode.com/problems/maximum-total-value-of-covered-indices/

### Approach 1: Brute Force (Backtracking)

**Algorithm**  
For each token we recursively try both possibilities (stay or move left) and keep track of the best total value seen. Because each token has two choices, the search space is `2^k` where `k` is the number of tokens.

**Implementation**

```python
from typing import List

class Solution:
    def maxTotal(self, nums: List[int], s: str) -> int:
        token_pos = [i for i, ch in enumerate(s) if ch == '1']
        n = len(token_pos)

        def dfs(idx: int, covered: List[bool]) -> int:
            if idx == n:
                # compute total value of covered indices
                return sum(nums[i] for i, cov in enumerate(covered) if cov)
            pos = token_pos[idx]
            # option 1: stay
            covered[pos] = True
            best = dfs(idx + 1, covered)
            covered[pos] = False          # undo
            # option 2: move left (if possible)
            if pos > 0:
                covered[pos-1] = True
                best = max(best, dfs(idx + 1, covered))
                covered[pos-1] = False    # undo
            return best

        return dfs(0, [False] * len(nums))
```

**Complexity Analysis**

- Time complexity: `O(2^k * n)` – we explore `2^k` leaf nodes and each leaf scans the array to sum values (`k` = number of tokens, worst‑case `k = n`).  
- Space complexity: `O(n)` – recursion depth `O(k)` plus the `covered` boolean array.

---

### Approach 2: Dynamic Programming (Linear)

**Intuition**  
Although the choices are independent, we can still formulate a DP that processes tokens from left to right. For each token we keep two states: the best total when the current token stays (`dp0`) and when it moves left (`dp1`). The transition only needs the previous token’s states because moving left from position `i` uses `nums[i‑1]`, which does not interfere with the decision of the token at `i‑1` (overlap is allowed).

**Algorithm**

1. Extract the indices of all tokens into a list `tokens`.
2. Initialise:
   - `dp0 = nums[tokens[0]]` (first token stays)
   - `dp1 = nums[tokens[0]-1]` if the first token can move left, otherwise `None`.
3. For each subsequent token at position `t` with previous token at `tp`:
   - `best_prev = max(dp0, dp1 if dp1 else dp0)` – the best total up to the previous token regardless of its state.
   - `new_dp0 = best_prev + nums[t]` (current token stays).
   - If `t > 0`:
        - If `t-1 == tp` (the left cell is exactly the previous token’s original spot) then moving left can only add `nums[t-1]` to `dp0` (because the previous token cannot also occupy that same cell after it has moved).  
          `val = dp0 + nums[t-1]`
        - Otherwise we may add `nums[t-1]` to either `dp0` or `dp1`.  
          `val = max(dp0, dp1 if dp1 else 0) + nums[t-1]`
        - Set `new_dp1 = val`.
   - Shift `dp0, dp1 <- new_dp0, new_dp1`.
4. Answer is `max(dp0, dp1 if dp1 else dp0)`.

**Implementation**

```python
from typing import List, Optional

class Solution:
    def maxTotal(self, nums: List[int], s: str) -> int:
        tokens = [i for i, ch in enumerate(s) if ch == '1']
        if not tokens:
            return 0

        dp0 = nums[tokens[0]]
        dp1: Optional[int] = (
            nums[tokens[0] - 1] if tokens[0] > 0 else None
        )

        for idx in range(1, len(tokens)):
            t = tokens[idx]
            tp = tokens[idx - 1]

            best_prev = dp0 if dp1 is None else max(dp0, dp1)
            new_dp0 = best_prev + nums[t]

            new_dp1: Optional[int] = None
            if t > 0:
                # value if we move the current token left
                if t - 1 == tp:          # left cell is the previous token's original spot
                    cand = dp0 + nums[t - 1]
                else:
                    cand = best_prev + nums[t - 1]
                new_dp1 = cand

            dp0, dp1 = new_dp0, new_dp1

        return dp0 if dp1 is None else max(dp0, dp1)
```

**Complexity Analysis**

- Time complexity: `O(n)` – one pass over the array to collect tokens and another pass over the token list.  
- Space complexity: `O(1)` – only a few scalar variables are used.

---

### Approach 3: Greedy (Optimal)

**Intuition**  
Because each token’s contribution is independent and additive, the locally optimal choice (pick the larger of staying or moving left) directly yields the globally optimal sum. No look‑ahead or coordination between tokens is necessary.

**Algorithm**

- Iterate through the array once.
- Whenever `s[i] == '1'`, add `max(nums[i], nums[i-1] if i>0 else 0)` to the answer.

**Implementation**

```python
from typing import List

class Solution:
    def maxTotal(self, nums: List[int], s: str) -> int:
        total = 0
        for i, ch in enumerate(s):
            if ch == '1':
                stay = nums[i]
                move = nums[i-1] if i > 0 else 0
                total += max(stay, move)
        return total
```

**Complexity Analysis**

- Time complexity: `O(n)` – single traversal.  
- Space complexity: `O(1)` – only a few integer variables.

---

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

### Dry Run

We trace the greedy algorithm on the first example:

- `nums = [9, 2, 6, 1]`
- `s    = "0101"`

| i | s[i] | nums[i] | nums[i-1] (if i>0) | max(stay,move) | cumulative total |
|---|------|---------|--------------------|----------------|------------------|
| 0 | '0'  | 9       | –                  | 0              | 0                |
| 1 | '1'  | 2       | 9                  | **9**          | 9                |
| 2 | '0'  | 6       | –                  | 0              | 9                |
| 3 | '1'  | 1       | 6                  | **6**          | 15               |

Final answer = **15**, matching the expected output.

---