# Greedy

## Video Solution

For more details about **Minimum Number of Pushes to Type Word I**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=BhCD0n8k-K0)

## Concept

We have 8 usable keys (2‑9).  
If a key holds `k` letters, the first letter costs 1 push, the second 2 pushes, …, the k‑th letter costs k pushes.  
To minimise the total number of pushes we should put the *most frequently used* letters in the cheapest slots (cost 1), the next most frequent in the next cheapest slots (cost 2), and so on.  
Because every letter in `word` appears exactly once, the problem reduces to:  

*Sort the letters by descending frequency (here all frequencies are 1) and assign the i‑th letter a cost of `i // 8 + 1`.*

This is a classic **greedy** strategy: make the locally optimal choice (fill the cheapest available slot) at each step, which leads to a globally optimal solution.

## When to Use It

- You need to **minimise a sum of weighted costs** where each item’s cost depends on its position in a limited‑capacity ordering (e.g., “first 8 items cost 1, next 8 cost 2, …”).  
- The items have **weights** (here all 1) and you can reorder them arbitrarily.  
- You see phrases like “assign to buckets”, “distribute among groups”, “minimise total pushes/presses/time”.  
- Constraints are small enough that sorting is feasible, but trying all assignments would be factorial.

## Template

```python
def min_pushes(word: str) -> int:
    # 1. Count frequencies of each character
    freq = [0] * 26
    for ch in word:
        freq[ord(ch) - ord('a')] += 1

    # 2. Sort frequencies descending (greedy: use highest weight first)
    freq.sort(reverse=True)

    # 3. Assign costs: first 8 slots → cost 1, next 8 → cost 2, …
    ans = 0
    for i, f in enumerate(freq):
        # i // 8 gives the “push level” (0‑based), add 1 for actual pushes
        ans += f * (i // 8 + 1)

    return ans
```

## LeetCode Problem Walkthrough

### Problem: 3014. Minimum Number of Pushes to Type Word I

https://leetcode.com/problems/minimum-number-of-pushes-to-type-word-i/

---

### Approach 1: Brute Force (Conceptual)

**Algorithm**  
- Enumerate every possible way to assign the distinct letters to the 8 keys (i.e., every permutation of the letters).  
- For each permutation, place the letters sequentially on the keys: the first 8 letters get cost 1, the next 8 cost 2, etc.  
- Compute the total push cost for that permutation and keep the minimum.

**Implementation** (only illustrative; infeasible for the full input size)

```python
from itertools import permutations

def min_pushes_brute(word: str) -> int:
    n = len(word)
    best = float('inf')
    for perm in permutations(word):          # O(n!) – impractical for n>8
        cost = 0
        for i, ch in enumerate(perm):
            cost += (i // 8 + 1)              # each letter weight = 1
        best = min(best, cost)
    return best
```

**Complexity Analysis**

- Time complexity: **O(n! · n)** – factorial blow‑up; only works for n ≤ 8 in practice.  
- Space complexity: **O(n)** – for the current permutation.

*We include this to show the naïve idea; the greedy method avoids the exponential blow‑up.*

---

### Approach 2: Greedy (Optimal)

**Intuition**  
Because each key’s cost increases linearly with the number of letters already placed on it, the cheapest available slot is always the next unused position in the ordered list of slots: `[1,1,…,1 (8 times), 2,2,…,2 (8 times), …]`.  
Placing a letter with higher weight (frequency) in an earlier (cheaper) slot can never hurt optimality. Hence we sort letters by descending frequency and assign them to the slots in that order.

**Algorithm**  
1. Count how many times each letter appears in `word`.  
2. Sort the frequency list in non‑increasing order.  
3. Walk through the sorted frequencies; the i‑th letter contributes `freq[i] * (i // 8 + 1)` pushes.  
4. Accumulate the sum.

**Implementation**

```python
def min_pushes(word: str) -> int:
    freq = [0] * 26
    for ch in word:
        freq[ord(ch) - ord('a')] += 1

    freq.sort(reverse=True)          # greedy: highest weight first
    ans = 0
    for i, f in enumerate(freq):
        ans += f * (i // 8 + 1)       # i//8 = push level (0‑based)
    return ans
```

**Complexity Analysis**

- Time complexity: **O(n + 26 log 26) → O(n)** – counting is linear, sorting 26 items is constant.  
- Space complexity: **O(26) → O(1)** – fixed‑size frequency array.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the greedy algorithm on the example `word = "abcde"` (all frequencies = 1).

| Step | i (sorted index) | Letter | freq | i // 8 + 1 (pushes) | Contribution |
|------|------------------|--------|------|----------------------|--------------|
| 0    | 0                | a      | 1    | 0//8+1 = 1           | 1 × 1 = 1    |
| 1    | 1                | b      | 1    | 1//8+1 = 1           | 1 × 1 = 1    |
| 2    | 2                | c      | 1    | 2//8+1 = 1           | 1 × 1 = 1    |
| 3    | 3                | d      | 1    | 3//8+1 = 1           | 1 × 1 = 1    |
| 4    | 4                | e      | 1    | 4//8+1 = 1           | 1 × 1 = 1    |

Total pushes = 1 + 1 + 1 + 1 + 1 = **5**, which matches the expected output.

---

**Follow the same steps for any input:** count, sort descending, multiply each frequency by `i // 8 + 1`, and sum. This yields the minimum number of pushes after an optimal key remapping.