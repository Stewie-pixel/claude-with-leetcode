# Bit Manipulation

## Video Solution

For more details about **Minimum Flips to Make a OR b Equal to c**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=R8D-6SchelA).

## Concept

We need to change bits of `a` and `b` so that `(a OR b) == c`.  
Consider each bit position independently because OR works per‑bit:

* If `c` has a `0` at this position, both `a` and `b` must have `0`.  
  Every `1` we see in `a` or `b` must be flipped → cost = number of `1`s in `a` + `b`.
* If `c` has a `1` at this position, at least one of `a` or `b` must be `1`.  
  If both are `0` we need to flip exactly one of them → cost = 1; otherwise cost = 0.

Summing the cost over all bit positions gives the answer.

## When to Use It

* The problem mentions **bitwise OR** and **flipping individual bits**.  
* Input limits (`≤ 10⁹`) imply at most 30‑31 bits → a per‑bit scan is trivial.  
* You see patterns like “count how many bits need to change” or “make two numbers satisfy a bitwise condition”.

## Template

```python
def minFlips(a: int, b: int, c: int) -> int:
    flips = 0
    while a or b or c:          # process until all numbers are 0
        bit_a = a & 1
        bit_b = b & 1
        bit_c = c & 1

        if bit_c == 0:          # need both bits to be 0
            flips += bit_a + bit_b
        else:                   # need at least one bit to be 1
            if bit_a == 0 and bit_b == 0:
                flips += 1

        a >>= 1
        b >>= 1
        c >>= 1
    return flips
```

---

## LeetCode Problem Walkthrough

### Problem: 1318. Minimum Flips to Make a OR b Equal to c

https://leetcode.com/problems/minimum-flips-to-make-a-or-b-equal-to-c/

---

### Approach 1: Brute Force Bit Scan

**Algorithm**  
Examine each bit position from least‑significant to most‑significant (up to 31 bits because `a,b,c ≤ 10⁹`).  
* Extract the current bits of `a`, `b`, `c` with `& 1`.  
* Apply the per‑bit rules described in the Concept section to compute the flips needed for this position.  
* Right‑shift `a`, `b`, `c` to move to the next bit.  
* Accumulate the flips and return the total.

**Implementation**

```python
class Solution:
    def minFlips(self, a: int, b: int, c: int) -> int:
        flips = 0
        while a or b or c:               # continue while any number still has bits
            bit_a = a & 1
            bit_b = b & 1
            bit_c = c & 1

            if bit_c == 0:               # target 0 → both must be 0
                flips += bit_a + bit_b
            else:                        # target 1 → need at least one 1
                if bit_a == 0 and bit_b == 0:
                    flips += 1

            a >>= 1
            b >>= 1
            c >>= 1
        return flips
```

**Complexity Analysis**  
- Time complexity: **O(log max(a,b,c))** → at most 31 iterations (constant).  
- Space complexity: **O(1)** – only a few integer variables.

---

### Approach 2: Optimized Loop (same logic, fewer comments)

**Intuition**  
The brute‑force scan is already optimal in asymptotic terms, but we can tighten the loop by processing exactly 32 bits (the width of an int) instead of checking `a or b or c` each iteration. This removes the branch and makes the loop deterministic.

**Algorithm**  
Iterate a fixed 32 times (covering all possible bits for the given constraints). At each step compute the three current bits and apply the same per‑bit rules.

**Implementation**

```python
class Solution:
    def minFlips(self, a: int, b: int, c: int) -> int:
        flips = 0
        for _ in range(32):              # 32 bits are enough for numbers ≤ 10⁹
            bit_a = a & 1
            bit_b = b & 1
            bit_c = c & 1

            if bit_c == 0:
                flips += bit_a + bit_b
            else:
                if bit_a == 0 and bit_b == 0:
                    flips += 1

            a >>= 1
            b >>= 1
            c >>= 1
        return flips
```

**Complexity Analysis**  
- Time complexity: **O(32) = O(1)**.  
- Space complexity: **O(1)**.

---

### Approach 3: Bit‑Count Formula (Most Optimal)

**Intuition**  
Instead of looping, we can compute the answer directly using population count (`bit_count`).  
* For bits where `c` is `0`, we must turn off any `1`s present in `a` **or** `b`.  
  The mask `~(a | b)` has `1`s exactly where both `a` and `b` are `0`. Its complement `(a | b)` marks positions where at least one of `a,b` is `1`.  
  Hence `(a | b) & ~c` isolates the positions that need to be cleared → each contributes one flip.  
* For bits where `c` is `1`, we need at least one `1` in `a` or `b`.  
  The positions where both are `0` are given by `~(a | b) & c`. Each such position needs one flip.  
Adding the two populations gives the total flips.

**Implementation**

```python
class Solution:
    def minFlips(self, a: int, b: int, c: int) -> int:
        # flips to turn off excess 1s where c has 0
        flips_off = (a | b) & ~c
        # flips to turn on a 1 where c has 1 but both a,b are 0
        flips_on  = ~(a | b) & c
        # Python 3.8+: int.bit_count() returns the number of set bits
        return flips_off.bit_count() + flips_on.bit_count()
```

**Complexity Analysis**  
- Time complexity: **O(1)** – a constant number of bitwise operations and two `bit_count` calls (each runs in O(number of machine words) = O(1) for 32‑bit ints).  
- Space complexity: **O(1)**.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the sample `a = 2 (010)`, `b = 6 (110)`, `c = 5 (101)`.

| Step | bit index | a_bit | b_bit | c_bit | Action (per‑bit) | flips added |
|------|-----------|-------|-------|-------|------------------|-------------|
| 0    | 0         | 0     | 0     | 1     | need ≥1 → both 0 → flip one | 1 |
| 1    | 1         | 1     | 1     | 0     | need 0 → both must be 0 → flip both | +2 |
| 2    | 2         | 0     | 1     | 1     | need ≥1 → already have a 1 → 0 | +0 |
| 3+   | ≥3        | 0     | 0     | 0     | all zero → no cost | 0 |

Total flips = 1 + 2 = **3**, matching the expected output.

---