# Predict the Winner

## Video Solution

For more details about **Predict the Winner**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=giomYhFJo6g).

## Concept

Predict the Winner is a two‑player zero‑sum game where each player tries to maximize his own total score.  
Instead of tracking both scores directly, we can think in terms of **score difference**:  
`diff(i, j)` = (current player’s score) – (opponent’s score) for the subarray `nums[i…j]`.  

If the current player picks the left end, he gains `nums[i]` and then the opponent becomes the “current player” on the remaining interval `[i+1, j]`.  
From the opponent’s perspective the difference he can achieve is `diff(i+1, j)`.  
Therefore, after the current player’s move the net difference becomes `nums[i] - diff(i+1, j)`.  
The same reasoning holds for picking the right end.

The current player will choose the move that gives him the larger difference:

```
diff(i, j) = max( nums[i] - diff(i+1, j),
                  nums[j] - diff(i,   j-1) )
```

Base case: `diff(i, i) = nums[i]` (only one number to take).  
If `diff(0, n-1) >= 0`, player 1 can finish with a score at least as large as player 2, so he wins (or ties).

This recurrence is a classic interval DP.

## When to Use It

Use this DP on intervals when:
- Players take turns picking from the **ends** of a sequence.
- Both play **optimally**.
- You need to know whether the first player can win or tie.
- The sequence length is modest (the O(n²) solution works for n up to a few thousand; constraints here are n ≤ 20, but the method scales).

Typical tags: *Game Theory*, *Minimax*, *Interval DP*, *Dynamic Programming*.

## Template

Reusable Python template for the interval‑DP game‑difference pattern:

```python
def predict_winner(nums):
    n = len(nums)
    # dp[i][j] = max score difference current player can achieve from i..j
    dp = [[0] * n for _ in range(n)]

    for i in range(n):
        dp[i][i] = nums[i]                # length 1

    for length in range(2, n + 1):        # increasing subarray size
        for i in range(n - length + 1):
            j = i + length - 1
            dp[i][j] = max(nums[i] - dp[i+1][j],
                           nums[j] - dp[i][j-1])

    return dp[0][n-1] >= 0
```

## LeetCode Problem Walkthrough

### Problem: 486. Predict the Winner

https://leetcode.com/problems/predict-the-winner/

### Approach 1: Brute Force (Recursion with Memoization)

**Algorithm**  
Define a recursive function `diff(l, r)` that returns the maximum score difference the current player can obtain from `nums[l…r]`.  
- If `l > r`: no numbers left → difference `0`.  
- Otherwise the player may take `nums[l]` (left) leaving the opponent to play on `[l+1, r]`, giving a net gain of `nums[l] - diff(l+1, r)`.  
- Or take `nums[r]` (right) giving `nums[r] - diff(l, r-1)`.  
- Return the larger of the two.  

Memoize results to avoid recomputation; each `(l, r)` pair is evaluated once.

**Implementation**

```python
from typing import List
from functools import lru_cache

class Solution:
    def predictTheWinner(self, nums: List[int]) -> bool:
        @lru_cache(maxsize=None)
        def diff(l: int, r: int) -> int:
            if l > r:
                return 0
            take_left  = nums[l] - diff(l+1, r)
            take_right = nums[r] - diff(l, r-1)
            return max(take_left, take_right)

        return diff(0, len(nums)-1) >= 0
```

**Complexity Analysis**  
- Time: Each state `(l, r)` (O(n²) of them) is processed once → **O(n²)**.  
- Space: Memoization table + recursion depth → **O(n²)** (dominated by the table).

### Approach 2: Bottom‑Up DP (2D Table)

**Intuition**  
The recurrence only depends on intervals that are strictly shorter (`[i+1, j]` and `[i, j-1]`).  
By filling the DP table in increasing order of interval length we guarantee that the needed sub‑problems are already known.

**Algorithm**  
1. Initialise `dp[i][i] = nums[i]`.  
2. For `length = 2 … n`:  
   - For each start `i`, compute `j = i + length - 1`.  
   - Set `dp[i][j] = max(nums[i] - dp[i+1][j], nums[j] - dp[i][j-1])`.  
3. The answer is `dp[0][n-1] >= 0`.

**Implementation**

```python
class Solution:
    def predictTheWinner(self, nums: List[int]) -> bool:
        n = len(nums)
        dp = [[0] * n for _ in range(n)]

        for i in range(n):
            dp[i][i] = nums[i]

        for length in range(2, n + 1):
            for i in range(n - length + 1):
                j = i + length - 1
                dp[i][j] = max(nums[i] - dp[i+1][j],
                               nums[j] - dp[i][j-1])

        return dp[0][n-1] >= 0
```

**Complexity Analysis**  
- Time: We fill `n*(n+1)/2` cells → **O(n²)**.  
- Space: The 2‑D table → **O(n²)**.

### Approach 3: Space‑Optimized DP (1D)

**Intuition**  
When computing `dp[i][j]` we only need:
- `dp[i+1][j]` (the value from the *next* row, same column)  
- `dp[i][j-1]` (the value from the *current* row, previous column)  

If we iterate `i` from right to left and reuse a one‑dimensional array where `dp[j]` holds the result for the current `i … j`, we can obtain the same result with O(n) space.

**Algorithm**  
1. Create `dp = [0]*n`.  
2. For `i` from `n‑1` down to `0`:  
   - Set `dp[i] = nums[i]` (interval of length 1).  
   - For `j` from `i+1` to `n‑1`:  
        `dp[j] = max(nums[i] - dp[j],      # dp[j] currently stores dp[i+1][j]  
                     nums[j] - dp[j-1])   # dp[j-1] stores dp[i][j-1] from this iteration`  
3. After the loops, `dp[n‑1]` equals `dp[0][n-1]`. Return `dp[n-1] >= 0`.

**Implementation**

```python
class Solution:
    def predictTheWinner(self, nums: List[int]) -> bool:
        n = len(nums)
        dp = [0] * n                     # dp[j] will hold dp[i][j] for current i

        for i in range(n-1, -1, -1):
            dp[i] = nums[i]              # base case: interval [i,i]
            for j in range(i+1, n):
                dp[j] = max(nums[i] - dp[j],
                            nums[j] - dp[j-1])

        return dp[-1] >= 0
```

**Complexity Analysis**  
- Time: Still O(n²) (same number of updates).  
- Space: Only the 1‑D array → **O(n)**.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

We trace the **1‑D space‑optimized** version on the example `nums = [1, 5, 2]`.

```
Initial dp = [0, 0, 0]

i = 2 (value 2):
    dp[2] = 2
    (no inner loop)
dp = [0, 0, 2]

i = 1 (value 5):
    dp[1] = 5
    j = 2:
        dp[2] = max( nums[1] - dp[2] , nums[2] - dp[1] )
               = max(5 - 2 , 2 - 5) = max(3, -3) = 3
dp = [0, 5, 3]

i = 0 (value 1):
    dp[0] = 1
    j = 1:
        dp[1] = max( nums[0] - dp[1] , nums[1] - dp[0] )
               = max(1 - 5 , 5 - 1) = max(-4, 4) = 4
    j = 2:
        dp[2] = max( nums[0] - dp[2] , nums[2] - dp[1] )
               = max(1 - 3 , 2 - 4) = max(-2, -2) = -2
dp = [1, 4, -2]

Result: dp[2] = -2  (< 0) → Player 1 cannot guarantee a win → return False.
```

The trace matches the given example where Player 1 loses.

--- 

Following this lecture, the learner should be able to:
- Recognize the “pick from ends, optimal play” pattern.
- Derive the score‑difference recurrence.
- Implement both the straightforward 2‑D DP and the space‑optimized 1‑D version.
- State time and space complexities for each approach.