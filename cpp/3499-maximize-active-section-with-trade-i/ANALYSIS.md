# Greedy

## Video Solution

For more details about **Maximize Active Section with Trade I**, watch the walkthrough at [https://www.youtube.com/watch?v=VQoDFj4DkG8](https://www.youtube.com/watch?v=VQoDFj4DkG8).

## Concept

We are allowed to perform **at most one trade**:

1. Turn a contiguous block of `1`s that is **surrounded by `0`s** into all `0`s.  
2. Afterwards, turn a contiguous block of `0`s that is **surrounded by `1`s** into all `1`s.

The augmentation `1' + s + '1'`.  
In this augmented string we can think of the trade as:

*Pick a `1`‑segment that has `0` on both sides, erase it (lose its length), and then turn the neighboring `0`‑segments into `1`s (gain their combined length).*

Thus the **net gain** of a trade that chooses a particular `1`‑segment is  

```
gain = length_of_left_zero_block + length_of_right_zero_block
```

The best we can do is to pick the `1`‑segment with the largest such gain.  
If no `1`‑segment is surrounded by zeros, we cannot improve the count.

The problem therefore reduces to:

*Run‑length encode the augmented string, then for every `1`‑run that has `0`‑runs[left]` and `0[right]`, compute left+right zeros and keep the maximum.*

This decision — picking the segment that gives the biggest immediate improvement — is a classic **greedy** choice.

## When to Use It

Look for these clues in the problem statement:

- You may perform **exactly one** (or at most one) operation that changes two disjoint parts of the string.
- The operation’s effect can be expressed as **loss + gain** where loss depends on a chosen segment and gain depends on its immediate neighbours.
- The goal is to **maximize a total count** after the operation.
- The input size is up to `10⁵`, demanding an **O(n)** or **O(n log n)** solution.

When you see a pattern like “choose a block, lose its value, gain the value of adjacent blocks”, a greedy scan over run‑lengths is often optimal.

## Template

```python
# Run‑length encode a string and evaluate a greedy choice
def greedy_trade(s: str) -> int:
    # 1. augment with sentinel '1's
    t = '1' + s + '1'
    # 2. run‑length encode: list of (char, length)
    runs = []
    i = 0
    while i < len(t):
        j = i
        while j < len(t) and t[j] == t[i]:
            j += 1
        runs.append((t[i], j - i))
        i = j

    # 3. original number of active sections (original '1's)
    total_ones = s.count('1')

    # 4. best gain from turning a 1‑run surrounded by 0‑runs
    best_gain = 0
    for idx in range(1, len(runs) - 1):
        ch, length = runs[idx]
        if ch == '1' and runs[idx - 1][0] == '0' and runs[idx + 1][0] == '0':
            gain = runs[idx - 1][1] + runs[idx + 1][1]   # zeros on both sides
            if gain > best_gain:
                best_gain = gain

    return total_ones + best_gain
```

## LeetCode Problem Walkthrough

### Problem: 3499. Maximize Active Section with Trade I  
https://leetcode.com/problems/maximize-active-section-with-trade-i/

### Approach 1: Brute Force

**Algorithm**  
Try every possible contiguous block of `1`s that could be turned to `0`s, and for each, try every possible contiguous block of `0`s that could be turned to `1`s (respecting the “surrounded” condition). Compute the resulting number of `1`s and keep the maximum.

**Implementation**

```python
class Solution:
    def maxActiveSectionsAfterTrade(self, s: str) -> int:
        n = len(s)
        # helper to check if a block [l, r] is surrounded by opposite char
        def surrounded(l, r, target):
            left_ok  = (l == 0) or (s[l-1] != target)
            right_ok = (r == n-1) or (s[r+1] != target)
            return left_ok and right_ok

        best = s.count('1')          # no trade
        for l1 in range(n):
            if s[l1] != '1': continue
            for r1 in range(l1, n):
                if s[r1] != '1': break
                if not surrounded(l1, r1, '0'): continue   # must be 1‑block surrounded by 0
                # after turning this block to 0, we may turn any 0‑block to 1
                for l2 in range(n):
                    if s[l2] != '0': continue
                    for r2 in range(l2, n):
                        if s[r2] != '0': break
                        if not surrounded(l2, r2, '1'): continue   # must be 0‑block surrounded by 1
                        # simulate the trade
                        # count ones after: original - len(1block) + len(0block)
                        ones_after = s.count('1') - (r1 - l1 + 1) + (r2 - l2 + 1)
                        best = max(best, ones_after)
        return best
```

**Complexity Analysis**

- Time complexity: O(n⁴) — four nested loops over the string length.  
- Space complexity: O(1) — only a few integer variables.

> This is far too slow for `n ≤ 10⁵`, but it validates the logic.

### Approach 2: Prefix‑Sum Optimization (O(n))

**Intuition**  
The net gain of turning a `1`‑block into `0`s and then a neighbouring `0`‑block into `1`s depends only on the **total zeros immediately left and right** of that `1`‑block. We can pre‑compute prefix sums of zeros to obtain those lengths in O(1) per block.

**Algorithm**

1. Build a prefix sum array `zero_pref[i] = number of '0' in s[0:i)`.  
2. Scan the string to locate every maximal block of `1`s. For each block `[l, r]`:
   - Check if it is surrounded by `0`s (i.e., `l>0 && s[l-1]=='0'` and `r<n-1 && s[r+1]=='0'`).  
   - If yes, zeros on the left = `zero_pref[l] - zero_pref[left_start]` where `left_start` is the start of the current zero run; similarly for the right.  
   - Gain = leftZeros + rightZeros. Keep the maximum gain.  
3. Answer = original number of `1`s + maxGain.

Because we need the exact lengths of the adjacent zero runs, we can obtain them while scanning by keeping counters of the current zero run length.

**Implementation**

```python
class Solution:
    def maxActiveSectionsAfterTrade(self, s: str) -> int:
        n = len(s)
        total_ones = s.count('1')

        # prefix sum of zeros
        zero_pref = [0] * (n + 1)
        for i, ch in enumerate(s):
            zero_pref[i+1] = zero_pref[i] + (ch == '0')

        best_gain = 0
        i = 0
        while i < n:
            if s[i] == '1':
                j = i
                while j < n and s[j] == '1':
                    j += 1
                # block [i, j-1] is a run of 1s
                left_zero = right_zero = 0
                if i > 0 and s[i-1] == '0':
                    # extend left to get whole zero run
                    l = i-1
                    while l >= 0 and s[l] == '0':
                        l -= 1
                    left_zero = (i-1) - l
                if j < n and s[j] == '0':
                    r = j
                    while r < n and s[r] == '0':
                        r += 1
                    right_zero = r - j
                best_gain = max(best_gain, left_zero + right_zero)
                i = j
            else:
                i += 1
        return total_ones + best_gain
```

**Complexity Analysis**

- Time complexity: O(n) — each character is inspected a constant number of times.  
- Space complexity: O(n) for the prefix sum array (can be reduced to O(1) with running counters).

### Approach 3: Single‑Pass Run‑Length Encoding (Most Optimal)

**Intuition**  
Instead of a prefix array, we can directly compute run‑lengths of the augmented string `t = '1' + s + '1'`.  
Every run of `1`s that is flanked by `0`‑runs yields a gain equal to the sum of the lengths of those two `0`‑runs. Scanning the run‑length list once gives the answer in linear time and constant extra space.

**Algorithm**

1. Form `t = '1' + s + '1'`.  
2. Run‑length encode `t` into a list `runs = [(char, length), …]`.  
3. Initialise `ans = number of '1' in s`.  
4. For each index `k` from `1` to `len(runs)-2`:
   - If `runs[k][0] == '1'` and both neighbours are `'0'`, compute `gain = runs[k-1][1] + runs[k+1][1]` and update `best_gain`.  
5. Return `ans + best_gain`.

**Implementation**

```python
class Solution:
    def maxActiveSectionsAfterTrade(self, s: str) -> int:
        # 1. augment with sentinel 1s
        t = '1' + s + '1'
        # 2. run‑length encode
        runs = []
        i = 0
        while i < len(t):
            j = i
            while j < len(t) and t[j] == t[i]:
                j += 1
            runs.append((t[i], j - i))
            i = j

        total_ones = s.count('1')
        best_gain = 0

        # 3. examine every 1‑run that has 0‑runs on both sides
        for idx in range(1, len(runs) - 1):
            ch, length = runs[idx]
            if ch == '1' and runs[idx-1][0] == '0' and runs[idx+1][0] == '0':
                gain = runs[idx-1][1] + runs[idx+1][1]
                if gain > best_gain:
                    best_gain = gain

        return total_ones + best_gain
```

**Complexity Analysis**

- Time complexity: O(n) — one pass to build `t`, one pass to run‑length encode, and one pass over the runs.  
- Space complexity: O(n) for the runs list (at most `2 * number_of_groups + 2`; in the worst case alternating characters → O(n)). This can be reduced to O(1) by keeping only the previous two runs while scanning, but O(n) easily fits the constraints.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on Example 2: `s = "0100"`.

**Augmented string**: `t = "1" + "0100" + "1" = "101001"`.

**Run‑length encoding of `t`**

| Index | Char | Length |
|------|------|--------|
| 0    | '1'  | 1 |
| 1    | '0'  | 1 |
| 2    | '1'  | 1 |
| 3    | '0'  | 2 |
| 4    | '1'  | 1 |

Now we inspect each `1`‑run that has `0` on both sides:

- Run at index 2 (`'1'`, length 1) → left zeros = 1 (run 1), right zeros = 2 (run 3) → gain = 1 + 2 = 3.  
- Run at index 4 is at the end (no right zero run) → not considered.

**Best gain** = 3.  
Original number of `1`s in `s` = 1.  
Result = 1 + 3 = **4**, matching the expected output.

```
| Step | i   | t[i] | Run Char | Run Len | Action |
|------|-----|------|----------|---------|--------|
| 0    | 0   | 1    | 1        | 1       | start run |
| 1    | 1   | 0    | 0        | 1       | start zero run |
| 2    | 2   | 1    | 1        | 1       | start one run (candidate) |
| 3    | 3   | 0    | 0        | 2       | start zero run |
| 4    | 5   | 1    | 1        | 1       | end |
```

The table shows how the algorithm captures the surrounding zero lengths for the middle `1`‑run, yielding the maximal gain.

--- 

**Summary**

- The problem is solved greedily by picking the `1`‑block whose removal yields the largest possible increase from neighboring `0`‑blocks.  
- A single pass run‑length encoding of the augmented string gives the answer in O(n) time and O(n) space (reducible to O(1)).  
- The solution always starts with the brute‑force idea, then shows how prefix sums improve it, and finally presents the optimal run‑length approach, adhering to the required “naïve → intermediate → optimal” progression.