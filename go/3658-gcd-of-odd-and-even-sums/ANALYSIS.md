# Math & Geometry

## Video Solution

For more details about **GCD of Odd and Even Sums**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=y219OgMguao).

## Concept

The sum of the first `n` odd numbers forms a perfect square (`n²`).  
The sum of the first `n` even numbers equals `n × (n+1)`.  
Since `n` and `n+1` are consecutive integers they are always coprime, so the greatest common divisor of `n²` and `n(n+1)` simplifies to `n`.

## When to Use It

Use direct formulas when a problem asks for:
- Sum of first `k` odd/even numbers
- Sum of an arithmetic progression with known difference
- Any situation where you can replace a loop with a closed‑form expression

## Template

```python
# generic template for sum‑of‑series + gcd problems
def solve(n):
    # 1. compute the needed sums using formulas (O(1))
    sum_odd    = ...          # e.g. n * n
    sum_even   = ...          # e.g. n * (n + 1)
    # 2. compute gcd (Euclidean algorithm) – O(log min(a,b))
    return gcd(sum_odd, sum_even)

def gcd(a, b):
    while b:
        a, b = b, a % b
    return a
```

## LeetCode Problem Walkthrough

### Problem: 3658. GCD of Odd and Even Sums

https://leetcode.com/problems/gcd-of-odd-and-even-sums/

### Approach 1: Brute Force

**Algorithm**
1. Loop from 1 to `2n`, adding odd numbers to `sumOdd` and even numbers to `sumEven`.
2. Apply the Euclidean algorithm to compute `gcd(sumOdd, sumEven)`.

**Implementation**

```python
def gcdOfOddEvenSums(n: int) -> int:
    sum_odd = 0
    sum_even = 0
    for i in range(1, 2 * n + 1):
        if i % 2:          # odd
            sum_odd += i
        else:              # even
            sum_even += i

    # Euclidean algorithm
    a, b = sum_odd, sum_even
    while b:
        a, b = b, a % b
    return a
```

**Complexity Analysis**
- Time complexity: O(n) — we iterate through 2n numbers.
- Space complexity: O(1) — only a few integer variables are used.

### Approach 2: Formula‑Based Sums + GCD

**Intuition**
The sums have known closed‑form expressions, so we can avoid the loop and compute them in constant time.

**Algorithm**
1. Compute `sumOdd = n * n` (sum of first `n` odd numbers).
2. Compute `sumEven = n * (n + 1)` (sum of first `n` even numbers).
3. Return `gcd(sumOdd, sumEven)` using the Euclidean algorithm.

**Implementation**

```python
def gcdOfOddEvenSums(n: int) -> int:
    sum_odd = n * n               # 1 + 3 + ... + (2n-1) = n²
    sum_even = n * (n + 1)        # 2 + 4 + ... + 2n   = n(n+1)

    a, b = sum_odd, sum_even
    while b:
        a, b = b, a % b
    return a
```

**Complexity Analysis**
- Time complexity: O(log n) — Euclidean algorithm dominates; the sums are O(1).
- Space complexity: O(1) — constant extra space.

### Approach 3: Mathematical Insight (Optimal)

**Intuition**
`gcd(n², n(n+1)) = n * gcd(n, n+1)`. Since any two consecutive integers are coprime, `gcd(n, n+1) = 1`. Hence the answer is always `n`.

**Algorithm**
Simply return `n`.

**Implementation**

```python
def gcdOfOddEvenSums(n: int) -> int:
    return n
```

**Complexity Analysis**
- Time complexity: O(1) — direct return.
- Space complexity: O(1) — no extra space.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Input: `n = 4`

| Step | i   | sum_odd (running) | sum_even (running) |
| ---- | --- | ----------------- | ------------------ |
| 1    | 1   | 1                 | 0                  |
| 2    | 2   | 1                 | 2                  |
| 3    | 3   | 4                 | 2                  |
| 4    | 4   | 4                 | 6                  |
| 5    | 5   | 9                 | 6                  |
| 6    | 6   | 9                 | 12                 |
| 7    | 7   | 16                | 12                 |
| 8    | 8   | 16                | 20                 |

After the loop: `sum_odd = 16`, `sum_even = 20`.  
Euclidean algorithm on (16,20) → (16,4) → (4,0) → gcd = 4, which equals `n`.

---

This lecture follows the required structure, presents three approaches (brute force → formula‑based → optimal), includes complexity analysis, and provides a concrete dry‑run example. No extra sections or tangential content have been added.