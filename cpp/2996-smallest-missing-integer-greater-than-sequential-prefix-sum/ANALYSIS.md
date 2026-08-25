# Arrays & Hashing

## Video Solution

For more details about **Smallest Missing Integer Greater Than Sequential Prefix Sum**, watch the walkthrough at [https://www.youtube.com/watch?v=G5ugauee9mk](https://www.youtube.com/watch?v=G5ugauee9mk).

## Concept

The problem asks for two things:
1. Find the **sum of the longest prefix** that starts at index 0 and where each element is exactly one more than the previous (`nums[j] = nums[j‑1] + 1`).
2. Starting from that sum, return the **smallest integer not present** in the whole array.

Because the required prefix must begin at the first element and be contiguous, we can scan the array once, accumulating the sum while the sequential condition holds. Once the condition fails, no longer prefix can exist, so we stop.

To find the smallest missing integer ≥ that sum, we store all array values in a hash set for O(1) look‑ups and keep incrementing the candidate sum until it is absent from the set.

## When to Use It

Use this pattern when you see:
- A condition that depends on a prefix starting at index 0 (or any fixed start).
- The predicate is something like “each next element equals previous + 1” (or another simple relation).
- After computing a value from that prefix, you need to query membership in the original array (hence a hash set).

## Template

```python
def longest_sequential_prefix_sum(nums):
    """Return sum of the longest prefix where each element increments by 1."""
    total = nums[0]
    for i in range(1, len(nums)):
        if nums[i] == nums[i - 1] + 1:
            total += nums[i]
        else:
            break
    return total

def smallest_missing_at_least(start, nums_set):
    """Return the smallest integer >= start that is not in nums_set."""
    x = start
    while x in nums_set:
        x += 1
    return x
```

## LeetCode Problem Walkthrough

### Problem: 2996. Smallest Missing Integer Greater Than Sequential Prefix Sum

https://leetcode.com/problems/smallest-missing-integer-greater-than-sequential-prefix-sum/

### Approach 1: Brute Force (Check Every Prefix)

**Algorithm**
1. Insert all numbers into a hash set for O(1) look‑ups.
2. For each possible end index `i` (from 0 to n‑1):
   - Verify that the prefix `nums[0..i]` is sequential by checking every adjacent pair inside it.
   - If it is sequential, compute its sum.
   - Keep the maximum sum among all sequential prefixes.
3. Starting from that maximum sum, increment until you find a value not in the set; return it.

**Implementation**

```python
class Solution:
    def missingInteger(self, nums: List[int]) -> int:
        n = len(nums)
        seen = set(nums)                 # O(n) space
        best_sum = 0

        # Try every prefix [0..i]
        for i in range(n):
            # check if prefix 0..i is sequential
            sequential = True
            for j in range(1, i + 1):
                if nums[j] != nums[j - 1] + 1:
                    sequential = False
                    break
            if sequential:
                cur_sum = sum(nums[0:i + 1])
                best_sum = max(best_sum, cur_sum)

        # find smallest missing >= best_sum
        ans = best_sum
        while ans in seen:
            ans += 1
        return ans
```

**Complexity Analysis**
- Time complexity: O(n²) — the outer loop runs n times and the inner verification scans up to O(n) elements.
- Space complexity: O(n) — the hash set stores all array elements.

### Approach 2: Optimized Single Pass

**Intuition**
Because the required prefix must start at index 0 and be contiguous, once we encounter a position where `nums[i] != nums[i‑1] + 1`, the prefix can no longer be extended. Therefore we can accumulate the sum in one linear scan and stop at the first break.

**Algorithm**
1. Insert all numbers into a hash set.
2. Initialise `prefix_sum` with `nums[0]`.
3. Iterate `i` from 1 to n‑1:
   - If `nums[i] == nums[i‑1] + 1`, add `nums[i]` to `prefix_sum`.
   - Else, break the loop (the longest sequential prefix ends at `i‑1`).
4. Starting from `prefix_sum`, increment until the value is absent from the set; return it.

**Implementation**

```python
class Solution:
    def missingInteger(self, nums: List[int]) -> int:
        seen = set(nums)                 # O(n) space
        prefix_sum = nums[0]             # at least the first element forms a sequential prefix

        for i in range(1, len(nums)):
            if nums[i] == nums[i - 1] + 1:
                prefix_sum += nums[i]
            else:
                break

        ans = prefix_sum
        while ans in seen:
            ans += 1
        return ans
```

**Complexity Analysis**
- Time complexity: O(n) — one pass to compute the prefix sum, plus at most O(n) increments in the worst case (when the array contains a long consecutive run).
- Space complexity: O(n) — the hash set.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: `nums = [1, 2, 3, 2, 5]`

| Step | i | nums[i] | nums[i‑1] + 1 | Condition met? | prefix_sum so far | Action |
|------|---|---------|--------------|----------------|-------------------|--------|
| init | – | 1       | –            | –              | 1                 | start with first element |
| 1    | 1 | 2       | 1+1 = 2      | Yes            | 1 + 2 = 3         | add |
| 2    | 2 | 3       | 2+1 = 3      | Yes            | 3 + 3 = 6         | add |
| 3    | 3 | 2       | 3+1 = 4      | No (2 ≠ 4)     | 6                 | break – longest prefix is [1,2,3] |
| 4    | – | –       | –            | –              | 6                 | prefix_sum = 6 |

Now find smallest missing ≥ 6 using the set `{1,2,3,2,5}` → `{1,2,3,5}`:

- 6 ∉ set → answer = 6.

---

**Summary**

- The optimal solution runs in linear time with a hash set.
- Always begin with the brute force to ensure correctness, then reveal the key insight that the prefix must start at index 0 and be contiguous, enabling a single‑pass scan.  
- Commit this lecture as `2025-09-26-arrays-hashing.md` under `/lectures/`.