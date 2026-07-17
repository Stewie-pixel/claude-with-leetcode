# Trie

## Video Solution

For more details about **Longest Word in Dictionary**, watch the walkthrough at [https://www.youtube.com/watch?v=waIgDZZp9Wc](https://www.youtube.com/watch?v=waIgDZZp9Wc)

## Concept

A **Trie** (pronounced “try”) is a tree‑like data structure that stores strings with common prefixes sharing nodes.  
Each edge represents a single character, and a node marks the end of a word if a flag is set.  
Because every prefix of a word corresponds to a path from the root, we can quickly verify whether *all* prefixes of a candidate word exist in the dictionary by simply walking the trie and checking the end‑of‑word flag at each step.

**Real‑world analogy** – Think of a phone’s autocomplete suggestion box. As you type letters, the box shows only those words that share the typed prefix; the underlying structure that makes this instantaneous is a trie.

## When to Use It

Use a trie when you see:
- Queries that ask whether **all prefixes** of a string are present in a set of strings.
- Problems involving **dictionary word building**, **auto‑complete**, **longest valid word**, or **replace‑with‑root**.
- Input size moderate (≤ 10⁵ total characters) where building the trie is O(total characters) and lookups are O(length of word).

## Template

```python
class TrieNode:
    def __init__(self):
        self.children = {}          # char -> TrieNode
        self.is_end = False         # marks a complete word

class Trie:
    def __init__(self):
        self.root = TrieNode()

    def insert(self, word: str) -> None:
        node = self.root
        for ch in word:
            if ch not in node.children:
                node.children[ch] = TrieNode()
            node = node.children[ch]
        node.is_end = True

    def all_prefixes_exist(self, word: str) -> bool:
        """Return True if every prefix of `word` ends at a node with is_end == True."""
        node = self.root
        for ch in word:
            if ch not in node.children:
                return False
            node = node.children[ch]
            if not node.is_end:          # prefix missing as a full word
                return False
        return True
```

---

## LeetCode Problem Walkthrough

### Problem: 720. Longest Word in Dictionary
https://leetcode.com/problems/longest-word-in-dictionary/

---

### Approach 1: Brute Force – Check Every Prefix

**Algorithm**
1. Put all words into a hash set for O(1) lookup.
2. For each word, iterate over its prefixes (`word[:i]` for i = 1 … len(word)).
3. If every prefix exists in the set, the word is a candidate.
4. Keep the longest candidate; break ties by lexicographical order.

**Implementation**

```python
class Solution:
    def longestWord(self, words: List[str]) -> str:
        word_set = set(words)
        best = ""

        for w in words:
            # quick skip if cannot beat current best
            if len(w) < len(best) or (len(w) == len(best) and w > best):
                continue

            # verify all prefixes
            ok = True
            for i in range(1, len(w) + 1):
                if w[:i] not in word_set:
                    ok = False
                    break
            if ok:
                best = w

        return best
```

**Complexity Analysis**
- Time complexity: O(N * L²) — for each of N words we check up to L prefixes, each prefix creation is O(L) in the worst case (string slicing).  
- Space complexity: O(N * L) — the hash set stores all words.

---

### Approach 2: Sorting + Hash Set (Prefix‑Built‑Incrementally)

**Intuition**
If we process words in **lexicographical order**, any valid prefix of a word will have been seen earlier (because it is shorter or equal and lexicographically not greater). Thus we can gradually build a set of “valid” words: a word is valid iff its prefix without the last character is already valid.

**Algorithm**
1. Sort `words` lexicographically.
2. Initialise a set `valid` with the empty string (base case).
3. Iterate over sorted words:
   - If `word[:-1]` is in `valid`, the word can be built; add it to `valid`.
   - Track the longest word seen so far (update when longer, or same length but lexicographically smaller – the sort already guarantees the latter).
4. Return the tracked best word.

**Implementation**

```python
class Solution:
    def longestWord(self, words: List[str]) -> str:
        words.sort()                           # lexicographic order
        valid = {""}                           # empty string is trivially valid
        best = ""

        for w in words:
            if w[:-1] in valid:                # all prefixes exist
                valid.add(w)
                if len(w) > len(best):         # longer -> better
                    best = w
        return best
```

**Complexity Analysis**
- Time complexity: O(N log N + N * L) — sorting dominates; each word’s prefix check is O(L) for the slice (constant‑time in Python due to hashing of the slice, but we can treat it as O(L)).  
- Space complexity: O(N * L) — the set stores all words.

---

### Approach 3: Trie – Prefix Validation in O(L)

**Intuition**
By inserting every word into a trie, we can verify that **all prefixes** of a word exist by walking from the root and ensuring each visited node marks the end of a word. This avoids repeatedly creating substrings and lets us reuse the prefix checks for many words.

**Algorithm**
1. Build a trie from all words, marking `is_end=True` at each terminal node.
2. For each word, traverse the trie:
   - At each character, move to the child node.
   - If the node does **not** have `is_end=True`, the prefix is missing → word invalid.
3. If we finish the traversal with all prefixes valid, the word is a candidate.
4. Keep the longest (lexicographically smallest on tie) candidate.

**Implementation**

```python
class Solution:
    def longestWord(self, words: List[str]) -> str:
        # -------- Trie construction ----------
        class TrieNode:
            __slots__ = ("children", "is_end")
            def __init__(self):
                self.children = {}
                self.is_end = False

        root = TrieNode()
        for w in words:
            node = root
            for ch in w:
                if ch not in node.children:
                    node.children[ch] = TrieNode()
                node = node.children[ch]
            node.is_end = True

        # -------- Helper to test a word ----------
        def all_prefixes(word: str) -> bool:
            node = root
            for ch in word:
                if ch not in node.children:
                    return False
                node = node.children[ch]
                if not node.is_end:
                    return False
            return True

        # -------- Find best word ----------
        best = ""
        for w in words:
            if all_prefixes(w):
                if len(w) > len(best) or (len(w) == len(best) and w < best):
                    best = w
        return best
```

**Complexity Analysis**
- Time complexity: O(N * L) — inserting all words (total characters) + checking each word once.  
- Space complexity: O(N * L) — the trie stores each character as a node.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run **Approach 2 (Sorting + Hash Set)** on the first example.

**Input:** `words = ["w","wo","wor","worl","world"]`

After sorting (already sorted): `["w","wo","wor","worl","world"]`

| Step | word | prefix (`word[:-1]`) | `prefix in valid?` | Action                           | `valid` set after step | best so far |
|------|------|----------------------|--------------------|----------------------------------|------------------------|-------------|
| 1    | "w"  | ""                   | True (base)        | add "w"                          | {"", "w"}              | "w"         |
| 2    | "wo" | "w"                  | True               | add "wo"                         | {"", "w", "wo"}        | "wo"        |
| 3    | "wor"| "wo"                 | True               | add "wor"                        | {"", "w", "wo", "wor"} | "wor"       |
| 4    | "worl"| "wor"                | True               | add "worl"                       | {"", "w", "wo", "wor", "worl"} | "worl" |
| 5    | "world"| "worl"               | True               | add "world"                      | {"", "w", "wo", "wor", "worl", "world"} | "world" |

Result: `"world"` – matches expected output.

A second dry‑run on Example 2 (`["a","banana","app","appl","ap","apply","apple"]`) shows how the algorithm picks `"apple"` over `"apply"` because both are valid and same length, but `"apple"` is lexicographically smaller (encountered first after sorting).
