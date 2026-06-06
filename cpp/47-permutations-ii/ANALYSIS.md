# Backtracking

## Video Solution

For more details about **Permutations II**, watch the walkthrough at [https://www.youtube.com/watch?v=3-mZwU87Ttw](https://www.youtube.com/watch?v=3-mZwU87Ttw)

## Concept

Backtracking is a systematic way to iterate through all possible configurations of a search space. We build a solution incrementally, and whenever we realize that the current partial solution cannot possibly lead to a valid final answer, we **backtrack** (undo the last choice) and try a different path.  
A real‑world analogy is solving a maze: you walk forward, marking your path; if you hit a dead end you retreat to the last junction and try another corridor.

## When to Use It

Use backtracking when you see:
- “Generate all …” (permutations, combinations, subsets)
- Need to explore every possibility but can prune invalid branches early
- Constraints are small enough that exponential exploration is feasible (e.g., `n ≤ 8` for permutations)
- Duplicate elements exist and you must avoid producing duplicate results

## Template

```python
def backtrack(state, choices, visited, result):
    # state   : current partial solution (list)
    # choices : original list of elements (often sorted)
    # visited : boolean list marking which indices are already used
    # result  : accumulator for complete solutions

    if is_complete(state):
        result.append(state.copy())
        return

    for i in range(len(choices)):
        if visited[i]:
            continue
        # ---- pruning rule for duplicates ----
        if i > 0 and choices[i] == choices[i-1] and not visited[i-1]:
            continue
        # -------------------------------------

        visited[i] = True
        state.append(choices[i])

        backtrack(state, choices, visited, result)

        state.pop()
        visited[i] = False
```

The template above works for permutation‑style problems; replace `is_complete` with the appropriate condition (e.g., `len(state) == len(choices)`).

---

## LeetCode Problem Walkthrough

### Problem: 47. Permutations II
https://leetcode.com/problems/permutations-ii/

### Approach 1: Brute Force + Dedup

**Algorithm**  
1. Generate every permutation of `nums` using a naïve recursive swap‑based method (ignores duplicates).  
2. Insert each permutation into a `set` of tuples to automatically drop duplicates.  
3. Convert the set back to a list of lists.

**Implementation**

```python
from typing import List

class Solution:
    def permuteUnique(self, nums: List[int]) -> List[List[int]]:
        def backtrack(start: int):
            if start == len(nums):
                # store as tuple to make it hashable
                seen.add(tuple(nums))
                return
            for i in range(start, len(nums)):
                nums[start], nums[i] = nums[i], nums[start]   # swap
                backtrack(start + 1)
                nums[start], nums[i] = nums[i], nums[start]   # undo swap

        seen = set()
        backtrack(0)
        return [list(p) for p in seen]
```

**Complexity Analysis**  
- Time complexity: O(n! * n) — we generate n! permutations and each copy costs O(n) to tuple‑ize.  
- Space complexity: O(n! * n) — the set stores every unique permutation (worst‑case all are unique).

### Approach 2: Backtracking with Used‑Array & Sorting (Optimized)

**Intuition**  
Sorting puts equal numbers next to each other. While iterating, if the current number equals the previous one **and** the previous one has not been used in the current position, then picking the current number would create a permutation we have already generated when the previous identical number was placed in this spot. Skipping it eliminates duplicate branches without needing a post‑hoc set.

**Algorithm**  
1. Sort `nums`.  
2. Run a depth‑first search that builds `path`.  
3. Maintain a `used` boolean array to know which indices are already in `path`.  
4. Before recursing, skip an index `i` if:  
   - `used[i]` is `True` (already taken), **or**  
   - `i > 0`, `nums[i] == nums[i-1]`, and `used[i-1]` is `False` (duplicate pruning).  
5. When `path` length equals `len(nums)`, add a copy to the answer.

**Implementation**

```python
from typing import List

class Solution:
    def permuteUnique(self, nums: List[int]) -> List[List[int]]:
        nums.sort()                                 # bring duplicates together
        n = len(nums)
        used = [False] * n
        path: List[int] = []
        ans: List[List[int]] = []

        def backtrack():
            if len(path) == n:
                ans.append(path.copy())
                return
            for i in range(n):
                if used[i]:
                    continue
                # duplicate pruning
                if i > 0 and nums[i] == nums[i-1] and not used[i-1]:
                    continue
                used[i] = True
                path.append(nums[i])

                backtrack()

                path.pop()
                used[i] = False

        backtrack()
        return ans
```

**Complexity Analysis**  
- Time complexity: O(k * n) where k is the number of *unique* permutations (≤ n!). The pruning avoids exploring duplicate branches, so we only spend O(n) work per valid permutation.  
- Space complexity: O(n) for the recursion stack + `used` + `path`; output space not counted.

### Approach 3: Backtracking with Frequency Counter (Alternative)

**Intuition**  
Instead of sorting and checking adjacency, we can count how many times each distinct number appears. At each recursion level we iterate over the *unique* numbers, use one if its count > 0, decrement the count, go deeper, then restore the count. This naturally avoids duplicates because we never consider the same value more than its available count at a given depth.

**Algorithm**  
1. Build a `Counter` (hash map) of `nums`.  
2. Recursively build `path`: for each distinct number with remaining count > 0, append it, decrement its count, recurse, then backtrack (restore count and pop).  
3. When `path` length equals original length, store a copy.

**Implementation**

```python
from typing import List
from collections import Counter

class Solution:
    def permuteUnique(self, nums: List[int]) -> List[List[int]]:
        counter = Counter(nums)
        n = len(nums)
        path: List[int] = []
        ans: List[List[int]] = []

        def backtrack():
            if len(path) == n:
                ans.append(path.copy())
                return
            for num in list(counter.keys()):      # list() to allow mutation during iteration
                if counter[num] == 0:
                    continue
                # choose num
                counter[num] -= 1
                path.append(num)

                backtrack()

                # undo choice
                path.pop()
                counter[num] += 1

        backtrack()
        return ans
```

**Complexity Analysis**  
- Time complexity: O(k * n) where k is the number of unique permutations (same reasoning as Approach 2).  
- Space complexity: O(n) for recursion stack + `path` + counter (at most O(distinct numbers) ≤ n).

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace Approach 2 on the input `nums = [1,1,2]` (sorted → `[1,1,2]`).

| Step | path            | used (index) | Action taken (i)                                   | Reason                                            |
|------|-----------------|--------------|----------------------------------------------------|---------------------------------------------------|
| 0    | []              | [F,F,F]      | i=0 → choose first `1`                             | not used, no duplicate issue                     |
| 1    | [1]             | [T,F,F]      | i=0 skipped (used)                                 |                                                   |
|      |                 |              | i=1 → `nums[1]==1` & `used[0]==True` → allowed    | duplicate allowed because previous identical used |
| 2    | [1,1]           | [T,T,F]      | i=0,1 skipped (used)                               |                                                   |
|      |                 |              | i=2 → choose `2`                                   |                                                   |
| 3    | [1,1,2]         | [T,T,T]      | length = 3 → store `[1,1,2]`                       |                                                   |
| 4    | backtrack: pop 2| [T,T,F]      |                                                   |                                                   |
| 5    | backtrack: pop second 1 | [T,F,F] | i=1 now sees `used[0]==True` → still allowed   |                                                   |
| …    | …               | …            | …                                                  | Continue exploring other branches …              |

The table shows how the algorithm builds the first permutation `[1,1,2]`. Similar traces generate `[1,2,1]` and `[2,1,1]`; duplicate branches such as picking the second `1` before the first at the same depth are pruned by the condition `i>0 and nums[i]==nums[i-1] and not used[i-1]`.

--- 

*End of lecture.*