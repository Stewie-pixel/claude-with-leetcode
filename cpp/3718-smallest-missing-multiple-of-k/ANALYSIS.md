# Arrays & Hashing

## Video Solution

For more details about **Smallest Missing Multiple of K**, watch the walkthrough at [https://www.youtube.com/watch?v=B7K0RegjHcY](https://www.youtube.com/watch?v=B7K0RegjHcY)

## Concept

The task is to find the smallest positive multiple of `k` that does **not** appear in the given array.  
Think of the multiples of `k` as a spaced‑out number line: `k, 2k, 3k, …`.  
We walk along this line and stop at the first spot that is not occupied by a number from `nums`.  
A hash set (or a simple boolean array) lets us test “is this multiple present?” in O(1) time.

## When to Use It

Use a hash‑set / direct‑address table when you need to:
- Check existence of many values quickly.
- The domain of possible values is bounded (here, multiples of `k` up to a small limit).
- You want to avoid nested loops that would lead to O(n²) time.

## Template

```python
# Step 1: put all numbers into a container for O(1) look‑up
container = set(nums)          # or a boolean list if range is small

# Step 2: iterate over multiples of k starting from k
multiple = k
while multiple in container:   # O(1) check
    multiple += k              # advance to the next multiple

# Step 3: the first missing multiple is the answer
return multiple
```

---

## LeetCode Problem Walkthrough

### Problem: 3718. Smallest Missing Multiple of K

https://leetcode.com/problems/smallest-missing-multiple-of-k/

### Approach 1: Brute Force with Hash Set

**Algorithm**  
1. Insert every element of `nums` into a hash set `seen`.  
2. Starting from `multiple = k`, repeatedly check if `multiple` is in `seen`.  
3. If it is, increase `multiple` by `k` and repeat.  
4. The first `multiple` not found in `seen` is the answer.

**Intuition**  
A hash set gives constant‑time membership tests, so we can simply scan the multiples of `k` until we find a gap.

**Implementation**

```python
class Solution:
    def missingMultiple(self, nums: List[int], k: int) -> int:
        seen = set(nums)          # O(n) time, O(n) space
        multiple = k
        while multiple in seen:   # O(1) average per check
            multiple += k
        return multiple
```

**Complexity Analysis**  
- Time complexity: O(n + answer/k) — we spend O(n) to build the set and then check each multiple of k until we find the missing one.  
- Space complexity: O(n) — the hash set stores the input numbers.

---

### Approach 2: Sort + Linear Scan

**Algorithm**  
1. Sort `nums`.  
2. Keep a pointer `i` at the start of the sorted array.  
3. For `multiple = k, 2k, 3k, …`:  
   - While `i < len(nums)` and `nums[i] < multiple`, advance `i`.  
   - If `i < len(nums)` and `nums[i] == multiple`, the multiple exists → `multiple += k` and continue.  
   - Otherwise (`nums[i] > multiple` or end of array) we have found the missing multiple.  
4. Return `multiple`.

**Intuition**  
Sorting puts the numbers in order, allowing us to walk through the array once while we generate multiples of `k`. We never need to look back, giving an O(n log n) solution with only O(1) extra space (aside from the sort).

**Implementation**

```python
class Solution:
    def missingMultiple(self, nums: List[int], k: int) -> int:
        nums.sort()                       # O(n log n)
        i = 0
        multiple = k
        while i < len(nums) and nums[i] < multiple:
            i += 1                        # skip numbers smaller than current multiple
        while i < len(nums) and nums[i] == multiple:
            multiple += k                 # current multiple exists, try next
            while i < len(nums) and nums[i] < multiple:
                i += 1
        return multiple
```

**Complexity Analysis**  
- Time complexity: O(n log n + answer/k) — dominated by the sorting step.  
- Space complexity: O(1) (or O(n) for Timsort in Python, but no extra asymptotic space).

---

### Approach 3: Direct‑Address Boolean Array (Frequency Array)

**Algorithm**  
1. Determine an upper bound for the answer.  
   Since `nums[i] ≤ 100` and `k ≤ 100`, the worst case is when the array contains the first 100 multiples of `k`; the answer will then be at most `101 * k`.  
   We allocate a boolean list `present` of size `limit = 101 * k + 1` (a small constant ≤ 10101).  
2. Mark `present[num] = True` for each `num` in `nums`.  
3. Iterate `multiple = k, 2k, 3k, …` up to `limit` and return the first `multiple` where `present[multiple]` is `False`.

**Intuition**  
When the value range is small and known, a direct‑address table gives O(1) look‑ups with virtually no overhead, eliminating the hash‑set’s constant factors.

**Implementation**

```python
class Solution:
    def missingMultiple(self, nums: List[int], k: int) -> int:
        # safe upper bound: 101 * k covers the case where first 100 multiples are present
        limit = 101 * k + 1
        present = [False] * (limit + 1)   # index 0 unused
        for num in nums:
            if num <= limit:
                present[num] = True
        multiple = k
        while multiple <= limit and present[multiple]:
            multiple += k
        return multiple
```

**Complexity Analysis**  
- Time complexity: O(n + limit/k) → O(n + 101) → O(n) because `limit` is a constant ≤ 10101.  
- Space complexity: O(limit) → O(1) (constant ≤ ~10 KB).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: `nums = [8,2,3,4,6], k = 2`

We'll trace the **Hash Set** approach.

| Step | multiple | `multiple in seen?` | Action                     |
|------|----------|---------------------|----------------------------|
| 0    | –        | –                   | Build `seen = {2,3,4,6,8}` |
| 1    | 2        | True                | `multiple += k → 4`       |
| 2    | 4        | True                | `multiple += k → 6`       |
| 3    | 6        | True                | `multiple += k → 8`       |
| 4    | 8        | True                | `multiple += k → 10`      |
| 5    | 10       | False               | **stop**, return 10       |

The first missing multiple of 2 is **10**, matching the expected output.

--- 

*Commit this file to `/lectures/YYYY-MM-DD-arrays-hashing.md` (using today’s date) following the filename convention.*