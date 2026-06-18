# Math & Geometry

## Video Solution

For more details about **Check Good Integer**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=fTy0BPhzMoM).

## Concept

This problem asks whether a number is “good” based on a simple property of its digits:  

- Compute `digitSum` = sum of each digit.  
- Compute `squareSum` = sum of each digit squared.  
- The number is good if `squareSum - digitSum >= 50`.

Because the condition only depends on the individual digits, we can examine each digit independently, accumulate the contribution `d² - d` (which equals `d * (d - 1)`), and check whether the total reaches at least 50.

## When to Use It

Use digit‑by‑digit arithmetic when you see:

- A problem that manipulates the decimal digits of an integer (sum, product, sum of squares, etc.).
- Constraints that allow iterating over at most 10 digits (since `n ≤ 10⁹` → at most 10 digits).
- A condition that can be expressed as a sum of per‑digit contributions.

## Template

A reusable Python template for digit‑based problems:

```python
def solve(n: int) -> bool:
    total = 0                     # accumulator for the per‑digit contribution
    while n > 0:                  # iterate over each digit (least‑significant first)
        d = n % 10                # extract current digit
        # --- insert per‑digit logic here ---
        n //= 10                  # drop the processed digit
    # --- final decision based on total ---
    return total >= 50
```

Replace the comment blocks with the specific formula (`d * d - d` for this problem).

---

## LeetCode Problem Walkthrough

### Problem: 3959. Check Good Integer

https://leetcode.com/problems/check-good-integer/

### Approach 1: Brute Force (String Conversion)

**Algorithm**  
1. Convert the integer to a string to easily access each digit.  
2. Initialise `digit_sum` and `square_sum` to 0.  
3. For each character in the string:  
   - Convert it back to an integer `d`.  
   - Add `d` to `digit_sum`.  
   - Add `d * d` to `square_sum`.  
4. After the loop, return `square_sum - digit_sum >= 50`.

**Implementation**

```python
class Solution:
    def checkGoodInteger(self, n: int) -> bool:
        s = str(n)
        digit_sum = 0
        square_sum = 0
        for ch in s:
            d = int(ch)
            digit_sum += d
            square_sum += d * d
        return square_sum - digit_sum >= 50
```

**Complexity Analysis**

- Time complexity: O(k) — where k is the number of digits (≤ 10).  
- Space complexity: O(k) — the string representation of the number.

---

### Approach 2: Optimized Integer Arithmetic

**Intuition**  
Instead of keeping two separate sums, notice that the condition only needs the difference  
`square_sum - digit_sum`. For each digit `d`, its contribution to this difference is `d² - d`.  
Thus we can accumulate a single variable `diff` while extracting digits with modulo/division, avoiding the cost of string conversion.

**Algorithm**  
1. Initialise `diff = 0`.  
2. While `n > 0`:  
   - Extract the last digit `d = n % 10`.  
   - Add `d * d - d` to `diff`.  
   - Remove the last digit: `n //= 10`.  
3. Return `diff >= 50`.

**Implementation**

```python
class Solution:
    def checkGoodInteger(self, n: int) -> bool:
        diff = 0
        while n:
            d = n % 10
            diff += d * d - d   # contribution of this digit to (squareSum - digitSum)
            n //= 10
        return diff >= 50
```

**Complexity Analysis**

- Time complexity: O(k) — one pass over the digits.  
- Space complexity: O(1) — only a few integer variables.

---

### Approach 3: Optimized with Early Exit

**Intuition**  
The per‑digit contribution `d * (d - 1)` is never negative (it is 0 for d=0 or 1, and positive otherwise). Therefore `diff` is monotonic non‑decreasing as we process digits. If at any point `diff` already reaches 50, we can stop early because further digits can only increase (or keep) the value.

**Algorithm**  
Same as Approach 2, but after updating `diff` we check `if diff >= 50: return True`.  
If the loop finishes without early exit, return `diff >= 50`.

**Implementation**

```python
class Solution:
    def checkGoodInteger(self, n: int) -> bool:
        diff = 0
        while n:
            d = n % 10
            diff += d * d - d
            if diff >= 50:          # early termination
                return True
            n //= 10
        return diff >= 50
```

**Complexity Analysis**

- Time complexity: O(k) in the worst case, but often faster because we may exit early.  
- Space complexity: O(1).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Input: `n = 19`

```
| Step | n (before) | d = n%10 | d*d - d | diff (after) | n //=10 |
|------|------------|----------|---------|--------------|----------|
| 1    | 19         | 9        | 72      | 72           | 1        |
| 2    | 1          | 1        | 0       | 72           | 0        |
```

- After processing digit 9, `diff = 72` which already satisfies `≥ 50`, so we could return `True` immediately (Approach 3).  
- The final result is `True`, matching the example.

---