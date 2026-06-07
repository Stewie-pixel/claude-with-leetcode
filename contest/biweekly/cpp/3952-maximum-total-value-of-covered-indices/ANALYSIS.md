# Dynamic Programming: Maximum Total Value of Covered Indices  

## Video Solution  

For more details about **Maximum Total Value of Covered Indices**, watch the walkthrough at [Neetcode 150 - All Questions Solved...!](https://www.youtube.com/watch?v=3-mZwU87Ttw).  

## Concept  

Think of each token as a worker who can either **stay** at his current desk or **shift one desk to the left** (if that desk exists).  
Each desk (array index) has a reward `nums[i]`.  
A desk contributes its reward **once** if at least one worker ends up there; putting multiple workers on the same desk does not increase the total.  
We need to decide for every token whether to stay or shift so that the sum of rewards of the occupied desks is as large as possible.  

This is a classic “make a local binary choice that influences the next choice” situation → solved with a **1‑dimensional DP** that keeps only the best result for the two possible states of the current token.

## When to Use It  

Use this DP pattern when you see:  

- A list of items (tokens) processed in order.  
- Each item can take **one of a few discrete actions** (here: stay or move left).  
- The reward of an action depends on whether a **neighboring position** was already taken by the previous item.  
- You want the **maximum total reward** and the number of items is up to 10⁵ → O(N) or O(N log N) is required.  

In short: *local binary choices + conflict with immediate predecessor → DP with two states*.

## Template  

```python
# dp0 = best total up to previous token when that token stayed at its original index
# dp1 = best total up to previous token when that token moved left (to index-1)
dp0, dp1 = initial values   # base case for the first token

for each token in order (skip the first):
    # best value we can have before deciding what this token does
    best_prev = max(dp0, dp1) if dp1 is not None else dp0

    # option 1: current token stays
    new_dp0 = best_prev + nums[pos]                     # pos = current token index

    # option 2: current token moves left (only if pos > 0)
    new_dp1 = None
    if pos > 0:
        # if the left slot equals the previous token's original index,
        # we cannot add nums[pos-1] again when the previous token stayed
        left_val = nums[pos-1]
        if pos - 1 == prev_pos:                         # conflict with previous token staying
            cand = dp0 + left_val
        else:
            cand = best_prev + left_val                 # we can safely add left_val
        # also consider the case where previous token already moved left
        if dp1 is not None:
            cand = max(cand, dp1 + left_val)
        new_dp1 = cand

    dp0, dp1 = new_dp0, new_dp1
answer = max(dp0, dp1) if dp1 is not None else dp0
```

The template keeps only two variables → **O(1) extra space** and **O(k) time**, where *k* is the number of tokens.

---

## LeetCode Problem Walkthrough  

### Problem: 2559. Maximum Total Value of Covered Indices  

https://leetcode.com/problems/maximum-total-value-of-covered-indices/  

### Approach 1: Brute Force  

**Algorithm**  
- Enumerate every subset of tokens’ decisions (stay = 0, move left = 1).  
- For each of the `2^k` possibilities (`k` = number of tokens):  
  1. Determine the final index each token occupies (`i` if stay, `i‑1` if move).  
  2. If two tokens end up on the same index, treat it as a single covered index (no extra reward).  
  3. Sum `nums[idx]` for all distinct covered indices.  
- Return the maximum sum found.  

**Implementation**  

```python
from itertools import product

class Solution:
    def maxTotal(self, nums: List[int], s: str) -> int:
        tokens = [i for i, ch in enumerate(s) if ch == '1']
        if not tokens:
            return 0

        best = 0
        for choices in product([0, 1], repeat=len(tokens)):   # 0 = stay, 1 = move left
            covered = set()
            for idx, move in zip(tokens, choices):
                pos = idx - 1 if move else idx
                covered.add(pos)
            total = sum(nums[p] for p in covered)
            best = max(best, total)
        return best
```

**Complexity Analysis**  
- Time: `O(2^k * k)` – exponential in the number of tokens (worst‑case `k = n`).  
- Space: `O(k)` for the `covered` set (at most `k` entries).  

This is infeasible for `n ≤ 10⁵`.

---

### Approach 2: DP with Two States (Optimized)  

**Intuition**  
When we process tokens from left to right, the only thing that can affect the current token’s decision is **whether the previous token occupied the index immediately to the left of the current token**.  
Thus we need to remember two scenarios for the prefix processed so far:  

1. The previous token **stayed** at its original index.  
2. The previous token **moved left** (to `index‑1`).  

From these we can compute the best total for the current token staying or moving left, guaranteeing no double‑count of overlapping indices.

**Algorithm**  
- Extract the list `tokens` of indices where `s[i] == '1'`.  
- Initialise `dp0` = value if the first token stays, `dp1` = value if it moves left (or `-inf` if moving left is impossible).  
- Iterate over the remaining tokens:  
  * `best_prev = max(dp0, dp1)` – the best total before deciding the current token’s action.  
  * **Stay**: `new_dp0 = best_prev + nums[t]`.  
  * **Move left** (if `t > 0`):  
        - If the left slot `t‑1` equals the previous token’s original index, we can only add `nums[t‑1]` to `dp0` (the case where the previous token stayed).  
        - Otherwise we may add it to `best_prev`.  
        - Also consider the scenario where the previous token already moved left (`dp1`).  
        - Take the maximum of the applicable candidates.  
  * Assign `dp0, dp1 = new_dp0, new_dp1`.  
- Answer is `max(dp0, dp1)` (or just `dp0` if `dp1` is invalid).  

**Implementation**  

```python
from typing import List, Optional

class Solution:
    def maxTotal(self, nums: List[int], s: str) -> int:
        tokens = [i for i, ch in enumerate(s) if ch == '1']
        if not tokens:
            return 0

        # first token
        first = tokens[0]
        dp0 = nums[first]                     # stayed
        dp1 = nums[first - 1] if first > 0 else None   # moved left (if possible)

        for j in range(1, len(tokens)):
            t = tokens[j]
            prev = tokens[j - 1]

            best_prev = dp0 if dp1 is None else max(dp0, dp1)

            # stay at t
            new_dp0 = best_prev + nums[t]

            # move left to t-1
            new_dp1: Optional[int] = None
            if t > 0:
                left_val = nums[t - 1]
                # case 1: left slot equals previous token's original index
                if t - 1 == prev:
                    cand = dp0 + left_val          # only possible if prev token stayed
                else:
                    cand = best_prev + left_val    # no conflict
                # case 2: previous token already moved left
                if dp1 is not None:
                    cand = max(cand, dp1 + left_val)
                new_dp1 = cand

            dp0, dp1 = new_dp0, new_dp1

        return dp0 if dp1 is None else max(dp0, dp1)
```

**Complexity Analysis**  
- Time: `O(k)` where `k` = number of tokens ≤ `n`. One pass, constant work per token.  
- Space: `O(1)` – only two variables (`dp0`, `dp1`) are kept.  

---

### Provide a Visual Demonstration  

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example  

We dry‑run the DP on the first example:

```
nums = [9, 2, 6, 1]
s    = "0101"
tokens = [1, 3]          # positions that initially hold a token
```

| Step | t (token index) | prev (previous token) | dp0 (stay) | dp1 (move left) | Explanation |
|------|-----------------|-----------------------|-----------|----------------|-------------|
| Init | t = 1           | –                     | dp0 = nums[1] = 2 | dp1 = nums[0] = 9 | first token: stay → 2, move left → 9 |
| 1    | t = 3           | prev = 1              | best_prev = max(2,9) = 9 → new_dp0 = 9 + nums[3] = 9 + 1 = 10 | left slot = 2 (≠ prev) → cand = best_prev + nums[2] = 9 + 6 = 15; dp1 from prev is 9 → cand2 = 9 + 6 = 15 → new_dp1 = 15 | After processing token 3 we have dp0=10 (both stay) and dp1=15 (first moved left, second stayed). |
| End  | –               | –                     | answer = max(dp0, dp1) = max(10,15) = 15 | – | Maximum total value = 15, matching the example. |

The table shows how the DP keeps track of the two mutually exclusive histories and picks the better one at each step.

---  

**Summary**  
- The problem reduces to assigning each token to either its index or the index left of it, maximizing the sum of distinct indices’ values.  
- A linear DP with two states (previous token stayed / moved left) yields an O(n) time, O(1) space solution.  
- The brute‑force check (exponential) confirms the need for this optimization.  

You now have a template you can reuse for similar “binary choice with neighbor conflict” problems (e.g., maximum sum of non‑adjacent elements, house robber variants, etc.). Happy coding!