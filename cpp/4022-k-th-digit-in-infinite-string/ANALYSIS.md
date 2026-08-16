# Math & Geometry: K‑th Digit in an Infinite String  

## Video Solution  
*(No directly relevant video was found in the provided search results, so this section is omitted.)*  

## Concept  
The infinite string is built by concatenating numbers in **blocks of size 10**.  
- Block 0 → numbers 1 … 9 (only one‑digit numbers, handled separately).  
- For every block b ≥ 1 we write the ten numbers `10·b … 10·b+9`.  
  * If **b** is even → increasing order.  
  * If **b** is odd  → decreasing order.  

The key observation is that **each block contributes the same total number of digits**, regardless of the order inside the block:  
`10 numbers × d digits per number = 10·d` digits, where `d` is the digit‑length of the numbers in that block.  

Thus we can skip whole blocks by subtracting their digit totals from `k` until we locate the block that contains the k‑th digit. Inside the block we again use simple arithmetic to find the exact number and the digit inside that number.

## When to Use It  
Use this counting‑by‑groups technique when you see:  
- A request for the *k‑th* character/digit in a explicitly defined infinite or very long constructed sequence.  
- `k` can be as large as 10¹⁵ (or larger), ruling out any solution that builds the string explicitly.  
- The sequence can be described by repeating patterns (fixed‑size groups, blocks, cycles) where the contribution of each whole group to the length can be computed arithmetically.  

Typical problems: “Nth Digit” (LeetCode 400), “K‑th Symbol in Grammar”, “Find K‑th Bit in N‑th Binary String”, etc.

## Template  

```python
def kth_digit(k: int) -> int:
    # 1. Handle the tiny prefix that does not follow the regular block size (if any)
    # 2. Determine the digit length d of the numbers in the current block
    # 3. While k exceeds the total digits contributed by all blocks of length d:
    #        subtract those digits, move to the next digit length
    # 4. Inside the block of length d:
    #        block_index   = (k-1) // (numbers_per_block * d)
    #        remainder     = (k-1) %  (numbers_per_block * d)
    #        number_index  = remainder // d
    #        digit_index   = remainder % d
    # 5. Reconstruct the actual number (taking into account increasing / decreasing order)
    # 6. Return the digit at digit_index of that number.
    pass
```

---

## LeetCode Problem Walkthrough  

**Problem:** 2379. K‑th Digit in Infinite String  
<https://leetcode.com/problems/k-th-digit-in-infinite-string/>

### Approach 1: Brute Force (Generate until length ≥ k)  

**Algorithm**  
1. Initialise an empty list `parts`.  
2. Iterate `num = 1, 2, 3, …` and append the string representation of each number to `parts`, respecting the block order (increasing for even block index, decreasing for odd).  
3. After each append, check if the total length of the concatenated string has reached or exceeded `k`.  
4. When it does, return the `k‑1`‑indexed character.

**Implementation**  

```python
class Solution:
    def kthDigit(self, k: int) -> int:
        # This will work only for very small k (e.g. k < 10⁵) because we build the string.
        res = []
        num = 1
        block = 0          # block index = num // 10
        while len(''.join(res)) < k:
            # Determine the order for the current block
            start = block * 10
            end   = start + 9
            if block % 2 == 0:          # even → increasing
                rng = range(start, end + 1)
            else:                       # odd → decreasing
                rng = range(end, start - 1, -1)
            for x in rng:
                res.append(str(x))
            block += 1
        s = ''.join(res)
        return int(s[k - 1])
```

**Complexity Analysis**  
- Time: O(k) in the worst case because we may generate O(k) characters.  
- Space: O(k) to store the generated string.  
*Not feasible for the constraint `k ≤ 10¹⁵`.*

---

### Approach 2: Mathematical Counting (Optimal)  

**Intuition**  
Instead of materialising the string, we treat each *block of numbers* as a **chunk** that contributes a known amount of digits:  

- For numbers with `d` digits, there are `9 × 10^{d‑1}` such numbers (e.g. for d=2 → 90 numbers from 10‑99).  
- These numbers are grouped into blocks of size 10, so the number of blocks for this digit‑length is  
  `blocks = (9 × 10^{d‑1}) / 10 = 9 × 10^{d‑2}`.  
- Each block contributes `10 × d` digits (10 numbers × d digits each).  

Therefore the total digit count contributed by *all* blocks having `d`‑digit numbers is  
`digits_in_group = blocks × (10 × d) = 9 × 10^{d‑2} × 10 × d = 9 × 10^{d‑1} × d`.  

We subtract whole groups from `k` until `k` falls inside the current group, then drill down to the exact block, number, and digit.

**Algorithm**  
1. If `k ≤ 9` return `k` (the prefix 1‑9).  
2. Set `k ← k - 9` to skip the one‑digit prefix.  
3. Initialise digit length `d = 2`.  
4. Loop:  
   - `blocks = 9 * 10^{d‑2}`  
   - `digits_per_block = 10 * d`  
   - `digits_in_group = blocks * digits_per_block`  
   - If `k ≤ digits_in_group`: break (we are inside the group of `d`‑digit numbers).  
   - Else: `k -= digits_in_group; d += 1`.  
5. Inside the `d`‑digit group:  
   - `block_index   = (k-1) // digits_per_block`  
   - `remainder     = (k-1) %  digits_per_block`  
   - `b = 10^{d‑2} + block_index`  (the actual block number)  
   - `number_index = remainder // d`  
   - `digit_index  = remainder %  d`  
6. Reconstruct the number inside block `b`:  
   - If `b` is even → numbers increase: `value = 10*b + number_index`  
   - If `b` is odd  → numbers decrease: `value = 10*b + 9 - number_index`  
7. Convert `value` to string and return the digit at `digit_index`.  

**Implementation**  

```python
class Solution:
    def kthDigit(self, k: int) -> int:
        if k <= 9:
            return k

        k -= 9                     # skip the prefix "123456789"
        d = 2                      # current digit length we are examining

        while True:
            blocks = 9 * (10 ** (d - 2))          # how many blocks of size 10 with d‑digit numbers
            digits_per_block = 10 * d
            digits_in_group = blocks * digits_per_block
            if k <= digits_in_group:
                break
            k -= digits_in_group
            d += 1

        # Locate the exact block inside the d‑digit group
        digits_per_block = 10 * d
        block_index = (k - 1) // digits_per_block
        remainder   = (k - 1) %  digits_per_block

        b = 10 ** (d - 2) + block_index          # actual block number
        number_index = remainder // d
        digit_index  = remainder %  d

        # Reconstruct the number taking the block order into account
        if b % 2 == 0:           # even block → increasing
            number = 10 * b + number_index
        else:                    # odd block → decreasing
            number = 10 * b + 9 - number_index

        return int(str(number)[digit_index])
```

**Complexity Analysis**  
- Time: O(D) where `D` is the number of digit‑length groups we skip. Since `k ≤ 10¹⁵`, `D ≤ 18` (the number of digits of the largest possible number). Effectively **O(log₁₀ k)**.  
- Space: O(1) – only a few integer variables are used.  

---

### Provide a Visual Demonstration  

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example  

We dry‑run the algorithm for `k = 15` (the second example).

```
k = 15
Step 1: k > 9 → k = 15 - 9 = 6   (skip "123456789")
d = 2
   blocks      = 9 * 10^{0} = 9
   digits_per_block = 10 * 2 = 20
   digits_in_group  = 9 * 20 = 180
   Since k (=6) ≤ 180 → we are inside the 2‑digit group.
```

Now locate inside the 2‑digit group:

```
digits_per_block = 20
block_index = (6-1) // 20 = 5 // 20 = 0
remainder   = (6-1) %  20 = 5 % 20 = 5

b = 10^{0} + block_index = 1 + 0 = 1          (block 1 → numbers 10…19)
number_index = remainder // 2 = 5 // 2 = 2
digit_index  = remainder %  2 = 5 % 2 = 1
```

Block 1 is **odd**, so numbers are written in decreasing order:  
`19, 18, 17, 16, 15, 14, 13, 12, 11, 10`.

`number_index = 2` → the 3rd number in this order (0‑based) → `17`.  
`digit_index = 1` → the second digit of `"17"` → `'7'`.

Hence the 15‑th digit of the infinite string is **7**, matching the example.  

---  

**Summary**  
- Start with a brute‑force generation to see the pattern (only for tiny `k`).  
- Leap to a mathematical solution that counts whole blocks of equal digit contribution, then zooms in to the exact digit.  
- The approach runs in logarithmic time relative to `k` and constant space, comfortably satisfying `k ≤ 10¹⁵`.  

Feel free to try the implementation with other values (e.g. `k = 4 → 4`, `k = 11 → 9`) to confirm the reasoning. Happy coding!