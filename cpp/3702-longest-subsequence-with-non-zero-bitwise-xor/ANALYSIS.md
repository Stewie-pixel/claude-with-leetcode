# Bitwise XOR

## Video Solution

For more details about **Longest Subsequence With Non-Zero Bitwise XOR**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=qtgrdY929oc).

## Concept

The XOR operation has two key properties that make it useful for subsequence problems:

1. **Self‑cancelling**: `a ^ a = 0`.  
   If a value appears an even number of times in a set, its contribution to the total XOR disappears.
2. **Zero‑only when everything cancels**: The XOR of a collection is `0` **iff** every bit is set an even number of times across the collection.

For a subsequence we can freely drop elements. Therefore, to obtain a **non‑zero** XOR we only need to avoid the situation where the XOR of the chosen elements cancels out completely.

*Real‑world analogy*: Think of each bit as a light switch. XOR tells us whether the switch is ON (odd number of toggles) or OFF (even). We want the final configuration to have at least one switch ON. If the whole array already leaves at least one switch ON, we can keep everything. If everything is OFF, we must remove a single element that contributed a toggle (any non‑zero number) to turn at least one switch back ON.

## When to Use It

Use bitwise XOR reasoning when you see:

- Problems asking for the XOR of a subset/subsequence.
- Checks whether a XOR can be made zero or non‑zero.
- Situations where elements can be freely included/excluded (subsequence, not subarray).
- Parity‑type arguments (odd/even counts of bits).

## Template

A common template for XOR‑based subsequence/array problems:

```python
def solve(nums):
    total_xor = 0          # XOR of all elements
    has_non_zero = False   # whether any element != 0

    for x in nums:
        total_xor ^= x
        if x != 0:
            has_non_zero = True

    # Apply the specific problem logic here
    # ...

    return answer
```

The template gathers the two pieces of information we usually need:
- the XOR of the whole array (`total_xor`)
- whether there exists at least one non‑zero element (`has_non_zero`)

From there we can decide the answer in O(1) time.

## LeetCode Problem Walkthrough

### Problem: 3702. Longest Subsequence With Non-Zero Bitwise XOR

https://leetcode.com/problems/longest-subsequence-with-non-zero-bitwise-xor/

### Approach 1: Brute Force

**Algorithm**
- Enumerate every possible subsequence (there are `2^n` of them).
- For each subsequence compute its XOR.
- Track the maximum length whose XOR ≠ 0.
- If none found, return 0.

**Implementation**

```python
from itertools import combinations

class Solution:
    def longestSubsequence(self, nums):
        n = len(nums)
        best = 0
        # try every size from n down to 1 – early exit when we find a valid one
        for size in range(n, 0, -1):
            for comb in combinations(nums, size):
                xor_val = 0
                for v in comb:
                    xor_val ^= v
                if xor_val != 0:
                    return size          # first (largest) size that works
        return 0
```

**Complexity Analysis**
- Time complexity: O(2^n * n) — we generate all subsets and compute XOR for each.
- Space complexity: O(n) — recursion/ combination generator uses O(n) auxiliary space.

### Approach 2: Use XOR Properties (Insight)

**Intuition**
The XOR of the whole array tells us whether the *full* sequence already has a non‑zero XOR.
- If `total_xor != 0`, we can keep **all** elements → answer = `n`.
- If `total_xor == 0`, the XOR of the whole array is zero. Removing any element `x` changes the XOR to `0 ^ x = x`.  
  Hence, if there exists at least one non‑zero element, deleting **one** such element yields a non‑zero XOR → answer = `n‑1`.
- If every element is zero, any subsequence XOR stays zero → answer = 0.

This reduces the problem to two simple scans.

**Implementation**

```python
class Solution:
    def longestSubsequence(self, nums):
        total_xor = 0
        has_non_zero = False

        for v in nums:
            total_xor ^= v
            if v != 0:
                has_non_zero = True

        if total_xor != 0:
            return len(nums)               # whole array works
        if has_non_zero:
            return len(nums) - 1           # drop one non‑zero
        return 0                           # all zeros
```

**Complexity Analysis**
- Time complexity: O(n) — single pass to compute XOR and flag.
- Space complexity: O(1) — only a few integer variables.

### Approach 3: Most Optimal (Same as Approach 2)

The solution above is already optimal; no further asymptotic improvement is possible because we must inspect each element at least once to know whether any non‑zero value exists and to compute the total XOR.

Thus we present the same code as the *most optimal* approach.

**Complexity Analysis**
- Time complexity: O(n)
- Space complexity: O(1)

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the example `nums = [1, 2, 3]`.

## Dry Run

Input: nums = [1, 2, 3]

| Step | i | nums[i] | total_xor (so far) | has_non_zero (so far) |
|------|---|---------|--------------------|-----------------------|
| start| - | -       | 0                  | False                 |
| 1    | 0 | 1       | 0 ^ 1 = 1          | True (1 ≠ 0)          |
| 2    | 1 | 2       | 1 ^ 2 = 3          | True                  |
| 3    | 2 | 3       | 3 ^ 3 = 0          | True                  |

After the loop:
- `total_xor = 0`
- `has_non_zero = True`

Since `total_xor == 0` and we have a non‑zero element, we return `len(nums) - 1 = 3 - 1 = 2`.

The longest subsequence with non‑zero XOR is any two‑element subset, e.g., `[2, 3]` → `2 ^ 3 = 1`.

---

**Summary**  
- The full array works iff its XOR is non‑zero.  
- Otherwise, dropping a single non‑zero element suffices.  
- If all elements are zero, no non‑zero XOR subsequence exists.  

This yields an O(n) time, O(1) space solution.