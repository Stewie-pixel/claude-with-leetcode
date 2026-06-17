# Binary Search

## Video Solution

For more details about **Median of Two Sorted Arrays**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=q6IEA26hvXc).

## Concept

Binary search is usually introduced for finding a target in a sorted array, but its core idea—**discarding half of the search space each step**—can be applied to many problems that involve ordering or partitioning.  
For the median of two sorted arrays we binary‑search on the *partition* of the shorter array: we decide how many elements from each array should go to the left half of the combined sorted order. If the partition is correct (every element on the left ≤ every element on the right), we have found the median; otherwise we move the partition left or right based on which side is too large.

**Analogy:** Imagine two sorted piles of cards. You want to split the combined deck into two equal halves without actually merging them. You guess how many cards to take from the smaller pile, look at the border cards, and adjust your guess—just like binary search narrows down a target.

## When to Use It

Use binary search on partitions when you see:

- Two (or more) sorted sequences and you need an order statistic (median, k‑th smallest, etc.).
- A requirement of **O(log (min(m,n)))** time.
- The problem can be phrased as “find a split point such that …”.

## Template

```python
def binary_search_partition(A, B):
    # Ensure A is the shorter array
    if len(A) > len(B):
        A, B = B, A
    m, n = len(A), len(B)
    total = m + n
    half = total // 2

    lo, hi = 0, m          # search on number taken from A
    while lo <= hi:
        i = (lo + hi) // 2   # elements from A in left part
        j = half - i         # elements from B in left part

        Aleft  = A[i-1] if i > 0 else float('-inf')
        Aright = A[i]   if i < m else float('inf')
        Bleft  = B[j-1] if j > 0 else float('-inf')
        Bright = B[j]   if j < n else float('inf')

        if Aleft <= Bright and Bleft <= Aright:
            # correct partition found
            if total % 2:          # odd length
                return min(Aright, Bright)
            else:                  # even length
                return (max(Aleft, Bleft) + min(Aright, Bright)) / 2.0
        elif Aleft > Bright:       # i is too big -> move left
            hi = i - 1
        else:                      # i is too small -> move right
            lo = i + 1
```

The template returns the median; you can adapt the return value for other order statistics.

---

## LeetCode Problem Walkthrough

### Problem: 4. Median of Two Sorted Arrays
https://leetcode.com/problems/median-of-two-sorted-arrays/

### Approach 1: Brute Force (Merge & Sort)

**Algorithm**  
1. Concatenate `nums1` and `nums2` into a new list.  
2. Sort the list.  
3. If the length is odd, return the middle element; otherwise return the average of the two middle elements.

**Implementation**
```python
def findMedianSortedArrays(nums1, nums2):
    merged = nums1 + nums2
    merged.sort()
    total = len(merged)
    if total % 2:
        return float(merged[total // 2])
    else:
        mid = total // 2
        return (merged[mid - 1] + merged[mid]) / 2.0
```

**Complexity Analysis**  
- Time complexity: O((m+n) log(m+n)) – dominated by sorting.  
- Space complexity: O(m+n) – the merged list.

---

### Approach 2: Two‑Pointer Merge (Linear Merge)

**Intuition**  
We do not need a full sort; we can walk through both arrays simultaneously, counting how many elements we have passed. When we have processed `half = (m+n)//2` elements, the next element(s) give the median.

**Algorithm**  
1. Ensure we iterate with two pointers `i` (for `nums1`) and `j` (for `nums2`).  
2. Advance the pointer that points to the smaller value, incrementing a counter `cnt`.  
3. Stop when `cnt == half`.  
   - If total length is odd, the median is the current smaller of `nums1[i]` and `nums2[j]`.  
   - If even, we also need the previous value; keep track of `prev` as we advance.

**Implementation**
```python
def findMedianSortedArrays(nums1, nums2):
    m, n = len(nums1), len(nums2)
    total = m + n
    half = total // 2
    i = j = 0
    prev = curr = None

    for _ in range(half + 1):          # we need to go one step beyond half for even case
        prev = curr
        if i < m and (j >= n or nums1[i] <= nums2[j]):
            curr = nums1[i]
            i += 1
        else:
            curr = nums2[j]
            j += 1

    if total % 2:
        return float(curr)
    else:
        return (prev + curr) / 2.0
```

**Complexity Analysis**  
- Time complexity: O(m+n) – each element is inspected at most once.  
- Space complexity: O(1) – only a few variables.

---

### Approach 3: Optimal Binary Search on Partition

**Intuition**  
The median splits the combined sorted array into left and right halves with equal size (or left larger by one when total is odd).  
If we pick `i` elements from `nums1` to be in the left half, then we must take `j = half - i` elements from `nums2`.  
The partition is correct when  
```
max(left part) ≤ min(right part)
=> max(A[i-1], B[j-1]) ≤ min(A[i], B[j])
```
If `A[i-1] > B[j]`, we have taken too many from `A` → move `i` left.  
If `B[j-1] > A[i]`, we have taken too few from `A` → move `i` right.  
Because `i` is monotonic, we can binary‑search it in O(log min(m,n)) time.

**Algorithm**  
1. Make sure `nums1` is the shorter array.  
2. Binary search `i` in `[0, len(nums1)]`.  
3. Compute `j = half - i`.  
4. Get border values (`Aleft`, `Aright`, `Bleft`, `Bright`) using ±∞ for out‑of‑range indices.  
5. If the partition condition holds, compute median from the borders.  
6. Otherwise adjust search range based on which side is too large.

**Implementation**
```python
def findMedianSortedArrays(nums1, nums2):
    # Ensure nums1 is the shorter array
    if len(nums1) > len(nums2):
        nums1, nums2 = nums2, nums1

    m, n = len(nums1), len(nums2)
    total = m + n
    half = total // 2

    lo, hi = 0, m
    while lo <= hi:
        i = (lo + hi) // 2          # elements from nums1 in left part
        j = half - i                # elements from nums2 in left part

        Aleft  = nums1[i-1] if i > 0 else float('-inf')
        Aright = nums1[i]   if i < m else float('inf')
        Bleft  = nums2[j-1] if j > 0 else float('-inf')
        Bright = nums2[j]   if j < n else float('inf')

        if Aleft <= Bright and Bleft <= Aright:
            # correct partition
            if total % 2:          # odd total length
                return float(min(Aright, Bright))
            else:                  # even total length
                return (max(Aleft, Bleft) + min(Aright, Bright)) / 2.0
        elif Aleft > Bright:       # i is too big -> move left
            hi = i - 1
        else:                      # i is too small -> move right
            lo = i + 1
```

**Complexity Analysis**  
- Time complexity: O(log (min(m,n))) – binary search on the shorter array.  
- Space complexity: O(1) – only a few variables.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the optimal binary search on the example `nums1 = [1,3]`, `nums2 = [2]`.

```
Input: nums1 = [1, 3], nums2 = [2]
m = 2, n = 1 → ensure nums1 is shorter? No, m > n → swap.
After swap: nums1 = [2], nums2 = [1, 3]
m = 1, n = 2, total = 3, half = 1

Initial lo=0, hi=1
-------------------------------------------------
Step | lo | hi | i | j | Aleft | Aright | Bleft | Bright | Condition (Aleft≤Bright & Bleft≤Aright) | Action
-----|----|----|---|---|-------|--------|-------|--------|------------------------------------------|-------
1    | 0  | 1  | 0 | 1 | -inf  | 2      | 1     | 3      | -inf≤3 & 1≤2 → True                     | Found partition
```

Since the condition is satisfied at the first iteration:
- Left part max = max(Aleft, Bleft) = max(-inf, 1) = 1  
- Right part min = min(Aright, Bright) = min(2, 3) = 2  
- Total length is odd → median = min(Aright, Bright) = 2.0

The algorithm terminates after **O(log m) = O(log 1) = 1** iteration, returning the correct median `2.0`.

---