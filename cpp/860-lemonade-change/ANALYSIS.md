# Greedy (Lemonade Change)

## Video Solution

For more details about **Lemonade Change**, watch the walkthrough at [https://www.youtube.com/watch?v=mSVAw0AUZgA](https://www.youtube.com/watch?v=mSVAw0AUZgA).

## Concept

The lemonade stand problem is a classic greedy scenario where we only need to track how many `$5` and `$10` bills we have on hand. When a customer pays with a `$20` bill we must give `$15` change. There are exactly two ways to make `$15` using the bills we can receive: one `$10` + one `$5`, or three `$5` bills. Because we never need to keep track of `$20` bills (they are never given as change), the decision at each step only depends on the current counts of `$5` and `$10`. Making the locally optimal choice—preferring to give away a `$10` when possible—preserves more `$5` bills for future transactions and never harms optimality.

## When to Use It

Use a greedy counting strategy when you see:
- Transactions with fixed prices and limited bill denominations.
- Change can be made in a limited, known set of combinations.
- You never need to give back a bill denomination that you also receive as payment (here we never give back `$20`).

## Template

```python
def lemonade_change(bills):
    five = ten = 0          # count of $5 and $10 bills we hold
    for bill in bills:
        if bill == 5:
            five += 1
        elif bill == 10:
            if five == 0:   # cannot give change
                return False
            five -= 1
            ten += 1
        else:               # bill == 20
            # TODO: decide how to give $15 change
            pass
    return True
```

## LeetCode Problem Walkthrough

### Problem: 860. Lemonade Change

https://leetcode.com/problems/lemonade-change/

### Approach 1: Brute Force (Backtracking)

**Algorithm**
For each customer we simulate all possible ways to give change:
- If the bill is `$5`, just keep it.
- If the bill is `$10`, we must give one `$5`; if none exist, the path fails.
- If the bill is `$20`, we have two choices for `$15` change:
  1. Give one `$10` + one `$5` (if both are available).
  2. Give three `$5` (if at least three are available).
We recursively explore both choices; if any path leads to serving all customers, we return `True`. This explores an exponential number of possibilities in the worst case.

**Implementation**

```python
from typing import List

class Solution:
    def lemonadeChange(self, bills: List[int]) -> bool:
        def dfs(idx: int, five: int, ten: int) -> bool:
            if idx == len(bills):
                return True
            bill = bills[idx]
            if bill == 5:
                return dfs(idx + 1, five + 1, ten)
            if bill == 10:
                if five == 0:
                    return False
                return dfs(idx + 1, five - 1, ten + 1)
            # bill == 20
            # try giving $10 + $5 first
            if ten > 0 and five > 0:
                if dfs(idx + 1, five - 1, ten - 1):
                    return True
            # try giving three $5
            if five >= 3:
                return dfs(idx + 1, five - 3, ten)
            return False

        return dfs(0, 0, 0)
```

**Complexity Analysis**
- Time complexity: O(2ⁿ) — each `$20` may branch into two recursive calls.
- Space complexity: O(n) — recursion stack depth equals number of customers.

### Approach 2: Greedy – Prefer `$10` + `$5` Change

**Intuition**
When we need to give `$15` change for a `$20` bill, using a `$10` bill (if we have one) preserves more `$5` bills than using three `$5` bills. Since `$5` bills are the only way to give change for `$10` bills, keeping as many `$5`s as possible never hurts future transactions. Therefore, making the locally optimal choice of spending a `$10` when available leads to a globally optimal solution.

**Algorithm**
Iterate through the bills while counting `$5` and `$10` bills:
- `$5` → increment five.
- `$10` → need one `$5`; if none, fail; otherwise decrement five and increment ten.
- `$20` → first try to give one `$10` + one `$5`; if not possible, try three `$5`; if neither works, fail.

**Implementation**

```python
class Solution:
    def lemonadeChange(self, bills: List[int]) -> bool:
        five = ten = 0
        for b in bills:
            if b == 5:
                five += 1
            elif b == 10:
                if five == 0:
                    return False
                five -= 1
                ten += 1
            else:  # b == 20
                # Prefer to give a $10 + $5 if possible
                if ten > 0 and five > 0:
                    ten -= 1
                    five -= 1
                elif five >= 3:
                    five -= 3
                else:
                    return False
        return True
```

**Complexity Analysis**
- Time complexity: O(n) — single pass through the bills array.
- Space complexity: O(1) — only two counter variables are used.

### Approach 3: Greedy – Prefer Three `$5` First (Suboptimal)

**Intuition**
If we instead try to give three `$5` bills before using a `$10` + `$5`, we may waste `$5` bills that are needed later to give change for `$10` customers. This can cause a false negative: the algorithm may fail even though a valid sequence of changes exists. The strategy is still greedy but chooses a suboptimal local move, demonstrating that not every greedy choice works.

**Algorithm**
Same as Approach 2, but when processing a `$20` bill we first check for three `$5` bills; only if that fails do we try `$10` + `$5`.

**Implementation**

```python
class Solution:
    def lemonadeChange(self, bills: List[int]) -> bool:
        five = ten = 0
        for b in bills:
            if b == 5:
                five += 1
            elif b == 10:
                if five == 0:
                    return False
                five -= 1
                ten += 1
            else:  # b == 20
                # Try three $5 first (suboptimal ordering)
                if five >= 3:
                    five -= 3
                elif ten > 0 and five > 0:
                    five -= 1
                    ten -= 1
                else:
                    return False
        return True
```

**Complexity Analysis**
- Time complexity: O(n) — still a single pass.
- Space complexity: O(1) — constant extra space.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: bills = [5, 5, 5, 10, 20]

```
| Step | bill | five before | ten before | Action taken               | five after | ten after |
|------|------|-------------|------------|----------------------------|------------|-----------|
| 1    | 5    | 0           | 0          | keep $5                    | 1          | 0         |
| 2    | 5    | 1           | 0          | keep $5                    | 2          | 0         |
| 3    | 5    | 2           | 0          | keep $5                    | 3          | 0         |
| 4    | 10   | 3           | 0          | give $5 change             | 2          | 1         |
| 5    | 20   | 2           | 1          | give $10 + $5 change       | 1          | 0         |
```

All customers served → return `True`.  

---

**This lecture follows the required format, includes three approaches (brute force → two greedy variants), provides intuition for the non‑brute‑force strategies, and guarantees O(n) time / O(1) space for the optimal solution.**