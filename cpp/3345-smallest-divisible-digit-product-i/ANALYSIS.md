# Math & Geometry

## Video Solution

For more details about **Smallest Divisible Digit Product I**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=ifWhEVV6BK8).

## Concept

The problem asks for the smallest integer `x ≥ n` such that the product of the digits of `x` is divisible by `t`.  
Because `t` is at most 10, we can simply test numbers starting from `n` upward until we find one that satisfies the condition. The key observation is that checking the digit product of a number is cheap (linear in the number of digits), so a straightforward linear scan is fast enough for the given constraints.

## When to Use It

Use a **brute‑force scan** when:
- The search space is small or the answer is expected to be close to the start value.
- Checking the condition for a candidate is inexpensive (e.g., O(number of digits)).
- Constraints allow a linear or near‑linear solution (here `n` ≤ 100, but the method works for much larger values as well).

## Template

Here is a reusable Python template for “find the first number ≥ start that satisfies a predicate”:

```python
def first_satisfying(start: int, predicate) -> int:
    x = start
    while True:
        if predicate(x):
            return x
        x += 1
```

You only need to supply the `predicate` that checks whether the digit product of a number is divisible by `t`.

---

## LeetCode Problem Walkthrough

### Problem: 3345. Smallest Divisible Digit Product I

https://leetcode.com/problems/smallest-divisible-digit-product-i/

### Approach 1: Brute Force – Linear Scan with Digit Product

**Algorithm**
1. Start at `x = n`.
2. Compute the product of the digits of `x`.
   - If any digit is `0`, the product is `0`, which is divisible by any `t`.
3. If the product `% t == 0`, return `x`.
4. Otherwise, increment `x` and repeat.

**Implementation**

```python
class Solution:
    def smallestNumber(self, n: int, t: int) -> int:
        def digit_product(num: int) -> int:
            prod = 1
            while num:
                prod *= num % 10
                num //= 10
            return prod

        x = n
        while True:
            if digit_product(x) % t == 0:
                return x
            x += 1
```

**Complexity Analysis**
- Time complexity: O(k · d) – where `k` is the number of iterations needed to reach the answer and `d` is the number of digits of each examined number (at most log₁₀(x)). In practice `k` is tiny because we stop as soon as we hit a valid number.
- Space complexity: O(1) – only a few integer variables are used.

### Approach 2: Optimized Scan – Skip Numbers That Cannot Work

**Intuition**
If the current number does **not** contain a zero digit, its digit product is non‑zero. For a non‑zero product to be divisible by `t`, the product must contain all prime factors of `t`. Instead of checking every single number, we can jump to the next candidate that could possibly satisfy the divisibility condition by adjusting the last digit. This reduces the number of digit‑product calculations in many cases, though the worst‑case complexity remains the same.

**Algorithm**
1. While `True`:
   - Compute the digit product of `x`.
   - If it is divisible by `t`, return `x`.
   - Otherwise, determine how much we need to add to the *last* digit to make the product divisible by `t`.  
     Let `last = x % 10` and `need = t // gcd(product_without_last, t)`.  
     The smallest `delta` in `[0,9]` such that `(last + delta) % need == 0` is `delta = (need - last % need) % need`.
   - If `last + delta < 10`, set `x = x + delta` (only the last digit changes).
   - Otherwise, we cause a carry: set `x = ((x // 10) + 1) * 10` (i.e., zero out the last digit and increase the higher part).
2. The loop terminates because we eventually reach a number with a zero digit (product = 0) which always works.

**Implementation**

```python
from math import gcd

class Solution:
    def smallestNumber(self, n: int, t: int) -> int:
        def prod_without_last(num: int) -> int:
            """product of all digits except the units place"""
            p = 1
            num //= 10          # discard last digit
            while num:
                p *= num % 10
                num //= 10
            return p

        x = n
        while True:
            last = x % 10
            if last == 0:               # product is zero → always works
                return x

            prod_rest = prod_without_last(x)
            # If rest product is zero (because a higher digit is zero) we are done
            if prod_rest == 0:
                return x

            need = t // gcd(prod_rest, t)   # we need last to be a multiple of `need`
            delta = (need - last % need) % need

            if last + delta < 10:
                return x + delta            # only change the last digit
            else:
                # cause a carry to the next higher digit
                x = ((x // 10) + 1) * 10
```

**Complexity Analysis**
- Time complexity: O(k · d) in the worst case, but often fewer digit‑product evaluations because we jump directly to a promising last digit.
- Space complexity: O(1).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

**Dry Run** (using Approach 1 on the sample `n = 15, t = 3`)

```
| Step | x   | digits | product | product % 3 | Action          |
|------|-----|--------|---------|-------------|-----------------|
| 1    | 15  | 1,5    | 5       | 2           | not ok → x+=1   |
| 2    | 16  | 1,6    | 6       | 0           | ok → return 16  |
```

The algorithm stops at `x = 16`, matching the expected output.