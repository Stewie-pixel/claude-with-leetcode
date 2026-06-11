# Backtracking

## Video Solution

For more details about **Restore IP Addresses**, watch the walkthrough at [https://www.youtube.com/watch?v=61tN4YEdiTM](https://www.youtube.com/watch?v=61tN4YEdiTM)

## Concept

Backtracking is a depth‑first search that builds candidates incrementally and abandons a candidate (“backtracks”) as soon as it determines that the candidate cannot possibly lead to a valid solution.  
Think of it like navigating a maze: you walk forward step‑by‑step, marking your path. If you hit a dead end, you retreat to the last junction and try a different direction. In the IP‑address problem, each “step” corresponds to choosing the next segment (1‑3 digits) and placing a dot. If a segment is invalid (leading zero or >255) we stop exploring that branch immediately.

## When to Use It

Use backtracking when you see:

- A need to **enumerate all possibilities** that satisfy a set of constraints.
- The solution can be built **piece by piece**, and you can **detect invalid partial solutions early**.
- The input size is modest (typically ≤ 20) so exponential exploration is feasible with pruning.
- Phrases like “all possible”, “combinations”, “partitions”, or “restore” appear in the statement.

Typical problems: generating subsets, permutations, permutations with duplicates, IP address restoration, Sudoku solver, word search, N‑Queens, etc.

## Template

```python
def backtrack(state, choices):
    # If the current state is a complete solution, record it
    if is_complete(state):
        results.append(state.copy())
        return

    # Otherwise, try each possible next choice
    for choice in choices:
        if not is_valid(state, choice):   # prune invalid branches early
            continue
        state.append(choice)              # make choice
        backtrack(state, updated_choices) # recurse
        state.pop()                       # undo choice (backtrack)
```

Key points:
- `state` holds the partial solution built so far.
- `choices` represents the options available at the current recursion level.
- `is_valid` performs **pruning** to cut off branches that cannot lead to a solution.
- After recursion, we **undo** the choice to explore alternatives.

## LeetCode Problem Walkthrough

### Problem: 93. Restore IP Addresses

https://leetcode.com/problems/restore-ip-addresses/

---

### Approach 1: Brute Force – Try All Split Positions

**Algorithm**
An IPv4 address has exactly four parts. We can place three dots to split the string into four substrings.  
Iterate over all possible lengths (1‑3) for the first three parts; the fourth part gets the remaining characters.  
For each combination, validate each part (no leading zeros unless the part is exactly `"0"` and value ≤ 255).  
If all four parts are valid, join them with dots and add to the answer.

**Implementation**

```python
class Solution:
    def restoreIpAddresses(self, s: str) -> List[str]:
        n = len(s)
        res = []

        # i, j, k are the end indices (exclusive) of the first, second, third parts
        for i in range(1, min(4, n - 2)):          # first part length 1‑3
            for j in range(i + 1, i + min(4, n - i - 1)):  # second part
                for k in range(j + 1, j + min(4, n - j)):  # third part
                    # fourth part is s[k:]
                    parts = [s[:i], s[i:j], s[j:k], s[k:]]
                    if all(self._valid(part) for part in parts):
                        res.append(".".join(parts))
        return res

    def _valid(self, part: str) -> bool:
        # empty or too long
        if not part or len(part) > 3:
            return False
        # leading zero not allowed unless the part is exactly "0"
        if part[0] == '0' and len(part) > 1:
            return False
        # value must be ≤ 255
        return int(part) <= 255
```

**Complexity Analysis**

- Time complexity: O(1) — the three loops each run at most 3 times, so at most 3³ = 27 iterations; validation is O(1) per iteration.  
- Space complexity: O(1) besides the output list (we only use a few variables).

*Why this works:* The input length ≤ 20, so enumerating all possible split positions is tiny. This approach is easy to understand but does not scale if the number of segments grew.

---

### Approach 2: Backtracking (DFS) with Basic Pruning

**Intuition**
Instead of pre‑computing all split positions, we build the address segment by segment. At each step we choose a length of 1‑3 for the next part, check if it is still a valid IP segment, and recurse. When we have placed four segments we verify that we have consumed the whole string; otherwise we backtrack. This mirrors the template: the state is the list of chosen segments, and the choices are the possible next lengths.

**Algorithm**
1. Define a recursive helper `dfs(index, path)` where `index` is the current position in `s` and `path` holds the selected segments.
2. If `len(path) == 4`:
   - If `index == len(s)`, we have a valid IP → add `".".join(path)` to results.
   - Return (whether or not we consumed the string).
3. Otherwise, try lengths 1‑3:
   - If `index + length > len(s)`, break (cannot take more characters).
   - Extract `segment = s[index:index+length]`.
   - Skip if segment has a leading zero and length > 1.
   - Skip if integer value > 255.
   - Append segment to `path`, recurse with `index + length`, then pop (backtrack).

**Implementation**

```python
class Solution:
    def restoreIpAddresses(self, s: str) -> List[str]:
        res = []

        def dfs(index: int, path: List[str]) -> None:
            # Base case: we already have 4 parts
            if len(path) == 4:
                if index == len(s):
                    res.append(".".join(path))
                return

            # Try segment lengths 1 to 3
            for length in range(1, 4):
                if index + length > len(s):
                    break
                segment = s[index:index + length]

                # Prune: leading zero not allowed unless the segment is exactly "0"
                if segment[0] == '0' and len(segment) > 1:
                    continue
                # Prune: value must be ≤ 255
                if int(segment) > 255:
                    continue

                path.append(segment)
                dfs(index + length, path)
                path.pop()   # backtrack

        dfs(0, [])
        return res
```

**Complexity Analysis**

- Time complexity: O(1) — the recursion depth is at most 4, and each level branches ≤ 3 times, giving ≤ 3⁴ = 81 leaf nodes; each node does O(1) work.  
- Space complexity: O(1) auxiliary (recursion stack depth ≤ 4) plus output.

*Why this works:* The backtracking explores only those prefixes that could still become a valid IP, pruning invalid leading‑zero or >255 cases early. It is more intuitive and extends naturally to similar partition problems.

---

### Approach 3: Backtracking with Length‑Remaining Pruning (Optional Optimization)

**Intuition**
We can prune even earlier by checking whether the remaining characters can possibly fill the remaining segments.  
If we have already chosen `k` segments, we need `4‑k` more segments. Each segment needs at least 1 character and at most 3 characters.  
Thus, before recursing we verify:
```
min_needed = (4 - len(path)) * 1
max_allowed = (4 - len(path)) * 3
remaining = len(s) - index
```
If `remaining` is not within `[min_needed, max_allowed]`, the current branch cannot succeed and we backtrack immediately. This cuts off useless search paths, especially for longer strings.

**Algorithm**
Same as Approach 2, but at the start of `dfs` we add the length‑feasibility check and return early if it fails.

**Implementation**

```python
class Solution:
    def restoreIpAddresses(self, s: str) -> List[str]:
        res = []

        def dfs(index: int, path: List[str]) -> None:
            # Prune: remaining characters cannot satisfy the remaining segments
            segments_left = 4 - len(path)
            min_needed = segments_left * 1
            max_allowed = segments_left * 3
            remaining = len(s) - index
            if remaining < min_needed or remaining > max_allowed:
                return

            if len(path) == 4:
                if index == len(s):
                    res.append(".".join(path))
                return

            for length in range(1, 4):
                if index + length > len(s):
                    break
                segment = s[index:index + length]
                if segment[0] == '0' and len(segment) > 1:
                    continue
                if int(segment) > 255:
                    continue

                path.append(segment)
                dfs(index + length, path)
                path.pop()

        dfs(0, [])
        return res
```

**Complexity Analysis**

- Time complexity: O(1) — still bounded by a constant factor because the string length ≤ 20; the extra check only reduces constant work.  
- Space complexity: O(1) auxiliary.

*Why this works:* The length‑feasibility test eliminates branches where, even if every remaining segment were valid, we could not finish with exactly four parts. This is a common optimization in partition‑type backtracking problems.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the example `s = "25525511135"` (length 11).

#### Dry Run

We show the recursion tree with the current `index` and `path`.  
Only branches that survive the leading‑zero/≤255 checks are displayed.

```
Start: index=0, path=[]
  len=1 -> segment="2"
    index=1, path=["2"]
      len=1 -> segment="5"
        index=2, path=["2","5"]
          len=1 -> segment="5"
            index=3, path=["2","5","5"]
              len=1 -> segment="2"
                index=4, path=["2","5","5","2"]  -> 4 segments, index≠11 → dead end
              len=2 -> segment="25"
                index=5, path=["2","5","5","25"] → dead end
              len=3 -> segment="255"
                index=6, path=["2","5","5","255"] → dead end
          len=2 -> segment="51"
            index=4, path=["2","5","51"]
              len=1 -> segment="5"
                index=5, path=["2","5","51","5"] → dead end
              …
          len=3 -> segment="511"
            …
  len=2 -> segment="25"
    index=2, path=["25"]
      …
  len=3 -> segment="255"   ← first valid choice
    index=3, path=["255"]
      len=1 -> segment="2"
        index=4, path=["255","2"]
          …
      len=2 -> segment="25"
        index=5, path=["255","25"]
          …
      len=3 -> segment="255"
        index=6, path=["255","255"]
          len=1 -> segment="1"
            index=7, path=["255","255","1"]
              len=1 -> segment="1"
                index=8, path=["255","255","1","1"] → dead end (remaining 3 chars)
              len=2 -> segment="11"
                index=9, path=["255","255","1","11"] → dead end (remaining 2 chars)
              len=3 -> segment="111"
                index=10, path=["255","255","1","111"] → dead end (remaining 1 char)
          len=2 -> segment="11"
            index=8, path=["255","255","11"]
              len=1 -> segment="1"
                index=9, path=["255","255","11","1"] → dead end (remaining 2 chars)
              len=2 -> segment="13"
                index=10, path=["255","255","11","13"] → dead end (remaining 1 char)
              len=3 -> segment="135"
                index=11, path=["255","255","11","135"] → index==len(s) → VALID → "255.255.11.135"
          len=3 -> segment="111"
            index=9, path=["255","255","111"]
              len=1 -> segment="3"
                index=10, path=["255","255","111","3"] → dead end (remaining 1 char)
              len=2 -> segment="35"
                index=11, path=["255","255","111","35"] → VALID → "255.255.111.35"
              len=3 -> out of bounds
```

Only two leaves satisfy both conditions (four segments and exact consumption of the string), yielding the expected output:
```
["255.255.11.135", "255.255.111.35"]
```

The trace demonstrates how backtracking incrementally builds candidates, prunes invalid prefixes early, and backtracks to explore alternatives until all valid addresses are found.