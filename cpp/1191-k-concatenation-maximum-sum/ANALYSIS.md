# Prefix Sum

## Video Solution

For more details about **K-Concatenation Maximum Sum**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=_6WhG9L-514).

## Concept

When an array is repeated `k` times, a maximum‑sum subarray can lie entirely inside one copy, or it can start in a suffix of one copy, span several whole copies, and end in a prefix of another copy.  
If we know:

* `kadane(arr)` – the best subarray inside a single copy  
* `maxPrefix` – the maximum sum of a prefix of `arr`  
* `maxSuffix` – the maximum sum of a suffix of `arr`  
* `total` – the sum of the whole array  

then for `k ≥ 2` the answer is either:

* `kadane(arr)` (best inside one copy)  
* `maxPrefix + maxSuffix` (best that uses a suffix of one copy and a prefix of the next)  
* `maxPrefix + maxSuffix + (k‑2)·total` (when `total > 0` we can add whole copies in the middle)

Thus the problem reduces to a few linear scans – a classic **prefix‑sum**‑style trick.

## When to Use It

Look for these clues in the problem statement:

* The array is **repeated** or **concatenated** a given number of times.  
* We need the **maximum subarray sum** (Kadane‑style) in the resulting array.  
* The answer may be huge → modulo required.  

When you see “repeat the array k times” + “maximum subarray sum”, think **prefix/suffix + total sum** (a prefix‑sum technique).

## Template

```python
def k_concatenation_max_sum(arr, k):
    MOD = 10**9 + 7

    # 1️⃣ Kadane on a single copy (max subarray inside one block)
    def kadane(a):
        cur = best = 0
        for x in a:
            cur = max(0, cur + x)
            best = max(best, cur)
        return best

    # 2️⃣ total sum of the array
    total = sum(arr)

    # 3️⃣ maximum prefix sum
    pref = cur = 0
    for x in arr:
        cur += x
        pref = max(pref, cur)

    # 4️⃣ maximum suffix sum
    suff = cur = 0
    for x in reversed(arr):
        cur += x
        suff = max(suff, cur)

    one = kadane(arr)                     # best inside a single copy
    two = pref + suff                     # best that uses suffix + prefix

    if k == 1:
        return one % MOD
    if total <= 0:
        return max(one, two) % MOD
    # total > 0 → we can add whole copies in the middle
    return (two + (k - 2) * total) % MOD
```

## LeetCode Problem Walkthrough

### Problem: 1191. K-Concatenation Maximum Sum

https://leetcode.com/problems/k-concatenation-maximum-sum/

### Approach 1: Brute Force

**Algorithm**  
1. Build the explicit repeated array `b = arr * k`.  
2. Run Kadane’s algorithm on `b` to obtain the maximum subarray sum.  
3. Return the result modulo `10⁹+7`.

**Implementation**

```python
def k_concatenation_max_sum_brute(arr, k):
    MOD = 10**9 + 7
    repeated = arr * k                     # O(n*k) memory, may overflow
    cur = best = 0
    for x in repeated:
        cur = max(0, cur + x)
        best = max(best, cur)
    return best % MOD
```

**Complexity Analysis**

- Time complexity: O(n·k) — we iterate over the concatenated array of length `n·k`.  
- Space complexity: O(n·k) — we store the repeated array (can be reduced to O(1) extra if we stream, but time remains O(n·k)).

### Approach 2: Kadane on Two Copies + Total Sum

**Intuition**  
If `k ≥ 2`, any optimal subarray either:
- stays inside one copy → Kadane on `arr`, or  
- starts in a suffix of one copy and ends in a prefix of another → we only need to examine at most two copies because extra whole copies in the middle contribute only their total sum.  
Thus we can compute the answer from `arr`, `arr+arr`, and the total sum.

**Algorithm**  
1. Compute `kadane(arr)`.  
2. Compute `kadane(arr + arr)` (two copies).  
3. If `k == 1` → answer from step 1.  
4. Else if `total ≤ 0` → answer is max(step 1, step 2) because adding whole copies does not help.  
5. Else (`total > 0`) → answer = `kadane(arr + arr) + (k‑2)·total`.

**Implementation**

```python
def k_concatenation_max_sum_two(arr, k):
    MOD = 10**9 + 7

    def kadane(a):
        cur = best = 0
        for x in a:
            cur = max(0, cur + x)
            best = max(best, cur)
        return best

    total = sum(arr)
    one = kadane(arr)
    two = kadane(arr + arr)          # at most 2n length

    if k == 1:
        return one % MOD
    if total <= 0:
        return max(one, two) % MOD
    return (two + (k - 2) * total) % MOD
```

**Complexity Analysis**

- Time complexity: O(n) — we scan `arr` once for `total` and twice for Kadane (on `arr` and `arr+arr`).  
- Space complexity: O(1) — only a few variables; the concatenation `arr+arr` creates a new list of size `2n`, but we can avoid it by iterating twice over `arr`. (The shown code uses the extra list for clarity; the optimal version below removes it.)

### Approach 3: Prefix / Suffix + Total (Optimal O(n) time, O(1) space)

**Intuition**  
The best subarray that uses more than one copy must consist of:
- a **suffix** of some copy,
- zero or more **whole** copies,
- a **prefix** of another copy.  

If the total sum of one copy is positive, we want to include as many whole copies as possible (`k‑2` of them when we already take a suffix and a prefix). If the total is non‑positive, adding whole copies never improves the sum, so we only need the best suffix+prefix combination.  
Thus we compute:
- `maxPrefix` – maximum prefix sum,
- `maxSuffix` – maximum suffix sum,
- `total` – sum of the whole array,
- `kadane(arr)` – best inside a single copy.

**Algorithm**  
1. Run Kadane to get `single = kadane(arr)`.  
2. Compute `total`, `maxPrefix`, `maxSuffix` in three linear passes (or one pass with running sums).  
3. If `k == 1` → return `single`.  
4. If `total ≤ 0` → answer = `max(single, maxPrefix + maxSuffix)`.  
5. Else → answer = `maxPrefix + maxSuffix + (k‑2)·total`.  
6. Return answer modulo `10⁹+7`.

**Implementation**

```python
def k_concatenation_max_sum(arr, k):
    MOD = 10**9 + 7

    # ---------- Kadane (single copy) ----------
    cur = best = 0
    for x in arr:
        cur = max(0, cur + x)
        best = max(best, cur)
    single = best

    # ---------- total, max prefix, max suffix ----------
    total = 0
    pref = cur = 0
    for x in arr:
        cur += x
        total += x
        pref = max(pref, cur)

    suff = cur = 0
    for x in reversed(arr):
        cur += x
        suff = max(suff, cur)

    # ---------- combine according to k ----------
    if k == 1:
        return single % MOD
    if total <= 0:
        return max(single, pref + suff) % MOD
    # total > 0 → we can add whole copies in the middle
    return (pref + suff + (k - 2) * total) % MOD
```

**Complexity Analysis**

- Time complexity: O(n) — one pass for Kadane, one for total/prefix, one for suffix (still linear).  
- Space complexity: O(1) — only a handful of integer variables.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

We walk through the optimal algorithm on **Example 2**:

```
arr = [1, -2, 1], k = 5
```

| Step | Variable | Value after processing element |
|------|----------|--------------------------------|
| Init | curKadane = 0, bestKadane = 0 |
| x=1  | curKadane = max(0,0+1)=1, bestKadane=1 |
| x=-2 | curKadane = max(0,1-2)=0, bestKadane=1 |
| x=1  | curKadane = max(0,0+1)=1, bestKadane=1 |
| **single (kadane)** | **bestKadane = 1** |

```
total = 1 + (-2) + 1 = 0
```

**Prefix scan**

| i | x | curPref | maxPref |
|---|---|---------|---------|
|0|1|1|1|
|1|-2| -1 → reset? we keep running sum, but maxPref stays 1|1|
|2|1|0|1|

→ `maxPref = 1`

**Suffix scan** (iterate reversed)

| i | x | curSuf | maxSuf |
|---|---|--------|--------|
|2|1|1|1|
|1|-2|0|1|
|0|1|1|1|

→ `maxSuf = 1`

Now apply the formula (`k = 5 > 1`, `total = 0 ≤ 0`):

```
answer = max(single, maxPref + maxSuf) = max(1, 1+1) = 2
```

Return `2 % MOD = 2`, matching the expected output.

---  

*End of lecture.*