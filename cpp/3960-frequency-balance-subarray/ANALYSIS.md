# Arrays & Hashing

## Video Solution

For more details about **Frequency Balance Subarray**, watch the walkthrough at [https://www.youtube.com/watch?v=j0sE478PjLU](https://www.youtube.com/watch?v=j0sE478PjLU)

## Concept

Arrays & Hashing problems often require us to count occurrences of values while scanning the array. A hash map (or unordered_map) lets us update frequencies in O(1) average time, which is ideal for subarray problems where we need to know how many times each number appears in the current window.

## When to Use It

Use arrays & hashing when you see:
- Need to track frequencies of elements in a subarray or prefix.
- Problems that ask about counts, duplicates, or conditions based on how many times each value appears.
- Constraints that allow O(n²) enumeration of subarrays if we can update frequencies incrementally.

## Template

```python
# Incremental frequency update while expanding right pointer
freq = defaultdict(int)      # value -> count in current window
freq_of_freq = defaultdict(int)  # count -> how many distinct values have this count

for left in range(n):
    freq.clear()
    freq_of_freq.clear()
    for right in range(left, n):
        val = nums[right]
        old = freq[val]
        # remove old count from freq_of_freq
        if old > 0:
            freq_of_freq[old] -= 1
            if freq_of_freq[old] == 0:
                del freq_of_freq[old]
        # add new count
        freq[val] = old + 1
        freq_of_freq[old + 1] += 1

        # check condition using freq_of_freq
        if condition_met(freq_of_freq):
            answer = max(answer, right - left + 1)
```

## LeetCode Problem Walkthrough

### Problem: 3960. Frequency Balance Subarray

https://leetcode.com/problems/frequency-balance-subarray/

### Approach 1: Brute Force (Re‑compute from Scratch)

**Algorithm**
For every possible start index `i`, iterate over every end index `j ≥ i`.  
For each subarray `nums[i..j]` build a fresh frequency map, then check:
- If only one distinct value → valid.
- Else, there must be exactly two different frequencies `f` and `2f`, and both frequencies must appear among the distinct values.  
Keep the maximum length found.

**Implementation**

```python
from collections import Counter

class Solution:
    def maxLengthBrute(self, nums):
        n = len(nums)
        best = 1
        for i in range(n):
            for j in range(i, n):
                freq = Counter(nums[i:j+1])
                if len(freq) == 1:
                    best = max(best, j - i + 1)
                else:
                    counts = list(freq.values())
                    # we need exactly two distinct counts f and 2f
                    uniq = set(counts)
                    if len(uniq) == 2:
                        f1, f2 = sorted(uniq)
                        if f2 == 2 * f1:
                            best = max(best, j - i + 1)
        return best
```

**Complexity Analysis**
- Time complexity: O(n³) — two loops for subarrays (O(n²)) and building Counter each time (O(n)).
- Space complexity: O(n) — the Counter for the current subarray.

### Approach 2: Incremental Frequency Update (Optimized Brute Force)

**Intuition**
Re‑building the frequency map for every subarray wastes work. While we expand the right pointer, we can update the frequencies in O(1) by adjusting the count of the newly added element. We also maintain a map `freq_of_freq` that tells how many distinct numbers have a given frequency, letting us test the balance condition quickly.

**Algorithm**
1. Outer loop fixes left boundary `i`.
2. Inner loop expands right boundary `j` from `i` to `n‑1`.
3. For each new element `nums[j]`:
   - Decrease the count of its old frequency in `freq_of_freq`.
   - Increase its frequency in `freq`.
   - Increase the count of the new frequency in `freq_of_freq`.
4. After the update, the subarray `nums[i..j]` is frequency‑balanced iff:
   - `len(freq) == 1` (only one distinct value), **or**
   - `len(freq_of_freq) == 2` and the two frequencies are `f` and `2f`.
5. Update answer with the current length.
6. Before moving `i` to `i+1`, clear the maps (or rebuild them) – simplicity favours rebuilding for each `i`.

**Implementation**

```python
from collections import defaultdict

class Solution:
    def maxLength(self, nums):
        n = len(nums)
        best = 1

        for left in range(n):
            freq = defaultdict(int)          # value -> count in window
            freq_of_freq = defaultdict(int)  # count -> how many values have this count

            for right in range(left, n):
                val = nums[right]
                old = freq[val]
                # remove old frequency contribution
                if old > 0:
                    freq_of_freq[old] -= 1
                    if freq_of_freq[old] == 0:
                        del freq_of_freq[old]
                # add new frequency
                freq[val] = old + 1
                freq_of_freq[old + 1] += 1

                # check balance condition
                if len(freq) == 1:
                    best = max(best, right - left + 1)
                elif len(freq_of_freq) == 2:
                    f1, f2 = sorted(freq_of_freq.keys())
                    if f2 == 2 * f1:
                        best = max(best, right - left + 1)

        return best
```

**Complexity Analysis**
- Time complexity: O(n²) — each pair (left, right) processed once; map updates are O(1) average.
- Space complexity: O(n) — at most O(distinct values) entries in the two hash maps.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: `nums = [1,2,2,1,2,3,3,3]`

We illustrate the inner loop for `left = 1` (starting at the first `2`).

```
| Step | right | val | freq (value:count)               | freq_of_freq (count:how many) | Condition met? | Length |
|------|-------|-----|----------------------------------|------------------------------|----------------|--------|
| 1    | 1     | 2   | {2:1}                            | {1:1}                        | one distinct   | 1      |
| 2    | 2     | 2   | {2:2}                            | {2:1}                        | one distinct   | 2      |
| 3    | 3     | 1   | {2:2, 1:1}                       | {1:1, 2:1}                   | {1,2} → 2=2*1 | 3      |
| 4    | 4     | 2   | {2:3, 1:1}                       | {1:1, 3:1}                   | {1,3} → 3≠2*1 | 4      |
| 5    | 5     | 3   | {2:3, 1:1, 3:1}                  | {1:2, 3:1}                   | {1,3} → 3≠2*1 | 5      |
| 6    | 6     | 3   | {2:3, 1:1, 3:2}                  | {1:1, 2:1, 3:1} → 3 keys    | fails          | 6      |
| 7    | 7     | 3   | {2:3, 1:1, 3:3}                  | {1:1, 3:2}                   | {1,3} → 3≠2*1 | 7      |
```

The longest valid window seen here is `[2,1,2,3,3]` (indices 1‑5) of length 5, matching the example answer.  

---  

*End of lecture.*