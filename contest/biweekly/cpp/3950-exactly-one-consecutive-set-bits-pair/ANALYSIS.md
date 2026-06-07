# Bit Manipulation – Detecting Exactly One Consecutive Set‑Bits Pair

## Concept
When we look at the binary representation of a number, a *pair of consecutive set bits* appears as the pattern `11`.  
If we shift the number right by one bit and AND it with the original number, every position that had `11` in the original becomes a single `1` in the result:

```
n      = … b3 b2 b1 b0
n>>1   = … 0  b3 b2 b1
n & (n>>1) = … (b3&b2) (b2&b1) (b1&b0)
```

Thus each `1` in `x = n & (n>>1)` corresponds to one occurrence of `11` in `n`.  
The problem reduces to: **does `x` contain exactly one `1`?**  
That is the classic “power‑of‑two” test: a non‑zero number with exactly one set bit satisfies `x & (x‑1) == 0`.

## When to Use It
Use bit‑wise tricks like this when the problem:
- Asks about patterns in the binary representation (consecutive bits, alternating bits, etc.).
- Can be expressed as a relationship between a number and its shifted version.
- Requires O(1) time and O(1) extra space.

## Template
```python
def has_exactly_one_consecutive_pair(n: int) -> bool:
    # Step 1: isolate positions where two consecutive 1s occur
    x = n & (n >> 1)
    # Step 2: check whether x has exactly one set bit (i.e., is a power of two)
    return x != 0 and (x & (x - 1)) == 0
```

---

## LeetCode Problem Walkthrough

### Problem: 2579. Exactly One Consecutive Set Bits Pair
https://leetcode.com/problems/exactly-one-consecutive-set-bits-pair/

### Approach 1: Brute Force – Scan Bit by Bit

**Algorithm**
1. Initialise `count = 0` and `prev_bit = 0`.
2. While `n > 0`:
   - Extract the current least‑significant bit `curr = n & 1`.
   - If `prev_bit == 1` and `curr == 1`, we found a consecutive pair → increment `count`.
   - Update `prev_bit = curr`.
   - Right‑shift `n` (`n >>= 1`) to process the next bit.
3. After the loop, return `count == 1`.

**Implementation**
```python
def consecutive_set_bits_brute(n: int) -> bool:
    count = 0
    prev = 0
    while n:
        curr = n & 1
        if prev == 1 and curr == 1:
            count += 1
        prev = curr
        n >>= 1
    return count == 1
```

**Complexity Analysis**
- Time complexity: O(log n) – we inspect each bit once (at most 17 bits for n ≤ 10⁵).
- Space complexity: O(1) – only a few integer variables.

### Approach 2: Bitwise Isolation + Power‑of‑Two Check

**Intuition**
The expression `n & (n >> 1)` leaves a `1` exactly where `n` had a `11` pattern.  
If there is **exactly one** such pattern, the result is a power of two (a single `1` bit).  
Testing for a power of two is done with `x & (x‑1) == 0` (and ensuring `x != 0`).

**Algorithm**
1. Compute `x = n & (n >> 1)`.
2. Return `True` iff `x` is non‑zero and `x & (x‑1) == 0`.

**Implementation**
```python
def consecutive_set_bits_optimal(n: int) -> bool:
    x = n & (n >> 1)          # isolate every "11" as a single 1
    return x != 0 and (x & (x - 1)) == 0   # power‑of‑two test
```

**Complexity Analysis**
- Time complexity: O(1) – constant number of machine operations.
- Space complexity: O(1) – only a few integer variables.

### Approach 3: Count Set Bits in the Isolated Pattern

**Intuition**
After obtaining `x = n & (n >> 1)`, we can directly count how many bits are set.  
If the count equals `1`, there was exactly one consecutive pair.  
Python’s `bin(x).count('1')` (or `x.bit_count()` in 3.8+) does this in O(number of set bits), which is at most O(log n) but still trivial for the given constraints.

**Algorithm**
1. Compute `x = n & (n >> 1)`.
2. Return `x.bit_count() == 1`.

**Implementation**
```python
def consecutive_set_bits_bitcount(n: int) -> bool:
    x = n & (n >> 1)
    return x.bit_count() == 1   # Python 3.8+: int.bit_count()
```

**Complexity Analysis**
- Time complexity: O(log n) in the worst case (proportional to number of set bits in x).
- Space complexity: O(1).

---

## Dry Run (Example: n = 6)

Binary of 6: `110`

| Step | n (binary) | n>>1 (binary) | n & (n>>1) (x) | x != 0? | x & (x-1) == 0? | Result |
|------|------------|---------------|----------------|---------|-----------------|--------|
| Start| 110        | 011           | 010 (=2)       | True    | 2 & 1 = 0 → True| True   |

Since `x` is non‑zero and a power of two, the function returns `True` – exactly one `11` pair.

--- 

**Key Takeaway:**  
Many binary‑pattern problems become trivial once you recognize that shifting and masking can isolate the pattern of interest, turning the task into a simple power‑of‑two or bit‑count check.