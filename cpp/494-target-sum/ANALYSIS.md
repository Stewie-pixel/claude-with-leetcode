# 1D Dynamic Programming

## Video Solution

For more details about **Target Sum**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=dwMOrl85Xes)

## Concept

The problem asks for the number of ways to assign ‘+’ or ‘‑’ to each number so that the total equals `target`.  
If we split the numbers into two groups:

- **P** – numbers prefixed with ‘+’
- **N** – numbers prefixed with ‘‑’

We need  

```
sum(P) - sum(N) = target
```

Adding `sum(P) + sum(N) = total` to both sides gives  

```
2 * sum(P) = target + total
```

Thus we only need to count subsets whose sum equals  

```
P = (target + total) / 2
```

If `target + total` is odd or `P` is negative, there is no solution.  
Counting subsets with a given sum is a classic **1‑D DP (subset‑sum)** problem.

## When to Use It

Use this pattern when you see:

- Assigning two symbols (+/‑) to each element and counting ways to reach a target.
- The problem can be reduced to “how many subsets sum to a specific value?”.
- Constraints on the total sum are modest (≤ 1000), allowing a DP array sized by that sum.

## Template

```python
def subset_sum_count(nums, target):
    """
    Returns the number of subsets of `nums` that sum exactly to `target`.
    Classic 0/1 knapsack counting DP.
    """
    dp = [0] * (target + 1)
    dp[0] = 1                     # empty subset makes sum 0
    for num in nums:
        # traverse backwards to avoid re‑using the same element
        for s in range(target, num - 1, -1):
            dp[s] += dp[s - num]
    return dp[target]
```

---

## LeetCode Problem Walkthrough

### Problem: 494. Target Sum

https://leetcode.com/problems/target-sum/

### Approach 1: Brute Force (Backtracking)

**Algorithm**  
Try every possible sign assignment recursively. At each index we branch into ‘+’ and ‘‑’. When we reach the end of the array, we check if the accumulated sum equals `target` and count it.

**Implementation**

```python
class Solution:
    def findTargetSumWays(self, nums: List[int], target: int) -> int:
        self.count = 0
        n = len(nums)

        def backtrack(i: int, current: int):
            if i == n:
                if current == target:
                    self.count += 1
                return
            # choose '+'
            backtrack(i + 1, current + nums[i])
            # choose '-'
            backtrack(i + 1, current - nums[i])

        backtrack(0, 0)
        return self.count
```

**Complexity Analysis**

- Time complexity: O(2ⁿ) — each of the n numbers has two independent choices.
- Space complexity: O(n) — recursion stack depth (ignoring output).

---

### Approach 2: Memoized DFS (Top‑Down DP)

**Intuition**  
The brute force explores many identical sub‑problems: the same `(index, current_sum)` pair can be reached via different sign choices. Caching these results avoids recomputation.

**Algorithm**  
Use a dictionary (or lru_cache) to store the number of ways to reach a given sum from a given index. Recurse as before, but return the cached value when available.

**Implementation**

```python
class Solution:
    def findTargetSumWays(self, nums: List[int], target: int) -> int:
        from functools import lru_cache

        @lru_cache(None)
        def dfs(i: int, total: int) -> int:
            if i == len(nums):
                return 1 if total == target else 0
            # add nums[i]
            add = dfs(i + 1, total + nums[i])
            # subtract nums[i]
            sub = dfs(i + 1, total - nums[i])
            return add + sub

        return dfs(0, 0)
```

**Complexity Analysis**

- Time complexity: O(n * S) where S is the range of possible sums (‑total … total). In practice bounded by O(n * sum(nums)).
- Space complexity: O(n * S) for the memoization table + O(n) recursion stack.

---

### Approach 3: Subset‑Sum DP (Bottom‑Up, Most Optimal)

**Intuition**  
As shown in the Concept section, the problem reduces to counting subsets that sum to `P = (total + target) / 2`. This is a classic 0/1 knapsack count, solvable with a 1‑D DP array in O(n * P) time and O(P) space.

**Algorithm**

1. Compute `total = sum(nums)`.
2. If `(total + target)` is odd or `total < abs(target)`, return 0 (no feasible partition).
3. Set `P = (total + target) // 2`.
4. Initialise `dp[0] = 1` (one way to make sum 0) and all other entries 0.
5. For each number `num` in `nums`, update dp backwards:
   ```
   for s from P down to num:
       dp[s] += dp[s - num]
   ```
6. Answer is `dp[P]`.

**Implementation**

```python
class Solution:
    def findTargetSumWays(self, nums: List[int], target: int) -> int:
        total = sum(nums)
        # impossible if parity mismatched or target out of reachable range
        if (total + target) % 2 != 0 or total < abs(target):
            return 0

        P = (total + target) // 2
        dp = [0] * (P + 1)
        dp[0] = 1

        for num in nums:
            for s in range(P, num - 1, -1):
                dp[s] += dp[s - num]

        return dp[P]
```

**Complexity Analysis**

- Time complexity: O(n * P) — each number updates at most P states.
- Space complexity: O(P) — one‑dimensional DP array.

---

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the bottom‑up DP on the example `nums = [1,1,1,1,1]`, `target = 3`.

- `total = 5`
- `P = (5 + 3) // 2 = 4`
- DP array size = 5 (indices 0…4)

| Step (num) | dp before update                     | Update (s from 4→num)                     | dp after update               |
|------------|--------------------------------------|-------------------------------------------|-------------------------------|
| init       | [1,0,0,0,0]                          |                                           | [1,0,0,0,0]                   |
| 1          | [1,0,0,0,0]                          | s=4→1: dp[4]+=dp[3]=0, dp[3]+=dp[2]=0, dp[2]+=dp[1]=0, dp[1]+=dp[0]=1 | [1,1,0,0,0] |
| 1          | [1,1,0,0,0]                          | s=4→1: dp[4]+=dp[3]=0, dp[3]+=dp[2]=0, dp[2]+=dp[1]=1, dp[1]+=dp[0]=2 | [1,2,1,0,0] |
| 1          | [1,2,1,0,0]                          | s=4→1: dp[4]+=dp[3]=0, dp[3]+=dp[2]=1, dp[2]+=dp[1]=3, dp[1]+=dp[0]=3 | [1,3,3,1,0] |
| 1          | [1,3,3,1,0]                          | s=4→1: dp[4]+=dp[3]=1, dp[3]+=dp[2]=4, dp[2]+=dp[1]=6, dp[1]+=dp[0]=4 | [1,4,6,4,1] |
| 1          | [1,4,6,4,1]                          | s=4→1: dp[4]+=dp[3]=5, dp[3]+=dp[2]=10, dp[2]+=dp[1]=10, dp[1]+=dp[0]=5 | [1,5,10,10,5] |

Final `dp[4] = 5`, which matches the expected output (five ways).

---