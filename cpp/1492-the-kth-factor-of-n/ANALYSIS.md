# Math & Geometry

## Video Solution

For more details about **The kth Factor of n**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=xfY7qQ270sY).

## Concept

The factors of a number `n` come in pairs: if `i` divides `n`, then `n / i` also divides `n`. By iterating only up to `√n` we can discover both members of each pair. This reduces the work from checking every number `1…n` to checking only up to the square root.

## When to Use It

Use factor‑pair enumeration when you need to:
- List or count divisors of an integer.
- Find the k‑th smallest/largest divisor.
- Work with problems where `n ≤ 10⁵` or `10⁶` and an O(n) scan is too slow.
- The constraint hints at a sub‑linear solution (often √n).

## Template

```python
def kth_factor(n: int, k: int) -> int:
    # 1. Iterate i from 1 to sqrt(n)
    # 2. For each divisor i, handle the pair (i, n//i)
    # 3. Keep track of how many factors we have seen
    # 4. Return the k‑th factor or -1 if not enough factors exist
    pass
```

## LeetCode Problem Walkthrough

### Problem: 1492. The kth Factor of n

https://leetcode.com/problems/the-kth-factor-of-n/

### Approach 1: Brute Force

**Algorithm**
- Scan every integer `i` from `1` to `n`.
- Whenever `i` divides `n`, decrement `k`.
- When `k` reaches zero, return `i`.
- If the loop finishes without hitting zero, return `-1`.

**Implementation**

```python
def kth_factor_bruteforce(n: int, k: int) -> int:
    for i in range(1, n + 1):
        if n % i == 0:          # i is a factor
            k -= 1
            if k == 0:
                return i
    return -1
```

**Complexity Analysis**
- Time complexity: O(n) — we potentially check every number up to n.
- Space complexity: O(1) — only a few integer variables are used.

---

### Approach 2: Square‑Root Decomposition (store pairs)

**Intuition**
Factors appear in symmetric pairs `(i, n//i)`. By iterating only to `√n` we can collect the small factors in order and the large factors in reverse order, then concatenate them to get the full sorted list.

**Algorithm**
1. Initialize two lists: `small` for factors `≤ √n`, `big` for the complementary factors `> √n`.
2. Loop `i` from `1` to `int(sqrt(n))`.
   - If `n % i == 0`:
     - Append `i` to `small`.
     - If `i != n // i`, append `n // i` to `big` (to avoid duplicating the square root).
3. The sorted factor list is `small + reversed(big)`.
4. Return the `(k‑1)`‑th element if it exists, otherwise `-1`.

**Implementation**

```python
import math

def kth_factor_sqrt(n: int, k: int) -> int:
    small, big = [], []
    limit = int(math.isqrt(n))
    for i in range(1, limit + 1):
        if n % i == 0:
            small.append(i)
            if i != n // i:
                big.append(n // i)
    factors = small + big[::-1]          # big reversed gives ascending order
    return factors[k - 1] if k <= len(factors) else -1
```

**Complexity Analysis**
- Time complexity: O(√n) — we loop only up to the square root of n.
- Space complexity: O(√n) — we store at most 2·√n factors.

---

### Approach 3: Square‑Root Decomposition (O(1) space)

**Intuition**
We do not need to store all factors; we can count them on the fly.
- While scanning `i` from `1` to `√n`, each divisor `i` contributes one factor (the small side) and possibly a second factor `n // i` (the big side) that appears later in the overall order.
- Keep a running count of how many factors we have passed.
- If the k‑th factor lies among the small side, we can return it immediately.
- Otherwise, after the loop we know the k‑th factor is among the big side, which we can retrieve by reversing the order of the big factors we have seen.

**Algorithm**
1. First pass: iterate `i` from `1` to `√n`.
   - If `n % i == 0`:
     - Increment `cnt` for the small factor `i`.
     - If `cnt == k`, return `i`.
     - If `i != n // i`, store the big factor `n // i` in a list `big` (to be used later).
2. After the loop, we have seen all small factors; `cnt` equals their number.
3. The remaining needed index among the big factors is `k - cnt`.
   - Because big factors were encountered in increasing `i`, they are in **descending** order when we need the final ascending list.
   - So the desired big factor is `big[-(k - cnt)]` (i.e., count from the end).
4. If `k` exceeds total factors, return `-1`.

**Implementation**

```python
import math

def kth_factor_sqrt_constant_space(n: int, k: int) -> int:
    big = []                # will hold the larger partners
    cnt = 0                 # how many factors we have passed so far
    limit = int(math.isqrt(n))

    for i in range(1, limit + 1):
        if n % i == 0:
            cnt += 1                     # count the small factor i
            if cnt == k:                 # k-th factor is i itself
                return i
            if i != n // i:              # avoid double‑counting a square root
                big.append(n // i)       # store the big partner

    # All small factors processed; now look into the big factors (in reverse)
    remaining = k - cnt
    if remaining <= len(big):
        # big list is in increasing i => decreasing factor value,
        # so we index from the end to get ascending order.
        return big[-remaining]
    return -1
```

**Complexity Analysis**
- Time complexity: O(√n) — single loop up to √n.
- Space complexity: O(√n) in the worst case for the `big` list, but we can argue it’s O(√n) → still sub‑linear; if we wanted strict O(1) we could compute the answer by a second backward pass, but the asymptotic bound stays O(√n) time and O(√n) space, which satisfies the follow‑up requirement of “less than O(n)”.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: `n = 12`, `k = 3`

We will trace **Approach 3** (constant‑space sqrt method).

```
| Step | i   | n % i == 0? | cnt before | Action (cnt after) | big list so far | k-th? |
|------|-----|-------------|------------|--------------------|-----------------|-------|
| 1    | 1   | True        | 0          | cnt = 1 (not k)    | [12]            | No    |
| 2    | 2   | True        | 1          | cnt = 2 (not k)    | [12, 6]         | No    |
| 3    | 3   | True        | 2          | cnt = 3 -> k hit!  | [12, 6, 4]      | Yes → return i = 3 |
```

The algorithm returns `3`, which matches the expected output.

---