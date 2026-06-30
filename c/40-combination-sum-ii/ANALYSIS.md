# Backtracking (Combination Sum II)

## Video Solution

For more details about **Combination Sum II**, watch the walkthrough at [https://www.youtube.com/watch?v=e7N56ii0JQI](https://www.youtube.com/watch?v=e7N56ii0JQI).

## Concept

Backtracking is a systematic way to explore all possible configurations (like subsets, permutations, or combinations) and abandon a path as soon as we determine it cannot lead to a valid solution.  
For Combination Sum II we sort the candidates first; then we recursively decide for each element whether to **take** it (once) or **skip** it.  
When we encounter duplicate values we skip the second (and later) occurrences at the same recursion depth to avoid generating duplicate combinations.

## When to Use It

Use backtracking when you see:
- “Find all …” or “list all …” solutions.
- Each element can be used **at most once**.
- The input may contain duplicates and the output must contain **unique** combinations.
- The constraints are small enough for exponential search (here `n ≤ 100` but pruning makes it feasible).

## Template

```python
def backtrack(start, path, remaining):
    # base case: found a valid combination
    if remaining == 0:
        result.append(path[:])
        return
    # base case: exceeded target or no more elements
    if remaining < 0 or start == len(candidates):
        return

    prev = None                      # used to skip duplicates at this level
    for i in range(start, len(candidates)):
        if candidates[i] == prev:    # skip same value we have‑been‑used duplicate
            continue
        # choose candidates[i]
        path.append(candidates[i])
        backtrack(i + 1, path, remaining - candidates[i])  # i+1 because each used once
        path.pop()                   # undo choice
        prev = candidates[i]         # remember what we just tried
```

---

## LeetCode Problem Walkthrough

### Problem: 40. Combination Sum II
https://leetcode.com/problems/combination-sum-ii/

### Approach 1: Brute Force (Bitmask + Deduplication)

**Algorithm**
1. Enumerate every subset of `candidates` using a bitmask from `0` to `2^n‑1`.
2. For each mask, compute the sum of the selected elements.
3. If the sum equals `target`, sort the selected elements and insert the tuple into a `set` to automatically discard duplicates.
4. Finally, convert the set of tuples back to a list of lists.

**Implementation**

```python
from typing import List

class Solution:
    def combinationSum2(self, candidates: List[int], target: int) -> List[List[int]]:
        n = len(candidates)
        seen = set()                     # stores tuple(sorted(combination))
        for mask in range(1 << n):       # 0 … 2^n‑1
            cur_sum = 0
            cur = []
            for i in range(n):
                if mask & (1 << i):
                    cur_sum += candidates[i]
                    cur.append(candidates[i])
            if cur_sum == target:
                seen.add(tuple(sorted(cur)))   # sorting canonicalizes order
        return [list(t) for t in seen]
```

**Complexity Analysis**
- Time complexity: O(2^n * n) — we iterate over all subsets (2^n) and spend O(n) to compute sum and build the combination.
- Space complexity: O(2^n) in the worst case for the set that may store many combinations (output size excluded).

---

### Approach 2: Backtracking with Sorting & Duplicate Skip (Optimized)

**Intuition**
Sorting puts equal numbers next to each other. While recursing, if we have already tried a value at the current depth, any further identical values would generate the same combinations, so we skip them. This avoids the need for a post‑hoc deduplication set and prunes useless branches early.

**Algorithm**
1. Sort `candidates`.
2. Define a recursive helper `dfs(idx, path, remain)`:
   - If `remain == 0`: record a copy of `path`.
   - If `remain < 0` or `idx == len(candidates)`: return.
   - Iterate `i` from `idx` to end:
     - If `i > idx` and `candidates[i] == candidates[i‑1]`: skip (duplicate at this level).
     - Choose `candidates[i]`: append to `path`, recurse with `i+1` (each used once) and `remain - candidates[i]`.
     - Backtrack: pop `path`.
3. Kick off the recursion with `dfs(0, [], target)`.

**Implementation**

```python
from typing import List

class Solution:
    def combinationSum2(self, candidates: List[int], target: int) -> List[List[int]]:
        candidates.sort()                 # bring duplicates together
        res: List[List[int]] = []

        def dfs(start: int, path: List[int], remain: int) -> None:
            if remain == 0:
                res.append(path[:])
                return
            if remain < 0 or start == len(candidates):
                return

            prev = None                   # remember value used at this level
            for i in range(start, len(candidates)):
                if candidates[i] == prev:     # skip duplicate
                    continue
                # choose candidates[i]
                path.append(candidates[i])
                dfs(i + 1, path, remain - candidates[i])  # each used once
                path.pop()
                prev = candidates[i]        # update prev after trying this value

        dfs(0, [], target)
        return res
```

**Complexity Analysis**
- Time complexity: O(2^n) in the worst case (still exponential), but pruning and duplicate‑skip drastically cut the search space for typical inputs.
- Space complexity: O(n) for recursion depth + O(k·m) for storing results, where `k` is the average combination length and `m` the number of valid combinations (output space excluded).

---

## Dry Run

**Input:** `candidates = [10,1,2,7,6,1,5]`, `target = 8`  
After sorting: `[1,1,2,5,6,7,10]`

```
| Step | start | path      | remain | i  | candidates[i] | Action (skip/take)                     |
|------|-------|-----------|--------|----|----------------|----------------------------------------|
| 1    | 0     | []        | 8      | 0  | 1              | take → path=[1]                        |
| 2    | 1     | [1]       | 7      | 1  | 1 (dup)        | skip (prev==1)                         |
| 3    | 1     | [1]       | 7      | 2  | 2              | take → path=[1,2]                      |
| 4    | 3     | [1,2]     | 5      | 3  | 5              | take → path=[1,2,5] → remain=0 → record |
| 5    | 3     | [1,2]     | 5      | 4  | 6              | 6>5 → break loop (remaining too big)  |
| 6    | 2     | [1]       | 7      | 3  | 5              | take → path=[1,5] → remain=2          |
| 7    | 4     | [1,5]     | 2      | 4  | 6              | 6>2 → break                           |
| 8    | 2     | [1]       | 7      | 5  | 7              | take → path=[1,7] → remain=0 → record |
| 9    | 2     | [1]       | 7      | 6  |10              | 10>7 → break                          |
|10    | 0     | []        | 8      | 1  | 1 (dup)        | skip (prev==1)                        |
|11    | 0     | []        | 8      | 2  | 2              | take → path=[2]                        |
|12    | 3     | [2]       | 6      | 3  | 5              | take → path=[2,5] → remain=1          |
|13    | 4     | [2,5]     | 1      | 4  | 6              | 6>1 → break                           |
|14    | 3     | [2]       | 6      | 4  | 6              | take → path=[2,6] → remain=0 → record |
|15    | 3     | [2]       | 6      | 5  | 7              | 7>6 → break                           |
|16    | 0     | []        | 8      | 3  | 5              | take → path=[5] → remain=3            |
|…    | …     | …         | …      | …  | …              | …                                      |
```

The recorded combinations are:
- `[1,2,5]`
- `[1,7]`
- `[2,6]`
- `[1,1,6]` (found via the duplicate‑skip logic when the first `1` is taken, the second `1` is allowed at the next depth)

All are unique and sum to 8, matching the expected output.

---