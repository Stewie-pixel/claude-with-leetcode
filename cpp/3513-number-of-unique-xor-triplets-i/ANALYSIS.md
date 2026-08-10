# Bitwise XOR

## Video Solution

For more details about **Number of Unique XOR Triplets I**, watch the walkthrough at [https://www.youtube.com/watch?v=ir3K-Yl3kEE](https://www.youtube.com/watch?v=ir3K-Yl3kEE).

## Concept

XOR (exclusive‑or) has two key properties that make it useful for counting distinct values:

1. **Self‑cancelling**: `x ^ x = 0`. An element used an even number of times disappears from the result.
2. **Associative & commutative**: The order of operands does not matter; we can regroup them freely.

When we are allowed to pick up to three elements (with replacement) from a set, each pick toggles the parity (odd/even count) of that element. After at most three toggles, the set of elements that appear an odd number of times can have size `0, 1, 2, or 3`. Therefore the XOR of a valid triplet is exactly the XOR of **any subset of size ≤ 3** drawn from the original array.

Because the input is a permutation of `[1 … n]`, the problem reduces to: *How many distinct XOR values can be formed by XOR‑ing 0, 1, 2, or 3 numbers from the set `{1,2,…,n}`?*  

A remarkable fact is that for the set `{1 … n}` the set of achievable XORs (using any number of elements, not limited to 3) already forms a complete range `[0, 2^k‑1]`, where `k` is the number of bits needed to represent `n`. Limiting ourselves to at most three elements does **not** shrink this range when `n ≥ 3`; we can still generate every value in that range. Consequently the answer is the smallest power of two that is **strictly greater** than `n`. For `n < 3` we can enumerate directly and the answer equals `n`.

## When to Use It

Use bit‑wise reasoning when you see:

- Problems that ask for the number of distinct XOR/OR/AND results.
- Input described as a permutation or consecutive range.
- Operations that are self‑inverting (e.g., XOR with itself yields 0).
- Constraints up to `10^5` where an O(n) or O(1) solution is expected.

## Template

```python
def solve_bitwise_xor(nums):
    """
    Returns the number of distinct XOR values obtainable from
    XOR-ing up to three elements (with replacement) of nums.
    Assumes nums is a permutation of [1 .. len(nums)].
    """
    n = len(nums)
    if n < 3:
        return n                     # brute‑force works for n = 1,2
    # smallest power of two > n
    return 1 << (n.bit_length())
```

## LeetCode Problem Walkthrough

### Problem: 3513. Number of Unique XOR Triplets I

https://leetcode.com/problems/number-of-unique-xor-triplets-i/

### Approach 1: Brute Force (for small n)

**Algorithm**  
- Enumerate all triples `(i, j, k)` with `0 ≤ i ≤ j ≤ k < n`.
- Compute `nums[i] ^ nums[j] ^ nums[k]` and insert into a set.
- Return the size of the set.

**Implementation**

```python
class Solution:
    def uniqueXorTriplets(self, nums):
        n = len(nums)
        seen = set()
        for i in range(n):
            for j in range(i, n):
                for k in range(j, n):
                    seen.add(nums[i] ^ nums[j] ^ nums[k])
        return len(seen)
```

**Complexity Analysis**

- Time complexity: O(n³) — three nested loops over `n`.
- Space complexity: O(n²) in the worst case (the set may hold up to O(n²) distinct values, but for n≤2 this is tiny).

*Only practical when n ≤ 2; otherwise we need a smarter method.*

### Approach 2: Observation – Power of Two (n ≥ 3)

**Intuition**  
XOR behaves like addition without carry in base 2. The set `{1,…,n}` contains all bit patterns up to the highest bit of `n`. By XOR‑ing up to three numbers we can toggle any subset of bits, because each pick flips the presence of a chosen number. With at most three toggles we can achieve any parity pattern (0, 1, 2, or 3 ones) and therefore any number in the range `[0, 2^k‑1]`, where `k = bit_length(n)`. Hence the count of distinct values equals the next power of two.

**Algorithm**  
- If `n < 3` return `n` (handle the tiny cases directly).
- Compute `k = n.bit_length()` → number of bits needed to represent `n`.
- Return `1 << k` (the smallest power of two greater than `n`).

**Implementation**

```python
class Solution:
    def uniqueXorTriplets(self, nums):
        n = len(nums)
        if n < 3:
            return n
        return 1 << n.bit_length()
```

**Complexity Analysis**

- Time complexity: O(1) — only a few arithmetic operations.
- Space complexity: O(1) — constant extra space.

### Approach 3: (Not needed) – Alternative view via linear basis

**Intuition**  
One could build a linear basis (Gaussian elimination over GF(2)) of the numbers `1 … n`. The basis size equals `bit_length(n)`, and the number of distinct XORs obtainable from any subset is `2^{basis_size}`. Since we are limited to at most three elements, the basis size does not shrink for `n ≥ 3`, yielding the same result. This approach is more general but overkill for this problem.

**Algorithm**  
- Insert each number into a XOR basis.
- Let `r` be the number of basis vectors.
- Answer is `1 << r` (or `n` when `n < 3`).

**Implementation** (omitted for brevity – follows the same O(1) result).

**Complexity Analysis**

- Time complexity: O(n) to build the basis (still linear, but the constant‑time solution above is preferred).
- Space complexity: O(log n) for the basis.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the formula on a couple of inputs.

#### Example 1: `nums = [1, 2]` (n = 2)

Since `n < 3`, we return `n = 2`.

| Step | n | Condition | Returned |
|------|---|-----------|----------|
| 1    | 2 | n < 3 → true | 2 |

Matches the expected output.

#### Example 2: `nums = [3, 1, 2]` (n = 3)

`n ≥ 3`, so we compute `bit_length(3) = 2` (binary `11` needs 2 bits).  
Answer = `1 << 2 = 4`.

| Step | n | bit_length | 1 << bit_length |
|------|---|------------|-----------------|
| 1    | 3 | 2          | 4               |

The distinct XORs are `{0,1,2,3}` → size 4, as shown in the problem statement.

#### Example 3: `nums = [1,2,3,4,5]` (n = 5)

`bit_length(5) = 3` (`101`). Answer = `1 << 3 = 8`.  
All XOR values from 0 to 7 are achievable.

---

**Summary**

- For `n < 3` the answer is simply `n` (brute‑force feasible).
- For `n ≥ 3` the answer is the smallest power of two greater than `n`, computed as `1 << n.bit_length()`.
- This runs in O(1) time and O(1) space, satisfying the constraints up to `10^5`.