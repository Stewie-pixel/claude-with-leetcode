# Arrays & Hashing

## Video Solution

For more details about **Maximum Ice Cream Bars**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=H1z-rfBlLBo).

## Concept

When we want to maximize the number of items we can buy given a fixed budget, the optimal strategy is to always purchase the cheapest available items first. This greedy choice works because any solution that skips a cheaper item in favor of a more expensive one can be improved by swapping them, yielding at least as many items for the same or lower cost.  
To efficiently access items in increasing order of price we can sort the prices. When the price range is limited (as in this problem, where `costs[i] ≤ 10⁵`), we can replace comparison‑based sorting with **counting sort**, which runs in linear time relative to the number of items plus the maximum price.

## When to Use It

Use counting sort (or frequency‑array) techniques when you see:
- A request to maximize/minimize the number of items under a sum constraint.
- The values are integers with a known, not‑too‑large maximum (here ≤ 10⁵).
- The problem explicitly mentions “you must solve by counting sort” or hints at a frequency‑based solution.

## Template

```python
# frequency array of size (max_value + 1)
freq = [0] * (max_val + 1)
for v in values:
    freq[v] += 1

# iterate from smallest to largest value
answer = 0
remaining = budget
for price in range(1, max_val + 1):
    # how many of this price we can afford
    can_take = min(freq[price], remaining // price)
    answer += can_take
    remaining -= can_take * price
    if remaining < price:          # no further purchases possible
        break
```

## LeetCode Problem Walkthrough

### Problem: 1833. Maximum Ice Cream Bars  
https://leetcode.com/problems/maximum-ice-cream-bars/

---

### Approach 1: Brute Force (Original Order)

**Algorithm**  
Traverse the `costs` array in the given order. Whenever the current bar’s price is ≤ the remaining coins, buy it and deduct the price. Continue until the end.  
This does **not** guarantee the maximum count because a costly early bar could prevent buying several cheaper later bars.

**Implementation**

```python
class Solution:
    def maxIceCream(self, costs: List[int], coins: int) -> int:
        count = 0
        for price in costs:
            if price <= coins:
                count += 1
                coins -= price
        return count
```

**Complexity Analysis**  
- Time complexity: O(n) — single pass through the array.  
- Space complexity: O(1) — only a few variables.

---

### Approach 2: Sort + Greedy

**Intuition**  
If we process the ice‑cream bars from cheapest to most expensive, we never waste money on a costly bar when a cheaper one is still available. Sorting guarantees this order.

**Algorithm**  
1. Sort `costs` in non‑decreasing order.  
2. Iterate through the sorted list, buying each bar while we have enough coins.  
3. Stop when the next bar’s price exceeds the remaining coins.

**Implementation**

```python
class Solution:
    def maxIceCream(self, costs: List[int], coins: int) -> int:
        costs.sort()                     # O(n log n)
        count = 0
        for price in costs:
            if price <= coins:
                count += 1
                coins -= price
            else:
                break
        return count
```

**Complexity Analysis**  
- Time complexity: O(n log n) — dominated by the sorting step.  
- Space complexity: O(1) — sorting is in‑place (Python’s Timsort uses O(n) worst‑case auxiliary space, but we treat it as O(1) for interview purposes; if strict, note O(n)).  

---

### Approach 3: Counting Sort + Greedy (Optimal)

**Intuition**  
Since each `costs[i]` is bounded (≤ 10⁵), we can count how many bars exist at each price using a frequency array. Then we walk prices from 1 upward, taking as many as we can afford at each price. This avoids the O(n log n) comparison sort and runs in linear time relative to the number of bars plus the maximum price.

**Algorithm**  
1. Find `max_cost = max(costs)`.  
2. Build `freq` of size `max_cost + 1`; increment `freq[c]` for each cost `c`.  
3. Initialize `bought = 0` and `remaining = coins`.  
4. For `price` from 1 to `max_cost`:  
   - `can_take = min(freq[price], remaining // price)`  
   - Increase `bought` by `can_take` and decrease `remaining` by `can_take * price`.  
   - If `remaining < price`, break early – no further purchases possible.  
5. Return `bought`.

**Implementation**

```python
class Solution:
    def maxIceCream(self, costs: List[int], coins: int) -> int:
        if not costs:
            return 0

        max_cost = max(costs)
        freq = [0] * (max_cost + 1)
        for c in costs:
            freq[c] += 1

        bought = 0
        remaining = coins
        for price in range(1, max_cost + 1):
            if remaining < price:
                break
            # maximum number of bars at this price we can afford
            can_take = min(freq[price], remaining // price)
            bought += can_take
            remaining -= can_take * price

        return bought
```

**Complexity Analysis**  
- Time complexity: O(n + max_cost) — one pass to build the frequency array and one pass over the price range.  
- Space complexity: O(max_cost) — the frequency array.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: `costs = [1,3,2,4,1]`, `coins = 7`

| Step | price | freq[price] | remaining before | can_take = min(freq, remaining//price) | bought after | remaining after |
|------|-------|-------------|------------------|----------------------------------------|--------------|-----------------|
| 1    | 1     | 2           | 7                | min(2, 7//1)=2                         | 2            | 7 - 2*1 = 5     |
| 2    | 2     | 1           | 5                | min(1, 5//2)=1                         | 3            | 5 - 1*2 = 3     |
| 3    | 3     | 1           | 3                | min(1, 3//3)=1                         | 4            | 3 - 1*3 = 0     |
| 4    | 4     | 1           | 0                | remaining < price → break              | 4            | 0               |

The boy buys 4 ice‑cream bars, matching the expected output.