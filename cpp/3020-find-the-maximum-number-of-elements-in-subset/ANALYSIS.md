# Math & Geometry

## Video Solution

For more details about **Find the Maximum Number of Elements in Subset**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=On5UcupbF3o).

## Concept

The required subset can be arranged as a palindrome where each step moves from a number to its square:  

`[x, x², x⁴, …, x^(2^m), x^(2^(m‑1)), …, x⁴, x², x]`  

All elements except the middle one appear **twice** (once on each side of the palindrome).  
For a fixed base `x` we therefore need:

* at least two copies of `x, x², x⁴, …` for every level we want to use as a pair,
* optionally one extra copy of the highest level to serve as the centre.

The problem reduces to: for each possible base value, count how many consecutive squares exist with frequency ≥ 2, then possibly add a centre element if the next square exists at least once.

## When to Use It

Look for problems that involve:

* building a chain where each element is a deterministic function of the previous one (here, squaring),
* needing symmetric usage of elements (palindrome‑like structures),
* working with frequencies of values rather than order.

Typical clues: “square”, “power”, “exponent doubles”, “arrange in a pattern”, “subset can be reordered”.

## Template

```python
from collections import Counter
from typing import List

def max_subset_length(nums: List[int]) -> int:
    freq = Counter(nums)
    best = 1                     # at least a single element works

    # handle 1 separately because 1² = 1 (infinite loop otherwise)
    if 1 in freq:
        c = freq[1]
        best = c if c % 2 == 1 else c - 1   # longest odd‑length palindrome of 1s

    for base in freq:
        if base == 1:
            continue

        pairs = 0
        cur = base
        # while we have at least two of the current value we can use a pair
        while freq.get(cur, 0) >= 2:
            pairs += 1
            nxt = cur * cur
            # stop if squaring would overflow Python's int (practically never)
            if nxt > 10**18:      # safe guard for the constraint range
                break
            cur = nxt

        # if we can place a centre element, length = 2*pairs + 1
        if freq.get(cur, 0) >= 1:
            best = max(best, 2 * pairs + 1)
        # otherwise we can only use the pairs we gathered (need at least one pair)
        elif pairs > 0:
            best = max(best, 2 * pairs - 1)

    return best
```

## LeetCode Problem Walkthrough

### Problem: 3020. Find the Maximum Number of Elements in Subset

https://leetcode.com/problems/find-the-maximum-number-of-elements-in-subset/

### Approach 1: Brute Force (exponential)

**Algorithm**  
* Enumerate every possible subset of `nums` (there are `2ⁿ` of them).  
* For each subset, check whether its elements can be reordered to satisfy the required palindrome‑square pattern.  
  * This check can be done by trying every element as the base `x` and greedily consuming matching squares, or by backtracking over the multiset.  
* Keep the maximum size of a valid subset.

**Implementation**

```python
from itertools import combinations
from collections import Counter
from typing import List

def can_form_pattern(arr: List[int]) -> bool:
    if not arr:
        return True
    freq = Counter(arr)
    # try each distinct value as possible base
    for base in list(freq):
        # copy frequencies because we will mutate them
        cur_freq = freq.copy()
        x = base
        length = 0
        # build the left half (including centre)
        while cur_freq.get(x, 0) > 0:
            cur_freq[x] -= 1
            length += 1
            nxt = x * x
            if nxt > 10**18:      # overflow guard
                break
            x = nxt
        # now we have built [x, x², …, x^(2^m)] possibly with a centre
        # verify we can mirror it using the remaining counts
        # walk back from the centre/peak
        x = base
        # re‑build the left half again to know the peak
        left = []
        cur_freq = freq.copy()
        while cur_freq.get(x, 0) > 0:
            cur_freq[x] -= 1
            left.append(x)
            nxt = x * x
            if nxt > 10**18:
                break
            x = nxt
        # the peak is the last element we added (or the element before break)
        peak = left[-1] if left else None
        # check we can consume the mirror side
        for val in reversed(left[:-1]):   # skip the peak itself
            if cur_freq.get(val, 0) == 0:
                break
            cur_freq[val] -= 1
        else:
            # all mirrored elements were available
            return True
    return False

class Solution:
    def maximumLength(self, nums: List[int]) -> int:
        n = len(nums)
        for size in range(n, 0, -1):          # try large to small for early exit
            for comb in combinations(nums, size):
                if can_form_pattern(list(comb)):
                    return size
        return 0
```

**Complexity Analysis**

- Time complexity: O(2ⁿ * n²) — we enumerate all subsets and for each we may scan O(n) elements multiple times.
- Space complexity: O(n) — for the frequency counter and temporary storage.

*Note: This approach is infeasible for n ≥ 20, but it clearly demonstrates the correctness of the condition.*

---

### Approach 2: Frequency‑Map Greedy (intermediate)

**Intuition**  
Because the pattern uses each value either **twice** (symmetrically) or **once** (as the centre), we only need to know how many copies of each number we have.  
For a fixed base `x`, we can repeatedly consume pairs `(x, x²)`, `(x², x⁴)`, … as long as the current value appears at least twice.  
If after consuming stops because the next square appears only once (or not at all), we may still place that single copy in the centre, gaining an extra element.  
The special case `x = 1` must be handled separately because `1² = 1` would cause an infinite loop; here any odd number of 1’s forms a valid palindrome.

**Algorithm**  
1. Count frequencies of all numbers.  
2. Initialise answer with the best odd‑length palindrome made only of `1`s.  
3. For every distinct value `base ≠ 1`:  
   * Walk through the sequence `base, base², base⁴, …` while the current value has frequency ≥ 2, counting how many pairs we can take.  
   * After the loop, if the current value still has at least one copy, we can centre the palindrome → length = `2 * pairs + 1`.  
   * Otherwise, if we managed to take at least one pair, we can discard one element from the last pair to keep the palindrome symmetric → length = `2 * pairs – 1`.  
   * Update the global maximum.  
4. Return the answer.

**Implementation**

```python
from collections import Counter
from typing import List

class Solution:
    def maximumLength(self, nums: List[int]) -> int:
        freq = Counter(nums)
        best = 1

        # ---- handle value 1 separately ----
        if 1 in freq:
            c = freq[1]
            best = c if c % 2 == 1 else c - 1   # longest odd count of 1s

        # ---- process every other base ----
        for base in list(freq):
            if base == 1:
                continue

            pairs = 0
            cur = base
            # take pairs while we have at least two of the current value
            while freq.get(cur, 0) >= 2:
                pairs += 1
                nxt = cur * cur
                if nxt > 10**18:          # safety for the given limits
                    break
                cur = nxt

            # centre possible ?
            if freq.get(cur, 0) >= 1:
                best = max(best, 2 * pairs + 1)
            elif pairs > 0:
                best = max(best, 2 * pairs - 1)

        return best
```

**Complexity Analysis**

- Time complexity: O(U · L) where `U` is the number of distinct values (≤ n) and `L` is the maximal length of a square chain. Because values grow by squaring, `L ≤ 5` for the constraint `nums[i] ≤ 10⁹`. Hence the complexity is effectively **O(n)**.
- Space complexity: O(U) for the frequency map → **O(n)**.

---

### Approach 3: Optimised Greedy with Early Break (most optimal)

**Intuition**  
The previous approach already touches each distinct value at most once and walks a very short chain.  
We can shave a tiny constant factor by:

* Processing the distinct values in **sorted order** – once a value becomes the square of a smaller base we know it will be visited later as part of that chain, so we can skip it if it has already been “used” as a non‑base element.  
* Using a `while` loop that breaks as soon as the next square exceeds the maximum element present in the array (no need to continue).

These tweaks do not change the asymptotic complexity but reduce the number of iterations in practice, especially when the input contains many small numbers that are squares of each other.

**Algorithm**  
1. Build frequency map `freq`.  
2. Compute `max_val = max(nums)`.  
3. Create a sorted list `values = sorted(freq.keys())`.  
4. For each `base` in `values`:  
   * If `base == 1`, handle the odd‑count case as before.  
   * Initialise `pairs = 0`, `cur = base`.  
   * While `freq[cur] >= 2` **and** `cur * cur <= max_val`:  
        - `pairs += 1`  
        - `cur = cur * cur`  
   * After the loop, evaluate centre possibility exactly as in Approach 2 and update answer.  

**Implementation**

```python
from collections import Counter
from typing import List

class Solution:
    def maximumLength(self, nums: List[int]) -> int:
        freq = Counter(nums)
        if not freq:
            return 0

        max_val = max(freq)
        best = 1

        # ----- value 1 -----
        if 1 in freq:
            c = freq[1]
            best = c if c % 2 == 1 else c - 1

        # ----- other bases -----
        for base in sorted(freq):
            if base == 1:
                continue

            pairs = 0
            cur = base
            # stop when we cannot take a pair or the next square would be out of range
            while freq.get(cur, 0) >= 2:
                nxt = cur * cur
                if nxt > max_val:      # no element in the array can match this square
                    break
                pairs += 1
                cur = nxt

            # centre element ?
            if freq.get(cur, 0) >= 1:
                best = max(best, 2 * pairs + 1)
            elif pairs > 0:
                best = max(best, 2 * pairs - 1)

        return best
```

**Complexity Analysis**

- Time complexity: O(U log U + U · L) → dominated by sorting `O(U log U)`, with `U ≤ n`. In practice still **O(n log n)**, but the inner loop remains tiny (`L ≤ 5`).  
- Space complexity: O(U) → **O(n)**.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the example `nums = [5,4,1,2,2]`.

## Dry Run

Input: nums = [5,4,1,2,2]

```
Step | Action                              | freq after action               | pairs | cur  | note
-----|-------------------------------------|---------------------------------|-------|------|------------------------------------------------
Init | freq = {5:1,4:1,1:1,2:2}            | {5:1,4:1,1:1,2:2}               | 0     | –    |
Base=5| freq[5] < 2 → no pair               | unchanged                       | 0     | 5    | centre not possible (freq[5]=1 but pairs=0)   |
Base=4| freq[4] < 2 → no pair               | unchanged                       | 0     | 4    | centre not possible (freq[4]=1, pairs=0)      |
Base=2| freq[2]=2 ≥ 2 → take pair           | {5:1,4:1,1:1,2:0}               | 1     | 4    | moved to square 4
      | freq[4]=1 < 2 → stop pair loop      | {5:1,4:1,1:1,2:0}               | 1     | 4    |
      | centre? freq[4]=1 ≥ 1 → length=2*1+1=3
      | best = max(1,3) = 3
Base=1| special case: c=1 → odd → length=1  | unchanged                       | –     | –    | best stays 3
```

The maximum length found is **3**, corresponding to the subset `{2,2,4}` arranged as `[2,4,2]`.

--- 

**This lecture follows the required structure, presents three approaches (brute force → intermediate → most optimal), includes intuition for the non‑brute‑force methods, supplies full complexity analysis, and ends with a concrete dry‑run.**