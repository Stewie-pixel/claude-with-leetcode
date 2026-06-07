# Bit Manipulation

## Concept
Bit manipulation techniques let us work directly with the binary representation of integers using bitwise operators (`&`, `|`, `^`, `~`, `<<`, `>>`). Many problems that ask about patterns of `1`s and `0`s can be solved in O(1) time by exploiting how these operators affect individual bits, without converting the number to a string.

## When to Use It
Consider bit manipulation when the problem statement mentions:
- Binary representation of a number
- Counting or locating set bits (`1`s)
- Patterns like “consecutive set bits”, “alternating bits”, “isolated bits”
- Operations that can be expressed as masks or shifts
- Constraints that allow O(1) or O(log n) solutions (since the number of bits is at most 32/64 for typical integer ranges)

## Template
```python
def solve_bit_problem(n: int) -> ...:
    # 1. Identify the bit pattern you need to check/create
    #    (e.g., n & (n >> 1) finds positions where two consecutive bits are both 1)
    # 2. Apply the appropriate mask/shift/combination
    # 3. Derive the answer from the resulting value
    #    (e.g., check if it is a power of two, count bits, etc.)
    return result
```
*Always comment each step so the reasoning behind the mask/shift is clear.*

## LeetCode Problem Walkthrough

### Problem: 2608. Exactly One Consecutive Set Bits Pair
https://leetcode.com/problems/exactly-one-consecutive-set-bits-pair/

### Approach 1: Brute Force (String Scan)

**Algorithm**
1. Convert `n` to its binary string without the `0b` prefix.
2. Scan the string and count how many times the substring `"11"` appears.
   - Each occurrence corresponds to a pair of consecutive set bits.
3. Return `True` if the count is exactly `1`, otherwise `False`.

**Implementation**
```python
def consecutiveSetBits_bruteforce(n: int) -> bool:
    # Step 1: binary representation
    bin_str = bin(n)[2:]          # e.g., 6 -> '110'
    # Step 2: count occurrences of "11"
    pairs = 0
    for i in range(len(bin_str) - 1):
        if bin_str[i] == '1' and bin_str[i + 1] == '1':
            pairs += 1
    # Step 3: exactly one pair?
    return pairs == 1
```

**Complexity Analysis**
- Time complexity: O(L) where L = number of bits of n (≤ 17 for n ≤ 10⁵) → effectively O(1).
- Space complexity: O(L) for the binary string → O(1) auxiliary besides the string.

---

### Approach 2: Bitwise Trick (Optimal)

**Intuition**
If we shift `n` right by one bit and AND it with the original `n`, every position where two consecutive bits were both `1` becomes a `1` in the result:
```
   n:      b_{k} … b_{2} b_{1} b_{0}
   n>>1:   0   b_{k} … b_{2} b_{1}
   n & (n>>1): 1 exactly where b_i = b_{i-1} = 1
```
Thus `x = n & (n>>1)` has a `1` for each consecutive‑set‑bit pair.  
We need **exactly one** such pair → `x` must be a power of two (only a single `1` bit) and not zero.

**Algorithm**
1. Compute `x = n & (n >> 1)`.
2. Return `True` iff `x != 0` and `x & (x - 1) == 0` (the classic power‑of‑two test).

**Implementation**
```python
def consecutiveSetBits_optimal(n: int) -> bool:
    x = n & (n >> 1)          # marks each pair of consecutive 1s
    # x is a power of two <=> it has exactly one set bit
    return x != 0 and (x & (x - 1)) == 0
```

**Complexity Analysis**
- Time complexity: O(1) – a constant number of bitwise operations.
- Space complexity: O(1) – only a few integer variables.

---

### Approach 3: Alternative Bitwise Count (Shift‑and‑Check)

**Intuition**
Instead of building the whole `x` mask, we can iterate through the bits of `n` and detect a pair on the fly:
- Keep track of the previous bit (`prev`).
- When the current bit and `prev` are both `1`, increment a counter.
- After processing all bits, the counter must be exactly `1`.

This approach is still O(number of bits) but avoids creating the intermediate mask and may be easier to follow for beginners.

**Algorithm**
1. Initialize `count = 0`, `prev = 0`.
2. While `n > 0`:
   - `curr = n & 1` (least‑significant bit)
   - If `curr == 1` and `prev == 1`: `count += 1`
   - Set `prev = curr`
   - Right‑shift `n >>= 1`
3. Return `count == 1`.

**Implementation**
```python
def consecutiveSetBits_shift(n: int) -> bool:
    count = 0
    prev = 0
    while n:
        curr = n & 1
        if curr == 1 and prev == 1:
            count += 1
        prev = curr
        n >>= 1
    return count == 1
```

**Complexity Analysis**
- Time complexity: O(L) where L = number of bits of n (≤ 17 for the given constraints) → O(1).
- Space complexity: O(1).

---

### Provide a Visual Demonstration (Dry Run)

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimal approach with `n = 6` (binary `110`).

```
Input: n = 6 (110₂)

Step | n (binary) | n >> 1 (binary) | x = n & (n>>1) | x != 0? | x & (x-1) | Power‑of‑Two? | Return
-----|------------|-----------------|----------------|---------|-----------|---------------|-------
Init | 110        | 011             | 010            | True    | 010 & 001 = 000 | Yes (single 1) | True
```

Explanation:
- `n & (n>>1)` isolates the overlapping `1`s → `010₂` (only the middle bit set).
- The result is non‑zero and has exactly one set bit (`x & (x-1) == 0`), so there is exactly one pair of consecutive set bits → `True`.

---

## Summary
- **Brute force** converts to a string and scans for `"11"` – easy to understand but involves string allocation.
- **Optimal bit trick** uses `n & (n>>1)` to mark consecutive pairs and checks whether the result is a power of two – O(1) time and space.
- **Shift‑and‑count** offers a middle ground, iterating bits while tracking the previous bit.

All three approaches satisfy the problem constraints; the bitwise trick is the most efficient and showcases the power of bit manipulation for pattern detection in binary numbers.