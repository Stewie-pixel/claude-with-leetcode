# Two Pointers

## Video Solution

For more details about **3Sum**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=jzZsG8n2R9A).

## Concept

The two‑pointer technique works on a **sorted** array (or list) and uses two indices that move toward each other to find pairs that satisfy a condition. By fixing one element and applying two pointers on the remaining sub‑array, we can solve problems that would otherwise require nested loops in O(n²) time instead of O(n³).

**Real‑world analogy:** Imagine you are trying to find two people on a straight line whose combined height equals a target value. If you line everyone up by height, you can start with the shortest person on the left and the tallest on the right. If their combined height is too short, you need a taller left person → move the left pointer right. If it's too tall, you need a shorter right person → move the right pointer left. This eliminates impossible pairs without checking every combination.

## When to Use It

Use two pointers when you see:
- The array is **sorted** (or can be sorted).
- You need to find **pairs/triplets** that meet a sum condition.
- The problem mentions **contiguous subarrays** or **window** ideas (sliding window is a variant).
- You want to reduce O(n²) or O(n³) brute force to O(n²) or O(n) by eliminating impossible combinations.

## Template

```python
def two_pointer_solution(arr, target):
    # 1. Sort if not already sorted
    arr.sort()
    n = len(arr)
    left, right = 0, n - 1          # start at both ends
    result = []

    while left < right:
        cur_sum = arr[left] + arr[right]
        if cur_sum == target:
            result.append([arr[left], arr[right]])
            # Skip duplicates
            left_val, right_val = arr[left], arr[right]
            while left < right and arr[left] == left_val:
                left += 1
            while left < right and arr[right] == right_val:
                right -= 1
        elif cur_sum < target:
            left += 1               # need a larger sum
        else:
            right -= 1              # need a smaller sum
    return result
```

## LeetCode Problem Walkthrough

### Problem: 15. 3Sum
https://leetcode.com/problems/3sum/

Given an integer array `nums`, return all unique triplets `[nums[i], nums[j], nums[k]]` such that `i != j != k` and the sum is zero. The solution set must not contain duplicate triplets.

---

### Approach 1: Brute Force

**Algorithm**
1. Iterate over all possible triples `(i, j, k)` with three nested loops.
2. For each triple, check if `nums[i] + nums[j] + nums[k] == 0`.
3. If true, sort the triplet and add it to a `set` to avoid duplicates.
4. Finally, convert the set to a list of lists and return.

**Implementation**

```python
def three_sum_brute(nums):
    n = len(nums)
    seen = set()
    for i in range(n):
        for j in range(i + 1, n):
            for k in range(j + 1, n):
                if nums[i] + nums[j] + nums[k] == 0:
                    triplet = tuple(sorted((nums[i], nums[j], nums[k])))
                    seen.add(triplet)
    return [list(t) for t in seen]
```

**Complexity Analysis**
- Time complexity: O(n³) — three nested loops over n.
- Space complexity: O(m) where m is the number of unique triplets (output) plus O(1) auxiliary.

---

### Approach 2: Hash‑Based Two‑Sum for Each Element

**Intuition**
If we fix the first element `nums[i]`, the problem reduces to finding two numbers in the rest of the array that sum to `-nums[i]`. The classic 2‑sum problem can be solved in linear time using a hash set. By repeating this for every index, we achieve O(n²) time while still handling duplicates via sorting each found triplet before adding to a set.

**Algorithm**
1. Sort the input array (helps with duplicate skipping later).
2. For each index `i` from `0` to `n‑3`:
   - Skip if `nums[i]` is the same as the previous element to avoid duplicate first elements.
   - Initialise an empty hash set `seen`.
   - For each `j` from `i+1` to `n‑1`:
     - Compute `needed = -nums[i] - nums[j]`.
     - If `needed` is in `seen`, we found a triplet `[nums[i], nums[j], needed]`. Sort it and add to result set.
     - Add `nums[j]` to `seen` for future look‑ups.
3. Convert the result set to the required list format.

**Implementation**

```python
def three_sum_hash(nums):
    nums.sort()
    n = len(nums)
    res_set = set()
    for i in range(n - 2):
        if i > 0 and nums[i] == nums[i - 1]:
            continue          # skip duplicate first element
        seen = set()
        for j in range(i + 1, n):
            needed = -nums[i] - nums[j]
            if needed in seen:
                triplet = tuple(sorted((nums[i], nums[j], needed)))
                res_set.add(triplet)
            seen.add(nums[j])
    return [list(t) for t in res_set]
```

**Complexity Analysis**
- Time complexity: O(n²) — outer loop O(n), inner loop O(n) with O(1) hash operations.
- Space complexity: O(n) for the hash set plus O(m) for output.

---

### Approach 3: Sorting + Two Pointers (Optimal)

**Intuition**
After sorting, for each fixed first element we can find the remaining two numbers with the classic two‑pointer pattern on the sub‑array to its right. Because the array is ordered, moving the left pointer increases the sum and moving the right pointer decreases it, letting us zero‑in on the target without extra space. Duplicate triplets are avoided by skipping over equal values for each pointer.

**Algorithm**
1. Sort `nums`.
2. Iterate `i` from `0` to `n‑3`:
   - If `i > 0` and `nums[i] == nums[i‑1]`, skip to avoid duplicate first element.
   - Set `left = i+1`, `right = n‑1`.
   - While `left < right`:
     - Compute `total = nums[i] + nums[left] + nums[right]`.
     - If `total == 0`:
        - Record the triplet.
        - Advance `left` past duplicates.
        - Retreat `right` past duplicates.
     - If `total < 0`, increment `left` (need larger sum).
     - If `total > 0`, decrement `right` (need smaller sum).
3. Return collected triplets.

**Implementation**

```python
def three_sum(nums):
    nums.sort()
    n = len(nums)
    res = []
    for i in range(n - 2):
        # Skip duplicate first elements
        if i > 0 and nums[i] == nums[i - 1]:
            continue
        left, right = i + 1, n - 1
        while left < right:
            total = nums[i] + nums[left] + nums[right]
            if total == 0:
                res.append([nums[i], nums[left], nums[right]])
                # Move left and right to next different numbers
                left_val, right_val = nums[left], nums[right]
                while left < right and nums[left] == left_val:
                    left += 1
                while left < right and nums[right] == right_val:
                    right -= 1
            elif total < 0:
                left += 1          # need a larger sum
            else:
                right -= 1         # need a smaller sum
    return res
```

**Complexity Analysis**
- Time complexity: O(n²) — sorting O(n log n) + outer loop O(n) × inner two‑pointer scan O(n).
- Space complexity: O(1) auxiliary (ignoring output), or O(n) for Timsort in Python.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the optimal two‑pointer algorithm on the example `nums = [-1,0,1,2,-1,-4]`.

#### Dry Run

Sorted `nums`: `[-4, -1, -1, 0, 1, 2]`

| Step | i | nums[i] | left | right | nums[left] | nums[right] | total | Action |
|------|---|---------|------|-------|------------|-------------|-------|--------|
| 1    | 0 | -4      | 1    | 5     | -1         | 2           | -3    | total < 0 → left++ |
| 2    | 0 | -4      | 2    | 5     | -1         | 2           | -3    | left++ |
| 3    | 0 | -4      | 3    | 5     | 0          | 2           | -2    | left++ |
| 4    | 0 | -4      | 4    | 5     | 1          | 2           | -1    | left++ |
| 5    | 0 | -4      | 5    | 5     | 2          | 2           | 0     | left == right → break (i loop continues) |
| 6    | 1 | -1      | 2    | 5     | -1         | 2           | 0     | **found triplet [-1,-1,2]** → store, skip dupes: left→3, right→4 |
| 7    | 1 | -1      | 3    | 4     | 0          | 1           | 0     | **found triplet [-1,0,1]** → store, skip dupes: left→4, right→3 (exit) |
| 8    | 2 | -1 (skip because nums[2]==nums[1]) | – | – | – | – | – | skip duplicate first element |
| 9    | 3 | 0       | 4    | 5     | 1          | 2           | 3     | total > 0 → right-- |
|10    | 3 | 0       | 4    | 4     | 1          | 1           | 2     | left == right → break |
| …    |   |         |      |       |            |             |       | remaining i values give no new sums |

Result set: `[-1,-1,2]` and `[-1,0,1]` — matches expected output.