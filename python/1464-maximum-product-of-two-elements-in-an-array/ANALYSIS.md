# Arrays & Hashing

## Video Solution

For more details about **Maximum Product of Two Elements in an Array**, watch the walkthrough at [https://www.youtube.com/watch?v=W0fxmagr994](https://www.youtube.com/watch?v=W0fxmagr994).

## Concept

When we need to compute a value that depends on two elements of an array (like the product after subtracting 1 from each), the optimal pair is usually formed by the **largest** numbers in the array.  
Think of picking the two tallest people in a room to maximize the combined height after they each crouch a little (subtract 1). No need to examine every possible pair—just locate the top two values.

## When to Use It

Use this pattern when you see:
- A problem asking for a maximum/minimum value derived from **two** distinct elements.
- The operation is **monotonic** with respect to each input (larger inputs → larger output after the transformation).
- Constraints allow a linear or O(n log n) scan (n ≤ 500 here, but we aim for O(n)).

## Template

```python
def max_product(nums):
    # Track the largest and second largest values while scanning once
    max1 = max2 = float('-inf')
    for x in nums:
        if x > max1:
            max2 = max1   # previous max becomes second max
            max1 = x
        elif x > max2:
            max2 = x
    # Apply the required transformation
    return (max1 - 1) * (max2 - 1)
```

## LeetCode Problem Walkthrough

### Problem: 1464. Maximum Product of Two Elements in an Array

https://leetcode.com/problems/maximum-product-of-two-elements-in-an-array/

### Approach 1: Brute Force

**Algorithm**
- Check every possible pair of distinct indices (i, j).
- Compute `(nums[i]-1)*(nums[j]-1)` and keep the maximum.

**Implementation**

```python
class Solution:
    def maxProduct(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        n = len(nums)
        best = 0
        for i in range(n):
            for j in range(i + 1, n):
                val = (nums[i] - 1) * (nums[j] - 1)
                if val > best:
                    best = val
        return best
```

**Complexity Analysis**
- Time complexity: O(n²) — we examine all n·(n‑1)/2 pairs.
- Space complexity: O(1) — only a few scalar variables are used.

### Approach 2: Sorting

**Intuition**
If we sort the array, the two largest numbers will end up at the end. Since the expression `(a‑1)*(b‑1)` grows when `a` and `b` grow, picking the last two elements yields the maximum.

**Algorithm**
1. Sort `nums` in non‑decreasing order.
2. Take the last two elements, apply the formula, and return the result.

**Implementation**

```python
class Solution:
    def maxProduct(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        nums.sort()                     # O(n log n)
        return (nums[-1] - 1) * (nums[-2] - 1)
```

**Complexity Analysis**
- Time complexity: O(n log n) — dominated by the sorting step.
- Space complexity: O(1) or O(n) depending on the sorting implementation (Python’s Timsort uses O(n) worst‑case auxiliary space).

### Approach 3: Single‑Pass Track Two Maximums

**Intuition**
We do not need the full ordering; we only need the biggest and second‑biggest values. By updating these two variables while scanning the array once, we obtain the answer in linear time and constant space.

**Algorithm**
- Initialize `max1` and `max2` to negative infinity.
- For each number `x`:
  - If `x` exceeds `max1`, shift `max1` to `max2` and store `x` in `max1`.
  - Else if `x` exceeds `max2`, store `x` in `max2`.
- After the loop, compute `(max1‑1)*(max2‑1)`.

**Implementation**

```python
class Solution:
    def maxProduct(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        max1 = max2 = float('-inf')
        for x in nums:
            if x > max1:
                max2 = max1
                max1 = x
            elif x > max2:
                max2 = x
        return (max1 - 1) * (max2 - 1)
```

**Complexity Analysis**
- Time complexity: O(n) — one pass through the array.
- Space complexity: O(1) — only two extra variables.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

We trace the single‑pass algorithm on the input `nums = [3,4,5,2]`.

| Step | x  | max1 before | max2 before | Action                              | max1 after | max2 after |
|------|----|-------------|-------------|-------------------------------------|------------|------------|
| 1    | 3  | -inf        | -inf        | x > max1 → shift, store             | 3          | -inf       |
| 2    | 4  | 3           | -inf        | x > max1 → shift, store             | 4          | 3          |
| 3    | 5  | 4           | 3           | x > max1 → shift, store             | 5          | 4          |
| 4    | 2  | 5           | 4           | x ≤ max1 and x > max2? no → unchanged| 5          | 4          |

Result: `(5‑1)*(4‑1) = 4*3 = 12`, matching the expected output.

--- 

*This lecture follows the Arrays & Hashing pattern: identify the two largest elements to maximize a monotonic function of a pair.*