# Arrays & Hashing

## Video Solution

For more details about **Find Missing Elements**, watch the walkthrough at [https://www.youtube.com/watch?v=yGmk8ouox84](https://www.youtube.com/watch?v=yGmk8ouox84).

## Concept

When we need to check whether elements belong to a collection and we care about **O(1)** average‑time look‑ups, a hash table (in Python, a `set`) is the ideal tool.  
The problem tells us that the original array contained every integer between its smallest and largest value, and those extremes are still present. Therefore the full range is simply `[min(nums), max(nums)]`. The missing numbers are those values in this interval that do **not** appear in the given array.

Using a hash set lets us test membership in constant time, turning what could be a quadratic scan into a linear one.

## When to Use It

Use the **Arrays & Hashing** pattern when you see:
- Need to test existence / duplicates (`x in collection`)
- Counting frequencies
- Finding missing or extra elements in a known range
- Problems where a brute‑force double loop would be O(n²) but a lookup table reduces it to O(n)

## Template

```python
def template(nums):
    # Build a hash set for O(1) look‑ups
    seen = set(nums)          # O(n) time, O(n) space

    # Example operation: iterate over a range and check membership
    result = []
    for x in range(low, high + 1):   # replace low/high with problem‑specific bounds
        if x not in seen:            # O(1) average lookup
            result.append(x)
    return result
```

---

# LeetCode Problem Walkthrough

### Problem: 3731. Find Missing Elements

https://leetcode.com/problems/find-missing-elements/

## Approach 1: Brute Force (Linear Scan for Each Candidate)

**Algorithm**
1. Compute `low = min(nums)` and `high = max(nums)`.
2. For each integer `x` in `[low, high]`:
   - Scan the entire array to see if `x` appears.
   - If not found, add `x` to the answer list.
3. Return the answer list (already in increasing order because we iterated in order).

**Implementation**
```python
class Solution:
    def findMissingElements(self, nums):
        """
        :type nums: List[int]
        :rtype: List[int]
        """
        low, high = min(nums), max(nums)
        missing = []
        for x in range(low, high + 1):
            # Linear scan to check presence
            found = False
            for num in nums:
                if num == x:
                    found = True
                    break
            if not found:
                missing.append(x)
        return missing
```

**Complexity Analysis**
- Time complexity: O(n * m) where n = size of the range (`high - low + 1`) and m = len(nums). In the worst case the range size is O(100) and nums length is O(100), giving O(n²) but with the given constraints it’s still quadratic in the input size.
- Space complexity: O(1) extra space (aside from the output list).

## Approach 2: Hash Set Lookup (Optimized)

**Intuition**
Checking whether a number exists in an array with a linear scan is wasteful. By storing all numbers in a hash set once, we can answer each existence query in O(1) average time, turning the overall algorithm linear.

**Algorithm**
1. Insert all elements of `nums` into a set `seen`.
2. Compute `low = min(nums)` and `high = max(nums)`.
3. Iterate `x` from `low` to `high` inclusive:
   - If `x` is not in `seen`, append it to the result.
4. Return the result.

**Implementation**
```python
class Solution:
    def findMissingElements(self, nums):
        """
        :type nums: List[int]
        :rtype: List[int]
        """
        seen = set(nums)               # O(n) time, O(n) space
        low, high = min(nums), max(nums)
        missing = []
        for x in range(low, high + 1): # O(range) iterations
            if x not in seen:          # O(1) average lookup
                missing.append(x)
        return missing
```

**Complexity Analysis**
- Time complexity: O(n + r) where n = len(nums) and r = high - low + 1 (size of the range). Both are O(n) given the constraints, so overall O(n).
- Space complexity: O(n) for the hash set.

## Approach 3: Boolean Array (Direct Address Table) – Alternative Optimized

**Intuition**
Since the values are bounded (1 ≤ nums[i] ≤ 100), we can use a fixed‑size boolean array as a direct lookup table. This eliminates hash‑set overhead and guarantees O(1) worst‑case look‑ups.

**Algorithm**
1. Find `low` and `high`.
2. Create a boolean list `present` of length `high + 1` (index 0 unused) initialized to `False`.
3. Mark `present[num] = True` for each `num` in `nums`.
4. Scan indices `low` through `high`; collect indices where `present[i]` is `False`.
5. Return the collected indices.

**Implementation**
```python
class Solution:
    def findMissingElements(self, nums):
        """
        :type nums: List[int]
        :rtype: List[int]
        """
        low, high = min(nums), max(nums)
        present = [False] * (high + 1)   # indices 0..high
        for num in nums:
            present[num] = True
        missing = [i for i in range(low, high + 1) if not present[i]]
        return missing
```

**Complexity Analysis**
- Time complexity: O(n + r) → O(n) (single pass to fill the table + one pass over the range).
- Space complexity: O(r) → O(high) ≤ O(100) → constant w.r.t. input size, but expressed as O(max(nums)).

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the hash‑set approach on Example 1: `nums = [1,4,2,5]`.

## Dry Run

Input: nums = [1,4,2,5]

| Step | Action                                      | seen set                | low | high | x | x in seen? | missing so far |
|------|---------------------------------------------|-------------------------|-----|------|---|------------|----------------|
| 1    | Build set from nums                         | {1,4,2,5}               | 1   | 5    |   |            |                |
| 2    | Start iteration x = low (1)                 | {1,4,2,5}               | 1   | 5    | 1 | Yes        | []             |
| 3    | x = 2                                       | {1,4,2,5}               | 1   | 5    | 2 | Yes        | []             |
| 4    | x = 3                                       | {1,4,2,5}               | 1   | 5    | 3 | No         | [3]            |
| 5    | x = 4                                       | {1,4,2,5}               | 1   | 5    | 4 | Yes        | [3]            |
| 6    | x = 5                                       | {1,4,2,5}               | 1   | 5    | 5 | Yes        | [3]            |
| 7    | End loop, return missing                    |                         |     |      |   |            | [3]            |

The algorithm correctly returns `[3]`.
