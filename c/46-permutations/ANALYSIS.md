# Backtracking

## Video Solution

For more details about **Permutations**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=FZe0UqISmUw).

## Concept

Backtracking is a systematic way to iterate through all possible configurations of a search space. We build candidates incrementally and abandon a candidate (“backtrack”) as soon as we determine it cannot lead to a valid solution. For permutations, we construct each permutation one element at a time, keeping track of which elements have already been used.

## When to Use It

Use backtracking when you see:
- “All possible …” (permutations, combinations, subsets)
- Need to explore every arrangement/selection
- Constraints are small enough that exhaustive search is feasible (typically n ≤ 10 for permutations)
- The problem asks for every valid configuration, not just one

## Template

```python
def backtrack(path, used):
    # path: current partial permutation
    # used: boolean list marking which elements are already in path

    # Base case: if we have used all numbers, record the permutation
    if len(path) == len(nums):
        result.append(path[:])   # make a copy
        return

    # Try every number that hasn't been used yet
    for i in range(len(nums)):
        if not used[i]:
            used[i] = True                 # choose
            path.append(nums[i])

            backtrack(path, used)          # explore

            path.pop()                     # un‑choose (backtrack)
            used[i] = False
```

---

## LeetCode Problem Walkthrough

### Problem: 46. Permutations

https://leetcode.com/problems/permutations/

### Approach 1: Brute Force (using Python’s itertools)

**Algorithm**
1. Import `itertools.permutations`.
2. Call `permutations(nums)` which lazily generates all n! permutations.
3. Convert each tuple to a list and collect them into the result list.

**Implementation**

```python
from itertools import permutations
from typing import List

class Solution:
    def permute(self, nums: List[int]) -> List[List[int]]:
        # itertools.permutations returns tuples; cast each to list
        return [list(p) for p in permutations(nums)]
```

**Complexity Analysis**
- Time complexity: O(n! * n) — we generate n! permutations and each conversion to list costs O(n).
- Space complexity: O(n! * n) — to store all permutations (output space).

---

### Approach 2: Backtracking with Visited Array

**Intuition**
At each recursion level we decide which unused number to place next. By marking numbers as used (`visited` array) we guarantee we never reuse an element within the same permutation, and when we backtrack we un‑mark it so it can be used in other branches.

**Algorithm**
1. Create a `visited` boolean array initialized to `False`.
2. Recursively build a `path` list:
   - If `len(path) == len(nums)`, we have a complete permutation → add a copy to `result`.
   - Otherwise, iterate over indices `i`:
        - If `visited[i]` is `False`, set it to `True`, append `nums[i]` to `path`, and recurse.
        - After recursion, pop the last element and set `visited[i]` back to `False` (backtrack).
3. Launch the recursion with empty `path` and return `result`.

**Implementation**

```python
from typing import List

class Solution:
    def permute(self, nums: List[int]) -> List[List[int]]:
        result: List[List[int]] = []
        visited = [False] * len(nums)

        def backtrack(path: List[int]):
            if len(path) == len(nums):
                result.append(path[:])          # copy current permutation
                return

            for i in range(len(nums)):
                if not visited[i]:
                    visited[i] = True
                    path.append(nums[i])

                    backtrack(path)             # explore deeper

                    path.pop()                  # undo choice
                    visited[i] = False          # make i available again

        backtrack([])
        return result
```

**Complexity Analysis**
- Time complexity: O(n! * n) — same reasoning as brute force; each of the n! permutations requires O(n) work to copy.
- Space complexity: O(n) auxiliary (recursion stack + `visited` + `path`) plus O(n! * n) for output.

---

### Approach 3: Backtracking with In‑Place Swapping

**Intuition**
Instead of a separate `visited` array, we can generate permutations by swapping elements in the original array. The prefix `[0:start)` holds the fixed part of the permutation, and we recursively permute the suffix `[start:)`. Swapping avoids extra memory for the `visited` array and the explicit `path` list.

**Algorithm**
1. Define a recursive function `backtrack(start)` that fixes the element at index `start`.
2. If `start == len(nums)`, we have a permutation → copy `nums` to `result`.
3. Otherwise, for each index `i` from `start` to `len(nums)-1`:
   - Swap `nums[start]` with `nums[i]` (put a new element at position `start`).
   - Recurse with `backtrack(start + 1)`.
   - Swap back (backtrack) to restore original ordering before the next iteration.
4. Kick off with `backtrack(0)`.

**Implementation**

```python
from typing import List

class Solution:
    def permute(self, nums: List[int]) -> List[List[int]]:
        result: List[List[int]] = []

        def backtrack(start: int):
            if start == len(nums):
                result.append(nums[:])          # snapshot of current permutation
                return

            for i in range(start, len(nums)):
                # place nums[i] at the current start position
                nums[start], nums[i] = nums[i], nums[start]
                backtrack(start + 1)
                # restore original order (backtrack)
                nums[start], nums[i] = nums[i], nums[start]

        backtrack(0)
        return result
```

**Complexity Analysis**
- Time complexity: O(n! * n) — we still produce n! permutations and each copy costs O(n).
- Space complexity: O(n) — recursion stack depth plus the in‑place array (no extra `visited` or `path`).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace Approach 2 (visited backtracking) on `nums = [1, 2, 3]`.

#### Dry Run

Input: `nums = [1, 2, 3]`


| Step | path        | visited (index:0 1 2) | Action                                 |
|------|-------------|-----------------------|----------------------------------------|
| 1    | []          | [F, F, F]             | start loop i=0 → choose 1              |
| 2    | [1]         | [T, F, F]             | recurse, i=0 skip, i=1 → choose 2      |
| 3    | [1,2]       | [T, T, F]             | recurse, i=0/1 skip, i=2 → choose 3    |
| 4    | [1,2,3]     | [T, T, T]             | base case → add [1,2,3] to result      |
| 5    | [1,2]       | [T, T, F]             | backtrack: un‑choose 3                 |
| 6    | [1]         | [T, F, F]             | backtrack: un‑choose 2, try i=2 → choose 3 |
| 7    | [1,3]       | [T, F, T]             | recurse, i=0 skip, i=1 → choose 2      |
| 8    | [1,3,2]     | [T, T, T]             | base case → add [1,3,2]                |
| 9    | [1,3]       | [T, F, T]             | backtrack: un‑choose 2                 |
|10    | [1]         | [T, F, F]             | backtrack: un‑choose 3, loop ends      |
|11    | []          | [F, F, F]             | backtrack: un‑choose 1, i=1 → choose 2 |
|12    | [2]         | [F, T, F]             | recurse, i=0 → choose 1                |
|13    | [2,1]       | [T, T, F]             | recurse, i=2 → choose 3                |
|14    | [2,1,3]     | [T, T, T]             | add [2,1,3]                            |
|15    | ...         | ...                   | (continue symmetric branches)          |
|…     | …           | …                     | …                                      |

Final result: [[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
