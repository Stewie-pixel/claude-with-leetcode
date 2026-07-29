# 1D Dynamic Programming

## Video Solution

For more details about **Word Break**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=Sx9NNgInc3A).

## Concept

Word Break asks whether a given string `s` can be split into a sequence of one or more words from a dictionary `wordDict`.  
Think of the string as a row of tiles. You want to know if you can place the dictionary words end‑to‑end to exactly cover the row.  
Dynamic programming works well because the decision for a prefix of the string only depends on earlier prefixes — once we know that `s[0..j]` can be segmented, we only need to check if the next chunk `s[j..i]` is a dictionary word.

## When to Use It

Use 1‑D DP (often called “prefix DP”) when you see:

- A string (or array) and a question about **segmenting / partitioning** it into valid pieces.
- The validity of a piece depends only on the substring itself (e.g., it appears in a set/dictionary).
- Overlapping subproblems: the same prefix is checked many times in a naive recursion.

## Template

```python
def word_break(s: str, wordDict: List[str]) -> bool:
    word_set = set(wordDict)          # O(1) look‑ups
    n = len(s)
    dp = [False] * (n + 1)            # dp[i] = can s[:i] be segmented?
    dp[0] = True                      # empty prefix is always valid

    for i in range(1, n + 1):         # consider prefix length i
        for j in range(i):            # try every possible cut before i
            if dp[j] and s[j:i] in word_set:
                dp[i] = True
                break                 # no need to check more j's
    return dp[n]
```

---

## LeetCode Problem Walkthrough

### Problem: 139. Word Break  
https://leetcode.com/problems/word-break/

### Approach 1: Brute Force (Recursion)

**Algorithm**  
Try every possible first word that matches a prefix of `s`. If the prefix is in the dictionary, recursively solve the suffix. Return `True` as soon as any recursion reaches the end of the string.

**Implementation**

```python
class Solution:
    def wordBreak(self, s: str, wordDict: List[str]) -> bool:
        word_set = set(wordDict)

        def can_break(start: int) -> bool:
            if start == len(s):
                return True
            # try every end position > start
            for end in range(start + 1, len(s) + 1):
                if s[start:end] in word_set and can_break(end):
                    return True
            return False

        return can_break(0)
```

**Complexity Analysis**

- Time complexity: O(2^n) — each position may spawn two recursive branches (take or skip), leading to exponential calls.
- Space complexity: O(n) — recursion stack depth at most `n`.

---

### Approach 2: Top‑Down DP with Memoization

**Intuition**  
The brute force solution recomputes the same suffix many times. By storing the result for each start index we turn the exponential recursion into a polynomial one.

**Algorithm**  
Same recursion as before, but memoize `can_break(start)`. If we have already computed the answer for a given `start`, reuse it.

**Implementation**

```python
class Solution:
    def wordBreak(self, s: str, wordDict: List[str]) -> bool:
        word_set = set(wordDict)
        memo = {}                         # start index -> bool

        def can_break(start: int) -> bool:
            if start == len(s):
                return True
            if start in memo:
                return memo[start]

            for end in range(start + 1, len(s) + 1):
                if s[start:end] in word_set and can_break(end):
                    memo[start] = True
                    return True

            memo[start] = False
            return False

        return can_break(0)
```

**Complexity Analysis**

- Time complexity: O(n^2 * w) — there are `n` start positions; for each we try up to `n` end positions, and substring extraction/comparison costs O(w) where w is average word length (bounded by 20). In practice we treat it as O(n^2).
- Space complexity: O(n) — memo table + recursion stack.

---

### Approach 3: Bottom‑Up DP (Iterative)

**Intuition**  
Instead of recursion, fill a table `dp[i]` meaning “can the prefix `s[:i]` be segmented?”.  
We build the answer from left to right: for each `i` we look back at every possible cut `j`. If `dp[j]` is true and `s[j:i]` is a dictionary word, then `dp[i]` becomes true.

**Algorithm**  
Initialize `dp[0] = True`. Iterate `i` from 1 to `n`. For each `i`, iterate `j` from 0 to `i-1`. If `dp[j]` and `s[j:i]` in word set, set `dp[i] = True` and break.

**Implementation**

```python
class Solution:
    def wordBreak(self, s: str, wordDict: List[str]) -> bool:
        word_set = set(wordDict)
        n = len(s)
        dp = [False] * (n + 1)
        dp[0] = True

        for i in range(1, n + 1):
            for j in range(i):
                if dp[j] and s[j:i] in word_set:
                    dp[i] = True
                    break   # no need to check other j's

        return dp[n]
```

**Complexity Analysis**

- Time complexity: O(n^2 * w) — two nested loops over the string length; substring check is O(w). With constraints (`n ≤ 300`) this is easily fast enough.
- Space complexity: O(n) — the DP array.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the bottom‑up DP on the example `s = "leetcode"`, `wordDict = ["leet","code"]`.

| i (prefix length) | j examined | s[j:i]   | dp[j] | dict? | dp[i] after check |
|-------------------|------------|----------|-------|-------|-------------------|
| 1                 | 0          | "l"      | T     | No    | F                 |
| 2                 | 0          | "le"     | T     | No    | F                 |
|                   | 1          | "e"      | F     | — | F                 |
| 3                 | 0          | "lee"    | T     | No    | F                 |
|                   | 1          | "ee"     | F     | — | F                 |
|                   | 2          | "e"      | F     | — | F                 |
| 4                 | 0          | "leet"   | T     | **Yes** | **T** (break)    |
| 5                 | 0          | "leetc"  | T     | No    | F                 |
|                   | 1          | "eetc"   | F     | — | F                 |
|                   | 2          | "etc"    | F     | — | F                 |
|                   | 3          | "tc"     | F     | — | F                 |
|                   | 4          | "c"      | T     | No    | F                 |
| 6                 | 0 … 5      | …        | …     | …     | F                 |
| 7                 | 0 … 6      | …        | …     | …     | F                 |
| 8                 | 0          | "leetcod"| T     | No    | F                 |
|                   | 1 … 3      | …        | …     | … | F                 |
|                   | 4          | "code"   | **T** (dp[4]) | **Yes** | **T** (break)    |

Final `dp[8] = True` → the string can be segmented.

--- 

**Summary**  
We explored three approaches: exponential brute force, memoized top‑down DP, and iterative bottom‑up DP. The bottom‑up version is the most common interview answer because it avoids recursion overhead and is easy to reason about. Always state the time/space complexity for each approach, and verify correctness with a dry‑run trace.