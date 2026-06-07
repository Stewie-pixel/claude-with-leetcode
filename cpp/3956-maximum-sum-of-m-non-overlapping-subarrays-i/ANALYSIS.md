# 1D Dynamic Programming

## Concept

Dynamic Programming (DP) solves problems by breaking them into overlapping subproblems and storing the results of those subproblems to avoid recomputation. In 1D DP we keep a single array (or a few variables) where `dp[i]` represents the best answer for the prefix ending at position `i`.  
A common real‑world analogy is **planning a road trip with refuel stops**: you want to know the minimum cost to reach each mile marker, and you can compute the cost for mile `i` by looking back at a limited set of previous miles where you could have last refueled.

## When to Use It

Use 1D DP when you notice:

- The problem asks for an optimal value (max/min) over a prefix or suffix of the input.
- The decision at position `i` depends only on a limited window of previous positions (e.g., the last `k` elements).
- You can express the solution as a recurrence like `dp[i] = best of { transition from dp[j] + cost(j,i) }` where `j` ranges over a contiguous interval.
- The constraints allow an O(n·states) solution (here `n ≤ 1000` and `m ≤ n`).

Typical clues: “at most m subarrays”, “length between l and r”, “maximize sum”, “contiguous”, “non‑overlapping”.

## Template

```python
def solve_1d_dp(nums, m, l, r):
    n = len(nums)
    # prefix sums for O(1) range sum queries
    pref = [0] * (n + 1)
    for i in range(n):
        pref[i + 1] = pref[i] + nums[i]

    # dp[k][i] = best sum using at most k subarrays in prefix [0..i)
    # we will keep only two rows (previous k-1 and current k) to save memory
    prev = [0] * (n + 1)          # dp for k-1 subarrays
    for k in range(1, m + 1):
        cur = [0] * (n + 1)       # dp for k subarrays
        # monotonic deque stores indices j where we can start a new subarray
        # and maximizes (prev[j] - pref[j])
        from collections import deque
        dq = deque()
        for i in range(1, n + 1):
            # make index i-l eligible as a possible start
            start = i - l
            if start >= 0:
                val = prev[start] - pref[start]
                while dq and (prev[dq[-1]] - pref[dq[-1]]) <= val:
                    dq.pop()
                dq.append(start)

            # remove indices that are too far left (start < i-r)
            while dq and dq[0] < i - r:
                dq.popleft()

            # option 1: do not end a subarray at i
            cur[i] = cur[i - 1]

            # option 2: end a subarray at i using the best start in dq
            if dq:
                best = prev[dq[0]] - pref[dq[0]]
                cur[i] = max(cur[i], pref[i] + best)

        prev = cur   # move to next k
    return prev[n]
```

## LeetCode Problem Walkthrough

### Problem: 2405. Maximum Sum of M Non-Overlapping Subarrays I

https://leetcode.com/problems/maximum-sum-of-m-non-overlapping-subarrays-i/

### Approach 1: Brute Force (DP with O(n²·m) transitions)

**Algorithm**  
Let `dp[k][i]` be the maximum sum we can obtain using exactly `k` subarrays from the first `i` elements (`nums[0..i-1]`).  
Transition: either we skip `nums[i-1]` (`dp[k][i] = dp[k][i-1]`) or we end a subarray at `i-1`. If the subarray has length `len` where `l ≤ len ≤ r`, its start is `j = i - len`. Then we add the sum of that subarray to `dp[k-1][j]`.  
We try every possible length in `[l, r]` for each `i` and each `k`.

**Implementation**

```python
class Solution:
    def maximumSum(self, nums: List[int], m: int, l: int, r: int) -> int:
        n = len(nums)
        pref = [0] * (n + 1)
        for i in range(n):
            pref[i + 1] = pref[i] + nums[i]

        # dp[k][i] = best sum using exactly k subarrays in first i elements
        dp = [[-10**18] * (n + 1) for _ in range(m + 1)]
        for i in range(n + 1):
            dp[0][i] = 0                     # zero subarrays give sum 0

        for k in range(1, m + 1):
            for i in range(1, n + 1):
                # skip i-1
                dp[k][i] = dp[k][i-1]
                # try to end a subarray at i-1
                for length in range(l, r + 1):
                    j = i - length
                    if j < 0:
                        break
                    if dp[k-1][j] > -10**17:   # reachable state
                        cand = dp[k-1][j] + (pref[i] - pref[j])
                        if cand > dp[k][i]:
                            dp[k][i] = cand

        # answer is the best using at most m subarrays
        return max(dp[k][n] for k in range(1, m + 1))
```

**Complexity Analysis**

- Time complexity: O(m * n * (r-l+1)) → worst case O(m·n²) because `r-l+1` can be O(n). With `n ≤ 1000` this is ~10⁹ operations → too slow.
- Space complexity: O(m·n) for the DP table.

### Approach 2: Optimized DP with Prefix Max (O(m·n))

**Intuition**  
The inner loop over all lengths `l..r` is computing  
`max_{j in [i-r, i-l]} ( dp[k-1][j] + pref[i] - pref[j] )`.  
Since `pref[i]` is constant for a fixed `i`, we just need the maximum of `dp[k-1][j] - pref[j]` over the sliding window `[i-r, i-l]`. This is a classic sliding‑window maximum problem solvable with a monotonic deque in O(1) amortized per step.

**Algorithm**  
Iterate `k = 1..m`. Maintain a deque that stores candidate indices `j` where the value `dp[k-1][j] - pref[j]` is decreasing. For each `i` from 1 to n:
1. Add index `i-l` to the deque (if it is ≥0) while preserving decreasing order.
2. Remove indices `< i-r` from the front (they are out of the window).
3. The best start is at the front of the deque; compute candidate = `pref[i] + (dp[k-1][deque[0]] - pref[deque[0]])`.
4. `dp[k][i] = max(dp[k][i-1], candidate)` (skip or take a subarray ending at `i-1`).

**Implementation**

```python
from collections import deque
from typing import List

class Solution:
    def maximumSum(self, nums: List[int], m: int, l: int, r: int) -> int:
        n = len(nums)
        pref = [0] * (n + 1)
        for i in range(n):
            pref[i + 1] = pref[i] + nums[i]

        # dp_prev[i] = best sum using exactly (k-1) subarrays in first i elements
        dp_prev = [0] * (n + 1)          # k = 0 → sum 0
        answer = 0

        for k in range(1, m + 1):
            dp_cur = [0] * (n + 1)
            dq = deque()                 # stores indices j with decreasing dp_prev[j] - pref[j]

            for i in range(1, n + 1):
                # make index i-l available as a possible start
                start = i - l
                if start >= 0:
                    val = dp_prev[start] - pref[start]
                    while dq and (dp_prev[dq[-1]] - pref[dq[-1]]) <= val:
                        dq.pop()
                    dq.append(start)

                # discard indices that are too far left (< i-r)
                while dq and dq[0] < i - r:
                    dq.popleft()

                # option 1: do not end a subarray at i-1
                dp_cur[i] = dp_cur[i-1]

                # option 2: end a subarray at i-1 using the best start in dq
                if dq:
                    best = dp_prev[dq[0]] - pref[dq[0]]
                    cand = pref[i] + best
                    if cand > dp_cur[i]:
                        dp_cur[i] = cand

            dp_prev = dp_cur
            answer = max(answer, dp_prev[n])   # we allow using ≤k subarrays

        return answer
```

**Complexity Analysis**

- Time complexity: O(m·n) – each of the `m` passes scans the array once, deque operations are O(1) amortized.
- Space complexity: O(n) – two 1D DP arrays and the deque.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the optimized DP on the first example:

**Input**: `nums = [4,1,-5,2]`, `m = 2`, `l = 1`, `r = 3`  
Prefix sums: `pref = [0,4,5,0,2]`

We'll show the DP for `k = 1` (first subarray) and then `k = 2`.

#### k = 1 (dp_prev initially all zeros)

| i | pref[i] | window [i-r, i-l] | deque (indices) | best = max(dp_prev[j]-pref[j]) | candidate = pref[i] + best | dp_cur[i] = max(dp_cur[i-1], candidate) |
|---|---------|-------------------|-----------------|-------------------------------|----------------------------|------------------------------------------|
|1|4|[ -2,0] → add 0|[0]|dp_prev[0]-pref[0]=0|4+0=4|max(0,4)=4|
|2|5|[ -1,1] → add1|[0,1] (values 0, -1?) actually compute: dp_prev[1]-pref[1]=0-4=-4, deque keeps decreasing so we pop 0? Wait dp_prev[0]-pref[0]=0, dp_prev[1]-pref[1]=-4 → we keep 0 at front, then push 1 → deque [0,1]|max =0|5+0=5|max(4,5)=5|
|3|0|[0,2] → add2|deque: start with [0,1]; val=dp_prev[2]-pref[2]=0-5=-5 → push2 → [0,1,2]; remove <0? none|max=0|0+0=0|max(5,0)=5|
|4|2|[1,3] → add3|val=dp_prev[3]-pref[3]=0-0=0 → pop from back while ≤0: pop2 ( -5 ≤0), pop1 (-4 ≤0), pop0 (0 ≤0) → deque empty, then push3 → [3]; remove <1? none|max = dp_prev[3]-pref[3]=0|2+0=2|max(5,2)=5|

Result after k=1: `dp_prev = [0,4,5,5,5]` (best sum using exactly 1 subarray).

#### k = 2

Now `dp_prev` holds the results for exactly 1 subarray.

| i | pref[i] | window [i-r, i-l] | deque (indices) | best = max(dp_prev[j]-pref[j]) | candidate = pref[i] + best | dp_cur[i] = max(dp_cur[i-1], candidate) |
|---|---------|-------------------|-----------------|-------------------------------|----------------------------|------------------------------------------|
|1|4|[-2,0] → add0|[0]|dp_prev[0]-pref[0]=0-0=0|4+0=4|max(0,4)=4|
|2|5|[-1,1] → add1|val=dp_prev[1]-pref[1]=4-4=0 → pop0 (0≤0), push1 → [1]|max=0|5+0=5|max(4,5)=5|
|3|0|[0,2] → add2|val=dp_prev[2]-pref[2]=5-5=0 → pop1 (0≤0), push2 → [2]|max=0|0+0=0|max(5,0)=5|
|4|2|[1,3] → add3|val=dp_prev[3]-pref[3]=5-0=5 → deque empty, push3 → [3]; remove<1? none|max=5|2+5=7|max(5,7)=7|

Final answer after k=2 is `dp_cur[4] = 7`, matching the expected output.

The dry run shows how the deque maintains the maximum of `dp_prev[j] - pref[j]` over the sliding window, allowing us to compute the best subarray ending at each position in O(1) time.

--- 

**Summary**  
- Start with a straightforward DP that tries every possible length (O(m·n²)).  
- Observe the recurrence depends on a sliding window maximum of `dp[k-1][j] - pref[j]`.  
- Replace the inner loop with a monotonic deque to achieve O(m·n) time and O(n) space.  
- This pattern appears in many problems where you need to pick up to `m` intervals with length constraints (e.g., “Maximum Sum of 3 Non‑Overlapping Subarrays”, “Partition Array for Maximum Sum”, etc.). Use the template above whenever you see a DP of the form `dp[i] = best of { dp[j] + cost(j,i) }` with `j` ranging over a contiguous interval.