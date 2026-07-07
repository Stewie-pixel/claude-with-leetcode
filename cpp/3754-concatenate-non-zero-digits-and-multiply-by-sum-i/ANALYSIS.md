#Math & Geometry

## Video Solution

For more details about **Concatenate Non-Zero Digits and Multiply by Sum I**, watch the walkthrough at [https://www.youtube.com/watch?v=x3_JwcC9TvA](https://www.youtube.com/watch?v=x3_JwcC9TvA)

## Concept

When a problem asks us to work with the individual digits of an integer, we can treat the number as a sequence of digits.  
A common technique is to **extract each digit** (either by converting to a string or using `% 10` and `/= 10`), process it (e.g., skip zeros, accumulate a sum, build a new number), and then combine the results.  
For this problem we need two pieces of information from the same digit pass:
1. The non‑zero digits concatenated in their original order → forms `x`.
2. The sum of those non‑zero digits → forms `sum`.

We can obtain both in a single linear scan, keeping running values for `x` (by shifting left one decimal place and adding the digit) and `sum`.

## When to Use It

Use digit‑by‑digit processing when you see:
- Manipulation of individual digits (e.g., sum of digits, product of digits, rearranging digits).
- Building a new number from selected digits.
- Constraints that guarantee at most ~10 digits (since `n ≤ 10⁹`), making an O(number of digits) solution trivial.
- No need for advanced data structures; simple arithmetic or string conversion suffices.

## Template

```python
def process_digits(n: int):
    """Extract digits of n and perform custom logic.
    Replace the body with the specific computation needed.
    """
    x = 0          # result built from selected digits
    total = 0      # accumulator (e.g., sum)
    while n > 0:
        d = n % 10          # current least‑significant digit
        n //= 10
        # <-- put your digit‑specific logic here -->
        # e.g., if d != 0: x = x * 10 + d; total += d
    # If you processed digits from least to most significant and need original order,
    # you may need to reverse x or build it differently (see implementation below).
    return x, total
```

When preserving the original left‑to‑right order is required (as in this problem), it is often simpler to convert the number to a string and iterate from left to right, or to collect digits in a list and then reconstruct.

## LeetCode Problem Walkthrough

### Problem: 3754. Concatenate Non-Zero Digits and Multiply by Sum I

https://leetcode.com/problems/concatenate-non-zero-digits-and-multiply-by-sum-i/

### Approach 1: String Conversion (Straightforward)

**Algorithm**
1. Convert `n` to a string to easily traverse digits in their original order.
2. Initialise `x = 0` (the concatenated number) and `digit_sum = 0`.
3. For each character `c` in the string:
   - If `c` is not `'0'`:
     - Convert `c` to its integer value `d`.
     - Append `d` to `x`: `x = x * 10 + d`.
     - Add `d` to `digit_sum`.
4. Return `x * digit_sum`.  
   (If no non‑zero digits were found, both `x` and `digit_sum` stay `0`, and the product is `0`.)

**Implementation**

```python
class Solution:
    def concatenatedSum(self, n: int) -> int:
        s = str(n)
        x = 0
        digit_sum = 0
        for ch in s:
            if ch != '0':
                d = int(ch)
                x = x * 10 + d
                digit_sum += d
        return x * digit_sum
```

**Complexity Analysis**

- Time complexity: O(k) — where k is the number of digits in `n` (at most 10 for `n ≤ 10⁹`). We scan each digit once.
- Space complexity: O(k) — the string representation of `n` uses linear space in the number of digits. (Auxiliary variables are O(1).)

### Approach 2: Integer Arithmetic (No Extra String)

**Intuition**  
Converting to a string is simple and clear, but we can avoid the allocation by extracting digits mathematically.  
To keep the original left‑to‑right order while using `% 10` (which gives digits from right to left), we first collect the non‑zero digits in a list, then reconstruct `x` by iterating the list in reverse. This still uses only O(k) extra space for the list (at most 10 integers) and avoids creating a full string.

**Algorithm**
1. Handle the edge case `n == 0` → return `0`.
2. Initialise an empty list `digits` to store non‑zero digits.
3. While `n > 0`:
   - Extract the last digit `d = n % 10`.
   - If `d != 0`, append `d` to `digits`.
   - Remove the last digit: `n //= 10`.
4. After the loop, `digits` holds the non‑zero digits in **reverse** order.
5. Initialise `x = 0` and `digit_sum = 0`.
6. Iterate over `digits` in reverse (to restore original order):
   - For each `d`: `x = x * 10 + d` and `digit_sum += d`.
7. Return `x * digit_sum`.

**Implementation**

```python
class Solution:
    def concatenatedSum(self, n: int) -> int:
        if n == 0:
            return 0

        digits = []
        while n > 0:
            d = n % 10
            if d != 0:
                digits.append(d)
            n //= 10

        x = 0
        digit_sum = 0
        for d in reversed(digits):   # restore original left‑to‑right order
            x = x * 10 + d
            digit_sum += d

        return x * digit_sum
```

**Complexity Analysis**

- Time complexity: O(k) — each digit is processed a constant number of times (once for extraction, once for reconstruction).
- Space complexity: O(k) — the list `digits` stores at most the number of digits of `n` (≤ 10). This is still constant for the given constraints, but technically linear in the number of digits.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the example `n = 10203004`.

#### Dry Run (String‑based Approach)

| Step | ch | ch != '0'? | d | x (before) | x = x*10 + d | digit_sum (before) | digit_sum += d |
|------|----|------------|---|------------|--------------|--------------------|----------------|
| 1    | '1'| Yes        | 1 | 0          | 1            | 0                  | 1              |
| 2    | '0'| No         | – | 1          | 1            | 1                  | 1              |
| 3    | '2'| Yes        | 2 | 1          | 12           | 1                  | 3              |
| 4    | '0'| No         | – | 12         | 12           | 3                  | 3              |
| 5    | '3'| Yes        | 3 | 12         | 123          | 3                  | 6              |
| 6    | '0'| No         | – | 123        | 123          | 6                  | 6              |
| 7    | '0'| No         | – | 123        | 123          | 6                  | 6              |
| 8    | '4'| Yes        | 4 | 123        | 1234         | 6                  | 10             |

Final values: `x = 1234`, `digit_sum = 10` → answer = `1234 * 10 = 12340`.

#### Dry Run (Integer‑Arithmetic Approach)

Extraction phase (building `digits` from least significant side):

| Iteration | n (before) | d = n % 10 | d != 0? | digits (after) | n //= 10 |
|-----------|------------|-----------|---------|----------------|----------|
| 1         | 10203004   | 4         | Yes     | [4]            | 1020300  |
| 2         | 1020300    | 0         | No      | [4]            | 102030   |
| 3         | 102030     | 0         | No      | [4]            | 10203    |
| 4         | 10203      | 3         | Yes     | [4, 3]         | 1020     |
| 5         | 1020       | 0         | No      | [4, 3]         | 102      |
| 6         | 102        | 2         | Yes     | [4, 3, 2]      | 10       |
| 7         | 10         | 0         | No      | [4, 3, 2]      | 1        |
| 8         | 1          | 1         | Yes     | [4, 3, 2, 1]   | 0        |

Now `digits = [4, 3, 2, 1]` (reverse of desired order).  
Reconstruction phase (iterate reversed list):

| Step | d (from reversed) | x (before) | x = x*10 + d | digit_sum (before) | digit_sum += d |
|------|-------------------|------------|--------------|--------------------|----------------|
| 1    | 1                 | 0          | 1            | 0                  | 1              |
| 2    | 2                 | 1          | 12           | 1                  | 3              |
| 3    | 3                 | 12         | 123          | 3                  | 6              |
| 4    | 4                 | 123        | 1234         | 6                  | 10             |

Result: `x = 1234`, `, `, `digit_sum = 10` → product = `12340`.

--- 

This lecture follows the required format, presents two approaches (naive string conversion and integer‑array reconstruction), includes intuition for the optimized version, gives full complexity analysis, and provides a concrete dry‑run trace. The topic is **Math & Geometry** (digit manipulation). The YouTube URL is taken verbatim from the supplied search results. No extra files were modified.