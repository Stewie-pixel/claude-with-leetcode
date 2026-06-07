# Bit Manipulation

## Concept

The condition `(n & x) == 0` means that **x can only have 1‑bits in positions where n has a 0‑bit**.  
Think of n as a mask of “forbidden” positions. Any compatible number x must avoid those positions entirely – it is a subset of the free bits.

## When to Use It

Use bit‑mask reasoning when you see:
- A requirement like `(a & b) == 0` (no overlapping set bits)
- Problems asking for numbers that do **not** share any set bits with a given number
- Subset / submask generation tasks
- Constraints are small enough to enumerate all subsets of the free bits (≤ 2¹⁰ is trivial)

## Template

```python
def solve_bitmask_problem(n, k):
    # 1. Determine the bits where n has 0 → these are the only places x may set bits
    free_mask = ~n                     # bits that are 0 in n become 1 here
    # Limit to a reasonable width (e.g. enough bits to cover n+k)
    max_bits = max(n, n + k).bit_length()
    free_mask &= (1 << max_bits) - 1   # keep only relevant bits

    # 2. Enumerate all submasks of free_mask (each submask is a candidate x)
    ans = 0
    sub = free_mask
    while True:
        x = sub
        if abs(n - x) <= k:            # also respect the distance constraint
            ans += x
        if sub == 0:
            break
        sub = (sub - 1) & free_mask    # next submask
    return ans
```

---

## LeetCode Problem Walkthrough

### Problem: 2476. Sum of Compatible Numbers in Range I
https://leetcode.com/problems/sum-of-compatible-numbers-in-range-i/

### Approach 1: Brute Force

**Algorithm**
- Iterate over every integer `x` in the interval `[max(1, n‑k), n+k]`.
- For each `x`, check if `(n & x) == 0`. If true, add `x` to the answer.

**Implementation**

```python
class Solution:
    def sumOfGoodIntegers(self, n: int, k: int) -> int:
        total = 0
        lo = max(1, n - k)
        hi = n + k
        for x in range(lo, hi + 1):
            if (n & x) == 0:
                total += x
        return total
```

**Complexity Analysis**
- Time complexity: O(k) — we examine at most `2k + 1` numbers.
- Space complexity: O(1) — only a few integer variables are used.

---

### Approach 2: Submask Enumeration (Optimized)

**Intuition**
Instead of scanning every number in the interval, we directly generate only those numbers that can possibly satisfy `(n & x) == 0`.  
These are exactly the submasks of the bitmask `free_mask` (the bits where `n` has 0).  
For each submask we then test the distance condition `abs(n‑x) ≤ k`.  
Since the number of free bits is at most the bit‑length of `n+k` (≤ 7 for the given constraints), at most `2⁷ = 128` candidates are examined — far fewer than the up to `201` numbers in the brute‑force scan when `k` is large.

**Algorithm**
1. Compute `free_mask = ~n` limited to the bits needed to represent `n+k`.
2. Enumerate all submasks `sub` of `free_mask` using the classic `sub = (sub‑1) & free_mask` loop.
3. For each submask `x = sub`, if `abs(n - x) ≤ k`, add `x` to the answer.
4. Return the accumulated sum.

**Implementation**

```python
class Solution:
    def sumOfGoodIntegers(self, n: int, k: int) -> int:
        # Determine how many bits we need to cover n+k
        max_val = n + k
        max_bits = max_val.bit_length()
        free_mask = (~n) & ((1 << max_bits) - 1)   # bits where n has 0

        ans = 0
        sub = free_mask
        while True:
            x = sub
            if abs(n - x) <= k:
                ans += x
            if sub == 0:
                break
            sub = (sub - 1) & free_mask   # next submask
        return ans
```

**Complexity Analysis**
- Time complexity: O(2ᶻ) where `z` = number of zero‑bits in `n` within the considered width ( ≤ 7 for constraints ), so at most 128 operations.
- Space complexity: O(1) — only a few variables.

---

### Approach 3: SOS‑DP Precomputation (Most Optimal for Many Queries)

**Intuition**
If we needed to answer many queries with different `(n, k)` pairs, repeatedly enumerating submasks would repeat work.  
We can pre‑compute, for every possible mask `m` (up to the maximum bit width), the sum of all its submasks.  
This is the classic **Sum Over Subsets (SOS) DP**.  
Then for a given `n` we obtain `free_mask`, look up the precomputed sum of all submasks of `free_mask`, and finally subtract those submasks that fall outside the `[n‑k, n+k]` interval.  
The interval correction can be done by iterating over the (at most 2·k + 1) numbers in the range and subtracting those that are submasks but out of bounds — still fast because `k ≤ 100`.

**Algorithm**
1. Choose a fixed bit width `B` large enough for the maximum possible `n+k` (here `B = 7` because `n+k ≤ 200 < 2⁸`; we use `B = 8` for safety).
2. Build an array `sum_submask[mask]` where `sum_submask[mask]` equals the sum of all submasks of `mask`.  
   Initialize `sum_submask[mask] = mask` (each mask contributes itself) and then for each bit `i` perform the SOS DP transition:
   ```
   if mask & (1 << i):
       sum_submask[mask] += sum_submask[mask ^ (1 << i)]
   ```
   After processing all bits, `sum_submask[mask]` holds the desired sum.
3. For each query `(n, k)`:
   - Compute `free_mask = (~n) & ((1 << B) - 1)`.
   - Start with `ans = sum_submask[free_mask]`.
   - For every `x` in `[max(1, n‑k), n+k]`:
        * If `(n & x) == 0` **and** `x` is **not** a submask of `free_mask` (i.e., `x & ~free_mask != 0`), it was mistakenly added — subtract it.
        * If `(n & x) != 0` (should never happen for a submask) we also subtract it to be safe.
   - Return `ans`.

**Implementation**

```python
class Solution:
    # ----- pre‑computation (run once) -----
    _MAX_BITS = 8                     # covers values up to 255 > 200
    _SIZE = 1 << _MAX_BITS
    _sum_submask = [_SIZE] * _SIZE    # placeholder, will fill below

    @classmethod
    def _init_sos(cls):
        if cls._sum_submask[0] != 0:   # already computed
            return
        # initialise: each mask contributes itself
        for mask in range(cls._SIZE):
            cls._sum_submask[mask] = mask
        # SOS DP
        for i in range(cls._MAX_BITS):
            for mask in range(cls._SIZE):
                if mask & (1 << i):
                    cls._sum_submask[mask] += cls._sum_submask[mask ^ (1 << i)]

    def sumOfGoodIntegers(self, n: int, k: int) -> int:
        self._init_sos()
        free_mask = (~n) & ((1 << self._MAX_BITS) - 1)

        # start with sum of all submasks of free_mask
        ans = self._sum_submask[free_mask]

        lo = max(1, n - k)
        hi = n + k
        for x in range(lo, hi + 1):
            # if x was added but actually violates the distance or bit condition, remove it
            if (n & x) != 0 or abs(n - x) > k:
                # x is not a valid compatible number; if it was a submask we need to subtract it
                # Check whether x is a submask of free_mask:
                if (x & ~free_mask) == 0:   # x ⊆ free_mask
                    ans -= x
        return ans
```

**Complexity Analysis**
- Pre‑computation (once):  
  *Time*: O(B·2ᴮ) = O(8·256) ≈ 2 k operations.  
  *Space*: O(2ᴮ) = 256 integers.
- Per query:  
  *Time*: O(k) to scan the interval (≤ 200) → effectively O(k).  
  *Space*: O(1) extra.
Thus after the one‑time setup, each test case runs in linear time in `k` with a very small constant factor.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

**Dry Run** – Example `n = 2 (010)_2`, `k = 3`

We need numbers `x` such that `abs(2‑x) ≤ 3` → `x ∈ [‑1,5]` → after `max(1,…)` → `[1,5]` and `(2 & x) == 0`.

| Step | x (dec) | x (bin) | n & x | \|n‑x\| | Valid? | Running sum |
|------|---------|---------|-------|--------|--------|-------------|
| 1    | 1       | 001     | 000   | 1      | ✅      | 1           |
| 2    | 2       | 010     | 010   | 0      | ❌ (overlap) | 1         |
| 3    | 3       | 011     | 010   | 1      | ❌      | 1           |
| 4    | 4       | 100     | 000   | 2      | ✅      | 1+4=5       |
| 5    | 5       | 101     | 000   | 3      | ✅      | 5+5=10      |

**Result:** `10` – matches the example.  

The submask view: free bits where `n` has 0 are positions 0 and 2 → mask `101₂ = 5`.  
All submasks of `5` are `{0,1,4,5}`; after applying the distance filter we keep `{1,4,5}` and sum them.