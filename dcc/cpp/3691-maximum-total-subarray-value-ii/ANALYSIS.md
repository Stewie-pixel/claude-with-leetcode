# Heap / Priority Queue

## Video Solution

For more details about **Maximum Total Subarray Value II**, watch the walkthrough at [https://www.youtube.com/watch?v=LZonQ4iUbZQ](https://www.youtube.com/watch?v=LZonQ4iUbZQ)

## Concept

A **max‑heap (priority queue)** lets us repeatedly extract the currently largest element from a collection while being able to insert new candidates efficiently.  
When the set of candidates is too large to materialise explicitly (e.g. all *O(n²)* subarrays), we can often generate the next candidates from a previously extracted one.  
The classic pattern is:

1. Push the best known candidate into the heap.  
2. Repeatedly pop the top, add its value to the answer, and push the *neighbors* that have not been seen before.  
3. Stop after extracting *k* elements.

This technique works whenever the value of a candidate can be derived from a small amount of state (here, the interval `[l, r]`) and the value of any neighbor can be computed quickly (using a sparse table for range max/min).

## When to Use It

Use a heap‑based “best‑first” search when you need the **k largest (or smallest)** values from an implicitly defined set and you can:

* Compute the value of a candidate in *O(1)* or *O(log n)* time.  
* Define a small set of “neighbors” (typically O(1) many) that are guaranteed to contain the next best candidates after removing the current one.  
* Avoid duplicates with a visited set (often a hash set of encoded states).

Typical problems: k‑maximum subarray sums, k‑smallest pairs in two sorted arrays, k‑largest sums of sub‑matrices, and, as we will see, k‑largest subarray `(max‑min)` values.

## Template

```python
import heapq
from typing import List, Tuple

def k_largest_values(get_value, n: int, k: int) -> int:
    """
    get_value(l, r) -> value of the interval [l, r] (inclusive)
    Returns the sum of the k largest values.
    """
    # max‑heap (store negative because heapq is a min‑heap)
    heap: List[Tuple[int, int, int]] = []          # (-value, l, r)
    seen = set()                                   # encoded (l, r)

    def push(l: int, r: int):
        if l > r: return
        key = l * n + r
        if key in seen: return
        seen.add(key)
        val = get_value(l, r)
        heapq.heappush(heap, (-val, l, r))

    # start with the whole array
    push(0, n - 1)

    ans = 0
    for _ in range(k):
        if not heap: break          # safety, should not happen if k is valid
        neg_val, l, r = heapq.heappop(heap)
        ans += -neg_val
        # generate neighbors: shrink from left or right
        push(l, r - 1)
        push(l + 1, r)

    return ans
```

## LeetCode Problem Walkthrough

### Problem: 3691. Maximum Total Subarray Value II  
https://leetcode.com/problems/maximum-total-subarray-value-ii/

We must pick **exactly** `k` distinct non‑empty subarrays.  
The value of a subarray `nums[l..r]` is `max(nums[l..r]) - min(nums[l..r])`.  
Return the maximum possible sum of values of the chosen subarrays.

--------------------------------------------------------------------
### Approach 1: Brute Force

**Algorithm**  
1. Enumerate every possible subarray `[l, r]` (`0 ≤ l ≤ r < n`).  
2. While extending `r` keep track of the current minimum and maximum – this yields the subarray value in *O(1)* per extension.  
3. Store each value in a list.  
4. After all `n·(n+1)/2` values are collected, sort them descending and sum the first `k`.

**Implementation**

```python
class Solution:
    def maxTotalValue(self, nums: List[int], k: int) -> int:
        n = len(nums)
        values = []

        for l in range(n):
            cur_min = cur_max = nums[l]
            for r in range(l, n):
                cur_min = min(cur_min, nums[r])
                cur_max = max(cur_max, nums[r])
                values.append(cur_max - cur_min)

        values.sort(reverse=True)
        return sum(values[:k])
```

**Complexity Analysis**

- Time:  
  *Outer loops* generate each subarray once → **O(n²)**.  
  Inside we update min/max in *O(1)*, so total **O(n²)**.  
  Sorting the list of size `O(n²)` costs **O(n² log n)**.  
  Overall **O(n² log n)**.  
- Space: We store all subarray values → **O(n²)**.

--------------------------------------------------------------------
### Approach 2: Sparse Table + Max‑Heap (Best‑First Search)

**Intuition**  
The value of a subarray depends only on its maximum and minimum.  
If we know the value of interval `[l, r]`, then the two intervals obtained by removing one element from either side — `[l, r‑1]` and `[l+1, r]` — are the *only* candidates that can possibly be the next largest values.  
Why? Any other subarray either lies completely inside `[l, r]` (and thus is reachable by repeatedly shrinking from the ends) or lies outside (and therefore has a different set of elements, whose value cannot exceed the current interval’s value without first passing through one of the two shrunken intervals).  
Consequently, we can perform a best‑first search: start with the whole array, repeatedly extract the current maximum, and push its two “children” if they have not been visited.  
To obtain `max` and `min` of any interval in *O(1)* we build a **Sparse Table** (RMQ) for both max and min.

**Algorithm**

1. Build two sparse tables: `max_sp[j][i]` = max of `nums[i … i+2^j-1]`; similarly for `min_sp`.  
   Preprocessing: **O(n log n)** time and space.  
2. Define a helper `query(l, r)` that returns `max - min` in *O(1)* using the tables.  
3. Initialise a max‑heap with the value of the whole array `[0, n-1]`.  
   Use a `set` (or hash set) of encoded pairs `l * n + r` to avoid pushing the same interval twice.  
4. Repeat `k` times:  
   * pop the interval with largest value, add it to the answer,  
   * push its left‑shrunk (`[l, r-1]`) and right‑shrunk (`[l+1, r]`) neighbors if valid and unseen.  
5. Return the accumulated sum.

**Implementation**

```python
import heapq
from typing import List, Tuple

class Solution:
    def maxTotalValue(self, nums: List[int], k: int) -> int:
        n = len(nums)

        # -------- build sparse tables for RMQ (max and min) ----------
        log = [0] * (n + 1)
        for i in range(2, n + 1):
            log[i] = log[i // 2] + 1
        K = log[n] + 1
        max_sp = [[0] * n for _ in range(K)]
        min_sp = [[0] * n for _ in range(K)]
        max_sp[0] = nums[:]
        min_sp[0] = nums[:]
        for j in range(1, K):
            step = 1 << (j - 1)
            for i in range(n - (1 << j) + 1):
                max_sp[j][i] = max(max_sp[j-1][i], max_sp[j-1][i + step])
                min_sp[j][i] = min(min_sp[j-1][i], min_sp[j-1][i + step])

        def query(l: int, r: int) -> int:
            """return max(nums[l..r]) - min(nums[l..r]) in O(1)"""
            if l == r:
                return 0
            j = log[r - l + 1]
            i2 = r - (1 << j) + 1
            mx = max(max_sp[j][l], max_sp[j][i2])
            mn = min(min_sp[j][l], min_sp[j][i2])
            return mx - mn

        # -------- best‑first search with a max‑heap ---------------
        heap: List[Tuple[int, int, int]] = []          # (-value, l, r)
        seen = set()

        def push(l: int, r: int):
            if l > r:
                return
            key = l * n + r
            if key in seen:
                return
            seen.add(key)
            val = query(l, r)
            heapq.heappush(heap, (-val, l, r))

        push(0, n - 1)

        ans = 0
        for _ in range(k):
            neg_val, l, r = heapq.heappop(heap)
            ans += -neg_val
            push(l, r - 1)
            push(l + 1, r)

        return ans
```

**Complexity Analysis**

- Sparse table construction: **O(n log n)** time, **O(n log n)** space.  
- Each heap operation (`pop` or `push`) costs **O(log h)** where `h ≤ k + 1` (the heap never holds more than the number of generated intervals). We perform at most `k` pops and up to `2k` pushes → **O(k log k)** time.  
- The visited set stores at most `O(k)` entries → **O(k)** space.  

Overall:  
- **Time:** `O(n log n + k log k)`  
- **Space:** `O(n log n + k)`

--------------------------------------------------------------------
### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We walk through the algorithm on the first example:

```
nums = [1, 3, 2], k = 2
n = 3
```

**Sparse table** (omitted for brevity) allows `query(l,r)` in O(1).

**Initial state**

```
heap: [ (-(query(0,2)), 0, 2) ]   # query(0,2) = max=3, min=1 → value=2
seen: {0*3+2 = 2}
```

| Step | Popped (value, l, r) | Action (add to answer) | Pushed neighbors (if unseen) | Heap after pushes (values) | Seen set |
|------|----------------------|------------------------|------------------------------|----------------------------|----------|
| 1    | (2, 0, 2)            | ans = 2                | [0,1] → query=2, [1,2] → value=2 | [(-2,0,1), (-2,1,2)] | {2, 0*3+1=1, 1*3+2=5} |
| 2    | (2, 0, 1) (tie broken by heap order) | ans = 4 | neighbors: [0,0] (value=0), [1,1] (value=0) | [(-2,1,2), (0,0,0), (0,1,1)] | add {0,3,4} |

We have extracted `k = 2` values → **answer = 4**, matching the expected output.

The trace shows how the heap always yields the currently largest subarray value, and how shrinking from either side systematically discovers the next candidates without ever enumerating all `O(n²)` subarrays explicitly.

--------------------------------------------------------------------
## Summary

* **When you need the k largest (or smallest) values from a huge implicit set**, a **max‑heap best‑first search** is a powerful pattern.  
* Pair it with an **O(1) range query structure** (here a Sparse Table) to compute each candidate’s value quickly.  
* Always start with the **whole range** (or the best known candidate) and expand by generating a **constant‑size neighbourhood** (usually removing one element from each side).  
* Use a **hash set** to avoid processing the same state twice.  

This approach reduces the naive **O(n² log n)** enumeration to **O(n log n + k log k)**, easily meeting the constraints (`n ≤ 5·10⁴`, `k ≤ 10⁵`).