# Arrays & Hashing

## Video Solution

For more details about **Longest Common Prefix**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=0sWShKIJoo4).

## Concept

The longest common prefix (LCP) of a set of strings is the longest string that appears at the **beginning** of every string.  
Think of a group of people lining up for a photo: you keep moving forward as long as everyone still shares the same letter at that position; the moment someone differs, you stop. The letters you have collected so far form the LCP.

## When to Use It

Use this technique when the problem statement includes any of the following clues:

- You are given an **array/list of strings**.
- You need to find a **common starting substring** (prefix) among them.
- The input size is modest (≤ 200 strings, each ≤ 200 characters) so an O(N · L) scan is fine.
- You are asked to return an empty string if no common prefix exists.

## Template

A reusable skeleton for solving LCP problems in Python:

```python
def longest_common_prefix(strs: List[str]) -> str:
    if not strs:                     # handle empty input
        return ""

    # ---- Choose one of the strategies below ----
    # 1. Horizontal scanning (reduce prefix)
    # 2. Vertical scanning (character‑by‑character)
    # 3. Divide‑and‑conquer / sorting
    # ------------------------------------------------

    return result
```

---

## LeetCode Problem Walkthrough

### Problem: 14. Longest Common Prefix
https://leetcode.com/problems/longest-common-prefix/

---

### Approach 1: Brute Force (Horizontal Scanning)

**Algorithm**  
1. Assume the whole first string is the current prefix.  
2. For each subsequent string, repeatedly shrink the prefix from the end until it matches the start of that string.  
3. If the prefix ever becomes empty, we can stop early – there is no common prefix.  

**Implementation**

```python
from typing import List

class Solution:
    def longestCommonPrefix(self, strs: List[str]) -> str:
        if not strs:
            return ""

        # start with the entire first string as the candidate prefix
        prefix = strs[0]

        for s in strs[1:]:
            # shrink prefix until it is a prefix of s
            while not s.startswith(prefix):
                prefix = prefix[:-1]          # drop last character
                if not prefix:                # no common prefix left
                    return ""
        return prefix
```

**Complexity Analysis**

- Time complexity: O(N · M) — where N is the number of strings and M is the length of the shortest string (each comparison may cut the prefix down character by character).  
- Space complexity: O(1) — only a few extra variables are used.

---

### Approach 2: Vertical Scanning (Optimized)

**Intuition**  
Instead of shrinking a candidate prefix, we can examine the strings **column‑wise**: check the character at position 0 in every string, then position 1, and so on. As soon as a mismatch appears, we know the prefix ends just before that column. This avoids repeatedly rebuilding the prefix and stops at the first disagreement.

**Algorithm**  
1. Use the first string as a reference.  
2. For each index `i` from 0 to len(first_string)-1:  
   - Take the character `c = first_string[i]`.  
   - Verify that every other string has the same character at position `i`.  
   - If any string is shorter than `i` or has a different character, return the prefix up to `i`.  
3. If the loop completes, the whole first string is the common prefix.

**Implementation**

```python
from typing import List

class Solution:
    def longestCommonPrefix(self, strs: List[str]) -> str:
        if not strs:
            return ""

        first = strs[0]
        for i, ch in enumerate(first):
            for other in strs[1:]:
                if i == len(other) or other[i] != ch:
                    return first[:i]          # mismatch found
        return first                         # all chars of first matched
```

**Complexity Analysis**

- Time complexity: O(N · M) — we compare each character of the first string with the same position in all other strings (N strings, M = length of the shortest string).  
- Space complexity: O(1) — only indices and a character are stored.

---

### Approach 3: Divide‑and‑Conquer (via Sorting)

**Intuition**  
If we sort the array lexicographically, the strings that are most dissimilar will appear at the ends of the sorted list. The longest common prefix of the entire array must therefore be a prefix of **both** the first and the last strings after sorting. Comparing just these two gives us the answer.

**Algorithm**  
1. Sort the list of strings.  
2. Take the first (`lo`) and last (`hi`) strings in the sorted order.  
3. Compare characters of `lo` and `hi` one by one until they differ or we reach the end of either string.  
4. Return the matching prefix.

**Implementation**

```python
from typing import List

class Solution:
    def longestCommonPrefix(self, strs: List[str]) -> str:
        if not strs:
            return ""

        strs.sort()                         # O(N log N * L) due to string comparison
        first, last = strs[0], strs[-1]

        i = 0
        while i < len(first) and i < len(last) and first[i] == last[i]:
            i += 1
        return first[:i]
```

**Complexity Analysis**

- Time complexity: O(N · L log N) — sorting dominates (string comparison costs O(L) where L is average length).  
- Space complexity: O(1) extra (or O(N) for the Timsort auxiliary space in Python, but we treat it as negligible for interview analysis).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the **vertical scanning** approach on the example `strs = ["flower","flow","flight"]`.

## Dry Run

Input: strs = ["flower","flow","flight"]

```
| Index i | ch from first | Check "flower"[i] | Check "flow"[i] | Check "flight"[i] | All match? | Prefix so far |
|---------|---------------|-------------------|-----------------|-------------------|------------|---------------|
| 0       | 'f'           | f                 | f               | f                 | Yes        | "f"           |
| 1       | 'l'           | l                 | l               | l                 | Yes        | "fl"          |
| 2       | 'o'           | o                 | o               | i                 | No         | (stop)        |
```

At index 2 the characters differ (`o` vs `i`), so we return the prefix accumulated up to index 1 → `"fl"`.

If we ran the same trace on `["dog","racecar","car"]`:

```
| Index i | ch from first | Check "dog"[i] | Check "racecar"[i] | Check "car"[i] | All match? |
|---------|---------------|----------------|--------------------|----------------|------------|
| 0       | 'd'           | d              | r                  | c              | No         |
```

Mismatch at the very first character → return `""`.

--- 

This completes the lecture for the **Arrays & Hashing** topic, covering the Longest Common Prefix problem with three progressively optimized approaches, full complexity analysis, and a step‑by‑step dry run.