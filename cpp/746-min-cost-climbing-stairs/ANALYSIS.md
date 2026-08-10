# Dynamic Programming

## Video Solution

For more details about **Min Cost Climbing Stairs**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=ktmzAZWkEZ0)

## Concept

Dynamic Programming (DP) solves problems by breaking them into overlapping sub‑problems and storing the results of those sub‑problems so each is solved only once.  
For stair‑climbing problems the *state* is “the minimum cost to reach a particular step”. The cost to reach step i depends only on the costs to reach the two previous steps (i‑1 and i‑2), which makes the problem ideal for a 1‑dimensional DP table.

A real‑world analogy: imagine you are walking along a hallway with tiles that each have a price to step on. You can move either one tile or two tiles forward. You want to know the cheapest total price to get past the last tile. Instead of trying every possible walk (which would repeat the same tile‑price sums many times), you keep a running note of the cheapest price to reach each tile as you go.

## When to Use It

Use DP when you see:

- A problem that asks for a **minimum / maximum / count** of ways to reach a goal.
- The ability to move in small, fixed steps (e.g., +1 or +2) – leading to **overlapping sub‑problems**.
- Constraints that allow an **O(n)** or **O(n²)** solution but rule out exponential brute force.
- A clear **recurrence relation** linking the solution for size *i* to solutions for smaller sizes.

In “Min Cost Climbing Stairs” the clues are: cost array, you may climb 1 or 2 steps, you need the minimum cost to reach the top (just beyond the last index).

## Template

Below is a reusable Python template for a 1‑dimensional DP where each state depends on the previous two states (the pattern used in this problem). Replace the comment with the specific recurrence for any similar problem.

```python
def min_cost_dp(cost):
    # cost[i] = price to stand on step i
    n = len(cost)
    # dp[i] = min cost to reach step i (we can start at 0 or 1)
    # We only need the last two values, so we keep two variables.
    dp_i_minus_2 = 0   # dp[0] when we start at step 0 (cost[0] will be added)
    dp_i_minus_1 = 0   # dp[1] when we start at step 1 (cost[1] will be added)

    for i in range(n):
        # To stand on step i we must pay cost[i] plus the cheaper of the two ways to get there.
        cur = cost[i] + min(dp_i_minus_1, dp_i_minus_2)
        # Shift the window for the next iteration.
        dp_i_minus_2, dp_i_minus_1 = dp_i_minus_1, cur

    # We can finish from either the last step or the second‑last step.
    return min(dp_i_minus_1, dp_i_minus_2)
```

## LeetCode Problem Walkthrough

### Problem: 746. Min Cost Climbing Stairs

https://leetcode.com/problems/min-cost-climbing-stairs/

---

### Approach 1: Brute Force (Recursion)

**Algorithm**  
From the current step `i` you have two choices: pay `cost[i]` and move to `i+1`, or pay `cost[i]` and move to `i+2`. Recurse until you step beyond the last index (i.e., reach the “top”). Return the smaller accumulated cost of the two possibilities.  
Start the recursion from both possible starting positions (0 and 1) and take the minimum.

**Implementation**

```python
class Solution:
    def minCostClimbingStairs(self, cost: List[int]) -> int:
        n = len(cost)

        def dfs(i: int) -> int:
            # If we have stepped off the staircase, no more cost.
            if i >= n:
                return 0
            # Pay cost[i] and try both moves.
            return cost[i] + min(dfs(i + 1), dfs(i + 2))

        # We may start at index 0 or index 1.
        return min(dfs(0), dfs(1))
```

**Complexity Analysis**

- Time complexity: O(2ⁿ) — each call branches into two, creating an exponential recursion tree.
- Space complexity: O(n) — recursion depth at most n (the call stack).

---

### Approach 2: Top‑Down Memoization

**Intuition**  
The recursive solution recomputes the same sub‑problems many times (e.g., `dfs(3)` is needed from both `dfs(1)` and `dfs(2)`). By caching the result for each index `i` after the first computation, we turn the exponential tree into a linear one.

**Algorithm**  
Create a memo array/dictionary initialized with a sentinel (e.g., `-1`). In `dfs(i)`, if `memo[i]` is already computed, return it; otherwise compute as before and store the result before returning.

**Implementation**

```python
class Solution:
    def minCostClimbingStairs(self, cost: List[int]) -> int:
        n = len(cost)
        memo = [-1] * (n + 1)          # extra slot for the “top” beyond n‑1

        def dfs(i: int) -> int:
            if i >= n:                 # reached or passed the top
                return 0
            if memo[i] != -1:          # already solved
                return memo[i]
            memo[i] = cost[i] + min(dfs(i + 1), dfs(i + 2))
            return memo[i]

        return min(dfs(0), dfs(1))
```

**Complexity Analysis**

- Time complexity: O(n) — each index `i` is processed once.
- Space complexity: O(n) — memo array plus recursion stack.

---

### Approach 3: Bottom‑Up DP with O(1) Space

**Intuition**  
Since `dp[i]` only depends on `dp[i‑1]` and `dp[i‑2]`, we can iterate forward while keeping just the two previous values, eliminating the need for an entire DP array. This yields constant auxiliary space.

**Algorithm**  
Initialize two variables representing the minimum cost to reach the “virtual” steps before the first actual step (both 0 because we may start at index 0 or index 1 without paying anything yet).  
For each step `i` from 0 to n‑1:
- The cost to stand on step `i` is `cost[i] + min(prev1, prev2)`.
- Shift the window: `prev2 <- prev1`, `prev1 <- cur`.
After processing all steps, the answer is the cheaper of the two ways to step off the top: `min(prev1, prev2)` (being on the last step or the second‑last step).

**Implementation**

```python
class Solution:
    def minCostClimbingStairs(self, cost: List[int]) -> int:
        # prev2 = dp[i-2], prev1 = dp[i-1]
        prev2 = prev1 = 0          # dp[-2] and dp[-1] are 0 (we can start at 0 or 1)

        for c in cost:
            cur = c + min(prev1, prev2)   # cost to reach this step
            prev2, prev1 = prev1, cur     # slide the window

        # We can finish from the last step or the one before it.
        return min(prev1, prev2)
```

**Complexity Analysis**

- Time complexity: O(n) — one pass through the cost array.
- Space complexity: O(1) — only two integer variables are used.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

We trace the bottom‑up O(1) solution on the example `cost = [10, 15, 20]`.

| i (index) | cost[i] | prev2 (dp[i‑2]) | prev1 (dp[i‑1]) | cur = cost[i] + min(prev1, prev2) | New prev2 | New prev1 |
|-----------|---------|-----------------|-----------------|-----------------------------------|-----------|-----------|
| 0         | 10      | 0               | 0               | 10 + min(0,0) = 10                | 0         | 10        |
| 1         | 15      | 0               | 10              | 15 + min(10,0) = 15               | 10        | 15        |
| 2         | 20      | 10              | 15              | 20 + min(15,10) = 30              | 15        | 30        |

After the loop:
- `prev1` = dp[2] = 30 (cost to stand on step 2)
- `prev2` = dp[1] = 15 (cost to stand on step 1)

Answer = `min(prev1, prev2)` = `min(30, 15)` = **15**, which matches the expected output (start at step 1, pay 15, jump two steps to the top).

---