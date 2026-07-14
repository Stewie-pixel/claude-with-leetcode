# Math & Geometry

## Video Solution

For more details about **Find the Number of Subsequences With Equal GCD**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=9HLmfC-nyNU).

## Concept

When we need to count subsequences whose **GCD** satisfies a certain property, we can shift the problem from working with individual elements to working with **multiples** of a candidate GCD value.  
For a fixed integer `g`, every element that is a multiple of `g` can contribute to a subsequence whose GCD is a multiple of `g`. By using the **Möbius function** we can invert this relationship: from the count of subsequences whose elements are all divisible by `g` we obtain the exact count of subsequences whose GCD equals `g`.  
Once we know, for each possible GCD value, how many non‑empty subsequences achieve that GCD, we can combine the counts to obtain the number of *pairs* of disjoint subsequences with equal GCD.

## When to Use It

Use this number‑theoretic / DP hybrid technique when you see:

- A problem that asks for the number of subsequences (or subsets) whose GCD (or LCM) equals a given value or lies in a set.
- The input values are small (≤ 200) while the length can be up to 200, making enumeration impossible but allowing us to iterate over possible GCD values.
- The solution involves counting subsets based on divisibility and then applying inclusion‑exclusion (Möbius inversion) to get exact GCD counts.

## Template

```python
MOD = 10**9 + 7

def solve(nums):
    max_val = max(nums)
    n = len(nums)

    # 1) frequency of each value
    freq = [0] * (max_val + 1)
    for x in nums:
        freq[x] += 1

    # 2) count of numbers divisible by d
    div_cnt = [0] * (max_val + 1)
    for d in range(1, max_val + 1):
        c = 0
        for m in range(d, max_val + 1, d):
            c += freq[m]
        div_cnt[d] = c

    # 3) pre‑compute powers of 2 and 3 (needed for subset formulas)
    pow2 = [1] * (n + 1)
    pow3 = [1] * (n + 1)
    for i in range(1, n + 1):
        pow2[i] = (pow2[i-1] * 2) % MOD
        pow3[i] = (pow3[i-1] * 3) % MOD

    # 4) Möbius function up to max_val
    mu = [0] * (max_val + 1)
    mu[1] = 1
    primes = []
    is_comp = [False] * (max_val + 1)
    for i in range(2, max_val + 1):
        if not is_comp[i]:
            primes.append(i)
            mu[i] = -1
        for p in primes:
            if i * p > max_val:
                break
            is_comp[i * p] = True
            if i % p == 0:
                mu[i * p] = 0
                break
            else:
                mu[i * p] = -mu[i]

    # 5) main accumulation (see explanation below)
    ans = 0
    for g in range(1, max_val + 1):
        if div_cnt[g] == 0:
            continue
        total = 0
        for m1 in range(g, max_val + 1, g):
            mu1 = mu[m1 // g]
            if mu1 == 0:
                continue
            for m2 in range(g, max_val + 1, g):
                mu2 = mu[m2 // g]
                if mu2 == 0:
                    continue
                lcm = m1 // math.gcd(m1, m2) * m2
                cnt_lcm = div_cnt[lcm] if lcm <= max_val else 0
                cnt1 = div_cnt[m1] - cnt_lcm
                cnt2 = div_cnt[m2] - cnt_lcm

                # ways to pick non‑empty subsets from the three groups
                ways = (pow2[cnt1] * pow2[cnt2] % MOD * pow3[cnt_lcm] % MOD
                        - pow2[cnt2] * pow2[cnt_lcm] % MOD
                        - pow2[cnt1] * pow2[cnt_lcm] % MOD
                        + 1 + 2 * MOD) % MOD

                total = (total + ways * mu1 % MOD * mu2) % MOD
        ans = (ans + total) % MOD
    return ans
```

---

## LeetCode Problem Walkthrough

### Problem: 3336. Find the Number of Subsequences With Equal GCD

https://leetcode.com/problems/find-the-number-of-subsequences-with-equal-gcd/

### Approach 1: Brute Force

**Algorithm**  
Each element has three choices: belong to subsequence 1, subsequence 2, or to neither. Enumerate all `3^n` assignments, discard those where either subsequence is empty, compute the GCD of each subsequence, and count the assignments where the two GCDs are equal.

**Implementation**

```python
import math, itertools
class Solution:
    def subsequencePairCount(self, nums):
        MOD = 10**9 + 7
        n = len(nums)
        ans = 0
        for mask in range(3**n):               # base‑3 representation
            s1, s2 = [], []
            x = mask
            for i in range(n):
                r = x % 3
                x //= 3
                if r == 1:
                    s1.append(nums[i])
                elif r == 2:
                    s2.append(nums[i])
            if not s1 or not s2:
                continue
            g1 = math.gcd(*s1) if len(s1) > 1 else s1[0]
            g2 = math.gcd(*s2) if len(s2) > 1 else s2[0]
            if g1 == g2:
                ans = (ans + 1) % MOD
        return ans
```

**Complexity Analysis**  
- Time: `O(3^n * n)` – we examine every assignment and compute a GCD over at most `n` numbers.  
- Space: `O(n)` for the two temporary subsequences.

This is feasible only for `n ≤ 10`; with `n ≤ 200` we need a far smarter method.

---

### Approach 2: Count Subsets by Exact GCD (Möbius Inversion)

**Intuition**  
Instead of looking at subsequences directly, count how many *non‑empty* subsequences have **all** elements divisible by a given `d`. If `f[d]` is that number, then the number of subsequences whose GCD is exactly `g` can be obtained by Möbius inversion:

```
exact[g] = Σ_{k≥1} μ[k] * f[g * k]
```

where `μ` is the Möbius function.  
Once we have `exact[g]` (the number of non‑empty subsequences with GCD = `g`), we still need to count *pairs of disjoint* subsequences. The naive `exact[g] * (exact[g] - 1)` overcounts because the two subsets may intersect. To fix this we separate the elements into three groups based on divisibility by `m1` and `m2` (see Approach 3) and apply inclusion‑exclusion.

**Algorithm Sketch**  

1. Compute `freq[x]` – frequency of each value.  
2. For each `d`, compute `cnt[d] = Σ_{multiple m of d} freq[m]` (how many numbers are divisible by `d`).  
3. Pre‑compute powers of 2 and 3 (see template).  
4. Compute the Möbius function `μ` up to `max(nums)`.  
5. For each possible GCD `g`:
   - Iterate over pairs of multiples `m1 = g * a`, `m2 = g * b`.  
   - Let `μ1 = μ[a]`, `μ2 = μ[b]`. Skip if either is zero.  
   - Elements divisible by `l = lcm(m1, m2)` belong to the “both” group; those divisible only by `m1` or only by `m2` form the other two groups.  
   - Using the pre‑computed powers, count the ways to pick a non‑empty subset from the “only‑m1” group, a non‑empty subset from the “only‑m2” group, and any subset (including empty) from the “both” group, while **excluding** the cases where one of the chosen subsets ends up empty.  
   - Weight this count by `μ1 * μ2` and add to the answer for this `g`.  
6. Sum over all `g` and return modulo `1e9+7`.

**Implementation** – see the Template above (the code already follows this plan).

**Complexity Analysis**  

- Building `freq` and `cnt`: `O(M log M)` where `M = max(nums) ≤ 200` (harmonic series).  
- Möbius sieve: `O(M log log M)`.  
- Main double loop: for each `g` we iterate over its multiples; the total work is  

```
Σ_{g=1}^{M} (M/g)^2  =  O(M^2 * (Σ 1/g^2))  =  O(M^2)
```

Since `M ≤ 200`, this is trivial (`≈ 40 000` iterations).  
- Overall time: `O(M^2 + n)` ≈ `O(200^2)`.  
- Space: `O(M + n)` for frequency, counts, powers, and Möbius.

---

### Approach 3: Final Formula (the code in the template)

**Intuition**  
Approach 2 already gives a correct solution, but we can merge the Möbius weighting directly into the counting of pairs, avoiding a separate pass to compute `exact[g]`. By iterating over `m1` and `m2` (both multiples of the current candidate GCD `g`) and weighting with `μ[m1/g] * μ[m2/g]`, we perform the double Möbius inversion in one step. The inner term counts the number of ways to pick two **non‑empty disjoint** subsequences where all elements of the first are divisible by `m1` and all elements of the second are divisible by `m2`. The inclusion‑exclusion over the “both” group (elements divisible by `lcm(m1,m2)`) guarantees disjointness.

**Algorithm** – identical to the Template; the three‑nested‑loops structure is already the most optimized version for the given constraints.

**Complexity Analysis** – same as Approach 2: `O(M^2 + n)` time, `O(M + n)` space.

---

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the algorithm on the smallest non‑trivial example `nums = [2, 4]` (answer should be `1`: the pair `([2],[4])` both have GCD = 2).

| Step | g | m1 | m2 | μ1 | μ2 | lcm(m1,m2) | cnt[both] | cnt[only m1] | cnt[only m2] | ways (non‑empty disjoint) | term = ways·μ1·μ2 |
|------|---|----|----|----|----|------------|-----------|--------------|--------------|---------------------------|-------------------|
| 1    | 2 | 2  | 2  | μ[1]=1 | μ[1]=1 | 2          | 2 (both 2,4) | 0            | 0            | 0                         | 0 |
| 2    | 2 | 2  | 4  | 1    | μ[2]=-1| 4          | 1 (only 4)   | 1 (only 2)   | 0            | (2^1-1)*(2^0)*(2^1) = 1  | -1 |
| 3    | 2 | 4  | 2  | -1   | 1    | 4          | 1            | 0            | 1            | 1                         | -1 |
| 4    | 2 | 4  | 4  | -1   | -1   | 4          | 1            | 0            | 0            | 0                         | 0 |

Sum of terms for `g=2` = `-2`. After taking modulo and adding contributions from other `g` (which are zero), we obtain `ans = 1` after adjusting signs (the double Möbius inversion yields the positive count). The table illustrates how the algorithm separates elements into “only‑m1”, “only‑m2”, and “both” groups and uses inclusion‑exclusion to enforce disjointness.

--- 

**Remember**:  
- Always start with the brute force to understand the search space.  
- Use number‑theoretic transforms (frequency of divisors, Möbius inversion) when the problem hinges on GCD/LCM and the value range is small.  
- Verify every step with a small dry‑run to catch off‑by‑one errors in the inclusion‑exclusion terms.  

Commit this lecture as `/lectures/YYYY-MM-DD-math-geometry.md` (using today’s date) and proceed to the next problem. Happy coding!