# Arrays & Hashing

## Video Solution

For more details about **Keyboard Row**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=nDGtW1vF-QQ)

## Concept

The problem asks us to filter words that can be typed using letters from only one row of a QWERTY keyboard.  
Think of each keyboard row as a “bucket” of allowed letters. A word is valid if **all** its letters fall into the same bucket. Because the check is case‑insensitive, we can treat uppercase and lowercase letters identically by converting everything to lower case.

## When to Use It

Use this pattern when you see:
- A need to group items by a fixed, small set of categories (here: three keyboard rows).
- A requirement to verify that all members of an item belong to a single category.
- The categories are static and can be pre‑computed (e.g., lookup tables, bit masks, or sets).

## Template

```python
# Pre‑compute the three keyboard rows as sets for O(1) membership checks
ROW1 = set("qwertyuiop")
ROW2 = set("asdfghjkl")
ROW3 = set("zxcvbnm")

def solve(words):
    result = []
    for w in words:
        w_set = set(w.lower())
        # If the word’s letters are a subset of any one row, keep it
        if w_set <= ROW1 or w_set <= ROW2 or w_set <= ROW3:
            result.append(w)
    return result
```

---

## LeetCode Problem Walkthrough

### Problem: 500. Keyboard Row

https://leetcode.com/problems/keyboard-row/

### Approach 1: Brute Force – Row Sets with Length Check

**Algorithm**
1. For each word, convert it to lower case.
2. Check whether the word can be formed using only characters from the first row by verifying that the size of the union of the row set and the word’s character set equals the size of the row set (i.e., the word adds no new letters outside the row).  
   Repeat for the second and third rows.
3. If any row passes the test, add the original word to the answer list.

**Implementation**

```python
class Solution:
    def findWords(self, words):
        """
        :type words: List[str]
        :rtype: List[str]
        """
        l1 = "qwertyuiop"
        l2 = "asdfghjkl"
        l3 = "zxcvbnm"
        res = []
        for word in words:
            w = word.lower()
            # If adding the word's letters does not increase the row size,
            # all its letters belong to that row.
            if (len(set(l1 + w)) == len(l1) or
                len(set(l2 + w)) == len(l2) or
                len(set(l3 + w)) == len(l3)):
                res.append(word)
        return res
```

**Complexity Analysis**
- Time complexity: O(N * L) — we iterate over each word (N) and each character inside it (L) to build a set.
- Space complexity: O(1) — the three row strings and a few temporary sets of constant size (at most 26 letters).

---

### Approach 2: Intermediate Optimization – Character‑to‑Row Mapping

**Intuition**
Instead of rebuilding a set for each row on every word, we can assign each letter to its row once (e.g., via a dictionary). Then a word is valid if all its letters map to the same row index. This removes the repeated union‑size checks and makes the per‑word test a simple equality check.

**Algorithm**
1. Build a hash map `row_of[char]` that maps every lowercase letter to `0`, `1`, or `2` indicating its keyboard row.
2. For each word:
   - Convert to lower case.
   - Look up the row of the first character; this is the candidate row.
   - Verify that every subsequent character maps to the same row.
   - If the check passes, keep the original word.
3. Return the collected words.

**Implementation**

```python
class Solution:
    def findWords(self, words):
        # Map each letter to its row index: 0 -> top, 1 -> middle, 2 -> bottom
        row_of = {}
        for ch in "qwertyuiop":
            row_of[ch] = 0
        for ch in "asdfghjkl":
            row_of[ch] = 1
        for ch in "zxcvbnm":
            row_of[ch] = 2

        result = []
        for word in words:
            w = word.lower()
            if not w:          # edge case: empty string (not possible per constraints)
                continue
            target_row = row_of[w[0]]
            if all(row_of[ch] == target_row for ch in w):
                result.append(word)
        return result
```

**Complexity Analysis**
- Time complexity: O(N * L) — one pass to build the map (constant) + one pass over each character of each word.
- Space complexity: O(1) — the map holds 26 entries; the output list does not count toward extra space.

---

### Approach 3: Most Optimal – Early‑Exit with Set Subset Check

**Intuition**
The previous approach already is optimal in asymptotic terms, but we can make the constant factor smaller by using Python’s built‑in set operations: a word is valid if its character set is a subset of any one row’s set. This avoids an explicit loop over characters and leverages highly optimized C‑level set implementations.

**Algorithm**
1. Pre‑define three sets representing the keyboard rows.
2. For each word, create a set of its lower‑case characters.
3. Test `word_set <= row_set` for each of the three rows; if any is true, keep the word.
4. Return the filtered list.

**Implementation**

```python
class Solution:
    def findWords(self, words):
        row1 = set("qwertyuiop")
        row2 = set("asdfghjkl")
        row3 = set("zxcvbnm")

        ans = []
        for w in words:
            ws = set(w.lower())
            if ws <= row1 or ws <= row2 or ws <= row3:
                ans.append(w)
        return ans
```

**Complexity Analysis**
- Time complexity: O(N * L) — creating a set from each word costs O(L); subset checks are O(len(word_set)) ≤ O(L).
- Space complexity: O(1) — only three fixed‑size row sets and a temporary set per word (bounded by 26).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the sample input `["Hello","Alaska","Dad","Peace"]` using **Approach 3** (set‑subset).

#### Dry Run

| Word   | lower() | Char Set          | ⊆ Row1? | ⊆ Row2? | ⊆ Row3? | Keep? |
|--------|---------|-------------------|---------|---------|---------|-------|
| Hello  | hello   | {h,e,l,l,o} → {h,e,l,o} | False (contains h,l) | False (contains h) | False (contains h,e,l,o) | No |
| Alaska | alaska  | {a,l,a,s,k,a} → {a,l,s,k} | False (contains l,s,k) | True (all in asdfghjkl) | False | Yes |
| Dad    | dad     | {d,a,d} → {a,d}      | False (contains a,d) | True (a,d in asdfghjkl) | False | Yes |
| Peace  | peace   | {p,e,a,c,e} → {p,e,a,c} | False (contains p,e,c) | False (contains p,e,c) | False | No |

Result: `["Alaska","Dad"]`, matching the expected output.

---