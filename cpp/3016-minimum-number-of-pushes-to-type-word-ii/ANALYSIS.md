# Greedy

## Video Solution

For more details about **Minimum Number of Pushes to Type Word II**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=3qNtetexxyI).

## Concept

We have 8 keys (2‑9) that can each hold any number of letters.  
If a letter is the *k‑th* letter on a key, it costs *k* pushes to type it.  
Because we can remap arbitrarily, the only thing that matters is **how many letters we place at each push‑cost level**:

- Cost = 1 push: up to 8 letters (first slot on each key)  
- Cost = 2 pushes: up to 8 letters (second slot on each key)  
- Cost = 3 pushes: up to 8 letters, and so on.

To minimize the total pushes we should put the **most frequent letters** into the cheapest slots.  
This is a classic greedy assignment: sort frequencies descending and fill the slots in order.

## When to Use It

- You need to assign items to buckets where the cost per bucket increases after a fixed capacity.  
- The objective is a weighted sum (frequency × cost) that you want to minimize.  
- You can reorder items freely; only the counts per cost level matter.

Typical clues: “minimum total cost”, “each key can hold X letters”, “cost grows with position”, “rearrange/map freely”.

## Template

```python
def min_pushes_greedy(freqs, slots_per_level=8):
    """
    freqs: list of non‑negative integers (counts for each item)
    slots_per_level: how many items can share the same cost before it increases
    Returns minimal total cost = Σ (level_i * freq_i)
    """
    freqs.sort(reverse=True)                 # greedy: biggest first
    total = 0
    for i, f in enumerate(freqs):
        level = i // slots_per_level + 1      # which cost block we are in
        total += level * f
    return total
```

## LeetCode Problem Walkthrough

### Problem: 3016. Minimum Number of Pushes to Type Word II

https://leetcode.com/problems/minimum-number-of-pushes-to-type-word-ii/

### Approach 1: Brute Force (Conceptual)

**Algorithm**  
Imagine we try every possible way to assign the 26 letters to the push‑cost levels. For each assignment we compute the total pushes and keep the minimum.

Because the letters are distinguishable, we would need to examine all permutations of the 26 letters and then split the permutation into blocks of size 8 (cost 1), next 8 (cost 2), … .  
The number of permutations is 26! ≈ 4 × 10^26 – completely infeasible.

**Implementation**  
(Only shown for completeness; not used in practice.)

```python
from itertools import permutations

def brute_force(word):
    freq = [0] * 26
    for ch in word:
        freq[ord(ch) - 97] += 1

    best = float('inf')
    # try every ordering of letters (26! possibilities)
    for perm in permutations(range(26)):          # pragma: no cover
        cost = 0
        for idx, letter in enumerate(perm):
            level = idx // 8 + 1
            cost += level * freq[letter]
        best = min(best, cost)
    return best
```

**Complexity Analysis**  
- Time: O(26! × 26) – astronomically large, not usable.  
- Space: O(1) besides the permutation generator.

---

### Approach 2: Greedy with Sorting

**Intuition**  
If we have two letters X and Y with frequencies fX ≥ fY, swapping them so that X occupies a cheaper slot (lower level) and Y a more expensive slot can never increase the total cost:

```
Δ = (levelX·fY + levelY·fX) - (levelX·fX + levelY·fY)
  = (levelY - levelX)·(fX - fY) ≥ 0   because levelY ≥ levelX and fX ≥ fY
```

Thus an optimal arrangement must have frequencies sorted in non‑increasing order aligned with increasing slot levels.  
So we simply count frequencies, sort them descending, and accumulate cost using the level formula `i // 8 + 1`.

**Algorithm**  
1. Count frequency of each letter in `word`.  
2. Sort the 26 frequencies in descending order.  
3. For each frequency at index `i`, add `(i // 8 + 1) * freq` to the answer.

**Implementation**

```python
class Solution:
    def minimumPushes(self, word: str) -> int:
        # 1. frequency count
        freq = [0] * 26
        for ch in word:
            freq[ord(ch) - ord('a')] += 1

        # 2. greedy ordering: most frequent first
        freq.sort(reverse=True)

        # 3. accumulate cost
        pushes = 0
        for i, f in enumerate(freq):
            if f == 0:
                break
            level = i // 8 + 1          # cost block
            pushes += level * f
        return pushes
```

**Complexity Analysis**  
- Time: O(n) to count + O(26 log 26) to sort → O(n) (n = len(word)).  
- Space: O(1) – only a fixed size array of 26 ints.

---

### Approach 3: Counting‑Sort Optimization (Optional)

**Intuition**  
Since there are only 26 possible letters, we can avoid the O(26 log 26) sort by using counting sort on the frequencies (which are at most `n`).  
We process frequencies from highest to lowest, filling the slots as we go.

**Algorithm**  
1. Build frequency array `freq[26]`.  
2. Create a bucket array `bucket[count]` where `bucket[c]` = how many letters appear exactly `c` times.  
3. Iterate `c` from `maxFreq` down to 1:  
   - For each of the `bucket[c]` letters, assign them to the next available slot, compute cost `level * c`, and accumulate.  
   - Increase the slot index accordingly.  
4. Stop when all letters have been placed.

**Implementation**

```python
class Solution:
    def minimumPushes(self, word: str) -> int:
        freq = [0] * 26
        for ch in word:
            freq[ord(ch) - 97] += 1

        max_f = max(freq)
        bucket = [0] * (max_f + 1)
        for f in freq:
            bucket[f] += 1

        pushes = 0
        slot = 0                     # how many letters already placed
        for f in range(max_f, 0, -1):
            while bucket[f]:
                # place one letter of frequency f
                level = slot // 8 + 1
                pushes += level * f
                slot += 1
                bucket[f] -= 1
        return pushes
```

**Complexity Analysis**  
- Time: O(n + max_f + 26) → O(n) because max_f ≤ n.  
- Space: O(max_f) ≤ O(n) for the bucket, but with n ≤ 10^5 this is fine; in practice it’s tiny.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the greedy algorithm on the third example from the statement.

**Input:** `word = "aabbccddeeffgghhiiiiii"`  
**Frequencies:** a=2, b=2, c=2, d=2, e=2, f=2, g=2, h=2, i=6, others=0  

**Sorted frequencies (descending):** `[6, 2, 2, 2, 2, 2, 2, 2, 2, 0, …]`

```
| Step | i | freq[i] | level = i//8 + 1 | contribution | cumulative |
|------|---|---------|------------------|--------------|------------|
| 1    | 0 | 6       | 1                | 6 * 1 = 6    | 6          |
| 2    | 1 | 2       | 1                | 2 * 1 = 2    | 8          |
| 3    | 2 | 2       | 1                | 2 * 1 = 2    | 10         |
| 4    | 3 | 2       | 1                | 2 * 1 = 2    | 12         |
| 5    | 4 | 2       | 1                | 2 * 1 = 2    | 14         |
| 6    | 5 | 2       | 1                | 2 * 1 = 2    | 16         |
| 7    | 6 | 2       | 1                | 2 * 1 = 2    | 18         |
| 8    | 7 | 2       | 1                | 2 * 1 = 2    | 20         |
| 9    | 8 | 2       | 2                | 2 * 2 = 4    | 24         |
|10+   | 9+| 0       | –                | 0            | 24         |
```

Total pushes = **24**, matching the expected output.

---