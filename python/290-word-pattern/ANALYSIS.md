# Arrays & Hashing

## Video Solution

For more details about **Word Pattern**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=W_akoecmCbM).

## Concept

The problem asks whether a string `s` follows a given `pattern` where each character in the pattern must map to exactly one word and each word must map back to exactly one character. This is a classic *bijection* (one‑to‑one and onto) mapping problem.  
We can think of it as a two‑way dictionary:  

* `char → word` ensures that the same pattern character never maps to two different words.  
* `word → char` ensures that two different pattern characters never map to the same word.

If both mappings stay consistent while we scan the pattern and the words in parallel, the string follows the pattern.

## When to Use It

Use this technique when you see:

* A pattern (string of characters) and a list of tokens (words, numbers, etc.) that must correspond one‑to‑one.  
* Statements like “each X maps to exactly one Y” and vice‑versa.  
* Problems that require checking a bijective relationship between two sets of elements.

## Template

```python
def bijection_check(pattern, tokens):
    """
    Returns True if there is a one‑to‑one mapping between pattern characters
    and tokens, otherwise False.
    """
    if len(pattern) != len(tokens):
        return False

    char_to_token = {}
    token_to_char = {}

    for ch, tok in zip(pattern, tokens):
        # check forward mapping
        if ch in char_to_token:
            if char_to_token[ch] != tok:
                return False
        else:
            char_to_token[ch] = tok

        # check reverse mapping
        if tok in token_to_char:
            if token_to_char[tok] != ch:
                return False
        else:
            token_to_char[tok] = ch

    return True
```

## LeetCode Problem Walkthrough

### Problem: 290. Word Pattern

https://leetcode.com/problems/word-pattern/

### Approach 1: Brute Force (Check All Pairs)

**Algorithm**  
1. Split `s` into a list of words.  
2. If the number of words differs from the length of `pattern`, return `False`.  
3. For every pair of indices `i < j`:  
   * If `pattern[i] == pattern[j]` then `words[i]` must equal `words[j]`.  
   * If `pattern[i] != pattern[j]` then `words[i]` must not equal `words[j]`.  
4. If all checks pass, return `True`.

**Implementation**

```python
class Solution:
    def wordPattern(self, pattern: str, s: str) -> bool:
        words = s.split()
        if len(words) != len(pattern):
            return False

        for i in range(len(pattern)):
            for j in range(i + 1, len(pattern)):
                same_pat = pattern[i] == pattern[j]
                same_word = words[i] == words[j]
                if same_pat != same_word:      # XOR – they must match or differ together
                    return False
        return True
```

**Complexity Analysis**

- Time complexity: O(n²) — double loop over the `n` characters/words.  
- Space complexity: O(n) — for the list of words (the split).

---

### Approach 2: One HashMap + Set (Intermediate Optimization)

**Intuition**  
We can store the forward mapping (`char → word`) in a hashmap.  
To guarantee that two different characters do not map to the same word we keep a `set` of words that have already been assigned.  
When we see a new character we check that its intended word is not already used; when we see an existing character we verify consistency.

**Algorithm**  
1. Split `s` into words; length check.  
2. Initialise empty hashmap `c2w` and empty set `used`.  
3. Iterate over `pattern` and `words` together:  
   * If the character is not in `c2w`:  
        - If its word is already in `used`, return `False` (two chars would map to same word).  
        - Otherwise record the mapping and add the word to `used`.  
   * If the character is already in `c2w`:  
        - Verify that the stored word equals the current word; otherwise return `False`.  
4. If loop finishes, return `True`.

**Implementation**

```python
class Solution:
    def wordPattern(self, pattern: str, s: str) -> bool:
        words = s.split()
        if len(words) != len(pattern):
            return False

        c2w = {}          # char -> word
        used = set()      # words already assigned to some char

        for ch, w in zip(pattern, words):
            if ch not in c2w:
                if w in used:          # another char already uses this word
                    return False
                c2w[ch] = w
                used.add(w)
            else:
                if c2w[ch] != w:       # inconsistent mapping
                    return False
        return True
```

**Complexity Analysis**

- Time complexity: O(n) — single pass over the pattern/words.  
- Space complexity: O(n) — hashmap and set may store up to n entries.

---

### Approach 3: Two HashMaps (Most Optimal)

**Intuition**  
Instead of a set we can store the reverse mapping (`word → char`) in a second hashmap.  
This makes the bijection check symmetric and eliminates the need for a separate `used` set, while still giving O(1) look‑ups for both directions.

**Algorithm**  
1. Split `s` into words; length check.  
2. Initialise two empty hashmaps: `c2w` and `w2c`.  
3. For each `(ch, w)` pair:  
   * If `ch` is in `c2w` but `c2w[ch] != w`, return `False`.  
   * If `w` is in `w2c` but `w2c[w] != ch`, return `False`.  
   * Otherwise insert both mappings.  
4. Return `True` after the loop.

**Implementation**

```python
class Solution:
    def wordPattern(self, pattern: str, s: str) -> bool:
        words = s.split()
        if len(words) != len(pattern):
            return False

        c2w = {}   # char -> word
        w2c = {}   # word -> char

        for ch, w in zip(pattern, words):
            # forward direction
            if ch in c2w:
                if c2w[ch] != w:
                    return False
            else:
                c2w[ch] = w

            # reverse direction
            if w in w2c:
                if w2c[w] != ch:
                    return False
            else:
                w2c[w] = ch

        return True
```

**Complexity Analysis**

- Time complexity: O(n) — one linear pass.  
- Space complexity: O(n) — two hashmaps storing at most n entries each.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: `pattern = "abba"`, `s = "dog cat cat dog"`  
Words after split: `["dog", "cat", "cat", "dog"]`

| Step | ch | w   | c2w (char→word)          | w2c (word→char)      | Action                                 |
|------|----|-----|--------------------------|----------------------|----------------------------------------|
| 0    | a  | dog | {}                       | {}                   | a→dog, dog→a                           |
| 1    | b  | cat | {a:dog}                  | {dog:a}              | b→cat, cat→b                           |
| 2    | b  | cat | {a:dog, b:cat}           | {dog:a, cat:b}       | b already maps to cat (ok), cat→b (ok)|
| 3    | a  | dog | {a:dog, b:cat}           | {dog:a, cat:b}       | a maps to dog (ok), dog→a (ok)        |

All checks pass → return `True`.
