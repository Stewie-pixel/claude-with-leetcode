# Stack (Monotonic Stack)

## Video Solution

For more details about **Smallest Subsequence of Distinct Characters**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=UHALIyneo_Y).

## Concept

A **monotonic stack** helps us build a result where we can safely remove larger characters that appear later in the string, as long as we know they will occur again.  
Think of arranging books on a shelf: you keep the shelf as lexicographically small as possible, but you never take away a book that you won’t be able to put back later.

## When to Use It

Use a monotonic stack when you see:
- A requirement to produce the **smallest/largest** possible sequence (lexicographically, numerically, etc.).
- You must use each distinct element **exactly once** (or a fixed number of times).
- You can look ahead to know whether an element will appear again (frequency count).

Typical problems: *Remove Duplicate Letters*, *Smallest Subsequence of Distinct Characters*, *Find the Most Competitive Subsequence*.

## Template

```python
def monotonic_stack_template(s: str) -> str:
    # 1. Count remaining occurrences of each character
    remaining characters
    # 2. visited tracks whether a char is already in the stack
    # 3. stack will hold the answer in correct order
    count = [0] * 26
    visited = [False] * 26
    stack = []

    for ch in s:
        count[ord(ch) - ord('a')] += 1

    for ch in s:
        idx = ord(ch) - ord('a')
        count[idx] -= 1               # this occurrence is being processed

        if visited[idx]:
            continue                  # already placed in answer

        # While we can make the sequence lexicographically smaller
        # by removing the top of the stack, and that top character
        # will appear later again, pop it.
        while stack and ch < stack[-1] and count[ord(stack[-1]) - ord('a')] > 0:
            visited[ord(stack.pop()) - ord('a')] = False

        stack.append(ch)
        visited[idx] = True

    return ''.join(stack)
```

## LeetCode Problem Walkthrough

### Problem: 1081. Smallest Subsequence of Distinct Characters
https://leetcode.com/problems/smallest-subsequence-of-distinct-characters/

---

### Approach 1: Brute Force (Generate All Valid Subsequences)

**Algorithm**
1. Identify the set of distinct characters in `s`. Let `k` be its size.
2. Generate every subsequence of length `k` that contains each distinct character exactly once.
   - This can be done via backtracking/bitmask, choosing positions for each character.
3. Keep the lexicographically smallest subsequence seen.

**Implementation**

```python
from itertools import combinations

class Solution:
    def smallestSubsequence(self, s: str) -> str:
        distinct = sorted(set(s))                # characters we must keep
        k = len(distinct)
        n = len(s)
        best = None

        # Try every combination of indices of length k
        for idx_tuple in combinations(range(n), k):
            # Build subsequence from these indices
            subseq = ''.join(s[i] for i in idx_tuple)
            # Check if it contains each distinct char exactly once
            if sorted(subseq) == distinct:
                if best is None or subseq < best:
                    best = subseq
        return best if best is not None else ""
```

**Complexity Analysis**
- Time complexity: O(C(n, k) * k log k) — we examine every k‑index combination and sort each candidate (k ≤ 26).
- Space complexity: O(k) for the temporary subsequence and the set of distinct characters.

*Note:* This approach is exponential and only feasible for very small strings; it illustrates the problem’s nature but is not practical for the given constraints (`|s| ≤ 1000`).

---

### Approach 2: Monotonic Stack (Greedy)

**Intuition**
We want the smallest possible lexicographic result. While scanning the string left‑to‑right, if the current character `c` is smaller than the character at the top of our stack **and** we know that the top character will appear again later (its remaining count > 0), we can safely discard the top character now and place `c` earlier, yielding a lexicographically smaller prefix. We also must avoid duplicating characters, so we only push a character if it isn’t already in the stack.

**Algorithm**
1. Count how many times each character appears in the remainder of the string.
2. Iterate through `s`:
   - Decrease the count of the current character (we are processing this occurrence).
   - If the character is already in the stack, skip it.
   - While the stack is non‑empty, the current character is smaller than the stack’s top, and the top character still appears later (`count[top] > 0`), pop the top and mark it as not visited.
   - Push the current character onto the stack and mark it visited.
3. The stack content is the answer.

**Implementation**

```python
class Solution:
    def smallestSubsequence(self, s: str) -> str:
        # frequency of each character remaining to be processed
        remaining = [0] * 26
        for ch in s:
            remaining[ord(ch) - ord('a')] += 1

        in_stack = [False] * 26
        stack = []

        for ch in s:
            idx = ord(ch) - ord('a')
            remaining[idx] -= 1          # this occurrence is being used now

            if in_stack[idx]:
                continue                 # already placed, skip duplicates

            # Remove larger characters that will appear again later
            while stack and ch < stack[-1] and remaining[ord(stack[-1]) - ord('a')] > 0:
                removed = stack.pop()
                in_stack[ord(removed) - ord('a')] = False

            stack.append(ch)
            in_stack[idx] = True

        return ''.join(stack)
```

**Complexity Analysis**
- Time complexity: O(n) — each character is pushed and popped at most once.
- Space complexity: O(Σ) where Σ = 26 (alphabet size) for the counting and visited arrays, plus O(k) for the stack (k ≤ Σ).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

We trace the algorithm on the example `s = "bcabc"` (expected output `"abc"`).

| Step | i | ch | remaining after decrement | in_stack before | Action (while loop) | stack after step | in_stack after |
|------|---|----|---------------------------|-----------------|---------------------|------------------|----------------|
| 1    | 0 | b  | b:1, c:2, a:1             | all False       | stack empty → push b| [b]              | b=True         |
| 2    | 1 | c  | b:1, c:1, a:1             | b=True          | c > b → no pop      | [b, c]           | c=True         |
| 3    | 2 | a  | b:1, c:1, a:0             | b=True,c=True   | a < c and remaining[c]=1>0 → pop c<br>a < b and remaining[b]=1>0 → pop b| []               | b=False,c=False|
|      |   |    |                           |                 | stack empty → push a| [a]              | a=True         |
| 4    | 3 | b  | b:0, c:1, a:0             | a=True          | b > a → no pop      | [a, b]           | b=True         |
| 5    | 4 | c  | b:0, c:0, a:0             | a=True,b=True   | c > b → no pop      | [a, b, c]        | c=True         |

Final stack: `"abc"` → matches expected output.

--- 

*End of lecture.*