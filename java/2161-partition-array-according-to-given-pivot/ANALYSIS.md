# Two Pointers

## Video Solution

For more details about **Partition Array According to Given Pivot**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=OX0bCSG7EfE).

## Concept

The problem asks us to reorder an array so that all elements `< pivot` come first, then all elements `== pivot`, and finally all elements `> pivot`.  
The relative order of the `< pivot` group and the `> pivot` group must stay the same as in the original array.  
Think of three buckets: **less**, **equal**, **greater**. We scan the original array once and place each element into its proper bucket while preserving the order inside each bucket.

## When to Use It

Use this pattern when you see:
- A pivot value that splits the array into three regions (<, ==, >).
- The requirement to keep the original order inside the “less‑than” and “greater‑than” groups.
- The need to return a new array (or overwrite the existing one) in linear time.

## Template

```python
def pivot_array(nums, pivot):
    # three indices for the three sections in the result array
    less_idx = equal_idx = greater_idx = 0
    n = len(nums)
    res = [0] * n                     # placeholder for the answer

    for num in nums:
        if num < pivot:
            res[less_idx] = num
            less_idx += 1
        elif num == pivot:
            # equal elements go right after the less‑than section
            res[less_idx + equal_idx] = num
            equal_idx += 1
        else:  # num > pivot
            # greater elements go after less + equal sections
            res[less_idx + equal_idx + greater_idx] = num
            greater_idx += 1

    return res
```

## LeetCode Problem Walkthrough

### Problem: 2161. Partition Array According to Given Pivot

https://leetcode.com/problems/partition-array-according-to-given-pivot/

### Approach 1: Brute Force – Three Separate Lists

**Algorithm**
1. Create three empty lists: `less`, `equal`, `greater`.
2. Iterate through `nums` once, appending each element to the appropriate list based on its comparison with `pivot`.
3. Concatenate the three lists (`less + equal + greater`) and return the result.

**Implementation**

```python
class Solution:
    def pivotArray(self, nums: List[int], pivot: int) -> List[int]:
        less, equal, greater = [], [], []
        for x in nums:
            if x < pivot:
                less.append(x)
            elif x == pivot:
                equal.append(x)
            else:
                greater.append(x)
        return less + equal + greater
```

**Complexity Analysis**
- Time complexity: O(n) — one pass through `nums`.
- Space complexity: O(n) — we store three auxiliary lists whose total size equals `n`.

---

### Approach 2: Two‑Pass Counting + Overwrite

**Intuition**
If we first count how many elements belong to each region, we know exactly where each section starts in the final array. A second pass can then place each element directly into its correct slot, using only O(1) extra space (besides the output array).

**Algorithm**
1. First pass: count `cnt_less`, `cnt_equal`, `cnt_greater`.
2. Compute the starting indices:
   - `less_start = 0`
   - `equal_start = cnt_less`
   - `greater_start = cnt_less + cnt_equal`
3. Second pass: for each `x` in `nums`,
   - if `x < pivot` → place at `less_start` and increment it.
   - if `x == pivot` → place at `equal_start` and increment it.
   - if `x > pivot` → place at `greater_start` and increment it.
4. Return the filled array.

**Implementation**

```python
class Solution:
    def pivotArray(self, nums: List[int], pivot: int) -> List[int]:
        n = len(nums)
        cnt_less = cnt_equal = cnt_greater = 0
        for x in nums:
            if x < pivot:
                cnt_less += 1
            elif x == pivot:
                cnt_equal += 1
            else:
                cnt_greater += 1

        res = [0] * n
        less_i = 0
        equal_i = cnt_less
        greater_i = cnt_less + cnt_equal

        for x in nums:
            if x < pivot:
                res[less_i] = x
                less_i += 1
            elif x == pivot:
                res[equal_i] = x
                equal_i += 1
            else:
                res[greater_i] = x
                greater_i += 1

        return res
```

**Complexity Analysis**
- Time complexity: O(n) — two linear passes.
- Space complexity: O(n) for the output array; O(1) auxiliary space.

---

### Approach 3: Single‑Pass Three‑Pointer Fill (Optimal)

**Intuition**
Instead of counting first, we can fill the result array on the fly by keeping three write pointers — one for each section. As we read each element, we write it to the current position of its section and advance that pointer. This achieves the same O(n) time with a single traversal.

**Algorithm**
1. Initialize `less_ptr = 0`, `equal_ptr = 0`, `greater_ptr = 0`.
2. Allocate result array `res` of size `n`.
3. For each `x` in `nums`:
   - If `x < pivot`: write to `res[less_ptr]`, increment `less_ptr`.
   - If `x == pivot`: write to `res[less_ptr + equal_ptr]`, increment `equal_ptr`.
   - If `x > pivot`: write to `res[less_ptr + equal_ptr + greater_ptr]`, increment `greater_ptr`.
4. Return `res`.

**Implementation**

```python
class Solution:
    def pivotArray(self, nums: List[int], pivot: int) -> List[int]:
        n = len(nums)
        res = [0] * n
        less_ptr = equal_ptr = greater_ptr = 0

        for x in nums:
            if x < pivot:
                res[less_ptr] = x
                less_ptr += 1
            elif x == pivot:
                # equal elements go right after the less‑than section
                res[less_ptr + equal_ptr] = x
                equal_ptr += 1
            else:  # x > pivot
                # greater elements go after less + equal sections
                res[less_ptr + equal_ptr + greater_ptr] = x
                greater_ptr += 1

        return res
```

**Complexity Analysis**
- Time complexity: O(n) — one pass through `nums`.
- Space complexity: O(n) for the output array; O(1) extra space.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: `nums = [9,12,5,10,14,3,10]`, `pivot = 10`

We'll trace Approach 3 (single‑pass fill).

| Step | x   | less_ptr | equal_ptr | greater_ptr | Action (write position) | res after step |
|------|-----|----------|-----------|-------------|-------------------------|----------------|
| 0    | –   | 0        | 0         | 0           | init                    | [_ ,_ ,_ ,_ ,_ ,_ ,_] |
| 1    | 9   | 0        | 0         | 0           | 9 → res[0] (less)       | [9,_,_,_,_,_,_] |
|      |     | 1        | 0         | 0           |                         | |
| 2    | 12  | 1        | 0         | 0           | 12 → res[1] (greater)   | [9,12,_,_,_,_,_] |
|      |     | 1        | 0         | 1           |                         | |
| 3    | 5   | 1        | 0         | 1           | 5 → res[1] (less)       | [9,5,_,_,_,_,_] |
|      |     | 2        | 0         | 1           |                         | |
| 4    | 10  | 2        | 0         | 1           | 10 → res[2] (equal)     | [9,5,10,_,_,_,_] |
|      |     | 2        | 1         | 1           |                         | |
| 5    | 14  | 2        | 1         | 1           | 14 → res[4] (greater)   | [9,5,10,_,14,_,_] |
|      |     | 2        | 1         | 2           |                         | |
| 6    | 3   | 2        | 1         | 2           | 3 → res[2] (less)       | [9,5,3,_,14,_,_] |
|      |     | 3        | 1         | 2           |                         | |
| 7    | 10  | 3        | 1         | 2           | 10 → res[4] (equal)     | [9,5,3,_,14,10,_] |
|      |     | 3        | 2         | 2           |                         | |
| End  | –   | 3        | 2         | 2           | fill remaining gaps →   | [9,5,3,10,10,12,14] |

The final array matches the expected output: `[9,5,3,10,10,12,14]`. The relative order of the `< pivot` elements (`9,5,3`) and `> pivot` elements (`12,14`) is preserved.