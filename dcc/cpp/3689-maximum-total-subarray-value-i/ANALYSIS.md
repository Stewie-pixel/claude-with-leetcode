# Greedy

## Video Solution

For more details about **Maximum Total Subarray Value I**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=jO9QWzIgAVo).

## Concept

The value of a subarray is `max(subarray) - min(subarray)`.  
Because we may choose **exactly** `k` subarrays and they may overlap or be identical, the total value is simply `k` multiplied by the value of the *single* subarray we pick each time.  
Thus the problem reduces to: **find one subarray with the largest possible `max - min`**, then repeat it `k` times.

The largest possible difference between a maximum and a minimum inside any contiguous segment is achieved when the segment contains both the global maximum and the global minimum of the whole array.  
Taking the interval from the leftmost occurrence of the min to the rightmost occurrence of the max (or vice‑versa) guarantees both extremes are inside, so its value equals `global_max - global_min`. No subarray can exceed this difference, because any subarray’s max ≤ global_max and its min ≥ global_min.

Hence the answer is `k * (global_max - global_min)`.

## When to Use It

- You are asked to maximize a sum over **multiple identical choices** (here, `k` subarrays) where each choice contributes independently.
- The contribution of each choice depends only on a property of a **contiguous segment** (here, `max - min`).
- You can reuse the same segment unlimited times (overlap allowed, repetition allowed).
- Look for a **global extreme** (max/min) that can be captured together in one subarray.

## Template

```python
# Compute global max and min in one pass
def max_total_value(nums, k):
    # O(n) scan
    mx = nums[0]
    mn = nums[0]
    for x in nums[1:]:
        if x > mx:
            mx = x
        if x < mn:
            mn = x
    return k * (mx - mn)
```

## LeetCode Problem Walkthrough

### Problem: 3689. Maximum Total Subarray Value I
https://leetcode.com/problems/maximum-total-subarray-value-i/

---

### Approach 1: Brute Force (Check Every Subarray)

**Algorithm**  
1. Enumerate all possible subarrays `[i, j]` (`0 ≤ i ≤ j < n`).  
2. For each subarray compute its maximum and minimum (by scanning the segment).  
3. Track the largest `max - min` seen.  
4. Return `k * best_value`.

**Implementation**

```python
class Solution:
    def maxTotalValue(self, nums: List[int], k: int) -> int:
        n = len(nums)
        best = 0
        for i in range(n):
            cur_min = cur_max = nums[i]
            for j in range(i, n):
                cur_min = min(cur_min, nums[j])
                cur_max = max(cur_max, nums[j])
                best = max(best, cur_max - cur_min)
        return best * k
```

**Complexity Analysis**

- Time complexity: **O(n²)** – two nested loops over the array.  
- Space complexity: **O(1)** – only a few scalar variables.

---

### Approach 2: Greedy Observation – Use Global Extremes

**Intuition**  
If a subarray contains both the global maximum and the global minimum of the entire array, its value equals `global_max - global_min`. No subarray can have a larger difference because any subarray’s max cannot exceed the global max and its min cannot be smaller than the global min. Therefore the optimal subarray is any segment that covers both extremes, and its value is simply `global_max - global_min`. Since we may repeat the same subarray `k` times, multiply by `k`.

**Algorithm**  
1. Scan the array once to find `global_max` and `global_min`.  
2. Compute `ans = k * (global_max - global_min)`.  
3. Return `ans`.

**Implementation**

```python
class Solution:
    def maxTotalValue(self, nums: List[int], k: int) -> int:
        mx = max(nums)   # O(n)
        mn = min(nums)   # O(n) – could be combined in one pass
        return k * (mx - mn)
```

**Complexity Analysis**

- Time complexity: **O(n)** – one (or two) linear scans.  
- Space complexity: **O(1)** – constant extra space.

---

### Approach 3: Single‑Pass Version (Optional)

**Intuition**  
Same as Approach 2, but we compute both extremes in a single loop to avoid traversing the array twice (useful when constants matter).

**Algorithm**  
- Initialise `mx` and `mn` with the first element.  
- For each subsequent element update `mx` and `mn`.  
- Return `k * (mx - mn)`.

**Implementation**

```python
class Solution:
    def maxTotalValue(self, nums: List[int], k: int) -> int:
        mx = mn = nums[0]
        for x in nums[1:]:
            if x > mx:
                mx = x
            if x < mn:
                mn = x
        return k * (mx - mn)
```

**Complexity Analysis**

- Time complexity: **O(n)** – one pass.  
- Space complexity: **O(1)** – only two integer variables.

---

## Visual Demonstration (Dry Run)

**Example:** `nums = [1, 3, 2], k = 2`

```
Step | idx | val | current mx | current mn | mx - mn (best so far)
-----|-----|-----|------------|------------|----------------------
init | -   | -   | 1          | 1          | 0
1    | 0   | 1   | 1          | 1          | 0
2    | 1   | 3   | 3          | 1          | 2   <-- best
3    | 2   | 2   | 3          | 1          | 2
```

`global_max = 3`, `global_min = 1`, best value = `3 - 1 = 2`.  
Answer = `k * 2 = 2 * 2 = 4`, matching the expected output.

---