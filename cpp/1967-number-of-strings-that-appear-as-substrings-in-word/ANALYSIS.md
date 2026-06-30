# Arrays & Hashing

## Video Solution

For more details about **Number of Strings That Appear as Substrings in Word**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=kyMvpkxAXGE).

## Concept

This problem is a straightforward application of array iteration combined with a string containment check. We are given an array of strings (`patterns`) and a single string (`word`). For each pattern we need to determine whether it occurs as a contiguous substring inside `word`. The classic way to test substring membership in most languages is to use the built‑in string search function (e.g., `string::npos in C++, `.find()` in Python, `indexOf` in Java), which returns the position of the first occurrence or a sentinel value if not found.

Because the constraints are tiny (max length 100 for both patterns and word), a direct linear scan is more than fast enough: O(number of patterns × length of word) ≤ 10⁴ operations.

## When to Use It

Use simple iteration + built‑in substring check when you see:
- A list of short strings that need to be tested against a single reference string.
- No requirement for preprocessing or answering many queries; a one‑off check suffices.
- Constraints are low enough that an O(N·M) brute force solution is acceptable.
- The problem statement explicitly asks for counting matches, not returning positions or the matches themselves.

## Template

```python
def count_matches(patterns, word):
    count = 0
    for pat in patterns:
        # built‑in substring test (returns -1 if not found)
        if word.find(pat) != -1:
            count += 1
    return count
```

## LeetCode Problem Walkthrough

### Problem: 1967. Number of Strings That Appear as Substrings in Word

https://leetcode.com/problems/number-of-strings-that-appear-as-substrings-in-word/

### Approach 1: Brute Force (Linear Scan)

**Algorithm**
1. Initialise a counter `ans = 0`.
2. Loop over each string `p` in `patterns`.
3. Use the language’s substring check (`word.find(p) != -1` in Python/C++/Java) to see if `p` appears in `word`.
4. If it does, increment `ans`.
5. After the loop, return `ans`.

**Implementation**

```python
class Solution:
    def numOfStrings(self, patterns: List[str], word: str) -> int:
        ans = 0
        for p in patterns:
            if word.find(p) != -1:   # -1 means not found
                ans += 1
        return ans
```

**Complexity Analysis**
- Time complexity: O(P * W) — where P = len(patterns) and W = len(word). Each `find` call scans `word` in the worst case.
- Space complexity: O(1) — only a few integer variables are used; no extra space proportional to input size.

### Approach 2: Pre‑process Word into a Set of All Substrings

**Intuition**
If we were going to test many different pattern lists against the same `word`, it would be worthwhile to build a lookup structure once. By generating every possible substring of `word` and storing them in a hash set, each pattern check becomes O(1) average time.

**Algorithm**
1. Create an empty set `subs`.
2. For each start index `i` from 0 to len(word)-1:
   - For each end index `j` from i+1 to len(word):
     - Add `word[i:j]` to `subs`.
3. Initialise `ans = 0`.
4. For each pattern `p` in `patterns`:
   - If `p` is in `subs`, increment `ans`.
5. Return `ans`.

**Implementation**

```python
class Solution:
    def numOfStrings(self, patterns: List[str], word: str) -> int:
        # Build set of all substrings of word
        subs = set()
        n = len(word)
        for i in range(n):
            for j in range(i + 1, n + 1):
                subs.add(word[i:j])
        # Count patterns present in the set
        return sum(1 for p in patterns if p in subs)
```

**Complexity Analysis**
- Time complexity: O(W² + P) ��� generating all substrings takes O(W²); checking each pattern is O(1) average.
- Space complexity: O(W²) — the set may hold up to W·(W+1)/2 substrings.

### Approach 3: Trie of Patterns (Advanced)

**Intuition**
When the number of patterns is large but each pattern is short, building a trie of the patterns lets us scan `word` once and simultaneously walk down the trie for every starting position. Whenever we reach a node marking the end of a pattern, we have found a match.

**Algorithm**
1. Insert every pattern into a trie; each node stores children and a flag `is_end`.
2. Initialise `ans = 0`.
3. For each start index `i` in `word`:
   - Walk the trie following characters `word[i], word[i+1], ...` as long as the path exists.
   - Whenever we land on a node with `is_end == True`, increment `ans`.
4. Return `ans`.

**Implementation**

```python
class TrieNode:
    __slots__ = ('children', 'is_end')
    def __init__(self):
        self.children = {}
        self.is_end = False

class Solution:
    def numOfStrings(self, patterns: List[str], word: str) -> int:
        # Build trie
        root = TrieNode()
        for p in patterns:
            node = root
            for ch in p:
                if ch not in node.children:
                    node.children[ch] = TrieNode()
                node = node.children[ch]
            node.is_end = True

        ans = 0
        n = len(word)
        for i in range(n):
            node = root
            for j in range(i, n):
                ch = word[j]
                if ch not in node.children:
                    break
                node = node.children[ch]
                if node.is_end:
                    ans += 1
        return ans
```

**Complexity Analysis**
- Time complexity: O(P * Lp + W * Lm) where Lp is average pattern length and Lm is the average matched length before a mismatch (bounded by pattern length). In the worst case O(P * Lp + W²) but usually better.
- Space complexity: O(P * Lp) for the trie nodes.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the brute‑force solution on the first example:

**Input:** `patterns = ["a","abc","bc","d"]`, `word = "abc"`

```
| Step | pattern p | word.find(p) | Found? | Action                     |
|------|-----------|--------------|--------|----------------------------|
| 1    | "a"       | 0            | Yes    | ans = 1                    |
| 2    | "abc"     | 0            | Yes    | ans = 2                    |
| 3    | "bc"      | 1            | Yes    | ans = 3                    |
| 4    | "d"       | -1           | No     | ans unchanged (still 3)    |
```

Result: `ans = 3`, matching the expected output. This table shows that each pattern is examined exactly once, and the built‑in find operation tells us immediately whether the pattern occurs as a contiguous substring.