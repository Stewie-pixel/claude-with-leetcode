# Backtracking

## Video Solution

For more details about **Smallest Palindromic Rearrangement II**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=dk8U3FyWurQ)

## Concept

Backtracking is a general algorithm for finding all (or some) solutions to a problem by incrementally building candidates and abandoning a candidate ("backtracking") as soon as it determines that the candidate cannot possibly be completed to a valid solution.  
Think of it like exploring a maze: you walk down a path, and if you hit a dead end you retreat to the last junction and try a different route.

In combinatorial problems (permutations, combinations, subsets, etc.) we often use backtracking to generate every possible arrangement, pruning branches that cannot lead to a distinct solution.

## When to Use It

Use backtracking when you see:
- A need to **enumerate** all possible configurations (e.g., permutations, subsets, combinations).
- The input size is small enough that exhaustive search is feasible **after pruning**.
- You are asked for the **k‑th** lexicographic element or to **count** valid configurations.
- The problem constraints hint at exponential possibilities but allow early termination (e.g., stop after finding k solutions).

## Template

```python
def backtrack(state, choices):
    # state: current partial solution
    # choices: remaining options to consider

    if is_valid_complete(state):
        # process a complete solution (e.g., store, count, compare to k)
        handle_solution(state)
        return

    for i, choice in enumerate(choices):
        # prune branches that cannot lead to a valid solution
        if not is_promising(state, choice):
            continue

        # choose
        state.append(choice)
        new_choices = choices[:i] + choices[i+1:]   # remove used choice (for permutations)

        # explore
        backtrack(state, new_choices)

        # un‑choose (backtrack)
        state.pop()
```

*Key points*  
- `state` holds the partial answer being built.  
- `choices` represents what can still be placed.  
- `is_promising` implements pruning (e.g., avoid duplicates, stop when we already have k answers).  
- After the recursive call we undo the choice (`pop`) to try the next alternative.

## LeetCode Problem Walkthrough

### Problem: 3518. Smallest Palindromic Rearrangement II
https://leetcode.com/problems/smallest-palindromic-rearrangement-ii/

We are given a palindrome string `s` and an integer `k`.  
We must return the **k‑th lexicographically smallest distinct palindrome** that can be formed by rearranging the characters of `s`. If fewer than `k` distinct palindromes exist, return `""`.

Because `s` is already a palindrome, any palindrome permutation is completely determined by its **first half** (the second half is the mirror).  
If `s` has odd length, the middle character stays fixed.

Thus the problem reduces to:  
*Find the k‑th lexicographically smallest distinct permutation of the multiset formed by the first ⌊n/2⌋ characters of `s`.*

---

### Approach 1: Brute‑Force Generate All Half‑Permutations

**Algorithm**
1. Count characters in the first half of `s`.  
2. Use backtracking to generate every distinct permutation of this half (taking duplicates into account).  
3. For each generated half, build the full palindrome: `half + middle + reverse(half)`.  
4. Collect all palindromes, sort them lexicographically, and pick the k‑th (1‑indexed).  
5. If the number of distinct palindromes `< k`, return `""`.

**Implementation**

```python
from collections import Counter
import itertools

class Solution:
    def smallestPalindrome(self, s: str, k: int) -> str:
        n = len(s)
        half_len = n // 2
        half_counter = Counter(s[:half_len])
        middle = s[half_len] if n % 2 else ""

        # generate distinct permutations of the half
        def gen_permutations(counter, path, length, res):
            if len(path) == length:
                res.append(''.join(path))
                return
            for ch in sorted(counter):          # iterate in lexicographic order
                if counter[ch] == 0:
                    continue
                counter[ch] -= 1
                path.append(ch)
                gen_permutations(counter, path, length, res)
                path.pop()
                counter[ch] += 1

        half_perms = []
        gen_permutations(half_counter, [], half_len, half_perms)

        # build full palindromes
        palindromes = [h + middle + h[::-1] for h in half_perms]
        palindromes.sort()                     # ensure lexicographic order

        return palindromes[k-1] if k <= len(palindromes) else ""
```

**Complexity Analysis**
- Let `m = n // 2`. The number of distinct half‑permutations is `m! / (∏ cnt_i!)`.  
- Generating each permutation costs `O(m)` (building the string).  
- **Time:** `O( number_of_half_permutations * m )` – factorial in the worst case.  
- **Space:** `O( number_of_half_permutations * m )` to store all results (plus recursion stack `O(m)`).

---

### Approach 2: Construct the k‑th Half Directly (Factorial Counting)

**Intuition**
Instead of enumerating all permutations, we can determine each character of the answer one by one using combinatorial counts.  
For a multiset, the number of distinct permutations that start with a given character `c` equals:

```
total_permutations * (count_of_c) / remaining_positions
```

where `total_permutations = (remaining_len)! / (∏ remaining_counts!)`.  
By iterating characters in lexicographic order and subtracting block sizes from `k`, we can locate the exact character for the current position without generating earlier blocks.

**Algorithm**
1. Compute the frequency of characters in the first half of `s`.  
2. Let `m = len(half)`. Compute `total = m! / ∏ freq[c]!`.  
3. If `k > total`, return `""`.  
4. For each position `i` from `0` to `m-1`:
   - Scan characters `'a'` … `'z'` in order.
   - For each character `c` with `freq[c] > 0`:
        - Compute `block = total * freq[c] // (m - i)`.  
          (`block` = number of distinct permutations that have `c` at position `i`.)
        - If `k ≤ block`, choose `c`:
            - Append `c` to answer half.
            - Decrement `freq[c]`.
            - Update `total = block` (the number of permutations for the suffix).
            - Break to next position.
        - Otherwise, `k -= block` (skip all permutations beginning with `c`).
5. After the loop, we have the smallest half. Build the final palindrome by mirroring and adding the middle character (if any).

**Implementation**

```python
from collections import Counter
import math
import string

class Solution:
    def smallestPalindrome(self, s: str, k: int) -> str:
        n = len(s)
        half_len = n // 2
        freq = Counter(s[:half_len])
        middle = s[half_len] if n % 2 else ""

        # total distinct permutations of the half
        total = math.factorial(half_len)
        for v in freq.values():
            total //= math.factorial(v)

        if k > total:
            return ""

        half = []
        # build the half character by character
        for i in range(half_len):
            remaining = half_len - i
            for c in string.ascii_lowercase:   # lexicographic order
                if freq[c] == 0:
                    continue
                # number of permutations if we put c at this position
                block = total * freq[c] // remaining
                if k <= block:
                    half.append(c)
                    freq[c] -= 1
                    total = block
                    break
                k -= block   # skip this whole block

        half_str = ''.join(half)
        return half_str + middle + half_str[::-1]
```

**Complexity Analysis**
- Computing factorials and the initial total: `O(Alphabet)` (constant 26).  
- The outer loop runs `half_len` times; the inner loop scans at most 26 letters.  
- **Time:** `O(half_len * Σ)` → `O(n)` where Σ = 26 (constant).  
- **Space:** `O(1)` extra (excluding output and counter).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on `s = "abba", k = 2`.

- `s` is already a palindrome.  
- Half length `m = 2`. Half = `"ab"` → frequencies: `{a:1, b:1}`.  
- Total distinct half permutations = `2! / (1!·1!) = 2`.  
- `k = 2 ≤ total`, so we continue.

**Position 0 (i = 0, remaining = 2)**  

| c | freq[c] | block = total * freq[c] // remaining | k before | Decision |
|---|---------|--------------------------------------|----------|----------|
| a | 1       | 2 * 1 // 2 = 1                       | 2        | k > block → k = 2‑1 = 1, skip a |
| b | 1       | 2 * 1 // 2 = 1                       | 1        | k ≤ block → choose b |

State after position 0: `half = ['b']`, `freq = {a:1, b:0}`, `total = block = 1`.

**Position 1 (i = 1, remaining = 1)**  

| c | freq[c] | block = total * freq[c] // remaining | k before | Decision |
|---|---------|--------------------------------------|----------|----------|
| a | 1       | 1 * 1 // 1 = 1                       | 1        | k ≤ block → choose a |

State after position 1: `half = ['b','a']`, `freq = {a:0, b:0}`, `total = 1`.

Half constructed = `"ba"`.  
Middle character = `""` (even length).  
Palindrome = `"ba" + "" + "ab" = "baab"`.

The distinct palindromes of `"abba"` are `"abba"` (k=1) and `"baab"` (k=2), matching the expected output.

---

**Summary**

- The **brute‑force** backtracking approach generates all distinct half‑permutations, builds palindromes, sorts them, and picks the k‑th. It is simple but factorial‑time.  
- The **optimized** approach treats the problem as constructing the k‑th permutation of a multiset using factorial counting, achieving linear time.  
- Both approaches illustrate the core idea of backtracking: exploring a search tree, but the optimized version prunes entire sub‑trees using combinatorial mathematics instead of traversing them node‑by‑node.  

This completes today’s lecture on the **Backtracking** pattern as applied to *Smallest Palindromic Rearrangement II*. Happy coding!