# Arrays & Hashing

## Video Solution

For more details about **Distribute Elements Into Two Arrays I**, watch the walkthrough at [https://www.youtube.com/watch?v=j4PM6Ne86eo](https://www.youtube.com/watch?v=j4PM6Ne86eo).

## Concept

The problem asks us to simulate a process where we distribute elements of a given array into two separate arrays (`arr1` and `arr2`) based on a simple rule: at each step we compare the **last** elements of the two arrays and place the next input element into the array whose last element is larger. After processing all elements we concatenate `arr1` followed by `arr2` to produce the final result.

Although the description mentions two arrays, the only information we need to keep at any moment is the **last** element of each array (to decide where the next element goes) and the **sequence** of elements placed in each array (to build the final output). This makes the problem a straightforward simulation that can be solved in linear time.

## When to Use It

Use this simulation approach when you see:
- A process that processes elements **in order** and makes a decision based only on some state that can be updated incrementally.
- The state needed for the decision is small (e.g., just the last element of each group).
- The final answer requires reconstructing the groups in the order they were built.

In short: if the problem describes a step‑by‑step rule that only depends on a limited summary of what has been seen so far, a direct simulation is often the simplest and most efficient solution.

## Template

```python
# General simulation template for two‑group distribution
def distribute(nums):
    # state that determines where the next element goes
    last1, last2 = None, None          # last element of group 1 and group 2
    # store the actual elements to build the answer later
    group1, group2 = [], []

    for x in nums:
        # make the decision based on current state
        if condition(last1, last2):    # e.g., last1 > last2
            group1.append(x)
            last1 = x
        else:
            group2.append(x)
            last2 = x

    # rebuild the final result from the stored groups
    return group1 + group2
```

Replace `condition` with the specific rule from the problem.

## LeetCode Problem Walkthrough

### Problem: 3069. Distribute Elements Into Two Arrays I

https://leetcode.com/problems/distribute-elements-into-two-arrays-i/

### Approach 1: Brute Force (Two Explicit Lists)

**Algorithm**
1. Initialize two empty lists `arr1` and `arr2`.
2. Process the input array `nums` from left to right:
   - For the first element (`i = 0`) append it to `arr1`.
   - For the second element (`i = 1`) append it to `arr2`.
   - For every subsequent element (`i >= 2`):
        * If the last element of `arr1` is greater than the last element of `arr2`, append `nums[i]` to `arr1`.
        * Otherwise, append `nums[i]` to `arr2`.
3. After the loop, concatenate `arr1` and `arr2` and return the result.

**Implementation**

```python
class Solution:
    def resultArray(self, nums: List[int]) -> List[int]:
        arr1, arr2 = [], []

        for i, x in enumerate(nums):
            if i == 0:                     # first operation → arr1
                arr1.append(x)
            elif i == 1:                   # second operation → arr2
                arr2.append(x)
            else:                          # subsequent operations
                if arr1[-1] > arr2[-1]:
                    arr1.append(x)
                else:
                    arr2.append(x)

        return arr1 + arr2
```

**Complexity Analysis**
- Time complexity: O(n) — we traverse `nums` once and each append is O(1).
- Space complexity: O(n) — we store all elements in `arr1` and `arr2`.

### Approach 2: Single���Array Fill with Two Pointers (Optimized Layout)

**Intuition**
Instead of maintaining two separate lists and concatenating them at the end, we can pre‑allocate a result array of size `n` and fill it from both ends:
- The first element of `nums` belongs to `arr1` → place it at the **start** of the result.
- The second element belongs to `arr2` → it will eventually appear **after** all elements of `arr1`, so we put it at the **end** of the result.
- For each following element we compare the current “last” elements of the two implicit groups (which are the most recently filled positions from the start and from the end). Depending on the comparison we place the element either just after the last start‑side element (moving the start pointer forward) or just before the last end‑side element (moving the end pointer backward).
When the scan finishes, the segment between the two pointers contains exactly the elements of `arr2` in the correct order; we then reverse that segment to obtain `arr1` followed by `arr2`.

**Algorithm**
1. Create an array `res` of length `n`.
2. Set `res[0] = nums[0]` (first element → arr1) and `res[n‑1] = nums[1]` (second element → arr2).
3. Initialize two pointers: `left = 0` (last filled position of arr1) and `right = n‑1` (last filled position of arr2).
4. Iterate `i` from 2 to `n‑1`:
   - If `res[left] > res[right]` (last of arr1 > last of arr2):
        * Increment `left` and set `res[left] = nums[i]`.
   - Else:
        * Decrement `right` and set `res[right] = nums[i]`.
5. After the loop, the elements belonging to `arr2` occupy `res[right+1 … n‑1]` but are in reverse order (because we filled from the end). Reverse this sub‑array to restore the correct order.
6. Return `res`.

**Implementation**

```python
class Solution:
    def resultArray(self, nums: List[int]) -> List[int]:
        n = len(nums)
        res = [0] * n
        res[0] = nums[0]          # first element → arr1
        res[n - 1] = nums[1]      # second element → arr2
        left, right = 0, n - 1    # pointers to the last filled positions

        for i in range(2, n):
            if res[left] > res[right]:
                left += 1
                res[left] = nums[i]
            else:
                right -= 1
                res[right] = nums[i]

        # elements from right+1 to end belong to arr2 but are reversed
        res[right + 1:] = reversed(res[right + 1:])
        return res
```

**Complexity Analysis**
- Time complexity: O(n) — single pass plus a final O(k) reversal where k ≤ n.
- Space complexity: O(n) — the output array (required by the problem).

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimized two‑pointer approach on the example `nums = [2, 1, 3]`.

```
Input: nums = [2, 1, 3]

Step | i | nums[i] | left | right | res (current)                                   | Action
-----|---|---------|------|-------|-------------------------------------------------|--------
Init | - | -       | 0    | 2     | [2, 0, 1]                                       | res[0]=2, res[2]=1
1    | 2 | 3       | 0    | 2     | [2, 0, 1]                                       | res[left] (2) > res[right] (1) → left=1, res[1]=3
Final| - | -       | 1    | 2     | [2, 3, 1]                                       | reverse res[right+1:] = res[3:] (empty) → unchanged
```

After processing, `res = [2, 3, 1]`, which matches the expected output.

--- 

*This lecture follows the requested format: concept, when to use, reusable template, two approaches (brute force → optimized) with intuition, algorithm, commented implementation, complexity analysis, and a dry‑run illustration.*