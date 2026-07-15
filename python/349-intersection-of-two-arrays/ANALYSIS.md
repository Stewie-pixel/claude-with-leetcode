# Arrays & Hashing

## Video Solution

For more details about **Intersection of Two Arrays**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=fwUTXaMom6U)

## Concept

When we need to know which values appear in **both** collections, we can treat one collection as a lookup table (hash set).  
Checking membership in a hash set is *O(1)* on average, so we can scan the second array and collect every element that exists in the set.  
Using a set also automatically removes duplicates, which satisfies the “unique” requirement of the problem.

## When to Use It

- You see phrases like “common elements”, “intersection”, “appear in both arrays”.
- Duplicates in the input should be ignored in the output.
- You need fast membership tests (O(1)) rather than scanning repeatedly (O(n)).
- The input size is moderate (≤ 1000 here) but the principle scales to much larger data.

## Template

```python
# General pattern for intersection using hash sets
def intersect_using_sets(arr1, arr2):
    # Convert one array to a set for O(1) look‑ups
    lookup = set(arr1)          # O(len(arr1)) time, O(len(arr1)) space
    result = set()              # to keep output unique
    for x in arr2:              # O(len(arr2)) iterations
        if x in lookup:         # O(1) average lookup
            result.add(x)       # O(1) average insertion
    return list(result)         # convert to list as required
```

## LeetCode Problem Walkthrough

### Problem: 349. Intersection of Two Arrays  
https://leetcode.com/problems/intersection-of-two-arrays/

### Approach 1: Brute Force (Nested Loops)

**Algorithm**  
1. Initialise an empty list `res`.  
2. For each element `a` in `nums1`, scan `nums2` to see if `a` appears.  
3. If it does and `a` is not already in `res`, append it.  
4. Return `res`.

**Implementation**
```python
class Solution:
    def intersection(self, nums1, nums2):
        """
        :type nums1: List[int]
        :type nums2: List[int]
        :rtype: List[int]
        """
        res = []
        for a in nums1:
            for b in nums2:
                if a == b and a not in res:   # avoid duplicates in output
                    res.append(a)
                    break                     # no need to keep scanning nums2
        return res
```

**Complexity Analysis**  
- Time complexity: O(n·m) — we compare every pair (`n = len(nums1)`, `m = len(nums2)`).  
- Space complexity: O(k) — only the result list (`k` ≤ min(n,m)) plus O(1) auxiliary space.

---

### Approach 2: Sorting + Two Pointers

**Intuition**  
If both arrays are sorted, we can walk through them simultaneously with two pointers, advancing the pointer that points to the smaller value. When the values match we have found a common element. Sorting brings equal values together, making it easy to skip duplicates.

**Algorithm**  
1. Sort `nums1` and `nums2`.  
2. Initialise two indices `i = 0`, `j = 0` and an empty set `res`.  
3. While `i < len(nums1)` and `j < len(nums2)`:
   - If `nums1[i] < nums2[j]`, increment `i`.  
   - Else if `nums1[i] > nums2[j]`, increment `j`.  
   - Else (equal): add the value to `res`, then increment both `i` and `j`.  
   - Skip over any duplicate values after each increment to keep the result unique.  
4. Return the list version of `res`.

**Implementation**
```python
class Solution:
    def intersection(self, nums1, nums2):
        """
        :type nums1: List[int]
        :type nums2: List[int]
        :rtype: List[int]
        """
        nums1.sort()
        nums2.sort()
        i = j = 0
        res = set()
        while i < len(nums1) and j < len(nums2):
            if nums1[i] < nums2[j]:
                i += 1
            elif nums1[i] > nums2[j]:
                j += 1
            else:                     # nums1[i] == nums2[j]
                res.add(nums1[i])
                i += 1
                j += 1
        return list(res)
```

**Complexity Analysis**  
- Time complexity: O(n log n + m log m) — dominated by sorting both arrays.  
- Space complexity: O(k) — the result set (ignoring the input‑sorting space, which is O(log n + log m) for Timsort).

---

### Approach 3: Hash Set (Optimal)

**Intuition**  
By storing one array in a hash set we gain constant‑time look‑ups. Scanning the second array and checking membership yields each common element in linear time. Using a second set for the result guarantees uniqueness without extra duplicate‑skipping logic.

**Algorithm**  
1. Convert `nums1` to a set `s1`.  
2. Initialise an empty set `result`.  
3. For each element `x` in `nums2`:
   - If `x` is in `s1`, add `x` to `result`.  
4. Return `list(result)`.

**Implementation**
```python
class Solution:
    def intersection(self, nums1, nums2):
        """
        :type nums1: List[int]
        :type nums2: List[int]
        :rtype: List[int]
        """
        s1 = set(nums1)          # O(len(nums1)) time & space
        result = set()
        for x in nums2:          # O(len(nums2)) time
            if x in s1:          # O(1) average lookup
                result.add(x)    # O(1) average insertion
        return list(result)      # convert to list
```

**Complexity Analysis**  
- Time complexity: O(n + m) — one pass to build the set, one pass to scan the second array.  
- Space complexity: O(min(n, m)) — the set storing the smaller array (or the result set, whichever is larger).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the optimal hash‑set approach on the first example:

**Input:** `nums1 = [1,2,2,1]`, `nums2 = [2,2]`

```
Step | Action                                 | s1 (set from nums1) | result set
-----|----------------------------------------|---------------------|-----------
1    | Build set from nums1                   | {1, 2}              | {}
2    | x = 2 (first in nums2) → 2 in s1? yes  | {1, 2}              | {2}
3    | x = 2 (second in nums2) → 2 in s1? yes | {1, 2}              | {2}  (no change, duplicate ignored)
```

Final result: `[2]` (order may vary).

The same trace works for the second example, yielding `{9,4}` → `[9,4]` or `[4,9]`.

---