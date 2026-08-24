# Math & Geometry (Digit Operations)

## Video Solution

For more details about **Check Divisibility by Digit Sum and Product**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=GdR5eLOJfjA).

## Concept

This technique isolates each digit of an integer so we can compute aggregate values like the **digit sum** (addition of all digits) and the **digit product** (multiplication of all digits).  
Think of it like counting coins in a pile: you repeatedly take the top coin (the least‑significant digit), record its value, and remove it from the pile until the pile is empty.

## When to Use It

Use digit‑extraction when you see:
- Problems that mention “sum of digits”, “product of digits”, or any property that depends on individual decimal digits.
- Constraints where the number length is small (≤ 7 digits for n ≤ 10⁶), making a linear scan over digits trivial.
- Situations where you need to transform a number into its digit‑based representation without extra libraries.

## Template

```python
def digit_sum_and_product(n: int) -> tuple[int, int]:
    """Return (sum of digits, product of digits) of n."""
    digit_sum = 0
    digit_product = 1          # start with 1 so multiplication works
    while n > 0:
        digit = n % 10         # extract least‑significant digit
        n //= 10               # drop the extracted digit
        digit_sum += digit
        digit_product *= digit
    return digit_sum, digit_product
```

## LeetCode Problem Walkthrough

### Problem: 3622. Check Divisibility by Digit Sum and Product

https://leetcode.com/problems/check-divisibility-by-digit-sum-and-product/

### Approach 1: Brute Force (Iterative Digit Extraction)

**Algorithm**  
1. Preserve the original value of `n` for the final divisibility test.  
2. Initialise `digit_sum = 0` and `digit_product = 1`.  
3. Repeatedly extract the last digit using `% 10`, add it to `digit_sum`, multiply it into `digit_product`, and remove the digit with `//= 10`.  
4. After the loop, compute `total = digit_sum + digit_product`.  
5. Return `True` if `original % total == 0`, else `False`.

**Implementation**

```python
class Solution:
    def checkDivisibility(self, n: int) -> bool:
        original = n
        digit_sum = 0
        digit_product = 1

        while n > 0:
            digit = n % 10          # get current digit
            n //= 10                # shift right
            digit_sum += digit
            digit_product *= digit

        total = digit_sum + digit_product
        return original % total == 0
```

**Complexity Analysis**  
- Time complexity: O(d) — we visit each digit once; d ≤ 7 for n ≤ 10⁶.  
- Space complexity: O(1) — only a few integer variables are used.

### Approach 2: String‑Based Digit Extraction (Readability Focus)

**Intuition**  
Converting the number to a string lets us iterate over its characters directly, which can be clearer and avoids manual modulus/division steps. The asymptotic cost stays the same because we still process each digit once.

**Algorithm**  
1. Convert `n` to a string.  
2. Initialise `digit_sum = 0` and `digit_product = 1`.  
3. For each character `c` in the string: convert to int, update sum and product.  
4. Compute `total = digit_sum + digit_product` and test divisibility of the original number.

**Implementation**

```python
class Solution:
    def checkDivisibility(self, n: int) -> bool:
        s = str(n)
        digit_sum = 0
        digit_product = 1

        for ch in s:
            d = int(ch)
            digit_sum += d
            digit_product *= d

        total = digit_sum + digit_product
        return n % total == 0
```

**Complexity Analysis**  
- Time complexity: O(d) — one pass over the string of length d.  
- Space complexity: O(d) — the string representation occupies O(d) extra space (still ≤ 7 characters).

### Approach 3: Early Exit on Zero Product (Micro‑optimisation)

**Intuition**  
If any digit is zero, the digit product becomes zero, making `total = digit_sum + 0 = digit_sum`.  
We can still compute the sum normally, but we know the product contribution is zero, so we could skip further multiplication once a zero is seen. This saves a few operations for numbers containing a zero digit.

**Algorithm**  
1. Initialise `digit_sum = 0`, `digit_product = 1`, and a flag `zero_seen = False`.  
2. Extract digits as in Approach 1.  
3. If the extracted digit is zero, set `zero_seen = True` and set `digit_product = 0` (since product will stay zero).  
4. Otherwise, multiply the digit into `digit_product` only if we haven’t seen a zero yet.  
5. After the loop, `total = digit_sum + digit_product` (product will be zero if any zero digit appeared).  
6. Return the divisibility test.

**Implementation**

```python
class Solution:
    def checkDivisibility(self, n: int) -> bool:
        original = n
        digit_sum = 0
        digit_product = 1
        zero_seen = False

        while n > 0:
            digit = n % 10
            n //= 10
            digit_sum += digit
            if digit == 0:
                zero_seen = True
                digit_product = 0          # product will stay zero
            elif not zero_seen:
                digit_product *= digit     # only update product while no zero seen

        total = digit_sum + digit_product
        return original % total == 0
```

**Complexity Analysis**  
- Time complexity: O(d) — still linear in the number of digits; the early exit only saves a constant factor.  
- Space complexity: O(1) — constant extra space.

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the sample input `n = 99` using the iterative digit‑extraction approach (Approach 1).

## Dry Run

Input: `n = 99`

| Step | n (before) | digit = n % 10 | n //= 10 (after) | digit_sum | digit_product | Action |
|------|------------|----------------|------------------|-----------|---------------|--------|
| 1    | 99         | 9              | 9                | 9         | 9             | add 9, mul 9 |
| 2    | 9          | 9              | 0                | 18        | 81            | add 9, mul 9 |
| 3    | 0          | –              | –                | 18        | 81            | loop ends |

After the loop:  
- `digit_sum = 18`  
- `digit_product = 81`  
- `total = 18 + 81 = 99`  
- `original % total = 99 % 99 = 0` → return **True**.

The table shows how each digit is processed, how the running sum and product evolve, and why the final divisibility test succeeds.