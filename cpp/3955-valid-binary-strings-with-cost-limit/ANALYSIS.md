# Backtracking

## Video Solution

For more details about **Valid Binary Strings With Cost Limit**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=WZObOSm5L4k).

## Concept

Backtracking is a systematic way to iterate through all possible configurations of a search space. We build candidates incrementally and abandon a candidate (“backtrack”) as soon as we determine that it cannot possibly lead to a valid solution.  
A real‑world analogy is solving a maze: you walk forward, marking your path; if you hit a dead end, you retreat to the last junction and try a different direction.

## When to Use It

Use backtracking when you see:
- “Generate all …” or “List all …”  
- Constraints are small enough for exhaustive search (typically n ≤ 15 for binary strings).  
- You can prune large parts of the search space early (e.g., cost > k or consecutive 1’s).  
- The problem naturally fits a decision tree (choose 0 or 1 at each position).

## Template

```python
def backtrack(state, pos, ...):
    # 1️⃣ Prune invalid states early
    if not is_valid(state, pos, ...):
        return

    # 2️⃣ If we have a complete solution, record it
    if is_complete(state, pos, ...):
        add_to_result(state)
        return

    # 3️⃣ Explore each choice at the current position
    for choice in choices:
        make_choice(state, choice)          # e.g., set cur[pos] = choice
        backtrack(state, pos + 1, ...)      # recurse
        undo_choice(state, choice)          # backtrack (restore state)
```

## LeetCode Problem Walkthrough

### Problem: 3355. Valid Binary Strings With Cost Limit
https://leetcode.com/problems/valid-binary-strings-with-cost-limit/

---

### Approach 1: Brute Force (Generate‑All + Filter)

**Algorithm**  
1. Generate every binary string of length `n` (there are `2ⁿ` of them).  
2. For each string, check two conditions:  
   - No two consecutive `'1'`.  
   - Cost = Σ indices where `s[i] == '1'` ≤ `k`.  
3. Collect the strings that satisfy both checks.

**Implementation**

```python
class Solution:
    def validStrings(self, n: int, k: int) -> List[str]:
        res = []
        # iterate over all bit patterns from 0 to 2**n - 1
        for mask in range(1 << n):
            # build string from mask
            s = []
            cost = 0
            prev_one = False
            ok = True
            for i in range(n):
                bit = (mask >> i) & 1          # i‑th bit (LSB = position 0)
                if bit:
                    if prev_one:               # consecutive 1's → invalid
                        ok = False
                        break
                    cost += i                  # add index to cost
                    prev_one = True
                else:
                    prev_one = False
                s.append('1' if bit else '0')
            if ok and cost <= k:
                res.append(''.join(s))
        return res
```

**Complexity Analysis**  
- Time complexity: **O(2ⁿ · n)** – we examine each of the `2ⁿ` masks and spend O(n) to build the string and check constraints.  
- Space complexity: **O(n)** auxiliary (the temporary string) plus O(output) for the result list.

---

### Approach 2: Backtracking with Pruning (Preferred)

**Intuition**  
Instead of generating all `2ⁿ` strings, we construct the answer character by character.  
While building, we keep track of:
- `cost_so_far`: sum of indices where we placed a `'1'`.  
- `prev_one`: whether the previous character was `'1'` (to enforce the no‑consecutive‑1 rule).  

If at any point `cost_so_far > k` we can stop exploring that branch because adding more `'1'`s will only increase the cost. Similarly, we never place a `'1'` after a `'1'`.

**Algorithm**  
1. Start with an empty (all‑`'0'`) character array of length `n`.  
2. Recurse from position `pos = 0`.  
3. At each position:  
   - Always try placing `'0'` (does not affect cost or `prev_one`).  
   - Try placing `'1'` only if `prev_one` is `False`; update `cost_so_far += pos` and set `prev_one = True`.  
4. When `pos == n`, we have a complete valid string – add it to the result.  
5. Backtrack by undoing the choice (the array is overwritten in the next iteration).

**Implementation**

```python
class Solution:
    def validStrings(self, n: int, k: int) -> List[str]:
        res = []
        cur = ['0'] * n                     # mutable list for O(1) updates

        def backtrack(pos: int, cost: int, prev_one: bool) -> None:
            # prune if cost already exceeds limit
            if cost > k:
                return
            if pos == n:                     # reached end → valid string
                res.append(''.join(cur))
                return

            # Option 1: place '0'
            cur[pos] = '0'
            backtrack(pos + 1, cost, False)

            # Option 2: place '1' only if previous char wasn't '1'
            if not prev_one:
                cur[pos] = '1'
                backtrack(pos + 1, cost + pos, True)
                # no need to explicitly reset cur[pos]; it will be overwritten

        backtrack(0, 0, False)
        return res
```

**Complexity Analysis**  
- Time complexity: **O(V · n)** where `V` is the number of valid strings (each string is built in O(n)). In the worst case `V` ≤ `2ⁿ`, so the bound is O(2ⁿ · n) but pruning often reduces it drastically.  
- Space complexity: **O(n)** recursion depth + O(n) for the mutable array, plus O(output) for results.

---

### Approach 3: Top‑Down DP with Memoization (Generate‑Once)

**Intuition**  
The recursive backtracking above may recompute the same suffix many times when different prefixes lead to identical `(pos, cost, prev_one)` states. By memoizing the list of suffixes for each state, we generate each distinct suffix only once and then concatenate prefixes. This is especially useful when `n` and `k` are at their upper bounds (n = 12, k ≈ 66) and many paths converge.

**Algorithm**  
Define a function `dfs(pos, cost, prev_one)` that returns **all valid suffixes** from position `pos` to the end given the accumulated `cost` and whether the previous character was `'1'`.  
- Base case: `pos == n` → return `[\"\"]` (empty suffix).  
- If `cost > k` → return `[]` (no valid suffix).  
- Memoize results in a dictionary keyed by `(pos, cost, prev_one)`.  
- Recursively obtain suffixes for placing `'0'` and (if allowed) `'1'`, prepend the chosen character, and combine.

**Implementation**

```python
class Solution:
    def validStrings(self, n: int, k: int) -> List[str]:
        from functools import lru_cache

        @lru_cache(maxsize=None)
        def dfs(pos: int, cost: int, prev_one: bool) -> List[str]:
            if cost > k:
                return []
            if pos == n:
                return [""]                     # empty suffix

            suffixes = []

            # place '0'
            for suf in dfs(pos + 1, cost, False):
                suffixes.append('0' + suf)

            # place '1' if allowed
            if not prev_one:
                for suf in dfs(pos + 1, cost + pos, True):
                    suffixes.append('1' + suf)

            return suffixes

        return dfs(0, 0, False)
```

**Complexity Analysis**  
- Number of distinct states: `pos` ∈ [0, n] (n+1), `cost` ∈ [0, k] (k+1), `prev_one` ∈ {0, 1} → **O(n · k · 2)**.  
- Each state processes at most two transitions and concatenates characters, which is O(1) per generated suffix.  
- Time complexity: **O(n · k + output)** – the DP work plus the time to actually build the output strings.  
- Space complexity: **O(n · k)** for the memo table plus O(output) for the results.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the backtracking approach on the example `n = 3, k = 1`.

| Step | pos | cur (so far) | cost | prev_one | Action                                 |
|------|-----|--------------|------|----------|----------------------------------------|
| 1    | 0   | 0??          | 0    | 0        | place `'0'` → recurse                  |
| 2    | 1   | 00?          | 0    | 0        | place `'0'` → recurse                  |
| 3    | 2   | 000          | 0    | 0        | pos==n → add `"000"`                   |
| 4    | 2   | 00?          | 0    | 0        | backtrack, try `'1'` (allowed)         |
| 5    | 2   | 001          | 2    | 1        | cost>k → prune                         |
| 6    | 1   | 0?0          | 0    | 0        | backtrack from step1, try `'1'` at pos0|
| 7    | 1   | 01?          | 0    | 1        | place `'0'` (only option)              |
| 8    | 2   | 010          | 1    | 0        | pos==n → add `"010"`                   |
| 9    | 1   | 01?          | 0    | 1        | backtrack, cannot place `'1'` (prev_one)|
|10    | 0   | 1??          | 0    | 1        | from root, try `'1'` at pos0           |
|11    | 1   | 10?          | 0    | 0        | place `'0'`                            |
|12    | 2   | 100          | 0    | 0        | pos==n → add `"100"`                   |
|13    | 2   | 10?          | 0    | 0        | try `'1'` (allowed)                    |
|14    | 2   | 101          | 2    | 1        | cost>k → prune                         |

The collected strings are `["000", "010", "100"]`, matching the expected output.

---