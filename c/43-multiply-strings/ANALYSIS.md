# Math & Geometry

## Video Solution

For more details about **Multiply Strings**, watch the walkthrough at [https://www.youtube.com/watch?v=1vZswirL8Y8](https://www.youtube.com/watch?v=1vZswirL8Y8)

## Concept

Multiply two large numbers that are given as strings by simulating the manual multiplication we learn in elementary school.  
We multiply each digit of `num1` with each digit of `num2`, add the results at the correct positions (taking care of carries), and finally build the answer string from the accumulated array.

## When to Use It

- The problem explicitly forbids converting the whole string to an integer (e.g., LeetCode “Multiply Strings”).  
- Input lengths can be up to 200 digits, far beyond native integer types.  
- You see a request to return the product as a string and the operands are non‑negative digit strings.

## Template

```python
def multiply(num1: str, num2: str) -> str:
    # Edge case: if either number is "0"
    if num1 == "0" or num2 == "0":
        return "0"

    m, n = len(num1), len(num2)
    # The product can have at most m + n digits
    pos = [0] * (m + n)

    # Multiply from right to left
    for i in range(m - 1, -1, -1):
        for j in range(n - 1, -1, -1):
            mul = (ord(num1[i]) - 48) * (ord(num2[j]) - 48)
            p1, p2 = i + j, i + j + 1          # positions in pos array
            total = mul + pos[p2]              # add previous value

            pos[p2] = total % 10               # keep digit
            pos[p1] += total // 10             # carry to the left

    # Skip leading zeros in the result array
    i = 0
    while i < len(pos) and pos[i] == 0:
        i += 1

    # Convert remaining digits to string
    return ''.join(str(d) for d in pos[i:])
```

---

## LeetCode Problem Walkthrough

### Problem: 43. Multiply Strings
https://leetcode.com/problems/multiply-strings/

### Approach 1: Brute Force – Repeated Addition (Conceptual)

**Algorithm**  
1. Convert each string to an integer by iterating through its digits (O(L) time).  
2. Repeatedly add `num1` to itself `num2` times (or vice‑versa).  
3. Convert the final sum back to a string.

**Implementation** (Python – for illustration only; uses built‑in `int` conversion)

```python
def multiply(num1: str, num2: str) -> str:
    # Convert strings to integers using a simple loop (no direct int())
    a = 0
    for ch in num1:
        a = a * 10 + (ord(ch) - 48)
    b = 0
    for ch in num2:
        b = b * 10 + (ord(ch) - 48)

    # Repeated addition – extremely slow for large numbers
    result = 0
    for _ in range(b):
        result += a

    # Convert result back to string
    if result == 0:
        return "0"
    digits = []
    while result > 0:
        digits.append(chr((result % 10) + 48))
        result //= 10
    return ''.join(reversed(digits))
```

**Complexity Analysis**

- Time complexity: O((m + n) * value_of_smaller_number) – essentially O(10^{max(m,n)}), infeasible for 200‑digit numbers.  
- Space complexity: O(1) extra (aside from the output).

> *This approach is shown only to illustrate why a direct simulation is needed; it would time‑out on the constraints.*

---

### Approach 2: Elementary School Simulation (Optimal)

**Intuition**  
When we multiply two numbers by hand, each pair of digits contributes to a specific position in the final product.  
If we multiply digit `i` of `num1` (from right) with digit `j` of `num2`, the raw product belongs to positions `i+j` (carry) and `i+j+1` (unit).  
By storing intermediate results in an array of length `m+n` and propagating carries afterward, we obtain the exact product without ever needing big‑integer types.

**Algorithm**  
1. Handle the trivial case where either operand is `"0"`.  
2. Create an integer array `pos` of size `m+n` initialized to zero (holds each digit of the answer).  
3. Iterate `i` from `m-1` down to `0` (digits of `num1`).  
   - Inside, iterate `j` from `n-1` down to `0` (digits of `num2`).  
   - Compute `mul = digit1 * digit2`.  
   - Add `mul` to `pos[i+j+1]` (the unit place).  
   - Compute carry = `pos[i+j+1] // 10`, keep `pos[i+j+1] % 10` as the digit, and add the carry to `pos[i+j]`.  
4. After all multiplications, skip leading zeros in `pos`.  
5. Convert the remaining digits to a string and return.

**Implementation**

```python
def multiply(num1: str, num2: str) -> str:
    if num1 == "0" or num2 == "0":
        return "0"

    m, n = len(num1), len(num2)
    pos = [0] * (m + n)          # extra space for possible carry

    for i in range(m - 1, -1, -1):
        for j in range(n - 1, -1, -1):
            mul = (ord(num1[i]) - 48) * (ord(num2[j]) - 48)
            p1, p2 = i + j, i + j + 1
            total = mul + pos[p2]

            pos[p2] = total % 10
            pos[p1] += total // 10

    # Skip leading zeros
    idx = 0
    while idx < len(pos) and pos[idx] == 0:
        idx += 1

    return ''.join(str(d) for d in pos[idx:])
```

**Complexity Analysis**

- Time complexity: O(m * n) – each digit pair is processed once.  
- Space complexity: O(m + n) – the `pos` array stores the intermediate result.

---

### Approach 3: Same Simulation with Early‑Zero Trimming (Refinement)

**Intuition**  
The core algorithm is already optimal, but we can shave a few operations by:
- Skipping the multiplication loop entirely when either string is `"0"`.  
- Trimming trailing zeros in the input strings (which correspond to leading zeros in the reversed processing) to reduce effective length.  
These tweaks do not change asymptotic complexity but improve constant factors.

**Algorithm**  
1. Remove any trailing `'0'` characters from `num1` and `num2` while counting how many zeros were stripped (`zero1`, `zero2`).  
2. If either stripped string becomes empty, the original number was zero → return `"0"`.  
3. Run the elementary simulation on the stripped strings.  
4. Append `zero1 + zero2` zeros to the final result (because stripping trailing zeros is equivalent to dividing by powers of 10).  
5. Return the constructed string.

**Implementation** (shows the refinement; still O(m*n))

```python
def multiply(num1: str, num2: str) -> str:
    # Quick zero check
    if num1 == "0" or num2 == "0":
        return "0"

    # Count and strip trailing zeros (they are leading zeros when reversed)
    zero1 = len(num1) - len(num1.rstrip('0'))
    zero2 = len(num2) - len(num2.rstrip('0'))
    num1 = num1.rstrip('0')
    num2 = num2.rstrip('0')

    if not num1 or not num2:          # one of them was all zeros
        return "0"

    m, n = len(num1), len(num2)
    pos = [0] * (m + n)

    for i in range(m - 1, -1, -1):
        for j in range(n - 1, -1, -1):
            mul = (ord(num1[i]) - 48) * (ord(num2[j]) - 48)
            p1, p2 = i + j, i + j + 1
            total = mul + pos[p2]

            pos[p2] = total % 10
            pos[p1] += total // 10

    # Skip leading zeros
    idx = 0
    while idx < len(pos) and pos[idx] == 0:
        idx += 1

    result = ''.join(str(d) for d in pos[idx:])
    # Append the zeros we stripped earlier
    if result != "0":
        result += '0' * (zero1 + zero2)
    return result
```

**Complexity Analysis**

- Time complexity: O((m - zero1) * (n - zero2)) – still O(m*n) in worst case.  
- Space complexity: O(m + n) – same as before.

---

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

### Dry Run

We trace the algorithm on `num1 = "123"` and `num2 = "456"`.

| Step | i (num1) | j (num2) | digit1 | digit2 | mul | p1 | p2 | pos before (p1,p2) | total = mul + pos[p2] | pos[p2] = total%10 | carry = total//10 added to pos[p1] | pos after step (relevant indices) |
|------|----------|----------|--------|--------|-----|----|----|--------------------|-----------------------|---------------------|------------------------------------|------------------------------------|
| Start| –        | –        | –      | –      | –   | –  | –  | all zeros          | –                     | –                   | –                                  | [0,0,0,0,0,0] |
| 1    | 2 (‘3’)  | 2 (‘6’)  | 3      | 6      | 18  | 4  | 5  | (0,0)              | 18+0=18               | 8                   | 1 added to pos[4]                  | pos[5]=8, pos[4]=1                 |
| 2    | 2 (‘3’)  | 1 (‘5’)  | 3      | 5      | 15  | 3  | 4  | (1,8)              | 15+8=23               | 3                   | 2 added to pos[3]                  | pos[4]=3, pos[3]=2                 |
| 3    | 2 (‘3’)  | 0 (‘4’)  | 3      | 4      | 12  | 2  | 3  | (2,3)              | 12+3=15               | 5                   | 1 added to pos[2]                  | pos[3]=5, pos[2]=1                 |
| 4    | 1 (‘2’)  | 2 (‘6’)  | 2      | 6      | 12  | 3  | 4  | (2,3)              | 12+3=15               | 5                   | 1 added to pos[3]                  | pos[4]=5, pos[3]=3                 |
| 5    | 1 (‘2’)  | 1 (‘5’)  | 2      | 5      | 10  | 2  | 3  | (3,5)              | 10+5=15               | 5                   | 1 added to pos[2]                  | pos[3]=5, pos[2]=2                 |
| 6    | 1 (‘2’)  | 0 (‘4’)  | 2      | 4      | 8   | 1  | 2  | (1,5)              | 8+5=13                | 3                   | 1 added to pos[1]                  | pos[2]=3, pos[1]=1                 |
| 7    | 0 (‘1’)  | 2 (‘6’)  | 1      | 6      | 6   | 2  | 3  | (2,3)              | 6+3=9                 | 9                   | 0 added to pos[2]                  | pos[3]=9, pos[2]=3                 |
| 8    | 0 (‘1’)  | 1 (‘5’)  | 1      | 5      | 5   | 1  | 2  | (1,3)              | 5+3=8                 | 8                   | 0 added to pos[1]                  | pos[2]=8, pos[1]=1                 |
| 9    | 0 (‘1’)  | 0 (‘4’)  | 1      | 4      | 4   | 0  | 1  | (0,1)              | 4+1=5                 | 5                   | 0 added to pos[0]                  | pos[1]=5, pos[0]=0                 |
