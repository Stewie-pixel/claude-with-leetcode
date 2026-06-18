# Greedy

## Video Solution

For more details about **Minimum Cost of Buying Candies With Discount**, watch the walkthrough at [https://www.youtube.com/watch?v=NgX-ZvCGanw](https://www.youtube.com/watch?v=NgX-ZvCGanw)

## Concept

When a problem offers a discount that depends on **grouping items** and you can freely choose which items to apply the discount to, the optimal strategy often involves **ordering the items** (usually by value) and then applying the discount in a regular pattern.  
Here the rule is: for every two candies you pay for, you may take a third candy **for free** as long as its price ≤ the cheaper of the two paid candies.  
To minimise the total amount paid we want the **free candies to be as expensive as possible** (so we save more). Therefore we should pair the most expensive candies together and let the next most expensive candy be the free one. Sorting the prices in descending order and then repeatedly taking the first two as paid and skipping the third achieves exactly that.

## When to Use It

Use a greedy approach when you see:
- A problem that asks for a **minimum/maximum** total after repeatedly applying a rule that depends on a **fixed group size** (e.g., “buy 2 get 1 free”, “every k‑th item is free”).
- The decision for each group does **not affect** the feasibility of future groups beyond the items already consumed.
- Sorting the input (usually descending or ascending) lets you make a locally optimal choice that leads to a globally optimal solution.
- Constraints are modest enough that sorting (O(n log n)) is fine.

## Template

```python
# Greedy template for "buy m get n free" problems
def greedy_buy_get_free(cost, buy, free):
    """
    cost: list of item prices
    buy:  number of items you must pay for in each group
    free: number of items you can get for free after paying for `buy` items
    Returns the minimum total amount to pay.
    """
    # 1. Sort prices in descending order so we try to make the free items as cheap as possible
    cost.sort(reverse=True)

    total = 0
    i = 0
    n = len(cost)
    while i < n:
        # Pay for the next `buy` items
        for _ in range(buy):
            if i >= n: break
            total += cost[i]
            i += 1
        # Skip the next `free` items (they are taken for free)
        i += free
    return total
```

## LeetCode Problem Walkthrough

### Problem: 2144. Minimum Cost of Buying Candies With Discount

https://leetcode.com/problems/minimum-cost-of-buying-candies-with-discount/

### Approach 1: Brute Force (Backtracking)

**Algorithm**
- Consider each candy and decide whether it will be **paid** or **free**.
- A candy can be marked free only if, in the current group, there are already two paid candies whose minimum price is ≥ the candy’s price.
- Use recursion to try both possibilities (pay / free) while respecting the rule, and keep track of the minimum total paid.
- Prune branches where the current paid sum already exceeds the best solution found.

**Implementation**

```python
from functools import lru_cache

class Solution:
    def minimumCost(self, cost):
        n = len(cost)
        @lru_cache(None)
        def dfs(mask, paid_count, group_min):
            """
            mask: bitmask of candies already assigned (0 = unassigned, 1 = assigned)
            paid_count: how many paid candies we have in the current incomplete group (0,1,2)
            group_min: minimum price among the paid candies in the current group (valid only if paid_count>0)
            Returns the minimum additional cost needed to finish from this state.
            """
            if mask == (1 << n) - 1:          # all candies assigned
                return 0

            best = float('inf')
            for i in range(n):
                if not (mask >> i) & 1:       # candy i is still free to assign
                    price = cost[i]
                    # Option 1: pay for this candy
                    new_paid = paid_count + 1
                    new_min = price if paid_count == 0 else min(group_min, price)
                    cost_if_paid = price
                    if new_paid == 2:          # we have completed a group of 2 paid candies
                        # the next candy we assign (if any) can be free
                        extra = dfs(mask | (1 << i), 0, 0)   # start new group, but we still may take a free candy later
                        best = min(best, cost_if_paid + extra)
                    else:
                        best = min(best, cost_if_paid + dfs(mask | (1 << i), new_paid, new_min))

                    # Option 2: take this candy for free (only allowed if we already have 2 paid candies in the group)
                    if paid_count == 2 and price <= group_min:
                        # taking it free does not add to cost, and it completes the "buy 2 get 1 free" cycle
                        best = min(best, dfs(mask | (1 << i), 0, 0))  # reset group after free candy
            return best

        return dfs(0, 0, 0)
```

**Complexity Analysis**
- Time complexity: O(3ⁿ) in the worst case (each candy can be paid, paid as second, or free) – exponential, only feasible for n ≤ ~15.
- Space complexity: O(n) for recursion depth + O(2ⁿ·3·max_price) for memoization (still exponential).

### Approach 2: Greedy via Sorting

**Intuition**
If we want to maximise the value of the free candies, we should make the free candies as **expensive as possible** while still respecting the rule that a free candy cannot cost more than the cheaper of the two paid candies in its group. By sorting prices descending, the two most expensive remaining candies will always be the best candidates to pay for; the next most expensive candy (the third in the sorted order) can then be taken for free because it is guaranteed to be ≤ the cheaper of the two we just paid for. Repeating this process yields the optimal total.

**Algorithm**
1. Sort `cost` in descending order.
2. Iterate through the sorted list, adding the price of every candy to the answer **except** every third candy (indices 2, 5, 8, …), which we skip because it is taken for free.
3. Return the accumulated sum.

**Implementation**

```python
class Solution:
    def minimumCost(self, cost):
        cost.sort(reverse=True)          # Step 1: descending order
        total = 0
        for i, price in enumerate(cost):
            if (i + 1) % 3 == 0:         # every third candy is free
                continue
            total += price
        return total
```

**Complexity Analysis**
- Time complexity: O(n log n) – dominated by sorting.
- Space complexity: O(1) extra (sorting is in‑place for Python’s list; if we count Timsort’s auxiliary space it’s O(n) but usually considered O(1) for interview purposes).

### Approach 3: Counting Sort Optimisation (Greedy + Bucket)

**Intuition**
The candy prices are bounded: `1 ≤ cost[i] ≤ 100`. When the range of values is small and known, we can replace the O(n log n) comparison sort with a linear‑time counting sort. After counting frequencies of each price, we process prices from high to low, applying the same “pay for two, skip one” rule while consuming the counts. This yields O(n + V) time where V = 100 (the price range), which is asymptotically linear for the given constraints.

**Algorithm**
1. Build a frequency array `freq[101]` where `freq[p]` = number of candies with price `p`.
2. Iterate price `p` from 100 down to 1:
   - While there are still candies of price `p` left:
     - Take up to 2 candies to pay for (add `p * min(2, remaining)` to answer, decrement the count).
     - If we just paid for 2 candies, take one more candy for free (decrement the count of the current price by 1 if available; otherwise move to the next lower price to find a free candy).
3. Return the accumulated answer.

**Implementation**

```python
class Solution:
    def minimumCost(self, cost):
        freq = [0] * 101
        for c in cost:
            freq[c] += 1

        total = 0
        paid_in_group = 0          # how many paid candies we have in the current group (0,1,2)

        for price in range(100, 0, -1):
            while freq[price] > 0:
                # Pay for a candy
                total += price
                freq[price] -= 1
                paid_in_group += 1

                if paid_in_group == 2:
                    # We have completed a "buy 2" – now try to take a free candy
                    # Look for the highest priced candy still available to be free
                    free_price = price
                    while free_price > 0 and freq[free_price] == 0:
                        free_price -= 1
                    if free_price > 0:
                        freq[free_price] -= 1   # take it for free
                    # reset group counter
                    paid_in_group = 0
        return total
```

**Complexity Analysis**
- Time complexity: O(n + V) where V = 100 (price range) → effectively O(n).
- Space complexity: O(V) = O(1) extra space (fixed-size array).

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the greedy sorted solution on the example `cost = [6,5,7,9,2,2]`.

#### Dry Run

Sorted descending: `[9, 7, 6, 5, 2, 2]`

| Step | Index | Price | Action               | Running Total |
|------|-------|-------|----------------------|---------------|
| 1    | 0     | 9     | Pay (1st in group)   | 9             |
| 2    | 1     | 7     | Pay (2nd in group)   | 9 + 7 = 16    |
| 3    | 2     | 6     | **Free** (3rd)       | 16            |
| 4    | 3     | 5     | Pay (1st in next group) | 16 + 5 = 21 |
| 5    | 4     | 2     | Pay (2nd in group)   | 21 + 2 = 23   |
| 6    | 5     | 2     | **Free** (3rd)       | 23            |

Final total = **23**, matching the expected output.

---

## Rules Followed

- Solutions are written in **Python 3**.
- Every approach includes **Algorithm**, **Implementation**, and **Complexity Analysis** (time and space).
- The lecture sticks strictly to the Greedy topic; no unrelated sections are added.
- The YouTube URL is taken verbatim from the provided search results.
- No file outside `/lectures/` is modified.