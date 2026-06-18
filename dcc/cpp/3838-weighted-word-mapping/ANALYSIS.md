# Arrays & Hashing

## Video Solution

For more details about **Weighted Word Mapping**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=OqZM_MNBS_Q).

## Concept

Arrays & Hashing is about using direct index‑based lookup (an array or hash table) to store and retrieve values in O(1) time.  
In this problem we have a fixed‑size array `weights[26]` that maps each letter `'a'`‑`'z'` to its weight.  
For each word we:

1. Look up the weight of every character via `weights[ch - 'a']`.  
2. Accumulate the sum.  
3. Reduce the sum modulo 26.  
4. Convert the remainder to a letter using **reverse alphabetical order**:  
   `0 → 'z', 1 → 'y', …, 25 → 'a'`, which can be computed as `chr(ord('z') - remainder)`.

The whole process is linear in the total number of characters.

## When to Use It

Use the Arrays & Hashing pattern when you see:

- A fixed mapping from a small set of items (e.g., letters, digits) to values.  
- Need to compute a sum, count, or aggregate based on that mapping.  
- The input size is modest enough that a simple linear scan is optimal.  
- No complex ordering or searching is required—just direct look‑ups.

## Template

```python
def map_items(strings, mapping):
    """
    Generic template:
    - strings: list of items (e.g., words) to process
    - mapping: array or dict giving a value for each possible element
    Returns a result built from per‑item aggregated values.
    """
    result = []
    for s in strings:
        total = 0
        for ch in s:                     # O(length of s)
            total += mapping[ord(ch) - base]   # O(1) lookup
        # post‑process total (e.g., modulo, threshold)
        processed = transform(total)    # problem‑specific
        result.append(processed)
    return ''.join(result) if isinstance(result[0], str) else result
```

Replace `base`, `transform`, and the return type to fit the specific problem.

## LeetCode Problem Walkthrough

### Problem: 3838. Weighted Word Mapping

https://leetcode.com/problems/weighted-word-mapping/

### Approach 1: Brute Force (Direct Loop)

**Algorithm**
- Initialise an empty answer list.  
- For each word in `words`:  
  1. Set `sum = 0`.  
  2. For each character `c` in the word, add `weights[ord(c) - ord('a')]` to `sum`.  
  3. Compute `rem = sum % 26`.  
  4. Map `rem` to a letter via `chr(ord('z') - rem)` and append to answer.  
- Join the answer list into a string and return it.

**Implementation**

```python
class Solution:
    def mapWordWeights(self, words: List[str], weights: List[int]) -> str:
        ans_chars = []
        for w in words:
            total = 0
            for ch in w:
                total += weights[ord(ch) - ord('a')]
            rem = total % 26
            mapped_char = chr(ord('z') - rem)   # 0->'z', 1->'y', ...
            ans_chars.append(mapped_char)
        return ''.join(ans_chars)
```

**Complexity Analysis**
- Time complexity: O(N * L) — we visit each character once, where N = number of words, L = average word length.  
- Space complexity: O(N) — we store one output character per word (the answer string).

### Approach 2: Using Python’s `sum` with Generator Expression

**Intuition**
The inner loop that accumulates the weight sum can be expressed more concisely with Python’s built‑in `sum` and a generator expression. This does not change the asymptotic complexity but improves readability and leverages optimized C loops.

**Algorithm**
- For each word, compute `total = sum(weights[ord(c) - ord('a')] for c in word)`.  
- Apply the same modulo and reverse‑alphabet mapping as before.  
- Collect the mapped characters and join them.

**Implementation**

```python
class Solution:
    def mapWordWeights(self, words: List[str], weights: List[int]) -> str:
        res = []
        for w in words:
            total = sum(weights[ord(c) - ord('a')] for c in w)
            res.append(chr(ord('z') - (total % 26)))
        return ''.join(res)
```

**Complexity Analysis**
- Time complexity: O(N * L) — each character is still processed once; the generator is evaluated in C but the work is identical.  
- Space complexity: O(N) — output storage only.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the first example:

**Input:** `words = ["abcd","def","xyz"]`,  
`weights = [5,3,12,14,1,2,3,2,10,6,6,9,7,8,7,10,8,9,6,9,9,8,3,7,7,2]`

#### Dry Run

| Step | Word | Char | weight[char] | Cumulative sum | sum % 26 | mapped char |
|------|------|------|--------------|----------------|----------|-------------|
| 1    | abcd | a    | 5            | 5              |          |             |
| 2    | abcd | b    | 3            | 8              |          |             |
| 3    | abcd | c    | 12           | 20             |          |             |
| 4    | abcd | d    | 14           | 34             | 34%26=8  | chr('z'-8) = 'r' |
| 5    | def  | d    | 14           | 14             |          |             |
| 6    | def  | e    | 1            | 15             |          |             |
| 7    | def  | f    | 2            | 17             | 17%26=17 | chr('z'-17) = 'i' |
| 8    | xyz  | x    | 7            | 7              |          |             |
| 9    | xyz  | y    | 7            | 14             |          |             |
|10    | xyz  | z    | 2            | 16             | 16%26=16 | chr('z'-16) = 'j' |

Result characters: `['r','i','j']` → `"rij"`.

---

## Summary

- The problem is a straightforward application of direct array look‑up (hashing) followed by a modular arithmetic mapping.  
- Both approaches run in linear time relative to the total input size and use only O(N) extra space for the answer.  
- Recognizing the fixed‑size letter‑to‑weight table is the key clue that the Arrays & Hashing pattern applies.  

**Next steps:** Practice similar problems that involve character‑frequency tables, letter scores, or simple aggregations (e.g., “Letter Tile Possibilities”, “Score of a String”, “Find Words That Can Be Formed by Characters”).