# Divide and Conquer

## Video Solution

For more details about **Pow(x, n)**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=g9YQyYi4IQQ).

## Concept

Divide and conquer solves a problem by breaking it into smaller sub‑problems of the same type, solving each sub‑problem independently, and then combining their answers.  
A real‑world analogy is cutting a loaf of bread in half repeatedly until you have slices of the desired size—each cut reduces the work needed.

## When to Use It

Use divide and conquer when you notice:
- The problem can be split into two (or more) halves that are similar to the original.
- Solving each half is easier than solving the whole (e.g., exponentiation, sorting, searching).
- The combination step is cheap relative to the split.

Typical clues: “power”, “search in sorted array”, “merge sort”, “quick‑select”, “maximum subarray”.

## Template

A generic divide‑and‑conquer skeleton in Python:

```python
def solve(n):
    # Base case: problem small enough to solve directly
    if n <= threshold:
        return base_case(n)

    # Divide: split the problem
    left = solve(n // 2)          # left half
    right = solve(n - n // 2)     # right hand (handles odd n)

    # Combine: merge results from halves
    return combine(left, right)
```

For exponentiation the combine step is multiplication, and the base case is `n == 0`.

## LeetCode Problem Walkthrough

### Problem: 50. Pow(x, n)

https://leetcode.com/problems/powx-n/

### Approach 1: Brute Force

**Algorithm**  
Multiply `x` by itself `abs(n)` times. If `n` is negative, take the reciprocal at the end.

**Implementation**

```python
def my_pow_brute(x: float, n: int) -> float:
    if n < 0:
        x = 1 / x
        n = -n
    result = 1.0
    for _ in range(n):
        result *= x
    return result
```

**Complexity Analysis**

- Time complexity: O(|n|) — we perform one multiplication per unit of exponent.
- Space complexity: O(1) — only a few scalar variables are used.

### Approach 2: Recursive Divide and Conquer

**Intuition**  
`x^n` can be expressed as `(x^{n/2}) * (x^{n/2})` when `n` is even, and as `x * (x^{n//2}) * (x^{n//2})` when `n` is odd. Each step halves the exponent, leading to a logarithmic number of steps.

**Algorithm**  
Recursively compute `half = my_pow(x, n // 2)`.  
- If `n` is even: return `half * half`.  
- If `n` is odd: return `half * half * x`.  
Handle negative `n` by inverting `x` and making `n` positive.

**Implementation**

```python
def my_pow_recursive(x: float, n: int) -> float:
    if n < 0:
        x = 1 / x
        n = -n

    def helper(x, n):
        if n == 0:
            return 1.0
        half = helper(x, n // 2)
        if n % 2 == 0:
            return half * half
        else:
            return half * half * x

    return helper(x, n)
```

**Complexity Analysis**

- Time complexity: O(log n) — each recursive call halves `n`.
- Space complexity: O(log n) — recursion stack depth equals number of halves.

### Approach 3: Iterative Binary Exponentiation (Most Optimal)

**Intuition**  
The same halving idea can be performed iteratively by examining the binary representation of `n`. Whenever the current bit is 1, multiply the result by the current power of `x`. Then square `x` and shift `n` right.

**Algorithm**  
1. If `n` < 0, invert `x` and make `n` positive.  
2. Initialize `result = 1`.  
3. While `n` > 0:  
   - If the lowest bit of `n` is 1 (`n & 1`), multiply `result` by `x`.  
   - Square `x` (`x *= x`).  
   - Shift `n` right (`n >>= 1`).  
4. Return `result`.

**Implementation**

```python
def my_pow_iterative(x: float, n: int) -> float:
    if n < 0:
        x = 1 / x
        n = -n

    result = 1.0
    while n:
        if n & 1:
            result *= x
        x *= x
        n >>= 1
    return result
```

**Complexity Analysis**

- Time complexity: O(log n) — each loop iteration processes one bit of `n`.
- Space complexity: O(1) — only a few variables are used.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Input: x = 2.00000, n = 10 (binary 1010)

| Iteration | n (binary) | n & 1 | result | x (current power) | Action                           |
|-----------|------------|------|--------|-------------------|----------------------------------|
| start     | 1010       | 0    | 1.0    | 2.0               | –                                |
| 1         | 101        | 0    | 1.0    | 4.0 (2²)          | shift → n=101                    |
| 2         | 10         | 0    | 1.0    | 16.0 (4²)         | shift → n=10                     |
| 3         | 1          | 1    | 16.0   | 256.0 (16²)       | result *= x → 1*16=16            |
| 4         | 0          | –    | 16.0   | –                 | shift → n=0, loop ends           |
| final     |            |      | 1024.0 | –                 | after last multiplication (16*64) |

(After iteration 3 we also square x to 256, but loop ends; the final result is 16 * 64 = 1024.)

## Practice Problems

**Easy** – 69. Sqrt(x)  
Use integer binary search (a divide‑and‑conquer search) to find the floor of the square root.

**Medium** – 50. Pow(x, n)  
(The problem just solved; try solving it with a different approach than the one you used.)

**Hard** – 4. Median of Two Sorted Arrays  
Apply divide and conquer by partitioning both arrays to locate the k‑th element in logarithmic time.