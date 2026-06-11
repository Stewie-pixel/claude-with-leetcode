# Backtracking

## Video Solution

For more details about **1415. The k-th Lexicographical String of All Happy Strings of Length n**, watch the walkthrough at [https://www.youtube.com/watch?v=tRwXzsXJArI](https://www.youtube.com/watch?v=tRwXzsXJArI)

## Concept

Backtracking is a systematic way to iterate through all possible configurations of a search space. We build candidates incrementally and abandon a candidate ("backtrack") as soon as we determine it cannot possibly lead to a valid solution.  
For happy strings we construct the string character‑by‑character, ensuring that no two adjacent characters are equal. At each step we try the letters `'a'`, `'b'`, `'c'` in lexicographic order, skip the letter that matches the previous character, and continue recursively until the length reaches `n`.

## When to Use It

Use backtracking when you see:
- A requirement to generate or count all objects that satisfy a constraint (e.g., no adjacent equal letters).
- The input size is small enough that exploring the exponential space is feasible (`n ≤ 10` here).
- You need the *k‑th* object in lexicographic order and can prune entire sub‑trees by counting how many objects they contain.

## Template

```python
def backtrack(path: List[str]) -> Optional[str]:
    # If we have built a complete happy string, update counter.
    if len(path) == n:
        nonlocal k
        k -= 1
        if k == 0:
            return "".join(path)
        return None   # keep looking

    # Try each possible next character in lexicographic order.
    for ch in ("a", "b", "c"):
        if path and ch == path[-1]:          # avoid equal adjacent chars
            continue
        path.append(ch)
        res = backtrack(path)
        if res is not None:                  # early exit when answer found
            return res
        path.pop()                           # backtrack
    return None
```

The driver simply calls `backtrack([])` and returns the result or `""` if `None`.

---

## LeetCode Problem Walkthrough

### Problem: 1415. The k-th Lexicographical String of All Happy Strings of Length n

https://leetcode.com/problems/the-k-th-lexicographical-string-of-all-happy-strings-of-length-n/

### Approach 1: Brute Force (Generate All)

**Intuition**  
Generate every happy string of length `n` using plain backtracking, collect them in a list (they are produced in lexicographic order because we iterate `'a' → 'b' → 'c'`), then index the list.

**Algorithm**
1. Run a depth‑first search that builds strings character by character, never placing the same character twice in a row.
2. Whenever the current string reaches length `n`, append it to a result list.
3. After the search, if the list has at least `k` elements, return `list[k‑1]`; otherwise return `""`.

**Implementation**

```python
class Solution:
    def getHappyString(self, n: int, k: int) -> str:
        res = []

        def dfs(curr: List[str]) -> None:
            if len(curr) == n:
                res.append("".join(curr))
                return
            for ch in ("a", "b", "c"):
                if curr and ch == curr[-1]:
                    continue
                curr.append(ch)
                dfs(curr)
                curr.pop()

        dfs([])
        return res[k - 1] if k <= len(res) else ""
```

**Complexity Analysis**

- Time complexity: O(3·2^{n‑1}) – we visit every happy string once.  
- Space complexity: O(3·2^{n‑1}) for the output list + O(n) recursion stack.

---

### Approach 2: Backtracking with Count‑Based Pruning

**Intuition**  
Instead of storing all strings, we can skip entire sub‑trees when we know they contain fewer than `k` strings. For a given prefix, the number of completions depends only on the remaining length: each remaining position has 2 choices (any letter except the previous one). Thus, a prefix of length `L` can be extended in `2^{n-L-1}` ways.

**Algorithm**
1. Iterate over positions `0 … n‑1`.
2. For each position, try `'a'`, `'b'`, `'c'` in order (skip the previous character).
3. Let `remaining = n - i - 1`. The number of strings that start with the current prefix + candidate ch is `block = 2^{remaining}`.
4. If `k > block`, the desired string is not in this block → subtract `block` from `k` and try the next candidate.
5. Otherwise, the desired string starts with this candidate → append it, set it as the new previous character, and move to the next position.
6. If we finish the loop, we have built the answer.

**Implementation**

```python
class Solution:
    def getHappyString(self, n: int, k: int) -> str:
        # total possible strings = 3 * 2^{n-1}
        total = 3 * (1 << (n - 1))
        if k > total:
            return ""

        ans = []
        prev = ""
        for i in range(n):
            for ch in ("a", "b", "c"):
                if ch == prev:
                    continue
                block = 1 << (n - i - 1)      # strings with this prefix
                if k > block:
                    k -= block                # skip this whole block
                else:
                    ans.append(ch)
                    prev = ch
                    break
        return "".join(ans)
```

**Complexity Analysis**

- Time complexity: O(n·Σ) where Σ = 3 (constant) → O(n).  
- Space complexity: O(n) for the answer string (output only).

---

### Approach 3: Direct Construction (Iterative Counting)

**Intuition**  
The same counting idea as Approach 2 can be written more compactly: at each step we compute how many strings start with each viable character, skip whole blocks when `k` is larger, and pick the character whose block contains the k‑th string. This yields an O(n) loop without explicit backtracking.

**Algorithm**  
Identical to Approach 2; presented here as a standalone iterative method.

**Implementation**

```python
class Solution:
    def getHappyString(self, n: int, k: int) -> str:
        total = 3 * (1 << (n - 1))
        if k > total:
            return ""

        res = []
        prev = ""
        for i in range(n):
            for c in ("a", "b", "c"):
                if c == prev:
                    continue
                cnt = 1 << (n - i - 1)   # number of strings with this prefix
                if k > cnt:
                    k -= cnt
                else:
                    res.append(c)
                    prev = c
                    break
        return "".join(res)
```

**Complexity Analysis**

- Time complexity: O(n) – constant work per position.  
- Space complexity: O(n) for the result.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the example `n = 3, k = 9`.

The total number of happy strings of length 3 is `3 * 2^{2} = 12`, so the answer exists.

We build the answer character by character.

| Step | i (pos) | prev | tried ch | block = 2^{n-i-1} | k before | Action                               | k after | chosen |
|------|---------|------|----------|-------------------|----------|--------------------------------------|---------|--------|
| 1    | 0       | ''   | a        | 2^{2}=4           | 9        | k > 4 → skip 'a*' block              | 9‑4=5   | –      |
|      |         |      | b        | 4                 | 5        | k > 4 → skip 'b*' block              | 5‑4=1   | –      |
|      |         |      | c        | 4                 | 1        | k ≤ 4 → pick 'c'                     | 1       | c      |
| 2    | 1       | c    | a        | 2^{1}=2           | 1        | k ≤ 2 → pick 'a'                     | 1       | a      |
| 3    | 2       | a    | b        | 2^{0}=1           | 1        | k ≤ 1 → pick 'b'                     | 1       | b      |
|      |         |      | (stop)   |                   |          | built "cab"                          |         |        |

The constructed string is **"cab"**, which matches the expected output.

---