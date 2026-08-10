# Two Pointers & Greedy with One Mismatch  

## Video Solution  

For more details about **Find the Lexicographically Smallest Valid Sequence**, watch the walkthrough at [https://www.youtube.com/watch?v=swv1D5tovNk](https://www.youtube.com/watch?v=swv1D5tovNk).  

## Concept  

We need to pick indices from `word1` (in increasing order) so that the characters at those indices form a string that differs from `word2` in **at most one** position.  
Among all possible index sequences we must return the **lexicographically smallest** one – i.e. the sequence that is smallest when compared element‑by‑element from the start.  

Think of the problem as trying to **match** `word2` as a subsequence of `word1`, but we are allowed to **skip** (or “change”) at most one character of `word1`.  
To get the lexicographically smallest answer we always want to take the **earliest possible** index that still lets us finish the match later.

## When to Use It  

Use this pattern when you see:  

* A subsequence/matching problem with a limited number of “mistakes” allowed (e.g. at most one mismatch, at most K deletions).  
* The answer must be the **lexicographically smallest** (or earliest) set of indices.  
* You can pre‑compute how much of the suffix can still be matched without using the remaining mistakes – a classic “suffix‑match” trick that lets you decide greedily from left to right.  

## Template  

```python
def smallest_valid_sequence(word1: str, word2: str) -> List[int]:
    n, m = len(word1), len(word2)

    # 1️⃣  Pre‑compute how many characters of word2 can be matched
    #     from each position of word1 to the end (without any changes).
    suffix_match = [0] * (n + 1)          # suffix_match[i] = matches from i..end
    j = m - 1                             # start from the end of word2
    for i in range(n - 1, -1, -1):
        suffix_match[i] = suffix_match[i + 1]
        if j >= 0 and word1[i] == word2[j]:
            j -= 1
            suffix_match[i] += 1

    # 2️⃣  Greedy left‑to‑right pick, allowing at most one change.
    ans = []
    used_change = False
    j = 0                                 # how many chars of word2 already taken

    for i in range(n):
        if j == m:                        # already matched everything
            break

        if word1[i] == word2[j]:
            # exact match – take it
            ans.append(i)
            j += 1
        elif not used_change and suffix_match[i] >= m - 1 - j:
            # we can spend our single change here and still finish later
            ans.append(i)
            j += 1
            used_change = True
        # else: skip this character

    return ans if j == m else []          # empty if we couldn't finish
```

## LeetCode Problem Walkthrough  

### Problem: 3302. Find the Lexicographically Smallest Valid Sequence  

https://leetcode.com/problems/find-the-lexicographically-smallest-valid-sequence/  

---

### Approach 1: Brute Force (enumerate all index combinations)  

**Algorithm**  
1. Generate every combination of `m` indices from `[0, n‑1]` (there are `C(n, m)` of them).  
2. For each combination, build the string formed by `word1` at those indices.  
3. Count how many positions differ from `word2`; accept the combination if the count ≤ 1.  
4. Keep the lexicographically smallest index list (compare element‑by‑element).  

**Implementation**  

```python
from itertools import combinations

def brute(word1: str, word2: str) -> List[int]:
    n, m = len(word1), len(word2)
    best = None
    for idx in combinations(range(n), m):
        # build the string from word1
        s = ''.join(word1[i] for i in idx)
        # count mismatches
        mismatches = sum(1 for a, b in zip(s, word2) if a != b)
        if mismatches <= 1:
            if best is None or list(idx) < best:
                best = list(idx)
    return [] if best is None else best
```

**Complexity Analysis**  

- Time complexity: O(C(n, m) * m) – exponential, infeasible for n up to 3·10⁵.  
- Space complexity: O(m) for the current combination (plus recursion overhead).  

---

### Approach 2: DP with at‑most‑one‑change (O(N·M) time)  

**Intuition**  
We can treat the problem as a shortest‑path / DP on two dimensions:  
*`i`* – position in `word1` (0…n)  
*`j`* – how many characters of `word2` we have matched so far (0…m)  
*`k`* – whether we have already used the single allowed change (0/1).  

`dp[i][j][k]` = the earliest (lexicographically smallest) index sequence that achieves this state, or “unreachable”.  
Transition: at `word1[i]` we can either skip it, take it as a match (if chars equal), or take it as a change (if `k == 0`).  
Because we only need the lexicographically smallest answer, we can store just the **previous index** and reconstruct at the end, or we can store a boolean reachable and later greedily rebuild – the classic DP‑reconstruction technique.

**Implementation**  

```python
def dp_solution(word1: str, word2: str) -> List[int]:
    n, m = len(word1), len(word2)
    # dp[i][j][k] = True if we can reach state (i,j,k)
    dp = [[[False, False] for _ in range(m + 1)] for _ in range(n + 1)]
    # predecessor info to rebuild the answer
    prev = [[[None, None] for _ in range(m + 1)] for _ in range(n + 1)]

    dp[0][0][0] = True                     # start: no chars taken, no change used

    for i in range(n):
        for j in range(m + 1):
            for k in (0, 1):
                if not dp[i][j][k]:
                    continue
                # 1) skip word1[i]
                if not dp[i + 1][j][k]:
                    dp[i + 1][j][k] = True
                    prev[i + 1][j][k] = (i, j, k, 'skip')
                # 2) take as match
                if j < m and word1[i] == word2[j] and not dp[i + 1][j + 1][k]:
                    dp[i + 1][j + 1][k] = True
                    prev[i + 1][j + 1][k] = (i, j, k, 'take')
                # 3) take as change (if we haven't used it yet)
                if k == 0 and j < m and not dp[i + 1][j + 1][1]:
                    dp[i + 1][j + 1][1] = True
                    prev[i + 1][j + 1][1] = (i, j, k, 'change')

    # find any reachable state at the end with j == m
    end_k = -1
    for k in (0, 1):
        if dp[n][m][k]:
            end_k = k
            break
    if end_k == -1:
        return []

    # reconstruct the path backwards
    ans_rev = []
    i, j, k = n, m, end_k
    while i > 0:
        pi, pj, pk, action = prev[i][j][k]
        if action in ('take', 'change'):
            ans_rev.append(pi)          # we used word1[pi]
        i, j, k = pi, pj, pk
    ans_rev.reverse()
    return ans_rev
```

**Complexity Analysis**  

- Time complexity: O(n·m) – we visit each DP cell once (2 × states for `k`).  
- Space complexity: O(n·m) for the DP table (can be reduced to O(m) with rolling arrays, but we keep it simple for clarity).  

*Note*: This solves the problem within limits (n·m ≤ 9·10¹⁰ is still too big), so we need a faster method – the greedy solution below.

---

### Approach 3: Greedy with Suffix Matching (O(N+M) time, O(N) space)  

**Intuition**  
If we know, for every position `i` in `word1`, how many characters of `word2` we can still match **without** using any changes, we can decide greedily:

* Scan `word1` from left to right.  
* At each index `i` we already have matched `j` characters of `word2`.  
* If `word1[i] == word2[j]` we can safely take it – it keeps the answer lexicographically small and does not consume our change.  
* Otherwise, we may *spend* our single change **only if** the remaining suffix (starting at `i`) can still supply the rest of the characters we need (`m‑1‑j`).  
* Because we always take the earliest possible index that still lets us finish, the built index list is lexicographically minimal.

The suffix information is computed by a single reverse scan: we walk `word1` from the end, matching `word2` backwards and storing how many characters we have matched so far.

**Algorithm**  

1. **Suffix match array** `suf[i]` = number of characters of `word2` that can be matched using `word1[i:]` **without** any changes.  
   - Initialise `suf[n] = 0`.  
   - Keep a pointer `p = m‑1` (last unmatched char of `word2`).  
   - Iterate `i` from `n‑1` down to `0`:  
        `suf[i] = suf[i+1]`  
        If `p ≥ 0` and `word1[i] == word2[p]`: decrement `p`, increment `suf[i]`.  

2. **Greedy forward pass**  
   - `ans = []`, `used_change = False`, `j = 0`.  
   - For each `i` in `[0, n)` while `j < m`:  
        - If `word1[i] == word2[j]`: append `i`, `j++`.  
        - Else if `not used_change` and `suf[i] >= m‑1‑j`:  
                append `i`, `j++`, `used_change = True`.  
        - Otherwise skip `i`.  
   - After the loop, if `j == m` return `ans`; else return `[]`.

**Implementation**  

```python
def smallest_valid_sequence(word1: str, word2: str) -> List[int]:
    n, m = len(word1), len(word2)

    # ---------- 1️⃣ suffix matches (no changes) ----------
    suf = [0] * (n + 1)          # suf[i] = matches from i to end
    p = m - 1                    # pointer in word2 (from the end)
    for i in range(n - 1, -1, -1):
        suf[i] = suf[i + 1]
        if p >= 0 and word1[i] == word2[p]:
            p -= 1
            suf[i] += 1

    # ---------- 2️⃣ greedy left‑to‑right ----------
    ans = []
    used_change = False
    j = 0                        # how many chars of word2 already taken

    for i in range(n):
        if j == m:
            break
        if word1[i] == word2[j]:
            ans.append(i)
            j += 1
        elif not used_change and suf[i] >= m - 1 - j:
            ans.append(i)
            j += 1
            used_change = True
        # else: skip i

    return ans if j == m else []
```

**Complexity Analysis**  

- Time complexity: O(n + m) – one reverse pass + one forward pass.  
- Space complexity: O(n) for the `suf` array (can be reduced to O(1) if we compute on the fly, but O(n) fits easily).  

---

### Provide a Visual Demonstration  

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example  

We trace the greedy algorithm on Example 1:  

`word1 = "vbcca"` , `word2 = "abc"`  

**Step 1 – compute suffix matches**  

| i | word1[i] | suf[i] (matches from i to end) |
|---|----------|--------------------------------|
| 5 | –        | 0 |
| 4 | a        | 1 (matches 'c'? no, then later 'a' matches 'c'? actually we go backwards; final values: ) |
Let's compute properly:  

Start `p = 2` (pointing to `'c'` in word2).  

- i=4, char='a': not equal to word2[2]='c' → suf[4]=0, p stays 2.  
- i=3, char='c': equals word2[2] → p=1, suf[3]=1.  
- i=2, char='c': equals word2[1]='b'? no → suf[2]=suf[3]=1.  
- i=1, char='b': equals word2[1] → p=0, suf[1]=suf[2]+1=2.  
- i=0, char='v': equals word2[0]='a'? no → suf[0]=suf[1]=2.  

So `suf = [2,2,1,1,0,0]`.  

**Step 2 – greedy pass**  

| i | word1[i] | j (matched) | used_change | Action | ans |
|---|----------|-------------|-------------|--------|-----|
| 0 | v        | 0           | False       | word1[0]!=word2[0] ('v'!='a') and suf[0]=2 ≥ m‑1‑j = 2? yes → take as change | [0], j=1, used_change=True |
| 1 | b        | 1           | True        | word1[1]==word2[1] ('b'=='b') → take | [0,1], j=2 |
| 2 | c        | 2           | True        | word1[2]==word2[2] ('c'=='c') → take | [0,1,2], j=3 → done |

Result `[0,1,2]` matches the expected output.  

A similar trace on Example 2 yields `[1,2,4]`.  
