# Math & Geometry

## Video Solution

For more details about **GCD of Odd and Even Sums**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=y219OgMguao).

## Concept

The problem reduces to computing two well‑known sums:
- Sum of the first `n` odd numbers = `n²`
- Sum of the first `n` even numbers = `n(n+1)`

The required answer is `gcd(n², n(n+1))`.  
Since `n` and `n+1` are consecutive integers, they are coprime (`gcd(n, n+1)=1`).  
Therefore `gcd(n², n(n+1)) = n * gcd(n, n+1) = n`.

So the solution is simply to return `n`.  
We still show the brute‑force and formula‑based approaches to illustrate the progression from naïve to optimal.

## When to Use It

Use this pattern when you see:
- A problem that asks for the GCD of two sums or aggregates.
- Sequences of consecutive odd/even numbers.
- Simple arithmetic series where closed‑form formulas exist (sum of first k odds/evens).

## Template

```python
def gcd_odd_even_sums(n: int) -> int:
    # Step 1: compute the two sums (either by loop or formula)
    # Step 2: compute gcd of the two sums (Euclidean algorithm or math.gcd)
    # Step 3: return the result
    pass
```

## LeetCode Problem Walkthrough

### Problem: 3658. GCD of Odd and Even Sums

https://leetcode.com/problems/gcd-of-odd-and-even-sums/

### Approach 1: Brute Force

**Algorithm**
1. Initialise `sum_odd = 0`, `sum_even = 0`.
2. Loop `i` from `1` to `n`:
   - Add the `i`‑th odd number (`2*i-1`) to `sum_odd`.
   - Add the `i`‑th even number (`2*i`) to `sum_even`.
3. Compute `gcd(sum_odd, sum_even)` using the Euclidean algorithm.
4. Return the gcd.

**Implementation**

```python
def gcd_odd_even_sums_brute(n: int) -> int:
    sum_odd = 0
    sum_even = 0
    for i in range(1, n + 1):
        sum_odd += 2 * i - 1          # i‑th odd number
        sum_even += 2 * i             # i‑th even number

    # Euclidean algorithm
    a, b = sum_odd, sum_even
    while b:
        a, b = b, a % b
    return a
```

**Complexity Analysis**
- Time complexity: O(n) — one loop over `n`.
- Space complexity: O(1) — only a few integer variables.

### Approach 2: Using Closed‑Form Formulas

**Intuition**
The sums have known formulas:
- Sum of first `n` odds = `n²`
- Sum of first `n` evens = `n(n+1)`
Using these eliminates the loop and lets us focus on the gcd step.

**Algorithm**
1. Compute `sum_odd = n * n`.
2. Compute `sum_even = n * (n + 1)`.
3. Return `gcd(sum_odd, sum_even)` via Euclidean algorithm.

**Implementation**

```python
def gcd_odd_even_sums_formula(n: int) -> int:
    sum_odd = n * n
    sum_even = n * (n + 1)

    a, b = sum_odd, sum_even
    while b:
        a, b = b, a % b
    return a
```

**Complexity Analysis**
- Time complexity: O(log min(sum_odd, sum_even)) — Euclidean algorithm, effectively O(log n).
- Space complexity: O(1).

### Approach 3: Direct Observation (Optimal)

**Intuition**
From the formulas:
```
sum_odd   = n²
sum_even  = n(n+1) = n² + n
```
Factor out `n`:
```
sum_odd   = n * n
sum_even  = n * (n + 1)
```
Thus `gcd(sum_odd, sum_even) = n * gcd(n, n+1)`.  
Since `n` and `n+1` are consecutive, their gcd is 1.  
Hence the answer is simply `n`.

**Algorithm**
1. Return `n`.

**Implementation**

```python
def gcd_odd_even_sums_optimal(n: int) -> int:
    return n
```

**Complexity Analysis**
- Time complexity: O(1) — constant time.
- Space complexity: O(1).

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimal solution for `n = 4`.

## Dry Run

Input: n = 4

```
| Step | n (input) | Action               | Return |
| ---- | --------- | -------------------- | ------ |
| 1    | 4         | return n             | 4      |
```

Result: `4`, which matches the expected GCD of 16 and 20.

--- 

*Tip:* Whenever a problem involves sums of regular sequences, first try to recall the closed‑form formulas; they often turn an O(n) loop into O(1) work and reveal hidden mathematical properties (like coprimality) that lead to the optimal answer.