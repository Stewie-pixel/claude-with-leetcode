# Bit Manipulation

## Video Solution

For more details about **Number of Unique XOR Triplets II**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=ZW8dmdP3w3s)

## Concept

The XOR operation has two key properties that make it amenable to bitset‑style tricks:

1. **Closure** – XOR of any two numbers that appear in the array produces another number whose bits are limited by the maximum bit‑length of the input values.  
2. **Self‑inverse** – `a ^ a = 0` and `a ^ 0 = a`. Because of this, the XOR of three elements can be seen as first XOR‑ing any pair, then XOR‑ing the result with a third element.

If we know which values occur in the array, we can enumerate all possible XORs of two values (the “pair XORs”). XOR‑ing each of those pair results with every array value yields every possible XOR of three elements. Using a boolean array (or Python `set`) to store intermediate results lets us avoid the cubic `O(n³)` brute force and work in `O(V²)` where `V` is the value range (≤ 1500 → we safely use a size of 2048 = 2¹¹).

## When to Use It

* The problem asks for the number of **distinct** results of a bitwise expression (XOR, OR, AND, etc.).
* Input values are small (≤ 1500) so we can afford an array indexed by value.
* The expression is associative and commutative, allowing us to regroup operations (e.g., `(a ^ b) ^ c`).
* We see a pattern like “compute all XORs of pairs, then combine with a third element”.

## Template

```python
def count_unique_xor_triplets(nums):
    # 1. Determine a safe limit (next power of two > max(nums))
    limit = 1
    while limit <= max(nums):
        limit <<= 1          # same as limit *= 2

    # 2. Presence of each value
    present = [False] * limit
    for v in nums:
        present[v] = True

    # 3. List of distinct values that actually appear
    values = [v for v, ok in enumerate(present) if ok]

    # 4. Compute all possible XOR of two values (order does not matter)
    pair_xor = [False] * limit
    n = len(values)
    for i in range(n):
        for j in range(i, n):          # i ≤ j ensures we consider a^b and b^a only once
            pair_xor[values[i] ^ values[j]] = True

    # 5. Combine each pair XOR with a third value to get triplet XORs
    triplet_xor = [False] * limit
    for x in range(limit):
        if not pair_xor[x]:
            continue
        for y in values:
            triplet_xor[x ^ y] = True

    # 6. Count distinct results
    return sum(triplet_xor)
```

## LeetCode Problem Walkthrough

### Problem: 3514. Number of Unique XOR Triplets II

https://leetcode.com/problems/number-of-unique-xor-triplets-ii/

### Approach 1: Brute Force

**Algorithm**  
Iterate over all index triples `(i, j, k)` with `0 ≤ i ≤ j ≤ k < n`, compute `nums[i] ^ nums[j] ^ nums[k]`, insert the result into a set, and finally return the set size.

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

- Time complexity: O(n³) — three nested loops over the array.
- Space complexity: O(m) — at most one entry per distinct XOR value; in the worst case m ≤ 2048.

### Approach 2: Pair‑XOR + Value Set (Optimized)

**Intuition**  
Because XOR is associative and commutative, the value of `a ^ b ^ c` depends only on the multiset `{a, b, c}`, not on their order. Therefore we can first compute every possible XOR of a *pair* of array elements (allowing the same index twice) and then XOR each pair result with a third array element. The intermediate sets are bounded by the value range, not by `n`.

**Algorithm**

1. Build a boolean array `present` marking which numbers occur in `nums`.
2. Extract the list `values` of distinct numbers that appear.
3. Compute `pair_xor[x] = True` if there exist `a, b` in `values` such that `a ^ b = x`.  
   Loop over all `i ≤ j` to avoid duplicate work.
4. Compute `triplet_xor[y] = True` if there exist `x` from `pair_xor` and `c` in `values` with `x ^ c = y`.  
   Again iterate over all `x` where `pair_xor[x]` is true and all `c`.
5. Count the number of `True` entries in `triplet_xor`.

**Implementation**

```python
class Solution:
    def uniqueXorTriplets(self, nums):
        LIMIT = 2048                     # 2^11 > 1500 (max nums[i])
        present = [False] * LIMIT
        for v in nums:
            present[v] = True

        values = [v for v, ok in enumerate(present) if ok]

        pair_xor = [False] * LIMIT
        m = len(values)
        for i in range(m):
            for j in range(i, m):
                pair_xor[values[i] ^ values[j]] = True

        triplet_xor = [False] * LIMIT
        for x in range(LIMIT):
            if not pair_xor[x]:
                continue
            for y in values:
                triplet_xor[x ^ y] = True

        return sum(triplet_xor)
```

**Complexity Analysis**

- Time complexity: O(V² + V·U) where `V` = limit (≈ 2048) and `U` = number of distinct values (≤ 1500). In practice this is well under a few million operations.
- Space complexity: O(V) — three boolean arrays of size `limit`.

### Approach 3: Fast Walsh–Hadamard Transform (FWHT) – Convolution View

**Intuition**  
The XOR of two numbers corresponds to the XOR‑convolution of their frequency vectors. If `f[v]` is the count of value `v` in `nums`, then the XOR‑convolution `f ⊗ f` gives the number of ways to obtain each XOR of a pair. Performing the convolution a second time (`(f ⊗ f) ⊗ f`) yields the counts for triplets. The FWHT computes XOR‑convolution in `O(V log V)` time. After the transform we simply count how many entries are non‑zero.

**Algorithm**

1. Determine `limit` as the smallest power of two > max(nums).
2. Build frequency array `freq` of length `limit`.
3. Apply FWHT to `freq` → `F`.
4. Pointwise cube: `G[i] = F[i] ** 3` (because we need three‑fold convolution).
5. Apply inverse FWHT to `G` → `conv` (now `conv[t]` = number of ordered triples yielding XOR `t`).
6. Count indices `t` where `conv[t] > 0`.

**Implementation**

```python
def fwht(a, invert):
    n = len(a)
    step = 1
    while step < n:
        for i in range(0, n, step * 2):
            for j in range(i, i + step):
                u = a[j]
                v = a[j + step]
                a[j] = u + v
                a[j + step] = u - v
        step <<= 1
    if invert:
        for i in range(n):
            a[i] //= n   # integer division works because all values are ints

class Solution:
    def uniqueXorTriplets(self, nums):
        mx = max(nums)
        limit = 1
        while limit <= mx:
            limit <<= 1

        freq = [0] * limit
        for v in nums:
            freq[v] += 1

        fwht(freq, False)          # forward transform
        for i in range(limit):
            freq[i] = freq[i] ** 3 # three‑fold convolution
        fwht(freq, True)           # inverse transform

        # freq[i] now holds the number of ordered triples with xor == i
        return sum(1 for x in freq if x > 0)
```

**Complexity Analysis**

- Time complexity: O(V log V) — FWHT runs in `V log V` where `V` is the power‑of‑two limit (≤ 2048).
- Space complexity: O(V) — the frequency array.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the sample `nums = [1, 3]`.

#### Dry Run (Approach 2)

| Step | i | values[i] | j | values[j] | values[i] ^ values[j] | pair_xor[ … ] set |
|------|---|-----------|---|-----------|-----------------------|-------------------|
| 1    | 0 | 1         | 0 | 1         | 0                     | pair_xor[0] = True |
| 2    | 0 | 1         | 1 | 3         | 2                     | pair_xor[2] = True |
| 3    | 1 | 3         | 1 | 3         | 0                     | pair_xor[0] stays True |

After the pair loop: `pair_xor` is `True` at indices `{0, 2}`.

Now combine with each value (`y ∈ {1, 3}`):

| x (pair_xor True) | y | x ^ y | triplet_xor[ … ] set |
|-------------------|---|-------|----------------------|
| 0                 | 1 | 1     | triplet_xor[1] = True |
| 0                 | 3 | 3     | triplet_xor[3] = True |
| 2                 | 1 | 3     | triplet_xor[3] stays True |
| 2                 | 3 | 1     | triplet_xor[1] stays True |

Final `triplet_xor` true at `{1, 3}` → count = 2, matching the expected answer.

---

This lecture covers the **Bit Manipulation** pattern, shows three progressively optimized approaches, and demonstrates the technique on a concrete example. Future lectures will follow the same structure, moving through the DSA topic list in order.