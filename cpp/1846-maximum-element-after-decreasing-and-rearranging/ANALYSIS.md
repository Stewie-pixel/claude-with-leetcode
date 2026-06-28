# Greedy

## Video Solution

For more details about **Maximum Element After Decreasing and Rearranging**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=o_hVl8IXuIE).

## Concept

When we can **reorder** the array arbitrarily and only **decrease** elements, the best we can do is to place the smallest numbers first and then grow the sequence as slowly as allowed (difference ≤ 1).  
After sorting, we fix the first element to 1 (the smallest possible start). Then we walk through the sorted list: each element can be at most one larger than the previous element; if it is already larger we trim it down to `prev + 1`, otherwise we keep it as‑is.  
The last value we obtain is the maximum possible element under the rules.

## When to Use It

Use a greedy‑after‑sorting pattern when you see:

* You may **reorder** the whole array (sorting is free).
* You are only allowed to **decrease** values (never increase).
* There is a **local constraint** between neighbours (e.g., `|a[i] - a[i‑1]| ≤ 1`).
* You need to **maximize** (or minimize) a global property (here the largest element).

The sorting step puts the most “flexible” small numbers early, and the greedy sweep respects the neighbour constraint while keeping each element as large as possible.

## Template

```python
# Greedy after sorting – reusable template
def greedy_after_sort(arr):
    arr.sort()               # O(n log n)
    arr[0] = 1               # enforce first element = 1
    for i in range(1, len(arr)):
        # each element can be at most previous + 1
        arr[i] = min(arr[i], arr[i-1] + 1)
    return arr[-1]           # the maximal possible value
```

---

## LeetCode Problem Walkthrough

### Problem: 1846. Maximum Element After Decreasing and Rearranging
https://leetcode.com/problems/maximum-element-after-decreasing-and-rearranging/

---

### Approach 1: Brute Force (Conceptual)

**Algorithm**  
* Generate every possible permutation of the array (there are `n!` of them).  
* For each permutation, decrease elements as needed to satisfy `|a[i] - a[i‑1]| ≤ 1` while keeping each element as large as possible (a simple left‑to‑right pass).  
* Track the largest element seen across all permutations.

**Implementation**  
```python
from itertools import permutations

def brute_force(arr):
    best = 0
    for perm in permutations(arr):
        # make a mutable copy
        cur = list(perm)
        cur[0] = 1                     # first must be 1
        for i in range(1, len(cur)):
            cur[i] = min(cur[i], cur[i-1] + 1)
        best = max(best, cur[-1])
    return best
```

**Complexity Analysis**  
* Time: `O(n! * n)` – factorial, infeasible for `n > 10`.  
* Space: `O(n)` for the temporary copy.

*Why we don’t use this:* It shows the problem is solvable by trying all options, but we need a smarter method.

---

### Approach 2: Sorting + Greedy Pass (O(n log n))

**Intuition**  
After sorting, the smallest numbers are front‑loaded. Starting from 1, each subsequent position can increase by at most 1. If the current sorted value is large enough we can raise it to `prev+1`; otherwise we must keep it lower (which can only hurt the final maximum). This local decision is globally optimal because any later element cannot exceed the value we could have placed at the current position given the monotonic increase restriction.

**Algorithm**  
1. Sort `arr`.  
2. Set `arr[0] = 1`.  
3. For each `i` from 1 to `n‑1`: `arr[i] = min(arr[i], arr[i-1] + 1)`.  
4. Return `arr[-1]`.

**Implementation**  
```python
class Solution:
    def maximumElementAfterDecrementingAndRearranging(self, arr):
        arr.sort()
        arr[0] = 1
        for i in range(1, len(arr)):
            arr[i] = min(arr[i], arr[i-1] + 1)
        return arr[-1]
```

**Complexity Analysis**  
* Time: `O(n log n)` – dominated by sorting.  
* Space: `O(1)` extra (sorting is in‑place for Python’s Timsort).

---

### Approach 3: Counting / Bucket Method (O(n))

**Intuition**  
The answer can never exceed `n` because we start at 1 and can increase by at most 1 each step, so the largest possible value at index `i` is `i+1`. Therefore we only need to know how many numbers we have that are **at least** each candidate value `1 … n`.  
We build a frequency array `freq` where `freq[v] = count of elements equal to v` (capping any value > n at `n`). Then we walk from `1` upward, keeping a running `available` count of how many elements can support the current target value. If `available` > 0 we place one element here and decrement `available`; otherwise we stop. The last successful target is the answer.

**Algorithm**  
1. Let `n = len(arr)`.  
2. Create `freq = [0] * (n + 1)`.  
3. For each `x` in `arr`:  
   * `v = min(x, n)` – values larger than `n` behave like `n`.  
   * `freq[v] += 1`.  
4. Initialize `missing = 0` (how many extra slots we have) and `ans = 0`.  
5. For `value` from `1` to `n`:  
   * `missing += freq[value]` – we now have this many elements that can reach at least `value`.  
   * If `missing == 0`: break (cannot assign this value).  
   * `missing -= 1` – we use one element to place `value`.  
   * `ans = value`.  
6. Return `ans`.

**Implementation**  
```python
class Solution:
    def maximumElementAfterDecrementingAndRearranging(self, arr):
        n = len(arr)
        freq = [0] * (n + 1)          # indices 0..n
        for x in arr:
            freq[min(x, n)] += 1

        missing = 0
        ans = 0
        for value in range(1, n + 1):
            missing += freq[value]
            if missing == 0:
                break
            missing -= 1
            ans = value
        return ans
```

**Complexity Analysis**  
* Time: `O(n)` – one pass to build freq, one pass to scan.  
* Space: `O(n)` for the frequency array.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

**Dry Run** (using the sorted‑greedy method) on `arr = [3, 1, 2, 2, 2]`

```
Sorted arr: [1, 2, 2, 2, 3]
Step 0: set arr[0] = 1                -> [1, 2, 2, 2, 3]
Step 1: arr[1] = min(2, 1+1) = 2     -> [1, 2, 2, 2, 3]
Step 2: arr[2] = min(2, 2+1) = 2     -> [1, 2, 2, 2, 3]
Step 3: arr[3] = min(2, 2+1) = 2     -> [1, 2, 2, 2, 3]
Step 4: arr[4] = min(3, 2+1) = 3     -> [1, 2, 2, 2, 3]
Result: last element = 3
```

The maximum achievable element is 3.

--- 

**Summary**  
* The greedy‑after‑sorting approach gives the optimal answer in `O(n log n)` time and constant extra space.  
* A linear‑time bucket method exists because the answer is bounded by the array length.  
* Always start with the brute‑force idea to understand why the greedy works, then move to the efficient implementations.