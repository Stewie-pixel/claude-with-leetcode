# Union Find

## Video Solution

For more details about **Largest Component Size by Common Factor**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=DNfNZwilaC4)

## Concept

Union‑Find (Disjoint Set Union, DSU) is a data structure that keeps track of a partition of a set into disjoint subsets. It supports two near‑constant‑time operations:

* **find(x)** – returns the representative (root) of the set containing *x* (with path compression).
* **unite(x, y)** – merges the sets containing *x* and *y* (union by rank/size).

In this problem we want to connect numbers that share a common factor > 1. Directly checking every pair would be O(n²). Instead, we notice that if two numbers share a prime factor *p*, they are both multiples of *p*. Thus we can union each number with **all of its prime factors**. After processing the whole array, any two numbers that belong to the same set are connected through a chain of shared prime factors, and the size of each set is exactly the size of a connected component in the original graph.

## When to Use It

Use Union‑Find when you see:

* “Connect / join items that satisfy a relation” (e.g., share a factor, are within distance, have the same parent).
* Questions about **largest component**, **number of groups**, or **whether two nodes are in the same group**.
* The relation can be expressed via a **common attribute** (prime factor, character, coordinate, etc.) that can be iterated over for each element.
* Constraints allow O(N α(N)) or O(N log MAX) where α is the inverse Ackermann function (practically constant).

## Template

Below is a generic Union‑Find class in Python with path compression and union by size (you can switch to rank if you prefer). The template also shows how to factorize a number using trial division up to √n.

```python
class DSU:
    def __init__(self, n):
        self.parent = list(range(n))
        self.size   = [1] * n          # size of each component (for union by size)

    def find(self, x):
        # Path compression
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]
            x = self.parent[x]
        return x

    def unite(self, x, y):
        xr, yr = self.find(x), self.find(y)
        if xr == yr:
            return
        # Union by size: attach smaller tree under larger one
        if self.size[xr] < self.size[yr]:
            xr, yr = yr, xr
        self.parent[yr] = xr
        self.size[xr] += self.size[yr]

# Example usage for the problem:
def largest_component_size(nums):
    max_val = max(nums)
    dsu = DSU(max_val + 1)          # we will use the numbers themselves as indices
    # map from prime factor to the first number that had it (optional optimization)
    factor_rep = {}

    for num in nums:
        x = num
        # factorize num
        d = 2
        while d * d <= x:
            if x % d == 0:
                # union num with this prime factor
                dsu.unite(num, d)
                # remember the first occurrence of this factor (optional)
                if d not in factor_rep:
                    factor_rep[d] = num
                else:
                    dsu.unite(num, factor_rep[d])
                while x % d == 0:
                    x //= d
            d += 1 if d == 2 else 2   # after 2, test only odd numbers
        if x > 1:                     # remaining prime factor
            dsu.unite(num, x)
            if x not in factor_rep:
                factor_rep[x] = num
            else:
                dsu.unite(num, factor_rep[x])

    # count component sizes for the original numbers only
    comp_cnt = {}
    ans = 0
    for num in nums:
        root = dsu.find(num)
        comp_cnt[root] = comp_cnt.get(root, 0) + 1
        ans = max(ans, comp_cnt[root])
    return ans
```

*Key points in the template*  
* `find` uses iterative path compression (fast and avoids recursion depth issues).  
* `unite` uses union by size (you could replace `size` with `rank`).  
* Factorization loop runs in O(√num) per number; with a pre‑computed smallest‑prime‑factor (SPF) sieve you can reduce it to O(log num) – see Approach 3.

## LeetCode Problem Walkthrough

### Problem: 952. Largest Component Size by Common Factor  
https://leetcode.com/problems/largest-component-size-by-common-factor/

---

### Approach 1: Brute Force (Pairwise GCD)

**Algorithm**  
For every pair `(i, j)` with `i < j`, compute `g = gcd(nums[i], nums[j])`.  
If `g > 1` we union the two indices (or the values themselves).  
After processing all pairs, count the size of each set and return the maximum.

**Implementation**

```python
from math import gcd

class DSU:
    def __init__(self, n):
        self.parent = list(range(n))
        self.size   = [1] * n
    def find(self, x):
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]
            x = self.parent[x]
        return x
    def unite(self, x, y):
        xr, yr = self.find(x), self.find(y)
        if xr == yr: return
        if self.size[xr] < self.size[yr]:
            xr, yr = yr, xr
        self.parent[yr] = xr
        self.size[xr] += self.size[yr]

class Solution:
    def largestComponentSize(self, nums):
        n = len(nums)
        dsu = DSU(n)
        for i in range(n):
            for j in range(i+1, n):
                if gcd(nums[i], nums[j]) > 1:
                    dsu.unite(i, j)
        cnt = {}
        ans = 0
        for i in range(n):
            r = dsu.find(i)
            cnt[r] = cnt.get(r, 0) + 1
            ans = max(ans, cnt[r])
        return ans
```

**Complexity Analysis**

- Time complexity: O(n² · log M) where `M = max(nums)` (the `gcd` cost).  
- Space complexity: O(n) for the DSU arrays.

*Why it’s too slow*: With `n ≤ 2·10⁴`, the quadratic term (~4·10⁸ pair checks) exceeds time limits.

---

### Approach 2: Union‑Find + Prime Factorization (Trial Division)

**Intuition**  
If two numbers share any prime factor `p > 1`, they are indirectly connected through that factor.  
Instead of comparing every pair, we **union each number with all of its distinct prime factors**.  
After all unions, numbers that share a factor (directly or via a chain) end up in the same set.  
Counting the size of each set among the original numbers yields the answer.

**Algorithm**  

1. Determine `max_val = max(nums)`.  
2. Initialize a DSU for values `0 … max_val` (we will use the actual number as the node).  
3. For each `num` in `nums`:
   * Factorize `num` by trial division up to `√num`.  
   * For each distinct prime factor `p` found, call `unite(num, p)`.  
   * If after the loop a remainder `> 1` remains, it is also a prime factor – union with it.  
4. After processing all numbers, iterate over `nums` again, find the root of each number, and count how many numbers map to each root.  
5. Return the largest count.

**Implementation**

```python
class DSU:
    def __init__(self, n):
        self.parent = list(range(n))
        self.size   = [1] * n
    def find(self, x):
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]
            x = self.parent[x]
        return x
    def unite(self, x, y):
        xr, yr = self.find(x), self.find(y)
        if xr == yr: return
        if self.size[xr] < self.size[yr]:
            xr, yr = yr, xr
        self.parent[yr] = xr
        self.size[xr] += self.size[yr]

class Solution:
    def largestComponentSize(self, nums):
        max_val = max(nums)
        dsu = DSU(max_val + 1)

        for num in nums:
            x = num
            d = 2
            while d * d <= x:
                if x % d == 0:
                    dsu.unite(num, d)
                    while x % d == 0:
                        x //= d
                d += 1 if d == 2 else 2   # 2, then only odds
            if x > 1:                     # leftover prime factor
                dsu.unite(num, x)

        cnt = {}
        ans = 0
        for num in nums:
            root = dsu.find(num)
            cnt[root] = cnt.get(root, 0) + 1
            ans = max(ans, cnt[root])
        return ans
```

**Complexity Analysis**

- Factorization per number: O(√num) in the worst case.  
  With `num ≤ 10⁵`, √num ≤ 316, so total time ≤ O(n·√MAX) ≈ 2·10⁴·3·10² ≈ 6·10⁶ operations – easily fits.  
- Time complexity: O(n · √M) where `M = max(nums)`.  
- Space complexity: O(M) for the DSU arrays (≈ 10⁵+1 integers).

---

### Approach 3: Union‑Find + Sieve‑Based Smallest Prime Factor (SPF)

**Intuition**  
Factorizing each number by trial division repeats work for numbers that share small primes.  
By pre‑computing the **smallest prime factor** for every integer up to `max_val` using a linear sieve (or classic O(N log log N) sieve), we can factor any number in O(log num) time by repeatedly dividing by its SPF.  
This brings the overall complexity close to O(n log M) and is the standard fast solution for factor‑heavy problems.

**Algorithm**

1. Build an array `spf[0 … max_val]` where `spf[x]` = smallest prime dividing `x` (for `x ≥ 2`).  
   *Initialize `spf[i] = i`; then for each prime `p`, mark multiples.*  
2. Initialize DSU for `0 … max_val`.  
3. For each `num` in `nums`:
   * While `num > 1`:
        - `p = spf[num]`
        - `unite(original_number, p)`
        - Divide `num` by `p` until it is no longer divisible (to skip duplicate factors).  
4. Count component sizes as before and return the maximum.

**Implementation**

```python
class DSU:
    def __init__(self, n):
        self.parent = list(range(n))
        self.size   = [1] * n
    def find(self, x):
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]
            x = self.parent[x]
        return x
    def unite(self, x, y):
        xr, yr = self.find(x), self.find(y)
        if xr == yr: return
        if self.size[xr] < self.size[yr]:
            xr, yr = yr, xr
        self.parent[yr] = xr
        self.size[xr] += self.size[yr]

def sieve_spf(limit):
    spf = list(range(limit + 1))
    spf[0] = spf[1] = 1
    for i in range(2, int(limit**0.5) + 1):
        if spf[i] == i:                     # i is prime
            for j in range(i * i, limit + 1, i):
                if spf[j] == j:
                    spf[j] = i
    return spf

class Solution:
    def largestComponentSize(self, nums):
        max_val = max(nums)
        spf = sieve_spf(max_val)
        dsu = DSU(max_val + 1)

        for num in nums:
            x = num
            last = 0
            while x > 1:
                p = spf[x]
                if p != last:               # avoid unioning same factor repeatedly
                    dsu.unite(num, p)
                    last = p
                while x % p == 0:
                    x //= p

        cnt = {}
        ans = 0
        for num in nums:
            root = dsu.find(num)
            cnt[root] = cnt.get(root, 0) + 1
            ans = max(ans, cnt[root])
        return ans
```

**Complexity Analysis**

- Sieve construction: O(M log log M) time, O(M) space.  
- Factorization per number: O(log num) because each step removes at least one prime factor.  
- Total time: O(M log log M + n log M) ≈ O(M log log M) (dominant term is the sieve).  
- Space: O(M) for `spf` and DSU arrays.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We illustrate Approach 2 on the first example `nums = [4, 6, 15, 35]`.

#### Dry Run

| Step | num | Prime factors found | Union operations performed |
|------|-----|---------------------|----------------------------|
| 1    | 4   | 2                   | unite(4, 2)                |
| 2    | 6   | 2, 3                | unite(6, 2) → now {2,4,6} in same set<br>unite(6, 3) → {2,3,4,6} |
| 3    | 15  | 3, 5                | unite(15, 3) → merges 15 into the big set<br>unite(15, 5) → adds 5 |
| 4    | 35  | 5, 7                | unite(35, 5) → merges 35 into the big set<br>unite(35, 7) → adds 7 |

After processing, the distinct roots among the original numbers are:

- `find(4)` → root representing `{2,3,4,5,6,7,15,35}`  
- `find(6)` → same root  
- `find(15)` → same root  
- `find(35)` → same root  

All four numbers share the same root, so the component size is **4**, which matches the expected answer.

--- 

This lecture covers the Union‑Find pattern, shows three progressively efficient solutions, and includes a concrete dry‑run to cement understanding. Use the provided template for future problems that involve grouping by a shared attribute.