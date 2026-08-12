# 1D Dynamic Programming

## Video Solution

For more details about **Best Time to Buy and Sell Stock with Transaction Fee**, watch the walkthrough at [https://www.youtube.com/watch?v=QOvDwbIC4Pk](https://www.youtube.com/watch?v=QOvDwbIC4Pk)

## Concept

The problem asks for the maximum profit from an arbitrary number of buy‑sell transactions when each transaction incurs a fixed fee.  
Think of each day as a decision point: you either **hold** a stock or you **do not hold** (cash).  
- If you hold a stock, you can either **sell** it today (pay the fee) or keep holding.  
- If you are in cash, you can either **buy** a stock today or stay in cash.  

The optimal decision for today only depends on the best profit you could have achieved yesterday in each of those two states. This leads to a classic *state‑machine* DP with two variables that we update in O(1) space per day.

## When to Use It

Use this technique when you see:
- A sequence of days/steps where you can make a decision that changes a binary state (hold / not hold, locked / unlocked, etc.).
- A cost or fee associated with changing the state.
- The goal is to maximise (or minimise) some cumulative value over the sequence.

In short: **any problem that can be modelled as “at each step I am in one of a small fixed number of states, and I may transition between them with a gain/cost”** fits this DP pattern.

## Template

```python
# state variables: profit when we are in each state
state_A = initial_value_A   # e.g., profit when we do NOT hold a stock
state_B = initial_value_B   # e.g., profit when we DO hold a stock

for value in sequence:
    # compute next state values based on current ones
    next_A = max( ... )      # stay in A or come from B
    next_B = max( ... )      # stay in B or come from A
    state_A, state_B = next_A, next_B

# answer is usually the profit in the final desired state
return state_A   # or state_B depending on definition
```

---

## LeetCode Problem Walkthrough

### Problem: 714. Best Time to Buy and Sell Stock with Transaction Fee  
https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/

### Approach 1: Brute Force (exponential recursion)

**Algorithm**  
We recursively consider every possible action on each day:
- If we are currently **holding** a stock we may either **sell** it (gain `price - fee`) or **keep** it.
- If we are currently **not holding** we may either **buy** a stock (spend `price`) or **skip** the day.

The recursion explores all `2^n` buy/sell patterns, keeping track of the best profit when we finish the last day **without** holding a stock (we must end in cash because holding a stock would mean we never sold it).

**Implementation**

```python
from typing import List

class Solution:
    def maxProfit(self, prices: List[int], fee: int) -> int:
        n = len(prices)

        def dfs(i: int, holding: bool) -> int:
            # i : current day index
            # holding : True if we own a stock, False otherwise
            if i == n:
                # no more days – profit is 0 only if we are not holding,
                # otherwise this state is invalid (we would have to sell at -inf)
                return 0 if not holding else float('-inf')

            if holding:
                # Option 1: sell today
                sell = prices[i] - fee + dfs(i + 1, False)
                # Option 2: do nothing
                keep = dfs(i + 1, True)
                return max(sell, keep)
            else:
                # Option 1: buy today
                buy = -prices[i] + dfs(i + 1, True)
                # Option 2: do nothing
                skip = dfs(i + 1, False)
                return max(buy, skip)

        return dfs(0, False)
```

**Complexity Analysis**

- Time complexity: **O(2ⁿ)** – each day branches into two choices, leading to exponential leaves.  
- Space complexity: **O(n)** – recursion stack depth equals the number of days.

---

### Approach 2: Top‑Down DP with Memoization

**Intuition**  
The brute‑force recursion recomputes the same sub‑problems many times (same `(i, holding)` pair). By caching the result of each state we turn the exponential recursion into a linear DP.

**Algorithm**  
Same recursion as above, but we store `memo[i][holding]` after the first computation. The number of distinct states is `2 * n`, so each is processed once.

**Implementation**

```python
from typing import List
import functools

class Solution:
    def maxProfit(self, prices: List[int], fee: int) -> int:
        n = len(prices)

        @functools.lru_cache(None)
        def dfs(i: int, holding: bool) -> int:
            if i == n:
                return 0 if not holding else float('-inf')

            if holding:
                sell = prices[i] - fee + dfs(i + 1, False)
                keep = dfs(i + 1, True)
                return max(sell, keep)
            else:
                buy = -prices[i] + dfs(i + 1, True)
                skip = dfs(i + 1, False)
                return max(buy, skip)

        return dfs(0, False)
```

**Complexity Analysis**

- Time complexity: **O(n)** – each of the `2n` states is evaluated once.  
- Space complexity: **O(n)** – memoization table + recursion stack.

---

### Approach 3: Bottom‑Up State Machine (O(1) space)

**Intuition**  
We only need the best profit achievable **so far** for each of the two states:
- `cash`: maximum profit when we do **not** hold a stock after day `i`.
- `hold`: maximum profit when we **do** hold a stock after day `i`.

Transition equations:
- `cash = max(cash, hold + price - fee)`  
  (we either stay in cash, or we sell the stock we were holding)
- `hold = max(hold, cash - price)`  
  (we either keep holding, or we buy a stock using the cash we had yesterday)

Initial values:
- Before any day we have `cash = 0` (no profit, no stock).
- `hold = -∞` (impossible to hold a stock without buying).

After processing all days, the answer is `cash` because we must end without holding a stock.

**Implementation**

```python
from typing import List

class Solution:
    def maxProfit(self, prices: List[int], fee: int) -> int:
        cash, hold = 0, float('-inf')          # day 0: no stock, impossible to hold
        for price in prices:
            # compute new values based on yesterday's states
            new_cash = max(cash, hold + price - fee)   # sell or rest
            new_hold = max(hold, cash - price)         # buy or rest
            cash, hold = new_cash, new_hold
        return cash
```

**Complexity Analysis**

- Time complexity: **O(n)** – single pass through the price array.  
- Space complexity: **O(1)** – only two scalar variables are used.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimal O(1) solution on the example  
`prices = [1, 3, 2, 8, 4, 9]`, `fee = 2`.

| Day | price | cash (prev) | hold (prev) | cash = max(cash, hold+price‑fee) | hold = max(hold, cash‑price) | cash (new) | hold (new) |
|-----|-------|-------------|-------------|----------------------------------|------------------------------|------------|------------|
| 0   | 1     | 0           | -∞          | max(0, -∞) = 0                   | max(-∞, 0‑1) = -1            | 0          | -1         |
| 1   | 3     | 0           | -1          | max(0, -1+3‑2=0) = 0             | max(-1, 0‑3=-3) = -1         | 0          | -1         |
| 2   | 2     | 0           | -1          | max(0, -1+2‑2=-1) = 0            | max(-1, 0‑2=-2) = -1         | 0          | -1         |
| 3   | 8     | 0           | -1          | max(0, -1+8‑2=5) = 5             | max(-1, 0‑8=-8) = -1         | 5          | -1         |
| 4   | 4     | 5           | -1          | max(5, -1+4‑2=1) = 5             | max(-1, 5‑4=1) = 1           | 5          | 1          |
| 5   | 9     | 5           | 1           | max(5, 1+9‑2=8) = 8              | max(1, 5‑9=-4) = 1           | 8          | 1          |

Final `cash = 8`, which matches the expected output.

---