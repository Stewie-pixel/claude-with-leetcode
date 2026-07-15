# Segment Tree

## Video Solution

For more details about **Range Sum Query - Mutable**, watch the walkthrough at [https://www.youtube.com/watch?v=G9GRvQRtGOc](https://www.youtube.com/watch?v=G9GRvQRtGOc)

## Concept

A **segment tree** is a binary tree that stores aggregated information (e.g., sum, min, max) for intervals of an array. Each leaf corresponds to a single array element, and each internal node stores the result of merging its two children.  
Because the tree height is `log n`, both point updates and range queries run in `O(log n)` time.

*Real‑world analogy*: Imagine a company’s organizational chart where each employee knows the total sales of their subtree. Updating a single employee’s sales requires updating all ancestors up to the CEO (log N steps). Asking for total sales in a department range corresponds to combining the pre‑computed totals of the relevant subtrees.

## When to Use It

Use a segment tree when you need to support **many** intermixed operations of:
- Point updates (change a single element)
- Range queries (aggregate over a contiguous interval)

Typical clues in a problem statement:
- “update an element” + “query sum/min/max in range”
- Constraints allow up to 10⁴‑10⁵ operations, making a naïve O(n) per query too slow.
- The array size is static (no insert/delete of positions).

## Template

```python
class SegmentTree:
    def __init__(self, data, default=0, func=lambda a, b: a + b):
        """build segment tree for `data` using `func` (e.g., sum, min, max)."""
        self.n = len(data)
        self.default = default          # neutral element for func
        self.func = func
        self.size = 1
        while self.size < self.n:       # next power of two
            self.size <<= 1
        self.tree = [default] * (2 * self.size)
        # fill leaves
        self.tree[self.size:self.size + self.n] = data
        # build internal nodes
        for i in range(self.size - 1, 0, -1):
            self.tree[i] = self.func(self.tree[i << 1], self.tree[i << 1 | 1])

    def update(self, pos, value):
        """set data[pos] = value"""
        i = pos + self.size
        self.tree[i] = value
        i >>= 1
        while i:
            self.tree[i] = self.func(self.tree[i << 1], self.tree[i << 1 | 1])
            i >>= 1

    def query(self, left, right):
        """return func(data[left:right+1])"""
        left += self.size
        right += self.size + 1   # make right exclusive
        res_left = self.default
        res_right = self.default
        while left < right:
            if left & 1:
                res_left = self.func(res_left, self.tree[left])
                left += 1
            if right & 1:
                right -= 1
                res_right = self.func(self.tree[right], res_right)
            left >>= 1
            right >>= 1
        return self.func(res_left, res_right)
```

*The generic template above works for any associative operation; for this problem we use `func=lambda a,b: a+b` and `default=0`.*

## LeetCode Problem Walkthrough

### Problem: 307. Range Sum Query - Mutable

https://leetcode.com/problems/range-sum-query-mutable/

We need a data structure that supports:
- `update(i, val)` – point assignment
- `sumRange(l, r)` – range sum query  

Both operations may be called up to 3·10⁴ times.

---

### Approach 1: Brute Force

**Algorithm**
- Store the array directly.
- `update`: assign `nums[index] = val` → O(1).
- `sumRange`: iterate from `left` to `right` and accumulate → O(n).

**Implementation**

```python
class NumArray:
    def __init__(self, nums):
        self.nums = nums

    def update(self, index, val):
        self.nums[index] = val

    def sumRange(self, left, right):
        return sum(self.nums[left:right + 1])
```

**Complexity Analysis**
- Time complexity:  
  - `update`: O(1)  
  - `sumRange`: O(n) — we scan the whole interval.
- Space complexity: O(n) — the original array.

---

### Approach 2: Square‑Root Decomposition

**Intuition**
If we split the array into blocks of size ≈√n, we can pre‑compute the sum of each block.  
A range query then touches at most two partial blocks (O(√n)) plus a few whole blocks (O(√n)).  
Point updates only need to adjust the sum of the block containing the index (O(1)).  
This gives O(√n) per operation, better than O(n) but not optimal.

**Algorithm**
1. Choose block size `B = ⌈√n⌉`.
2. Maintain `nums` and an auxiliary array `block_sum` where `block_sum[i]` stores the sum of block `i`.
3. `update(i, val)`:  
   - Compute block `b = i // B`.  
   - Update `nums[i]` and adjust `block_sum[b]` by `delta = val - old`.
4. `sumRange(l, r)`:  
   - Sum the left partial block from `l` to the end of its block.  
   - Sum whole blocks that lie completely inside `[l, r]`.  
   - Sum the right partial block from start of its block to `r`.

**Implementation**

```python
import math

class NumArray:
    def __init__(self, nums):
        self.nums = nums
        self.n = len(nums)
        self.B = int(math.sqrt(self.n)) + 1
        self.block_sum = [0] * ((self.n + self.B - 1) // self.B)
        for i, v in enumerate(nums):
            self.block_sum[i // self.B] += v

    def update(self, index, val):
        delta = val - self.nums[index]
        self.nums[index] = val
        self.block_sum[index // self.B] += delta

    def sumRange(self, left, right):
        res = 0
        start_block = left // self.B
        end_block = right // self.B
        if start_block == end_block:
            for i in range(left, right + 1):
                res += self.nums[i]
            return res
        # left partial block
        end_left = (start_block + 1) * self.B
        for i in range(left, min(end_left, self.n)):
            res += self.nums[i]
        # middle whole blocks
        for b in range(start_block + 1, end_block):
            res += self.block_sum[b]
        # right partial block
        start_right = end_block * self.B
        for i in range(start_right, right + 1):
            res += self.nums[i]
        return res
```

**Complexity Analysis**
- Time complexity:  
  - `update`: O(1)  
  - `sumRange`: O(√n) — at most two partial blocks + O(number of whole blocks) ≤ 2·√n.
- Space complexity: O(n) for `nums` + O(√n) for `block_sum` → O(n).

---

### Approach 3: Segment Tree (Optimal)

**Intuition**
A segment tree stores the sum of each segment in a binary tree.  
Both updating a leaf and querying a range require visiting only O(log n) nodes because we move from the leaf (or query interval) up to the root, merging results along the way.  
This yields the optimal O(log n) per operation while keeping linear space.

**Algorithm**
1. Build a segment tree where each node holds the sum of its interval.  
   - Use an array `seg` of size 4·n (safe upper bound).  
   - Recursively build: leaf = `nums[pos]`; internal node = sum of children.
2. `update(index, val)`:  
   - Recurse to the leaf representing `index`, set its value to `val`, then recompute sums on the way back.
3. `sumRange(left, right)`:  
   - Recurse; if the current node interval is fully inside the query, return its stored sum.  
   - If outside, return 0.  
   - Otherwise, query both children and add the results.

**Implementation**

```python
class NumArray:
    def __init__(self, nums):
        self.n = len(nums)
        self.seg = [0] * (4 * self.n)          # enough space for a full binary tree
        self._build(nums, 1, 0, self.n - 1)

    def _build(self, nums, idx, l, r):
        """build segment tree node idx covering [l, r]"""
        if l == r:
            self.seg[idx] = nums[l]
            return
        mid = (l + r) // 2
        self._build(nums, idx * 2, l, mid)
        self._build(nums, idx * 2 + 1, mid + 1, r)
        self.seg[idx] = self.seg[idx * 2] + self.seg[idx * 2 + 1]

    def _update(self, idx, l, r, pos, val):
        """point update: set position pos to val"""
        if l == r:
            self.seg[idx] = val
            return
        mid = (l + r) // 2
        if pos <= mid:
            self._update(idx * 2, l, mid, pos, val)
        else:
            self._update(idx * 2 + 1, mid + 1, r, pos, val)
        self.seg[idx] = self.seg[idx * 2] + self.seg[idx * 2 + 1]

    def _query(self, idx, l, r, ql, qr):
        """range sum query on [ql, qr]"""
        if qr < l or ql > r:               # no overlap
            return 0
        if ql <= l and r <= qr:            # total overlap
            return self.seg[idx]
        mid = (l + r) // 2
        left_sum = self._query(idx * 2, l, mid, ql, qr)
        right_sum = self._query(idx * 2 + 1, mid + 1, r, ql, qr)
        return left_sum + right_sum

    def update(self, index, val):
        self._update(1, 0, self.n - 1, index, val)

    def sumRange(self, left, right):
        return self._query(1, 0, self.n - 1, left, right)
```

**Complexity Analysis**
- Time complexity:  
  - `build`: O(n) (each node visited once).  
  - `update`: O(log n) — height of the tree.  
  - `sumRange`: O(log n) — at most two nodes per level are visited.  
- Space complexity: O(n) for the segment tree array (≈4n).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the operations on the example `nums = [1, 3, 5]`.

#### Build

```
                [0,2] sum=9
               /           \
          [0,1] sum=4    [2,2] sum=5
          /      \
     [0,0] sum=1  [1,1] sum=3
```

#### sumRange(0,2)

- Query root `[0,2]` → fully inside → return 9.

#### update(1,2)  (change index 1 from 3 → 2, delta = -1)

1. Leaf `[1,1]` becomes 2.
2. Recompute `[0,1]`: 1 + 2 = 3.
3. Recompute root `[0,2]`: 3 + 5 = 8.

Tree after update:

```
                [0,2] sum=8
               /           \
          [0,1] sum=3    [2,2] sum=5
          /      \
     [0,0] sum=1  [1,1] sum=2
```

#### sumRange(0,2) after update

- Root `[0,2]` fully inside → return 8.

The dry‑run matches the expected outputs `[9, null, 8]`.

---