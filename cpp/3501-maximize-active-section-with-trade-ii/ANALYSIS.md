# Segment Tree (Range Maximum Query)

## Video Solution

For more details about **Maximize Active Section with Trade II**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=JG9zeMKwtQw).

## Concept

A **segment tree** is a binary tree data structure that stores information about intervals (segments) of an array. Each node represents a segment and stores an aggregate value (e.g., sum, minimum, maximum) for that segment. The tree allows us to:

* **Query** any interval in `O(log n)` time to get the aggregate (here, the maximum).
* **Update** a single element in `O(log n)` time (not needed for this problem because the array is static).

Think of a segment tree as a set of overlapping windows that together cover the whole array. By climbing the tree we can combine the answers from a few windows to answer any arbitrary range query quickly.

## When to Use It

Use a segment tree (or a static RMQ structure like a sparse table) when you see:

* Many offline queries asking for an aggregate (max, min, sum, gcd) over sub‑arrays.
* The underlying array does **not** change between queries (static case) – a sparse table is even faster to build.
* The naïve solution would scan the whole range for each query, leading to `O(n·q)` time, which is too slow for `n, q ≤ 10⁵`.

In this problem we need, for many queries `[l, r]`, the maximum value of a certain pre‑computed array `V` over a sub‑range of indices. A segment tree (or sparse table) gives us that in `O(log n)` per query.

## Template

Below is a reusable **sparse table** (static RMQ) template for range maximum query. It builds in `O(n log n)` and answers each query in `O(1)`. If you prefer a classic segment tree, replace the build/query parts accordingly – the interface stays the same.

```python
def build_sparse_table(arr):
    """
    Preprocesses arr for O(1) range maximum queries.
    Returns a list `st` where st[k][i] is the max of arr[i : i + 2^k].
    """
    n = len(arr)
    # st[0] is the original array
    st = [arr[:]]
    k = 1
    while (1 << k) <= n:
        prev = st[-1]
        cur = [max(prev[i], prev[i + (1 << (k - 1))) for i in range(n - (1 << k) + 1)]
        st.append(cur)
        k += 1
    return st

def query_sparse(st, L, R):
    """
    Returns max(arr[L..R]) inclusive using the sparse table `st`.
    """
    length = R - L + 1
    k = length.bit_length() - 1          # floor(log2(length))
    return max(st[k][L], st[k][R - (1 << k) + 1])
```

*If you need point updates, use a classic segment tree instead (build in `O(n)`, query/update in `O(log n)`).*  

## LeetCode Problem Walkthrough

### Problem: 3501. Maximize Active Section with Trade II
https://leetcode.com/problems/maximize-active-section-with-trade-ii/

We are given a binary string `s`. For each query `[l, r]` we may perform **at most one trade** on the substring `s[l..r]` (augmented with a leading and trailing `'1'` that do **not** count toward the answer). A trade consists of:

1. Choose a contiguous block of `'1'`s that is surrounded by `'0'`s and turn it into `'0'`s.
2. Choose a contiguous block of `'0'`s that is surrounded by `'1'`s and turn it into `'1'`s.

The goal is to maximize the total number of `'1'`s in the whole string after the trade (the augmented `'1'`s are ignored).

### Approach 1: Brute Force

**Algorithm**

For each query independently:

1. Extract the substring `t = s[l..r]`.
2. Try every possible choice of a `'1'`‑block to turn to `'0'` and every possible `'0'`‑block to turn to `'1'` (respecting the “surrounded by opposite bit” rule).
3. Simulate the trade, count the resulting `'1'`s in the whole string, keep the maximum.
4. If no valid pair exists, answer is just the original count of `'1'`s.

**Implementation**

```python
def brute(s, queries):
    n = len(s)
    total_ones = s.count('1')
    ans = []
    for l, r in queries:
        sub = s[l:r+1]
        m = len(sub)
        best = total_ones          # case: no trade
        # enumerate all possible 1‑blocks inside sub that are surrounded by 0 in the augmented string
        for i in range(m):
            if sub[i] != '1':
                continue
            j = i
            while j < m and sub[j] == '1':
                j += 1
            # block [i, j-1] is all 1s
            # check it is surrounded by 0 in the augmented string:
            left_ok  = (i == 0 and True) or (sub[i-1] == '0')
            right_ok = (j == m and True) or (sub[j] == '0')
            if not (left_ok and right_ok):
                continue
            # now enumerate 0‑blocks similarly
            for p in range(m):
                if sub[p] != '0':
                    continue
                q = p
                while q < m and sub[q] == '0':
                    q += 1
                # block [p, q-1] is all 0s
                left_ok0  = (p == 0 and True) or (sub[p-1] == '1')
                right_ok0 = (q == m and True) or (sub[q] == '1')
                if not (left_ok0 and right_ok0):
                    continue
                # simulate trade: turn 1‑block to 0, 0‑block to 1
                # net change = (+len0) - (+len1) because we lose len1 ones and gain len0 ones
                gain = (q - p) - (j - i)
                ans_val = total_ones + gain
                if ans_val > best:
                    best = ans_val
        ans.append(best)
    return ans
```

**Complexity Analysis**

*Time*: For each query we may examine `O(m²)` pairs of blocks, where `m = r-l+1`. In the worst case `m = n`, giving `O(q·n²)`. With `n, q ≤ 10⁵` this is far too slow.  
*Space*: `O(1)` besides the input and output.

### Approach 2: Prefix‑Sum + Block Insight (Intermediate Optimization)

**Intuition**

Observe that a trade essentially **swaps** a block of `1`s with a neighboring block of `0`s (the augmented `1`s at the ends guarantee the “surrounded” condition).  
If we turn a `1`‑block of length `x` into `0`s and a `0`‑block of length `y` into `1`s, the total number of `1`s changes by `y - x`.  
Therefore, for a fixed query we want to **maximize** `y - x` where:

* `x` is the length of a `1`‑block that is surrounded by `0`s in the *augmented* substring.
* `y` is the length of a `0`‑block that is surrounded by `1`s in the *augmented* substring.

If we look at the augmented string `t = '1' + s[l..r] + '1'`, the condition “surrounded by opposite bit” simply means we are picking **adjacent** blocks of equal bits in `t`.  
Specifically, a valid trade corresponds to choosing two consecutive blocks in `t` where the first block is `1`s and the second block is `0`s (or vice‑versa, depending on which we turn first). After the swap the net gain equals `len(second) - len(first)`.

Thus the problem reduces to: *within the augmented substring, find the maximum difference `len(second block) - len(first block)` over all adjacent blocks where the first block consists of `1`s and the second of `0`s.*  

If we pre‑process the original string into **zero‑blocks** (contiguous `'0'`s) and remember their start/end indices, then any adjacent pair of zero‑blocks is separated by exactly one block of `1`s. The gain for turning that middle `1`‑block to `0` and the right zero‑block to `1` is:

```
gain = length(right zero block) - length(middle one block)
```

Similarly, turning the left zero‑block to `1` and the middle `1`‑block to `0` gives `gain = length(left zero block) - length(middle one block)`.  
Taking the absolute best we can write the gain as:

```
gain = max( len(left zero) , len(right zero) ) - len(middle one)
```

If we define for each *gap* between two consecutive zero‑blocks the value  

```
V[i] = len(zero_i) + len(zero_{i+1})
```

and let `lenOne =` length of the `1`‑block between them, then  

```
gain_i = V[i] - 2 * lenOne
```

Because `lenOne` is constant for a given gap, maximizing gain is equivalent to maximizing `V[i]` over the gaps that lie completely inside the query range (after adjusting for partial overlap at the ends).  

Hence we need a data structure that can return the **maximum V[i]** for gaps whose indices fall inside a query interval – a classic **range maximum query**.

**Algorithm**

1. Scan `s` once to collect all zero‑blocks: store their start (`zs[i]`) and end (`ze[i]`).
2. For each adjacent pair of zero‑blocks compute `V[i] = (ze[i]-zs[i]+1) + (ze[i+1]-zs[i+1]+1)`.
3. Build a static RMQ (sparse table) over the array `V` to answer max on any sub‑range in `O(1)`.
4. For each query `[l, r]`:
   * Find the first zero‑block that ends at or after `l` (`ja`).
   * Find the last zero‑block that starts at or before `r` (`jb`).
   * The candidate gaps are those with indices from `ja` to `jb-1`.  
     * If the range is empty → no valid trade.
   * The best gain inside the range is the maximum of:
        * `V[ja]` clipped by the query borders (partial overlap on the left side),
        * `V[jb-1]` clipped on the right side,
        * the full maximum of any completely‑contained gap (`ja+1 … jb-2`) obtained from the RMQ.
   * Answer = `originalOnes + maxGain`.

**Implementation**

```python
from bisect import bisect_left, bisect_right
from math import log2, floor

class Solution:
    def maxActiveSectionsAfterTrade(self, s: str, queries: List[List[int]]) -> List[int]:
        n = len(s)
        total_ones = s.count('1')

        # 1. collect zero‑blocks
        zs, ze = [], []          # start and end indices (inclusive) of each zero block
        i = 0
        while i < n:
            if s[i] == '0':
                start = i
                while i < n and s[i] == '0':
                    i += 1
                end = i - 1
                zs.append(start)
                ze.append(end)
            else:
                i += 1

        nblocks = len(zs)
        # 2. build V for each adjacent zero‑block pair
        V = []
        for j in range(nblocks - 1):
            len_left  = ze[j] - zs[j] + 1
            len_right = ze[j+1] - zs[j+1] + 1
            V.append(len_left + len_right)          # = len(left zero) + len(right zero)

        # 3. build sparse table for RMQ (max) on V
        if V:
            st = [V[:]]
            k = 1
            while (1 << k) <= len(V):
                prev = st[-1]
                cur = [max(prev[i], prev[i + (1 << (k-1))]) for i in range(len(V) - (1 << k) + 1)]
                st.append(cur)
                k += 1
        else:
            st = []

        def rmq(L, R):          # inclusive indices on V
            """return max(V[L..R]) using sparse table"""
            length = R - L + 1
            k = length.bit_length() - 1
            return max(st[k][L], st[k][R - (1 << k) + 1])

        # helpers to clip a V value when the query cuts the zero‑block partially
        def clip_gap(idx, l, r):
            """V[idx] corresponds to zero blocks idx and idx+1.
               Return its contribution after limiting to [l, r]."""
            left_len  = ze[idx] - zs[idx] + 1
            right_len = ze[idx+1] - zs[idx+1] + 1
            # how much of each block lies inside [l, r]?
            left_inside  = max(0, min(ze[idx], r) - max(zs[idx], l) + 1)
            right_inside = max(0, min(ze[idx+1], r) - max(zs[idx+1], l) + 1)
            return left_inside + right_inside

        ans = []
        for l, r in queries:
            if nblocks < 2:
                ans.append(total_ones)
                continue

            # first zero block that could contribute (its end >= l)
            ja = bisect_left(ze, l)          # index in ze/ zs
            # last zero block that could contribute (its start <= r)
            jb = bisect_right(zs, r) - 1

            if ja > jb:          # no zero block touches the query range
                ans.append(total_ones)
                continue

            # we need gaps between zero blocks: indices ja .. jb-1 in V
            left_gap = ja
            right_gap = jb - 1
            best_gain = 0

            if left_gap == right_gap:
                # only one possible gap, may be clipped on both sides
                best_gain = clip_gap(left_gap, l, r)
            else:
                # consider leftmost gap (may be clipped on left)
                best_gain = max(best_gain, clip_gap(left_gap, l, r))
                # consider rightmost gap (may be clipped on right)
                best_gain = max(best_gain, clip_gap(right_gap, l, r))
                # gaps fully inside: left_gap+1 .. right_gap-1
                if left_gap + 1 <= right_gap - 1:
                    inner = rmq(left_gap + 1, right_gap - 1)
                    best_gain = max(best_gain, inner)

            ans.append(total_ones + best_gain)

        return ans
```

**Complexity Analysis**

*Preprocessing*  
- Scanning `s` → `O(n)`  
- Building `V` → `O(number of zero blocks) ≤ O(n)`  
- Sparse table construction → `O(n log n)` time, `O(n log n)` space.

*Per query*  
- Two binary searches on `ze`/`zs` → `O(log n)`  
- Up to two `clip_gap` calls → `O(1)`  
- One RMQ on the sparse table → `O(1)`  

Overall: `O((n + q) log n)` time, `O(n log n)` space – easily fits the limits.

### Approach 3: Most Optimal (Same as Intermediate, but with tidy code)

The solution above already achieves the optimal asymptotic complexity. The only possible micro‑optimizations are:

* Using the built‑in `bit_length` for log₂.
* Storing `V` as a plain list and the sparse table as a list of lists (as shown).
* Early exit when `nblocks < 2`.

No further asymptotic improvement is possible because we must at least read the input and answer each query.

**Algorithm** – identical to Approach 2.  
**Implementation** – the code shown in Approach 2 is the final submitted version.  
**Complexity Analysis** – same as Approach 2: `O((n+q) log n)` time, `O(n log n)` space.

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the algorithm on the first example from the statement.

**Example**  
`s = "0100"`  
`queries = [[0,3]]`

### Step 1: Identify zero‑blocks

| Index | Char |
|-------|------|
| 0     | 0 |
| 1     | 1 |
| 2     | 0 |
| 3     | 0 |

Zero‑blocks:  
- Block 0: start=0, end=0 → length = 1  
- Block 1: start=2, end=3 → length = 2  

`zs = [0, 2]`, `ze = [0, 3]`, `nblocks = 2`.

### Step 2: Build V

Only one adjacent pair (block 0 & block 1):

```
V[0] = len(block0) + len(block1) = 1 + 2 = 3
```

`V = [3]`

Sparse table on `[3]` is trivial.

### Step 3: Process query [0,3]

- `total_ones` in original string = 1 (the `'1'` at position 1).
- Find first zero block with `end ≥ 0`: `ja = bisect_left([0,3], 0) = 0`.
- Find last zero block with `start ≤ 3`: `jb = bisect_right([0,2], 3) - 1 = 1`.
- Gaps indices: from `ja` to `jb-1` → `0 … 0` → only gap 0.

Now compute clipped gain for gap 0:

```
clip_gap(0, 0, 3):
    left_inside  = min(ze[0],3) - max(zs[0],0) + 1 = min(0,3)-max(0,0)+1 = 0-0+1 = 1
    right_inside = min(ze[1],3) - max(zs[1],0) + 1 = min(3,3)-max(2,0)+1 = 3-2+1 = 2
    return 1 + 2 = 3
```

No inner gaps exist.

`best_gain = 3`.

Answer = `total_ones + best_gain = 1 + 3 = 4`.

This matches the expected output `[4]`.

The same procedure applied to the other queries yields `[4,3,1,1]` as shown in the problem statement.
