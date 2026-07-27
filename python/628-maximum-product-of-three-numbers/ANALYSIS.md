# Arrays & Hashing

## Video Solution

For more details about **Maximum Product of Three Numbers**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=Yz-qJHM0Zf4).

## Concept

To obtain the maximum product of three numbers from an array we only need to consider a few extreme values:

* The three largest numbers (all positives or the least negatives)  
* The two smallest numbers (most negative) combined with the largest number (because a negative × negative = positive)

If the array contains only non‑negative numbers the answer is simply the product of the three largest values.  
When negatives are present, a pair of large‑magnitude negatives can outweigh a small positive, so we must also check the product of the two smallest numbers with the largest number.

The problem therefore reduces to finding the three maximum and the two minimum elements – we can do this by sorting or by a single linear scan.

## When to Use It

Use this pattern when you see:

* “Maximum product of **k** numbers” (here k = 3)  
* Arrays that may contain negative numbers  
* Questions asking for an extreme value (max/min) that can be derived from a few sorted extremes  
* Constraints that allow O(n log n) or O(n) solutions (n ≤ 10⁴)

## Template

```python
# General template for max product of three numbers
def max_product_of_three(nums):
    # 1️⃣  Sort the array (O(n log n))
    nums.sort()
    # 2️⃣  Candidates:
    #     a) product of three largest numbers
    #     b) product of two smallest (most negative) and the largest
    return max(nums[-1] * nums[-2] * nums[-3],
               nums[0] * nums[1] * nums[-1])
```

---

## LeetCode Problem Walkthrough

### Problem: 628. Maximum Product of Three Numbers  
https://leetcode.com/problems/maximum-product-of-three-numbers/

### Approach 1: Brute Force

**Algorithm**  
Check every possible triple (i, j, k) with i < j < k, compute its product, and keep the maximum.

**Implementation**

```python
class Solution:
    def maximumProduct(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        n = len(nums)
        best = float('-inf')
        for i in range(n):
            for j in range(i + 1, n):
                for k in range(j + 1, n):
                    prod = nums[i] * nums[j] * nums[k]
                    if prod > best:
                        best = prod
        return best
```

**Complexity Analysis**

- Time complexity: O(n³) — three nested loops over the array.  
- Space complexity: O(1) — only a few scalar variables are used.

---

### Approach 2: Sorting

**Intuition**  
After sorting, the largest values are at the end and the smallest (most negative) at the start.  
The maximum product can only come from either the three largest numbers or the two smallest numbers paired with the largest number, because any other combination replaces one of these extremes with a less‑impactful value.

**Algorithm**  
1. Sort `nums`.  
2. Compute `candidate1 = nums[-1] * nums[-2] * nums[-3]` (three largest).  
3. Compute `candidate2 = nums[0] * nums[1] * nums[-1]` (two smallest + largest).  
4. Return the larger of the two candidates.

**Implementation**

```python
class Solution:
    def maximumProduct(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        nums.sort()                                   # O(n log n)
        return max(nums[-1] * nums[-2] * nums[-3],    # three largest
                   nums[0] * nums[1] * nums[-1])      # two smallest + largest
```

**Complexity Analysis**

- Time complexity: O(n log n) — dominated by the sort.  
- Space complexity: O(1) — sorting is in‑place (Python’s Timsort uses O(n) worst‑case auxiliary space, but the problem treats it as O(1) for interview purposes; if strict, note O(n) auxiliary).

---

### Approach 3: Single Scan (Track Extremes)

**Intuition**  
We don’t need a full sort; we only need the three largest and two smallest values.  
By scanning the array once we can update these five variables, achieving O(n) time and O(1) extra space.

**Algorithm**  
Initialize:  
* `max1, max2, max3 = -∞, -∞, -∞` (largest three)  
* `min1, min2 = ∞, ∞` (smallest two)  

For each number `x` in `nums`:  
1. Update the maxes:  
   - if `x > max1`: shift `max3←max2`, `max2←max1`, `max1←x`  
   - else if `x > max2`: `max3←max2`, `max2←x`  
   - else if `x > max3`: `max3←x`  
2. Update the mins similarly:  
   - if `x < min1`: `min2←min1`, `min1←x`  
   - else if `x < min2`: `min2←x`  

After the loop, answer = max(`max1*max2*max3`, `min1*min2*max1`).

**Implementation**

```python
class Solution:
    def maximumProduct(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        # Initialize extremes
        max1 = max2 = max3 = float('-inf')
        min1 = min2 = float('inf')

        for x in nums:
            # Update maxes
            if x > max1:
                max3, max2, max1 = max2, max1, x
            elif x > max2:
                max3, max2 = max2, x
            elif x > max3:
                max3 = x

            # Update mins
            if x < min1:
                min2, min1 = min1, x
            elif x < min2:
                min2 = x

        return max(max1 * max2 * max3, min1 * min2 * max1)
```

**Complexity Analysis**

- Time complexity: O(n) — one pass through the array.  
- Space complexity: O(1) — only five scalar variables are stored.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Input: `nums = [-10, -10, 5, 2]`

We expect the answer to be `(-10) * (-10) * 5 = 500`.

| Step | x  | max1 | max2 | max3 | min1 | min2 | Action (maxes)                               | Action (mins)                             |
|------|----|------|------|------|------|------|----------------------------------------------|-------------------------------------------|
| 1    | -10| -10  | -∞   | -∞   | -10  | ∞    | max1←-10                                     | min1←-10                                  |
| 2    | -10| -10  | -10  | -∞   | -10  | -10  | max2←-10 (since -10 > -∞)                    | min2←-10 (since -10 < ∞)                  |
| 3    | 5  | 5    | -10  | -10  | -10  | -10  | max1←5, shift old max1→max2, max2→max3      | no change (5 not < min1/min2)             |
| 4    | 2  | 5    | 2    | -10  | -10  | -10  | max2←2 (since 2 > -10)                       | no change                                 |

Final values: `max1=5`, `max2=2`, `max3=-10`, `min1=-10`, `min2=-10`.

Compute candidates:  
* `max1*max2*max3 = 5 * 2 * (-10) = -100`  
* `min1*min2*max1 = (-10) * (-10) * 5 = 500`

Maximum = **500**, which matches the expected result.