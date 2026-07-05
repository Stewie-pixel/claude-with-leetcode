# Two Pointers

## Video Solution

For more details about **Subsequence After One Replacement**, watch the walkthrough at [https://www.youtube.com/watch?v=DZ-S0UuOT0M](https://www.youtube.com/watch?v=DZ-S0UuOT0M)

## Concept

The two‑pointer technique walks through two sequences simultaneously, advancing pointers based on matches.  
When we are allowed to **change at most one character** in `s`, we can still use two pointers: we move through `t` and try to match characters of `s`. If a mismatch occurs we may “spend” our one replacement to treat the current characters as matching, then continue normally. If we ever need a second mismatch, the answer is false.

A real‑world analogy: imagine you are lining up two queues of people (the characters of `s` and `t`). You can let at most one person from the first queue wear a disguise so they look like the person in front of them in the second queue. After that disguise, the rest must line up exactly.

## When to Use It

Use two pointers when you see:
- Checking if one string is a subsequence of another.
- Problems that allow a limited number of mismatches/edits (e.g., “at most one change”, “k deletions”).
- Scanning two arrays/strings in order without backtracking.

## Template

```python
def two_pointer_check(s: str, t: str, max_edits: int = 0) -> bool:
    i = j = 0               # i -> s, j -> t
    edits_used = 0
    while i < len(s) and j < len(t):
        if s[i] == t[j]:
            i += 1
            j += 1
        else:
            if edits_used == max_edits:
                return False
            # use an edit: pretend s[i] matches t[j] (or skip t[j])
            edits_used += 1
            # For subsequence with replacement we can treat the mismatch as a match
            i += 1
            j += 1
    # If we finished s, it's a subsequence; remaining t characters are irrelevant
    return i == len(s)
```

---

## LeetCode Problem Walkthrough

### Problem: 2671. Subsequence After One Replacement  
https://leetcode.com/problems/subsequence-after-one-replacement/

### Approach 1: Brute Force – Brute Force – try every possible replacement

**Algorithm**
1. For each index `i` in `s` (including the option of “no replacement”), try replacing `s[i]` with every letter `'a'` … `'z'`.
2. After each replacement, check if the new string is a subsequence of `t` using the standard two‑pointer subsequence test.
3. If any trial succeeds, return `True`; otherwise return `False`.

**Implementation**

```python
class Solution:
    def canMakeSubsequence(self, s: str, t: str) -> bool:
        def is_subseq(x: str) -> bool:
            it = iter(t)
            return all(ch in it for ch in x)   # classic subsequence test

        # try “no change”
        if is_subseq(s):
            return True
        # try changing one position to any letter
        for i in range(len(s)):
            original = s[i]
            for c in map(chr, range(ord('a'), ord('z')+1)):
                if c == original:
                    continue
                if is_subseq(s[:i] + c + s[i+1:]):
                    return True
        return False
```

**Complexity Analysis**
- Time complexity: O(26 * n * (n + m)) – for each of n positions we try 26 letters and each subsequence check scans both strings.
- Space complexity: O(1) – only a few indices are used.

---

### Approach 2 – Prefix‑Suffix Matching (Optimized)

**Intuition**
If we know how many characters of `s` can be matched as a prefix of `t` (without any change) and similarly how many can be matched as a suffix, then a single replacement can “bridge” a gap between the prefix and suffix.  
We pre‑compute:
- `pre[i]` = number of characters of `s[0..i-1]` that can be matched in order in `t` (prefix length).
- `suf[i]` = number of characters of `s[i..]` that can be matched in order in `t` when scanning from the end (suffix length).

If for any split point `i` we have `pre[i] + suf[i+1] >= len(s)`, then the characters before `i` and after `i` already cover the whole string, meaning the single character at `i` can be changed to whatever is needed.

**Implementation**

```python
class Solution:
    def canMakeSubsequence(self, s: str, t: str) -> bool:
        n, m = len(s), len(t)

        # prefix matches: pre[i] = how many of s[0:i] are matched in t
        pre = [0] * (n + 1)
        j = 0
        for i in range(n):
            while j < m and t[j] != s[i]:
                j += 1
            if j < m:
                j += 1
                pre[i + 1] = j          # we have matched i+1 chars
            else:
                pre[i + 1] = m + 1      # impossible to match further

        # suffix matches: suf[i] = how many of s[i:] are matched when scanning t from right
        suf = [0] * (n + 1)
        j = m - 1
        for i in range(n - 1, -1, -1):
            while j >= 0 and t[j] != s[i]:
                j -= 1
            if j >= 0:
                suf[i] = j              # position in t where s[i] matches
                j -= 1
            else:
                suf[i] = -1             # impossible to match this char from the right

        # If whole s matches without any change
        if pre[n] <= m:
            return True

        # Try to place the single replacement at each position i
        for i in range(n):
            # prefix part s[0:i] matched up to pre[i] (exclusive)
            # suffix part s[i+1:] matched from suf[i+1] onward
            if pre[i] > m:          # prefix already impossible, break
                break
            if suf[i + 1] < 0:      # suffix impossible, skip
                continue
            # If there is room between the prefix end and suffix start,
            # we can change s[i] to a character that fits in that gap.
            if pre[i] < suf[i + 1]:
                return True
        return False
```

**Complexity Analysis**
- Time complexity: O(n + m) – one forward pass for prefix, one backward pass for suffix, plus a linear scan.
- Space complexity: O(n) – two integer arrays of size n+1.

---

### Approach 3 – Two Pointers with One Allowed Mismatch (Most Optimal)

**Intuition**
While scanning `s` and `t` with two pointers, we are allowed to “skip” a mismatch at most once (treating it as if we replaced the character in `s`).  
We keep a flag `used` indicating whether we have already spent our replacement.  
- If characters match, advance both pointers.
- If they differ and we haven’t used the replacement, we pretend we replaced `s[i]` to match `t[j]` and advance both pointers while marking `used = True`.
- If they differ and we already used the replacement, we cannot proceed → return `False`.
At the end, if we have consumed all of `s`, the answer is true.

**Implementation**

```python
class Solution:
    def canMakeSubsequence(self, s: str, t: str) -> bool:
        i = j = 0
        n, m = len(s), len(t)
        used = False                     # have we already replaced a character?

        while i < n and j < m:
            if s[i] == t[j]:
                i += 1
                j += 1
            else:
                if used:                 # second mismatch → impossible
                    return False
                # use the one allowed replacement: act as if s[i] == t[j]
                used = True
                i += 1
                j += 1

        # If we finished s, it's a subsequence (remaining t chars are irrelevant)
        return i == n
```

**Complexity Analysis**
- Time complexity: O(n + m) – each pointer moves forward at most once.
- Space complexity: O(1) – only a few integer/boolean variables.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimal two‑pointer algorithm on the example `s = "cat"`, `t = "chat"`.

| Step | i (s) | j (t) | s[i] | t[j] | equal? | used | Action |
|------|-------|-------|------|------|--------|------|--------|
| 0    | 0     | 0     | c    | c    | ✅     | F    | match → i=1, j=1 |
| 1    | 1     | 1     | a    | h    | ❌     | F    | mismatch, use replacement → used=T, i=2, j=2 |
| 2    | 2     | 2     | t    | t    | ✅     | T    | match → i=3, j=3 |
| End  | 3     | 3     | —    | —    | —      | —    | i == len(s) → True |

The single replacement at position 1 (`a → h`) allows the rest of the characters to line up, so `"cht"` is a subsequence of `"chat"`.

---
