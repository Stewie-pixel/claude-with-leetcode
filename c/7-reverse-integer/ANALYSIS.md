# Math & Geometry

## Video Solution

For more details about **Reverse Integer**, watch the walkthrough at [https://www.youtube.com/watch?v=WCmavcsVw4A](https://www.youtube.com/watch?v=WCmavcsVw4A)

## Concept

Reversing an integer means taking its decimal digits and writing them in the opposite order.  
A signed 32‑bit integer lives in the range **[−2³¹, 2³¹ − 1] = [−2147483648, 2147483647]**.  
If the reversed number would fall outside this range we must return 0.  
Because we cannot rely on 64‑bit storage, we have to check for overflow **while** we build the reversed number, not after.

## When to Use It

Use integer‑reversal logic when you see:
- “reverse the digits” or “palindrome” style problems
- Need to manipulate a number digit‑by‑digit without converting to a string
- Overflow checks are required (common in low‑level language interviews)

## Template

```python
def reverse_integer(x: int) -> int:
    """
    Template for digit‑by‑digit reversal with overflow protection.
    Replace the placeholder logic with the specific approach.
    """
    sign = -1 if x < 0 else 1
    x_abs = abs(x)
    rev = 0
    INT_MAX = 2**31 - 1   # 2147483647
    INT_MIN = -2**31      # -2147483648

    while x_abs != 0:
        pop = x_abs % 10          # extract last digit
        x_abs //= 10              # drop last digit

        # ---- overflow check goes here ----
        # (depends on the approach)

        rev = rev * 10 + pop      # append digit

    return sign * rev
```

---

## LeetCode Problem Walkthrough

### Problem: 7. Reverse Integer

https://leetcode.com/problems/reverse-integer/

### Approach 1: Brute Force – String Conversion

**Algorithm**  
1. Record the sign of `x`.  
2. Work with the absolute value, convert it to a string, reverse the string, and convert back to an integer.  
3. Re‑apply the sign.  
4. If the result lies outside the 32‑bit signed range, return 0; otherwise return the result.

**Implementation**

```python
def reverse(x: int) -> int:
    sign = -1 if x < 0 else 1
    rev_str = str(abs(x))[::-1]          # reverse the digit string
    rev = int(rev_str) * sign

    # 32‑bit overflow check
    if rev < -2**31 or rev > 2**31 - 1:
        return 0
    return rev
```

**Complexity Analysis**

- Time complexity: **O(d)** where *d* is the number of digits (at most 10 for 32‑bit ints). Converting to string and reversing each digit is linear.
- Space complexity: **O(d)** for the temporary string representation.

---

### Approach 2: Optimized – Arithmetic with Pre‑Overflow Check

**Intuition**  
Building the reversed number digit by digit lets us detect overflow **before** it happens.  
Before we do `rev = rev * 10 + pop`, we know that `rev * 10` must stay within the 32‑bit limits.  
If `rev` is already greater than `INT_MAX // 10`, then `rev * 10` will overflow.  
If `rev` equals `INT_MAX // 10`, we can only safely add a digit `pop` that is ≤ `INT_MAX % 10` (which is 7).  
The same logic applies symmetrically for negative numbers using `INT_MIN`.

**Algorithm**  
1. Extract the sign and work with `|x|`.  
2. For each digit `pop` from the right:
   - Check if `rev > INT_MAX // 10` or (`rev == INT_MAX // 10` and `pop > 7`).  
   - If true, return 0 (overflow would occur).  
   - Otherwise update `rev = rev * 10 + pop`.  
3. Apply the original sign to `rev` and return.

**Implementation**

```python
def reverse(x: int) -> int:
    INT_MAX = 2**31 - 1   # 2147483647
    INT_MIN = -2**31      # -2147483648

    rev = 0
    while x != 0:
        # In Python, the modulo of a negative number yields a negative remainder,
        # so we work with the absolute value and keep track of the sign separately.
        pop = x % (-10 if x < 0 else 10)   # get last digit with correct sign
        # Truncate toward zero like C/C++ integer division
        x = int(x / 10)                    # same as x // 10 for positive, but truncates toward zero for negative

        # Overflow check for positive side
        if rev > INT_MAX // 10 or (rev == INT_MAX // 10 and pop > 7):
            return 0
        # Overflow check for negative side
        if rev < INT_MIN // 10 or (rev == INT_MIN // 10 and pop < -8):
            return 0

        rev = rev * 10 + pop

    return rev
```

**Complexity Analysis**

- Time complexity: **O(d)** – each iteration removes one digit.
- Space complexity: **O(1)** – only a few integer variables are used.

---

### Approach 3: Most Optimal – Early Exit Using Limits (Same as Approach 2)

For this problem the arithmetic method with pre‑overflow checks is already optimal; there is no asymptotically faster algorithm because we must inspect every digit at least once.  
Thus Approach 2 is presented as the most optimal solution.  
(If desired, one could unroll the loop for a fixed maximum of 10 iterations, but that does not change asymptotic complexity.)

**Implementation** – identical to Approach 2 above.

**Complexity Analysis** – same as Approach 2.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Input: `x = -123`

We'll trace the arithmetic version (Approach 2).  
`INT_MAX = 2147483647`, `INT_MIN = -2147483648`.

| Step | x (before) | pop (last digit) | x (after trunc) | rev (before) | Overflow check | rev (after) |
|------|------------|------------------|-----------------|--------------|----------------|-------------|
| 1    | -123       | -3               | -12             | 0            | rev (0) > 214748364? No; rev == … and pop < -8? No | -3 |
| 2    | -12        | -2               | -1              | -3           | rev (-3) > …? No; rev == …? No | -32 |
| 3    | -1         | -1               | 0               | -32          | rev (-32) > …? No; rev == …? No | -321 |
| 4    | 0          | loop ends        |                 | -321         |                | -321 |

Result: `-321`, which is within limits, so we return `-321`.

If we tried `x = 1534236469` (which reversed would be `9646324351 > INT_MAX`), the overflow check would fire on the step where `rev = 964632435` and next `pop = 1`; since `rev > INT_MAX // 10` (`964632435 > 214748364`), we return 0.

---