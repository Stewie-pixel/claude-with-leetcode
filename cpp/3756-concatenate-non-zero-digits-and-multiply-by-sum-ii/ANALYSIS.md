# Prefix Sum

## Video Solution

For more details about **Concatenate Non-Zero Digits and Multiply by Sum II**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=wikD48h_hjQ)

## Concept

Prefix sums let us answer range queries in *O(1)* after an *O(n)* preprocessing step.  
Imagine you have a row of houses numbered 1…n and each house has a certain number of mailboxes. If you want to know the total mailboxes in any contiguous block of houses, you could add them up each time (slow), or you could pre‑compute a running total: `pref[i] = total mailboxes in houses 1…i`. Then the answer for houses [l…r] is simply `pref[r] – pref[l‑1]`.  

The same idea works for any operation that is **associative** and **can be undone** by subtraction (addition, multiplication modulo a prime, XOR, etc.). Here we need two pieces of information for each query:

1. The **sum of digits** in the filtered string (just addition → classic prefix sum).  
2. The **concatenated number** formed by the non‑zero digits. Concatenation in base 10 is also associative if we treat it as:  
   `new = old × 10 + digit`.  
   By pre‑computing `prefCat[i] = (value of concatenated non‑zero digits up to i) mod MOD` and the powers of 10, we can retrieve the concatenated value for any sub‑range using the same subtraction trick (with modular arithmetic).

Thus, the problem reduces to:  
*Find the first and last non‑zero digit inside each query range* → binary search on the list of non‑zero positions, then use two prefix arrays to get sum and concatenated number in *O(1)*.

## When to Use It

Use the prefix‑sum technique when you see:

- Queries that ask for an aggregate over a contiguous segment (sum, product, XOR, etc.).
- The aggregate can be built incrementally from left to right.
- You need to answer many queries (≳10⁴) fast after a linear‑time preprocessing.
- The data is static (no updates between queries (or updates are handled separately).

In this problem the “aggregate” is a pair: (sum of digits, concatenated number). Both are incrementally built, so prefix sums work.

## Template

```python
MOD = 10**9 + 7

def preprocess(arr):
    """Return prefix sum array where pref[i] = aggregate of arr[0..i-1]."""
    n = len(arr)
    pref = [0] * (n + 1)
    for i in range(n):
        pref[i+1] = (pref[i] + arr[i]) % MOD   # change + to * or ^ as needed
    return pref

def range_query(pref, l, r):
    """Return aggregate of arr[l..r] inclusive using prefix array."""
    return (pref[r+1] - pref[l]) % MOD   # adjust for non‑additive ops
```

*For concatenation we also need a `pow10` array: `pow10[i] = 10ⁱ mod MOD`.*

## LeetCode Problem Walkthrough

### Problem: 3755. Concatenate Non-Zero Digits and Multiply by Sum II

https://leetcode.com/problems/concatenate-non-zero-digits-and-multiply-by-sum-ii/

---

### Approach 1: Brute Force

**Algorithm**  
For each query `[l, r]`:
1. Slice the substring `s[l:r+1]`.
2. Iterate over its characters, building:
   - `x` by appending each non‑zero digit (`x = x*10 + digit`).
   - `digit_sum` by adding each digit (including zeros – they don’t affect the sum).
3. If no non‑zero digit was seen, `x = 0`.
4. Answer = `(x * digit_sum) % MOD`.

**Implementation**

```python
class Solution:
    MOD = 10**9 + 7

    def sumAndMultiply(self, s: str, queries: List[List[int]]) -> List[int]:
        ans = []
        for l, r in queries:
            x = 0
            digit_sum = 0
            has_nz = False
            for ch in s[l:r+1]:
                d = ord(ch) - 48          # faster than int(ch)
                digit_sum += d
                if d != 0:
                    has_nz = True
                    x = (x * 10 + d) % MOD
            if not has_nz:
                x = 0
            ans.append((x * digit_sum) % self.MOD)
        return ans
```

**Complexity Analysis**

- Time complexity: O(Q · L) where L is average query length → worst‑case O(Q·m) = 10¹⁰.  
- Space complexity: O(1) besides the output.

---

### Approach 2: Prefix Sum + Binary Search (Iterate Over Non‑Zero Sublist)

**Intuition**  
If we know the positions of all non‑zero digits, a query only needs to consider those digits inside `[l, r]`.  
We can obtain the list of relevant non‑zero indices with two binary searches (`lower_bound`/`upper_bound`).  
Then we iterate over that sub‑list to compute the sum and concatenated number.  
This reduces work from scanning the whole substring to scanning only the non‑zero digits inside it.

**Algorithm**  
Preprocess:
- Store `nz_pos` – indices where `s[i] != '0'`.
- Store `nz_val` – the integer value of those digits.

For each query:
1. Use binary search to find `L` = first index in `nz_pos` ≥ l.
2. Use binary search to find `R` = last index in `nz_pos` ≤ r (`upper_bound‑1`).
3. If `L > R` → no non‑zero digit → answer 0.
4. Otherwise, iterate `i = L … R`:
   - Accumulate `digit_sum += nz_val[i]`.
   - Build `x = (x * 10 + nz_val[i]) % MOD`.
5. Answer = `(x * digit_sum) % MOD`.

**Implementation**

```python
import bisect

class Solution:
    MOD = 10**9 + 7

    def sumAndMultiply(self, s: str, queries: List[List[int]]) -> List[int]:
        nz_pos = []
        nz_val = []
        for i, ch in enumerate(s):
            if ch != '0':
                nz_pos.append(i)
                nz_val.append(ord(ch) - 48)

        ans = []
        for l, r in queries:
            L = bisect.bisect_left(nz_pos, l)
            R = bisect.bisect_right(nz_pos, r) - 1
            if L > R:
                ans.append(0)
                continue

            x = 0
            digit_sum = 0
            for i in range(L, R + 1):
                d = nz_val[i]
                digit_sum += d
                x = (x * 10 + d) % self.MOD
            ans.append((x * digit_sum) % self.MOD)
        return ans
```

**Complexity Analysis**

- Time complexity: O(m + Q·k) where k = number of non‑zero digits in the queried range. In the worst case (string all non‑zeros) this is still O(Q·m), but average case is much better when zeros are frequent.
- Space complexity: O(m) for the two auxiliary arrays.

---

### Approach 3: Prefix Sum + Prefix Concatenated Number + Binary Search (Most Optimal)

**Intuition**  
We can answer both the sum and the concatenated number in *O(1)* per query if we have prefix aggregates.  
- The digit sum is a classic prefix sum.  
- For concatenation, observe that building a number left‑to‑right follows the recurrence:  
  `cat[i] = (cat[i‑1] × 10 + val[i]) mod MOD`.  
  Hence `cat` is also a prefix array.  
  To get the concatenated value of a sub‑range `[L…R]` we need to “remove” the contribution of the prefix before `L`.  
  Because each step multiplies by 10, removing a prefix of length `lenL` requires multiplying `cat[L]` by `10^{len}` where `len = R‑L+1`.  
  We pre‑compute `pow10[i] = 10ⁱ mod MOD` to obtain this factor in O(1).  
Thus:
```
sum_range = prefSum[R+1] - prefSum[L]
cat_range = (prefCat[R+1] - prefCat[L] * pow10[len]) mod MOD
```
Both are obtained in O(1) after we locate `L` and `R` via binary search on the non‑zero positions list.

**Algorithm**  
Preprocess:
- Build arrays `nz_pos`, `nz_val` as before.
- Build `prefSum[i+1] = prefSum[i] + nz_val[i]`.
- Build `prefCat[i+1] = (prefCat[i] * 10 + nz_val[i]) % MOD`.
- Build `pow10[i+1] = (pow10[i] * 10) % MOD` with `pow10[0] = 1`.

For each query `[l, r]`:
1. `L = lower_bound(nz_pos, l)`.
2. `R = upper_bound(nz_pos, r) - 1`.
3. If `L > R` → answer 0.
4. `len = R - L + 1`.
5. `sum_range = prefSum[R+1] - prefSum[L]`.
6. `cat_range = (prefCat[R+1] - prefCat[L] * pow10[len]) % MOD` (add MOD before % to avoid negativity).
7. Answer = `(cat_range * sum_range) % MOD`.

**Implementation**

```python
import bisect

class Solution:
    MOD = 10**9 + 7

    def sumAndMultiply(self, s: str, queries: List[List[int]]) -> List[int]:
        # 1️⃣ collect non‑zero digits
        nz_pos = []
        nz_val = []
        for i, ch in enumerate(s):
            if ch != '0':
                nz_pos.append(i)
                nz_val.append(ord(ch) - 48)

        k = len(nz_val)
        # 2️⃣ prefix sums for digit sum
        pref_sum = [0] * (k + 1)
        # 3️⃣ prefix concatenated number (mod MOD)
        pref_cat = [0] * (k + 1)
        # 4️⃣ powers of 10
        pow10 = [1] * (k + 1)

        for i in range(k):
            d = nz_val[i]
            pref_sum[i + 1] = pref_sum[i] + d
            pref_cat[i + 1] = (pref_cat[i] * 10 + d) % self.MOD
            pow10[i + 1] = (pow10[i] * 10) % self.MOD

        ans = []
        for l, r in queries:
            L = bisect.bisect_left(nz_pos, l)
            R = bisect.bisect_right(nz_pos, r) - 1
            if L > R:
                ans.append(0)
                continue

            length = R - L + 1
            # sum of digits in the range
            digit_sum = pref_sum[R + 1] - pref_sum[L]
            # concatenated number in the range
            x = (pref_cat[R + 1] - pref_cat[L] * pow10[length]) % self.MOD
            ans.append((x * digit_sum) % self.MOD)
        return ans
```

**Complexity Analysis**

- Time complexity:  
  - Preprocessing: O(m) to scan the string and build the three prefix arrays.  
  - Each query: two binary searches O(log k) + O(1) arithmetic → O(log m).  
  - Total: O(m + Q·log m) which easily fits the limits (m, Q ≤ 10⁵).  
- Space complexity: O(m) for the position/value lists and the three prefix arrays.

---

## Dry Run

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the optimal algorithm on the first example:

```
s = "10203004"
queries = [[0,7], [1,3], [4,6]]
```

**Step 1 – Preprocess**

| i | s[i] | nz_pos | nz_val |
|---|------|--------|--------|
|0|'1'|0|1|
|1|'0'| – | – |
|2|'2'|1|2|
|3|'0'| – | – |
|4|'3'|2|3|
|5|'0'| – | – |
|6|'0'| – | – |
|7|'4'|3|4|

`nz_pos = [0,2,4,7]`  
`nz_val = [1,2,3,4]`

Build prefix arrays (mod MOD, but numbers stay small):

```
pref_sum: [0, 1, 3, 6, 10]
pref_cat: [0,
           (0*10+1)=1,
           (1*10+2)=12,
           (12*10+3)=123,
           (123*10+4)=1234] → [0,1,12,123,1234]
pow10:    [1,10,100,1000,10000]
```

**Query 0 – [0,7]**

- `L = lower_bound([0,2,4,7],0) = 0`
- `R = upper_bound(... ,7)-1 = 3`
- `length = 3-0+1 = 4`
- `digit_sum = pref_sum[4] - pref_sum[0] = 10 - 0 = 10`
- `x = pref_cat[4] - pref_cat[0]*pow10[4] = 1234 - 0*10000 = 1234`
- `answer = 1234 * 10 = 12340` ✓

**Query 1 – [1,3]**

- `L = lower_bound(...,1) = 1` (first nz ≥ 1 is at index 1 → pos 2)
- `R = upper_bound(...,3)-1 = 1` (last nz ≤ 3 is also at index 1)
- `length = 1`
- `digit_sum = pref_sum[2] - pref_sum[1] = 3 - 1 = 2`
- `x = pref_cat[2] - pref_cat[1]*pow10[1] = 12 - 1*10 = 2`
- `answer = 2 * 2 = 4` ✓

**Query 2 – [4,6]**

- `L = lower_bound(...,4) = 2` (pos 4)
- `R = upper_bound(...,6)-1 = 2` (pos 4)
- `length = 1`
- `digit_sum = pref_sum[3] - pref_sum[2] = 6 - 3 = 3`
- `x = pref_cat[3] - pref_cat[2]*pow10[1] = 123 - 12*10 = 3`
- `answer = 3 * 3 = 9` ✓

All results match the expected output. The dry run confirms that each step of the formula works correctly.