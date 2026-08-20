# 2D Dynamic Programming

## Video Solution

For more details about **Stone Game V**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=zIy0DkjsNuI)

## Concept

Interval DP solves problems where the optimal answer for a subarray depends on optimal answers of its smaller sub‑intervals. We define `dp[l][r]` as the best score obtainable from the subarray `stoneValue[l…r]`. By trying every possible split point `k` (where `l ≤ k < r`) we can combine the results of the left part `[l,k]` and the right part `[k+1,r]` while respecting the game’s rules (Bob discards the larger‑sum part, or Alice chooses when sums are equal).

## When to Use It

Use interval DP when you see:
- The problem involves repeatedly splitting or removing contiguous sections of an array.
- The score of a move depends on the sum (or another aggregate) of the remaining part.
- The optimal solution for a segment can be built from optimal solutions of smaller segments.
- Constraints allow an O(n²) or O(n³) DP table (n ≤ 500 here).

## Template

```python
# prefix sums for O(1) range sum queries
prefix = [0] * (n + 1)
for i in range(n):
    prefix[i + 1] = prefix[i] + stoneValue[i]

def range_sum(l, r):          # inclusive l, r
    return prefix[r + 1] - prefix[l]

# dp[l][r] = answer for stoneValue[l…r]
dp = [[0] * n for _ in range(n)]

# iterate over increasing interval length
for length in range(2, n + 1):          # length 1 gives 0 automatically
    for l in range(0, n - length + 1):
        r = l + length - 1
        best = 0
        for k in range(l, r):
            left_sum  = range_sum(l, k)
            right_sum = range_sum(k + 1, r)
            if left_sum < right_sum:
                cand = left_sum + dp[l][k]
            elif left_sum > right_sum:
                cand = right_sum + dp[k+1][r]
            else:   # equal sums, Alice chooses the better side
                cand = max(left_sum + dp[l][k],
                           right_sum + dp[k+1][r])
            best = max(best, cand)
        dp[l][r] = best

return dp[0][n-1]
```

## LeetCode Problem Walkthrough

### Problem: 1563. Stone Game V

https://leetcode.com/problems/stone-game-v/

### Approach 1: Brute Force (Plain Recursion)

**Algorithm**
- Recursively try every split point `k`.
- Compute left and right sums via a loop or prefix array.
- If left < right, Alice gains `left` and we recurse on `[l,k]`.
- If left > right, Alice gains `right` and we recurse on `[k+1,r]`.
- If equal, Alice gains either side and we take the max of both recursions.
- Base case: when `l == r` (single stone) return 0.

**Implementation**

```python
class Solution:
    def stoneGameV(self, stoneValue: List[int]) -> int:
        n = len(stoneValue)
        prefix = [0] * (n + 1)
        for i, v in enumerate(stoneValue):
            prefix[i + 1] = prefix[i] + v

        def range_sum(l, r):
            return prefix[r + 1] - prefix[l]

        def dfs(l, r):
            if l >= r:
                return 0
            best = 0
            for k in range(l, r):
                left = range_sum(l, k)
                right = range_sum(k + 1, r)
                if left < right:
                    best = max(best, left + dfs(l, k))
                elif left > right:
                    best = max(best, right + dfs(k + 1, r))
                else:
                    best = max(best,
                               left + dfs(l, k),
                               right + dfs(k + 1, r))
            return best

        return dfs(0, n - 1)
```

**Complexity Analysis**
- Time complexity: O(2ⁿ) — each call branches to up to O(n) subcalls without memoization, leading to exponential blow‑up.
- Space complexity: O(n) — recursion depth plus prefix array.

### Approach 2: Top‑Down DP with Memoization

**Intuition**
The brute‑force recursion recomputes the same `(l, r)` states many times. Storing the result of each interval eliminates duplicate work, turning the exponential recursion into a polynomial DP.

**Algorithm**
- Use a memo table `dp[l][r]` initialized to `-1`.
- The recursive function first checks `dp[l][r]`; if known, returns it.
- Otherwise compute as in brute force, store the answer, and return it.

**Implementation**

```python
class Solution:
    def stoneGameV(self, stoneValue: List[int]) -> int:
        n = len(stoneValue)
        prefix = [0] * (n + 1)
        for i, v in enumerate(stoneValue):
            prefix[i + 1] = prefix[i] + v

        def range_sum(l, r):
            return prefix[r + 1] - prefix[l]

        dp = [[-1] * n for _ in range(n)]

        def dfs(l, r):
            if l >= r:
                return 0
            if dp[l][r] != -1:
                return dp[l][r]
            best = 0
            for k in range(l, r):
                left = range_sum(l, k)
                right = range_sum(k + 1, r)
                if left < right:
                    best = max(best, left + dfs(l, k))
                elif left > right:
                    best = max(best, right + dfs(k + 1, r))
                else:
                    best = max(best,
                               left + dfs(l, k),
                               right + dfs(k + 1, r))
            dp[l][r] = best
            return best

        return dfs(0, n - 1)
```

**Complexity Analysis**
- Time complexity: O(n³) — there are O(n²) states and each state scans O(n) split points.
- Space complexity: O(n²) — memo table plus recursion stack O(n).

### Approach 3: Bottom‑Up DP (Iterative)

**Intuition**
Instead of recursion we fill the DP table in increasing order of interval length. When processing `[l, r]` all smaller intervals needed (`[l,k]` and `[k+1,r]`) have already been computed, guaranteeing correctness without recursion overhead.

**Algorithm**
- Precompute prefix sums for O(1) range queries.
- Initialize `dp[l][r] = 0` for all `l == r`.
- For each length from 2 to n:
  - For each start `l`, compute `r = l + length - 1`.
  - Try every split `k` and apply the same transition as the memoized version.
  - Store the best value in `dp[l][r]`.
- Answer is `dp[0][n‑1]`.

**Implementation**

```python
class Solution:
    def stoneGameV(self, stoneValue: List[int]) -> int:
        n = len(stoneValue)
        prefix = [0] * (n + 1)
        for i, v in enumerate(stoneValue):
            prefix[i + 1] = prefix[i] + v

        def range_sum(l, r):
            return prefix[r + 1] - prefix[l]

        dp = [[0] * n for _ in range(n)]

        for length in range(2, n + 1):
            for l in range(0, n - length + 1):
                r = l + length - 1
                best = 0
                for k in range(l, r):
                    left = range_sum(l, k)
                    right = range_sum(k + 1, r)
                    if left < right:
                        cand = left + dp[l][k]
                    elif left > right:
                        cand = right + dp[k + 1][r]
                    else:
                        cand = max(left + dp[l][k],
                                   right + dp[k + 1][r])
                    best = max(best, cand)
                dp[l][r] = best

        return dp[0][n - 1]
```

**Complexity Analysis**
- Time complexity: O(n³) — three nested loops (length, left start, split point).
- Space complexity: O(n²) — DP table.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: stoneValue = [6, 2, 3, 4, 5, 5]

We'll trace the top‑down memoized version on the first split that leads to the optimal answer.

```
Initial call: dfs(0,5)   # whole array [6,2,3,4,5,5], sum = 25
Try k = 0: left=[6]=6, right=[2,3,4,5,5]=19  -> left<right
            candidate = 6 + dfs(0,0) = 6 + 0 = 6
Try k = 1: left=[6,2]=8, right=[3,4,5,5]=17  -> left<right
            candidate = 8 + dfs(0,1)
Try k = 2: left=[6,2,3]=11, right=[4,5,5]=14 -> left<right
            candidate = 11 + dfs(0,2)   <-- this is the split used in the optimal path
...
```

Now evaluate `dfs(0,2)` on subarray [6,2,3] (sum = 11):

```
dfs(0,2):
  k=0: left=6, right=5 -> left>right -> cand = 5 + dfs(1,2)
  k=1: left=8, right=3 -> left>right -> cand = 3 + dfs(0,1)
  Choose max; suppose dfs(1,2) later yields 5, etc.
```

Eventually the recursion unwinds and the total becomes 18, matching the example. The table below shows the memoized results for intervals of length 2 (already computed) that are reused:

| (l,r) | subarray            | dp[l][r] |
|-------|---------------------|----------|
| (0,1) | [6,2]               | 6        |
| (1,2) | [2,3]               | 2        |
| (2,3) | [3,4]               | 3        |
| (3,4) | [4,5]               | 4        |
| (4,5) | [5,5]               | 5        |

These values are consulted when evaluating longer intervals, drastically reducing repeated work.

---