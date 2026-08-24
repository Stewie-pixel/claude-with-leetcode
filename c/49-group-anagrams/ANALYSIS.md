# Arrays & Hashing

## Video Solution

For more details about **Group Anagrams**, watch the walkthrough at [https://www.youtube.com/watch?v=vzdNOK2oB2E](https://www.youtube.com/watch?v=vzdNOK2oB2E).

## Concept

An **anagram** is a word formed by rearranging the letters of another word, using all the original letters exactly once.  
Two strings are anagrams **iff** they have identical character frequency counts.  
Thus, we can group strings by a canonical representation of their frequencies — e.g., the sorted string or a 26‑element count tuple.

## When to Use It

Use the anagram‑grouping pattern when you see:
- Problems that ask to **group** or **collect** strings that are permutations of each other.
- Clues like “same characters”, “rearrange”, “anagram”, or “identical letter counts”.
- The need to compare strings based on **character frequency** rather than order.

## Template

```python
from collections import defaultdict
from typing import List

def group_anagrams(strs: List[str]) -> List[List[str]]:
    """
    Template for grouping anagrams using a hashmap.
    Replace `key_fn` with the desired canonical representation.
    """
    groups = defaultdict(list)          # key -> list of original strings
    for s in strs:
        key = key_fn(s)                 # <-- define this (sorted string or count tuple)
        groups[key].append(s)
    return list(groups.values())

# Example key functions:
# 1) Sorted string:      key_fn = lambda s: ''.join(sorted(s))
# 2) Character count:    key_fn = lambda s: tuple([s.count(chr(ord('a')+i)) for i in range(26)])
```

---

# LeetCode Problem Walkthrough

### Problem: 49. Group Anagrams  
https://leetcode.com/problems/group-anagrams/

Given an array of strings `strs`, group the anagrams together. You may return the groups in any order.

---

## Approach 1: Brute Force (Pairwise Comparison)

**Algorithm**
1. For each string, compare it with every later string to see if they are anagrams (by sorting or counting).
2. Use a `visited` boolean array to avoid adding a string to multiple groups.
3. When a match is found, add both strings to the current group and mark them visited.

**Implementation**

```python
from typing import List

def group_anagrams_bruteforce(strs: List[str]) -> List[List[str]]:
    n = len(strs)
    visited = [False] * n
    ans = []

    def are_anagrams(a: str, b: str) -> bool:
        return sorted(a) == sorted(b)          # O(m log m) where m = len(a)

    for i in range(n):
        if visited[i]:
            continue
        group = [strs[i]]
        visited[i] = True
        for j in range(i + 1, n):
            if not visited[j] and are_anagrams(strs[i], strs[j]):
                group.append(strs[j])
                visited[j] = True
        ans.append(group)
    return ans
```

**Complexity Analysis**
- Time: `O(n² * m log m)` – we compare each pair (`n²`) and sort each string (`m log m`).
- Space: `O(n * m)` for the output plus `O(n)` for the `visited` array.

---

## Approach 2: Sort‑String Key + Hashmap

**Intuition**
If two strings are anagrams, their sorted characters are identical.  
We can use the sorted string as a hashable key in a dictionary; all strings sharing the same key belong to one group.

**Algorithm**
1. Initialise an empty `defaultdict(list)`.
2. For each string `s`:
   - Compute `key = ''.join(sorted(s))`.
   - Append `s` to `groups[key]`.
3. Return the dictionary’s values as a list of lists.

**Implementation**

```python
from collections import defaultdict
from typing import List

def group_anagrams_sort(strs: List[str]) -> List[List[str]]:
    groups = defaultdict(list)
    for s in strs:
        key = ''.join(sorted(s))          # O(m log m)
        groups[key].append(s)
    return list(groups.values())
```

**Complexity Analysis**
- Time: `O(n * m log m)` – each of the `n` strings is sorted (`m log m`).
- Space: `O(n * m)` – we store all original strings in the hashmap.

---

## Approach 3: Character‑Count Tuple Key (Most Optimal)

**Intuition**
Sorting carries an `O(m log m)` factor. Since the alphabet is limited to 26 lowercase letters, we can represent each string by a fixed‑size 26‑int tuple of character frequencies. Two strings share the same tuple **iff** they are anagrams. Building the tuple is linear in the string length.

**Algorithm**
1. Create a `defaultdict(list)`.
2. For each string `s`:
   - Initialise a list `cnt = [0] * 26`.
   - Increment `cnt[ord(ch) - ord('a')]` for each character `ch` in `s`.
   - Convert `cnt` to a tuple (hashable) and use it as the key.
   - Append `s` to `groups[key]`.
3. Return `list(groups.values())`.

**Implementation**

```python
from collections import defaultdict
from typing import List

def group_anagrams_count(strs: List[str]) -> List[List[str]]:
    groups = defaultdict(list)
    for s in strs:
        cnt = [0] * 26
        for ch in s:
            cnt[ord(ch) - 97] += 1          # 97 == ord('a')
        groups[tuple(cnt)].append(s)
    return list(groups.values())
```

**Complexity Analysis**
- Time: `O(n * m)` – we scan each character once (`m` per string) and do constant‑size work for the 26‑length tuple.
- Space: `O(n * m)` for the output plus `O(n)` for the hashmap keys (each key is 26 integers).

---

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run **Approach 3** (character‑count key) on the example:

```
Input: strs = ["eat","tea","tan","ate","nat","bat"]
```

| Step | String | Count tuple (a..z)                     | Groups after processing                               |
|------|--------|----------------------------------------|-------------------------------------------------------|
| 1    | eat    | (1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0) | { (1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0): ["eat"] } |
| 2    | tea    | (1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0) | same key → ["eat","tea"]                              |
| 3    | tan    | (1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0) | new key → [..., (1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0): ["tan"]] |
| 4    | ate    | (1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0) | same as step 1 → ["eat","tea","ate"]                  |
| 5    | nat    | (1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0) | same as step 3 → ["tan","nat"]                        |
| 6    | bat    | (1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0) | new key → [..., (1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0): ["bat"]] |

**Final groups (values of the hashmap):**
- `[["eat","tea","ate"], ["tan","nat"], ["bat"]]`  
(Any order of groups or strings inside a group is acceptable.)

---

### Summary

- **Brute force** is simple but quadratic in the number of strings.
- **Sorting‑based hashmap** reduces the comparison to `O(m log m)` per string.
- **Character‑count tuple** achieves optimal linear time `O(n * m)` with modest extra space and is the preferred solution for interview settings.  

Pick the approach that best matches the constraints you face; for the given limits (`strs.length ≤ 10⁴`, each string ≤ 100 chars) the count‑tuple method is both fast and easy to implement.