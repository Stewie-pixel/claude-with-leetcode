# Math & Geometry

## Video Solution

For more details about **Sorted GCD Pair Queries**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=F60Hsd5q1KY).

## Concept

The naive way to answer each query is to generate all `n·(n‑1)/2` GCDs, sort them, and then index into the sorted list.  
That is `O(n² log n)` time and `O(n²)` space – impossible for `n ≤ 10⁵`.

The key observation is that the **value of a GCD can only be a divisor of some array element**.  
If we know, for every integer `d`, how many array elements are multiples of `d`, we can compute how many pairs have GCD **exactly** `d` using inclusion‑exclusion (a sieve‑like subtraction).  
Once we have the exact count of pairs for each possible GCD, we build a prefix sum over GCD values.  
The prefix sum tells us how many pair‑GCDs are `≤ x`.  
Answering a query `k` then becomes: find the smallest GCD value `g` such that `prefix[g] > k`.  
This is a binary search on the prefix array – `O(log M)` per query, where `M = max(nums)`.

The overall steps:

1. Count frequency of each number (`freq[x]`).
2. For each possible divisor `d`, compute `cnt[d] = Σ freq[m]` over all multiples `m` of `d` (how many numbers are divisible by `d`).
3. The number of pairs whose **both** numbers are divisible by `d` is `C(cnt[d], 2)`.  
   From this we subtract pairs already counted for multiples of `d` to obtain `exact[d]` = number of pairs whose GCD is exactly `d`.
4. Build `prefix[d] = Σ exact[i]` for `i ≤ d`.
5. For each query `k`, binary‑search `prefix` to find the smallest `d` with `prefix[d] > k`.

## When to Use It

Use this sieve‑based counting technique when you see:

- Problems asking for the **k‑th smallest** (or order statistic) over all pairwise GCDs, LCMs, or similar pairwise functions.
- Constraints where `n` is large (up to 10⁵) but the value range `max(nums)` is moderate (≤ 5·10⁴).
- The answer can be derived from counting how many pairs satisfy a divisor property, then using inclusion‑exclusion to get exact counts.

## Template

```python
import bisect
from typing import List

class Solution:
    def gcdValues(self, nums: List[int], queries: List[int]) -> List[int]:
        # 1. frequency of each value
        max_val = max(nums)
        freq = [0] * (max_val + 1)
        for x in nums:
            freq[x] += 1

        # 2. count of numbers divisible by d
        cnt = [0] * (max_val + 1)
        for d in range(1, max_val + 1):
            for mult in range(d, max_val + 1, d):
                cnt[d] += freq[mult]

        # 3. exact number of pairs with GCD == d (inclusion–exclusion)
        exact = [0] * (max_val + 1)
        for d in range(max_val, 0, -1):
            pairs = cnt[d] * (cnt[d] - 1) // 2          # all pairs where both numbers are multiples of d
            for mult in range(2 * d, max_val + 1, d):   # subtract those whose GCD is a multiple of d
                pairs -= exact[mult]
            exact[d] = pairs

        # 4. prefix sum of exact counts => how many pair‑GCDs are ≤ d
        prefix = [0] * (max_val + 1)
        for d in range(1, max_val + 1):
            prefix[d] = prefix[d - 1] + exact[d]

        # 5. answer each query via binary search on prefix
        ans = []
        for k in queries:
            # smallest d with prefix[d] > k
            g = bisect.bisect_right(prefix, k)
            ans.append(g)
        return ans
```

## LeetCode Problem Walkthrough

### Problem: 3312. Sorted GCD Pair Queries

https://leetcode.com/problems/sorted-gcd-pair-queries/

### Approach 1: Brute Force

**Algorithm**
1. Generate every pair `(i, j)` with `i < j`.
2. Compute `g = gcd(nums[i], nums[j])` and append to a list.
3. Sort the list.
4. For each query `k`, return `list[k]`.

**Implementation**

```python
import math
from typing import List

class Solution:
    def gcdValues(self, nums: List[int], queries: List[int]) -> List[int]:
        pair_gcds = []
        n = len(nums)
        for i in range(n):
            for j in range(i + 1, n):
                pair_gcds.append(math.gcd(nums[i], nums[j]))
        pair_gcds.sort()
        return [pair_gcds[k] for k in queries]
```

**Complexity Analysis**
- Time complexity: O(n² log n) — we compute O(n²) GCDs and sort them.
- Space complexity: O(n²) — we store all pairwise GCDs.

### Approach 2: Sieve‑style Counting + Exact GCD Pairs

**Intuition**
Instead of materialising every GCD, we count how many pairs yield each possible GCD value.  
If we know `cnt[d]` = how many array elements are divisible by `d`, then `C(cnt[d], 2)` counts pairs where both numbers are multiples of `d`.  
By subtracting contributions from multiples of `d` (processed from large to small) we obtain the exact number of pairs whose GCD equals `d`.

**Algorithm**
1. Build `freq[x]` for each value.
2. For each divisor `d`, compute `cnt[d] = Σ freq[m]` over multiples `m` of `d`.
3. Process `d` from `max_val` down to `1`:
   - `pairs = cnt[d] * (cnt[d] - 1) // 2`
   - For each multiple `m = 2*d, 3*d, …`: `pairs -= exact[m]`
   - Store `exact[d] = pairs`.
4. Build prefix sums of `exact`.
5. Answer each query by binary searching the prefix array.

**Implementation**

```python
import bisect
from typing import List

class Solution:
    def gcdValues(self, nums: List[int], queries: List[int]) -> List[int]:
        max_val = max(nums)
        freq = [0] * (max_val + 1)
        for x in nums:
            freq[x] += 1

        cnt = [0] * (max_val + 1)
        for d in range(1, max_val + 1):
            for mult in range(d, max_val + 1, d):
                cnt[d] += freq[mult]

        exact = [0] * (max_val + 1)
        for d in range(max_val, 0, -1):
            pairs = cnt[d] * (cnt[d] - 1) // 2
            for mult in range(2 * d, max_val + 1, d):
                pairs -= exact[mult]
            exact[d] = pairs

        prefix = [0] * (max_val + 1)
        for d in range(1, max_val + 1):
            prefix[d] = prefix[d - 1] + exact[d]

        ans = []
        for k in queries:
            g = bisect.bisect_right(prefix, k)
            ans.append(g)
        return ans
```

**Complexity Analysis**
- Time complexity: O(M log M + n + Q log M) where M = max(nums).  
  The double loop for `cnt` runs in Σ_{d=1}^{M} M/d = O(M log M).  
  The inclusion‑exclusion loop is similar.  
  Each query binary‑searches in O(log M).
- Space complexity: O(M) — three arrays of length M+1.

### Approach 3: Optimised Sieve with Linear‑time Multiple Enumeration

**Intuition**
The two nested loops for building `cnt` and for subtracting multiples can be tightened by iterating over multiples only once per divisor, which is already optimal for this value range.  
The main gain comes from using integer array operations and early exits when `cnt[d]` is zero (no numbers divisible by `d`).  
In practice this reduces constant factors, but asymptotically it stays the same as Approach 2.

**Algorithm**
Same as Approach 2, but we skip divisors with `cnt[d] < 2` because they cannot form any pair, and we break the inner subtraction loop when the multiple exceeds `max_val`.

**Implementation**

```python
import bisect
from typing import List

class Solution:
    def gcdValues(self, nums: List[int], queries: List[int]) -> List[int]:
        max_val = max(nums)
        freq = [0] * (max_val + 1)
        for x in nums:
            freq[x] += 1

        # count of numbers divisible by d
        cnt = [0] * (max_val + 1)
        for d in range(1, max_val + 1):
            if freq[d] == 0:
                # still need to accumulate from higher multiples
                for mult in range(2 * d, max_val + 1, d):
                    cnt[d] += freq[mult]
            else:
                # start from d itself
                for mult in range(d, max_val + 1, d):
                    cnt[d] += freq[mult]

        exact = [0] * (max_val + 1)
        for d in range(max_val, 0, -1):
            if cnt[d] < 2:
                exact[d] = 0
                continue
            pairs = cnt[d] * (cnt[d] - 1) // 2
            for mult in range(2 * d, max_val + 1, d):
                pairs -= exact[mult]
            exact[d] = pairs

        prefix = [0] * (max_val + 1)
        for d in range(1, max_val + 1):
            prefix[d] = prefix[d - 1] + exact[d]

        ans = []
        for k in queries:
            g = bisect.bisect_right(prefix, k)
            ans.append(g)
        return ans
```

**Complexity Analysis**
- Time complexity: O(M log M + n + Q log M) — same asymptotic bound as Approach 2 with a smaller constant factor.
- Space complexity: O(M).

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the first example: `nums = [2,3,4]`, `queries = [0,2,2]`.

**Step 1 – Frequency**

| value | freq |
|-------|------|
| 1     | 0    |
| 2     | 1    |
| 3     | 1    |
| 4     | 1    |

**Step 2 – cnt[d] (how many numbers are multiples of d)**

| d | multiples present | cnt[d] |
|---|-------------------|--------|
| 1 | 2,3,4             | 3      |
| 2 | 2,4               | 2      |
| 3 | 3                 | 1      |
| 4 | 4                 | 1      |

**Step 3 – exact[d] (pairs with GCD exactly d)**, processed from high to low

- d=4: cnt=1 → pairs=0 → exact[4]=0
- d=3: cnt=1 → pairs=0 → exact[3]=0
- d=2: cnt=2 → pairs=1 → subtract exact[4] (0) → exact[2]=1
- d=1: cnt=3 → pairs=3 → subtract exact[2] (1) + exact[3] (0) + exact[4] (0) → exact[1]=2

So we have:
- exact[1] = 2 pairs with GCD = 1
- exact[2] = 1 pair with GCD = 2

**Step 4 – prefix[d] (how many pair‑GCDs ≤ d)**

| d | exact[d] | prefix[d] |
|---|----------|-----------|
| 0 | –        | 0         |
| 1 | 2        | 2         |
| 2 | 1        | 3         |
| 3 | 0        | 3         |
| 4 | 0        | 3         |

The sorted list of all pair‑GCDs is `[1,1,2]` (prefix tells us there are 2 ≤1, and 3 ≤2).

**Step 5 – Answer queries via binary search on prefix**

- query 0 → first index where prefix > 0 is d=1 → answer 1
- query 2 → first index where prefix > 2 is d=2 → answer 2
- query 2 → same → answer 2

Result `[1,2,2]` matches the expected output. 
