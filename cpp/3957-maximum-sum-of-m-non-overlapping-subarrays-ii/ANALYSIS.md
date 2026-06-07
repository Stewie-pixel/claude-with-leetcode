# Dynamic Programming

## Concept

Dynamic Programming (DP) solves problems by breaking them into overlapping sub‑problems and storing the results of those sub‑problems to avoid recomputation.  
Think of DP as memorizing the answer to a question you’ve already solved so you never have to solve it again.  
In this problem we keep track of the best total sum we can obtain using a certain number of sub‑arrays while scanning the array from left to right.

## When to Use It

Use DP when you see:

- **Maximum / minimum** of something that can be built from smaller parts (e.g., max sum, min cost).  
- **Overlapping sub‑problems** – the same prefix/suffix appears many times.  
- **A constraint on the number of groups/segments** (here, at most *m* sub‑arrays).  
- **Contiguous segments** with length bounds (*l* ≤ length ≤ *r*).  

If the problem asks for an optimal selection of a limited number of contiguous pieces, DP is a natural fit.

## Template

Below is a reusable Python template for DP where we keep two dimensions:  
`i` – prefix length processed,  
`k` – number of sub‑arrays taken so far.  
Transition tries every allowed length for the next sub‑array and picks the best.

```python
def max_sum(nums, m, l, r):
    n = len(nums)
    # prefix sums for O(1) range sum
    pref = [0] * (n + 1)
    for i in range(n):
        pref[i + 1] = pref[i] + nums[i]

    # dp[i][k] = best sum using first i elements with exactly k sub‑arrays
    # initialize with -inf (unreachable)
    dp = [[-10**18] * (m + 1) for _ in range(n + 1)]
    dp[0][0] = 0

    for i in range(1, n + 1):
        # option 1: skip element i‑1
        for k in range(m + 1):
            dp[i][k] = dp[i - 1][k]

        # option 2: end a sub‑array at i‑1, try all allowed lengths
        for length in range(l, r + 1):
            if i - length < 0:
                break
            sub_sum = pref[i] - pref[i - length]
            for k in range(1, m + 1):
                if dp[i - length][k - 1] != -10**18:
                    dp[i][k] = max(dp[i][k],
                                   dp[i - length][k - 1] + sub_sum)

    # answer is the best using ≤ m sub‑arrays
    return max(dp[n][k] for k in range(1, m + 1))
```

The template above is **O(n·m·(r‑l+1))** – correct but too slow for *n* = 10⁵.  
The lecture shows how to drop the inner length loop with a monotonic queue and how to eliminate the *m* dimension via Lagrangian relaxation (binary search on a penalty per sub‑array).

---

## LeetCode Problem Walkthrough

### Problem: 2246. Maximum Sum of M Non‑Overlapping Subarrays II  
https://leetcode.com/problems/maximum-sum-of-m-non-overlapping-subarrays-ii/

You are given an array `nums`, and integers `m`, `l`, `r`.  
Pick **at least one** and **at most `m`** non‑overlapping sub‑arrays, each with length in `[l, r]`, to maximize the total sum.

---

### Approach 1: Brute Force (Backtracking)

**Algorithm**  
- Recursively decide, starting at each index, whether to start a sub‑array of any allowed length or skip the element.  
- Keep track of the number of sub‑arrays used and the current sum.  
- When the end of the array is reached, update the answer if the number of sub‑arrays is between 1 and *m*.  
- Prune branches that already exceed `m` sub‑arrays.

**Implementation**

```python
class Solution:
    def maximumSum(self, nums: List[int], m: int, l: int, r: int) -> int:
        n = len(nums)
        best = -10**18

        def dfs(i: int, used: int, cur_sum: int):
            nonlocal best
            if i == n:
                if 1 <= used <= m:
                    best = max(best, cur_sum)
                return
            # option: skip nums[i]
            dfs(i + 1, used, cur_sum)
            # option: take a sub‑array starting at i
            for length in range(l, r + 1):
                if i + length > n:
                    break
                sub_sum = sum(nums[i:i + length])   # O(length) – fine for tiny n
                dfs(i + length, used + 1, cur_sum + sub_sum)

        dfs(0, 0, 0)
        return best
```

**Complexity Analysis**

- Time: `O((r‑l+1)^m * n)` in the worst case – exponential, only works for tiny `n`.  
- Space: `O(m)` recursion stack + `O(1)` extra.

---

### Approach 2: DP with Prefix + Monotonic Queue (O(n·m))

**Intuition**  
If we fix the number of sub‑arrays `k`, the optimal solution for the prefix ending at `i` either:
1. does **not** use `i` → inherit `dp[i‑1][k]`, or  
2. ends a sub‑array at `i` → we need the best `dp[j][k‑1]` where the sub‑array is `nums[j … i‑1]` and its length is in `[l, r]`.  

The transition becomes  

```
dp[i][k] = max(dp[i‑1][k],
               max_{len∈[l,r]} ( dp[i‑len][k‑1] + sum(i‑len, i‑1) ))
```

The inner max over lengths can be maintained in **O(1)** amortized time with a monotonic deque that stores candidates `dp[pos][k‑1] - prefix[pos]`.  
We repeat this for every `k = 1 … m`.

**Algorithm**

1. Build prefix sums `pref`.  
2. Initialize `dp[0][0] = 0`, all others `-∞`.  
3. For each `k` from 1 to `m`:
   - Clear a deque.
   - Iterate `i` from 1 to `n`:
     - Add index `i‑l` to the deque (if ≥ 0) while maintaining decreasing order of `value = dp[idx][k‑1] - pref[idx]`.
     - Remove indices `< i‑r` from the front (out of window).
     - `best = dp[i‑1][k]` (skip case).
     - If deque not empty, candidate = `pref[i] + deque[0].value` → update `dp[i][k]`.
4. Answer = max_{k=1..m} dp[n][k].

**Implementation**

```python
from collections import deque
class Solution:
    def maximumSum(self, nums: List[int], m: int, l: int, r: int) -> int:
        n = len(nums)
        pref = [0] * (n + 1)
        for i, v in enumerate(nums):
            pref[i + 1] = pref[i] + v

        NEG = -10**18
        dp = [[NEG] * (m + 1) for _ in range(n + 1)]
        dp[0][0] = 0

        for k in range(1, m + 1):
            dq = deque()
            for i in range(1, n + 1):
                # make i‑l available for future windows
                left = i - l
                if left >= 0:
                    val = dp[left][k - 1] - pref[left]
                    while dq and dq[-1][1] <= val:
                        dq.pop()
                    dq.append((left, val))

                # drop indices that are too far left ( > r length )
                while dq and dq[0][0] < i - r:
                    dq.popleft()

                # skip i‑1
                dp[i][k] = dp[i - 1][k]

                # take a sub‑array ending at i‑1
                if dq:
                    cand = pref[i] + dq[0][1]   # pref[i] + max(dp[j][k-1]-pref[j])
                    if cand > dp[i][k]:
                        dp[i][k] = cand

        return max(dp[n][k] for k in range(1, m + 1))
```

**Complexity Analysis**

- Time: `O(m * n)` – each `k` scans the array once, deque operations are amortized O(1).  
- Space: `O(m * n)` for the DP table (can be reduced to two rows → O(m·n) → O(n) if we keep only previous `k‑1`).

---

### Approach 3: DP + Lagrangian Relaxation + Monotonic Queue (O(n log C))

**Intuition**  
The DP above still has a factor `m` (up to 10⁵).  
Notice that the DP transition is linear in the *number of sub‑arrays* used.  
We can **penalize each chosen sub‑array** by a constant `λ` (lambda) and transform the problem into:

```
max  ( sum of chosen sub‑array sums ) – λ * (number of sub‑arrays)
```

For a fixed `λ`, the penalty makes the objective **additive** over sub‑arrays, and the DP no longer needs to track the count — we only need the best value (which implicitly stores the best count via the penalty).  
We can solve this penalized DP in **O(n)** using the same monotonic‑queue trick as in Approach 2 (now the DP has only one dimension).  
The resulting function `f(λ)` = maximum penalized value is **monotonic decreasing** in `λ`.  
Moreover, if we know the optimal number of sub‑arrays `k*` for the original problem, then:

```
f(λ*) = optimal_sum – λ* * k*
```

where `λ*` is any value such that the penalized solution uses exactly `k*` sub‑arrays (or the closest we can get).  
Thus we can binary‑search on `λ` to find the largest `λ` for which the penalized solution still uses **≥ m** sub‑arrays, then recover the true answer.

**Algorithm**

1. Prefix sums as before.
2. Define a helper `solve(penalty)` that returns a pair `(best_value, count)`:
   - DP[i] = best penalized sum for prefix `i`.
   - cnt[i] = number of sub‑arrays used to achieve DP[i].
   - Transition (same as Approach 2 but with `‑penalty` added each time we take a sub‑array):
        ```
        cand = prefix[i] - penalty + best_{j in [i‑r, i‑l]} ( DP[j] - prefix[j] )
        ```
     The inner max is maintained with a monotonic deque on `DP[j] - prefix[j]`.
   - If `cand` ties DP[i‑1], we keep the larger count (to favor more sub‑arrays when values equal).
3. Binary search `λ` in `[0, 2·10¹⁰]` (enough to exceed any possible element sum).  
   - If `solve(mid).count >= m`, we can try a larger penalty → move `low = mid`.  
   - Else, move `high = mid‑1`.
4. After the search, `low` is the maximum penalty yielding at least `m` sub‑arrays.  
   - Call `solve(low)` to get `(value, cnt)`.  
   - The true maximum sum is `value + low * m` (we subtracted `low` per sub‑array, so add it back for exactly `m` sub‑arrays; if we got more than `m`, the extra penalty cancels out because we used the *largest* penalty still allowing ≥ m).

**Implementation**

```python
from collections import deque
class Solution:
    def maximumSum(self, nums: List[int], m: int, l: int, r: int) -> int:
        n = len(nums)
        pref = [0] * (n + 1)
        for i, v in enumerate(nums):
            pref[i + 1] = pref[i] + v

        NEG = -10**18

        def solve(penalty: int):
            """Return (best_penalized_sum, used_subarrays) for whole array."""
            dp = [NEG] * (n + 1)
            cnt = [0] * (n + 1)
            dp[0] = 0
            dq = deque()          # stores indices with decreasing dp[idx]-pref[idx]

            for i in range(1, n + 1):
                # make i‑l available
                left = i - l
                if left >= 0:
                    val = dp[left] - pref[left]
                    while dq and dq[-1][1] <= val:
                        dq.pop()
                    dq.append((left, val))

                # drop outdated indices (< i‑r)
                while dq and dq[0][0] < i - r:
                    dq.popleft()

                # option 1: skip i‑1
                dp[i] = dp[i - 1]
                cnt[i] = cnt[i - 1]

                # option 2: take a sub‑array ending at i‑1
                if dq:
                    best_j_val = dq[0][1]   # max(dp[j]-pref[j])
                    cand = pref[i] - penalty + best_j_val
                    if cand > dp[i] or (cand == dp[i] and cnt[dq[0][0]] + 1 > cnt[i]):
                        dp[i] = cand
                        cnt[i] = cnt[dq[0][0]] + 1

            return dp[n], cnt[n]

        # first check if we can even take m sub‑arrays (all negative case handled later)
        _, k0 = solve(0)
        if k0 == 0:                     # no positive‑sum sub‑array exists
            # fall back to plain max sub‑array with length in [l,r] (choose at most m)
            best = NEG
            dq = deque()
            for i in range(l, n + 1):
                j = i - l
                while dq and pref[dq[-1]] > pref[j]:
                    dq.pop()
                dq.append(j)
                while dq and dq[0] < i - r:
                    dq.popleft()
                best = max(best, pref[i] - pref[dq[0]])
            return best

        if k0 <= m:                     # already ≤ m sub‑arrays gives optimum
            g0, _ = solve(0)
            return g0

        # binary search on penalty
        low, high = 0, 2 * 10**10       # safe upper bound (|nums[i]| ≤ 1e5, n ≤ 1e5)
        while low < high:
            mid = low + (high - low + 1) // 2
            if solve(mid)[1] >= m:      # we can still take ≥ m sub‑arrays
                low = mid
            else:
                high = mid - 1

        g, _ = solve(low)
        return g + low * m
```

**Complexity Analysis**

- Time: `O(n log C)` where `C` is the search range for the penalty (`~2·10¹⁰` → ~35 iterations). Each `solve` runs in `O(n)` thanks to the monotonic deque.  
- Space: `O(n)` for the DP and count arrays (can be reduced to `O(1)` if we keep only the current value, but we keep them for clarity).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the core of **Approach 3** (penalized DP) on a tiny example to see how the deque maintains the maximum of `dp[j]‑pref[j]`.

#### Example

```
nums = [4, 1, -5, 2]
m = 2, l = 1, r = 3
penalty λ = 0   (first call to solve)
```

Prefix sums: `pref = [0, 4, 5, 0, 2]`

We iterate `i = 1 … 4` and show the deque state before computing `dp[i]`.

| i | nums[i‑1] | left = i‑l | right‑excl = i‑r | Deque (index, dp‑pref) before query | best = max(dp[j]‑pref[j]) | cand = pref[i] − λ + best | dp[i] (skip vs take) | cnt[i] |
|---|-----------|------------|------------------|--------------------------------------|--------------------------|---------------------------|----------------------|--------|
| 1 | 4         | 0          | -2 (none)        | after adding 0: [(0, dp0‑pref0 = 0)]  | 0                        | 0 + 0 + 0 = 0? Actually pref[1]=4 → cand = 4 + 0 = 4 | max(dp[0]=0, 4) = 4 | cnt from j=0 +1 = 1 |
| 2 | 1         | 1          | -1 (none)        | add index 1: dp1‑pref1 = 4‑4=0 → deque [(0,0),(1,0)] (pop back because equal & cnt smaller? we keep both) | max = 0 | pref[2]=5 → cand = 5 + 0 = 5 | max(dp[1]=4,5) =5 | cnt = cnt[0]+1 =1 (from j=0) or cnt[1]+1=2? Actually best j is 0 (same val, choose larger cnt? we favor larger cnt on tie) → cnt = cnt[0]+1 =1 |
| 3 | -5        | 2          | 0                | add index 2: dp2‑pref2 =5‑5=0 → deque [(0,0),(1,0),(2,0)] | remove indices < i‑r =0? none | max =0 | pref[3]=0 → cand =0+0=0 | max(dp[2]=5,0)=5 (skip) | cnt stays 1 |
| 4 | 2         | 3          | 1                | add index 3: dp3‑pref3 =5‑0=5 → pop back while smaller: pop (2,0),(1,0),(0,0) then push (3,5) → deque [(3,5)] | drop indices < i‑r =1 → none | max =5 | pref[4]=2 → cand =2‑0 +5 =7 | max(dp[3]=5,7)=7 (take) | cnt = cnt[3]+1 = cnt[3] from j=3? cnt[3] was 1 → now 2 |

Result: `dp[4]=7`, `cnt[4]=2`. This matches the optimal answer (two sub‑arrays `[4,1]` and `[2]`).

The table shows how the deque always gives the maximum `dp[j]‑pref[j]` for the sliding window `[i‑r, i‑l]`, enabling an O(1) transition per `i`.

--- 

**Summary**

- Start with a brute‑force back‑tracking solution to grasp the problem.  
- Optimize to a classic DP with two dimensions (`position × #subarrays`).  
- Remove the inner length loop using a monotonic queue → O(m·n).  
- Eliminate the `m` dimension via Lagrangian relaxation and binary search on a per‑sub‑array penalty → O(n log C), fast enough for the constraints.  

This lecture covers the full spectrum from intuition to a production‑ready implementation, complete with complexity analysis and a step‑by‑step dry run.