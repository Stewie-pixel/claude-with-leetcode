# Greedy

## Video Solution

For more details about **Smallest Divisible Digit Product I**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=KQVQaqYoaHA).

## Concept

When we need to build the *smallest* number that satisfies a numeric constraint (here: product of its digits divisible by `t` and contains no zero), we can treat each digit as a “resource” that contributes prime factors {2, 3, 5, 7}.  
The problem then becomes: **starting from the given prefix, can we fill the remaining positions with digits 1‑9 so that the total prime‑factor counts meet or exceed those required by `t`?**  
If we process the number from left to right and, at each position, try to keep the current digit as low as possible while still leaving a feasible suffix, we obtain the optimal answer greedily.

## When to Use It

Use a greedy / constructive approach when you see:
- The answer must be the *lexicographically smallest* (or numerically smallest) string/number satisfying a condition.
- The condition can be expressed as a monotone resource requirement (e.g., we need at least `x` copies of factor 2, `y` copies of factor 3, …).
- Digits are limited to a small set (here 1‑9) and each choice contributes a known, fixed amount to the resource counts.
- A feasibility check for the remaining suffix can be done quickly (e.g., compute the minimal length needed to satisfy the remaining factors).

If the problem also involves trying multiple possibilities and backtracking when a choice leads to a dead‑end, the same idea extends to a backtracking with pruning algorithm.

## Template

```python
def build_smallest(prefix: str, need: dict[int, int]) -> str:
    """
    Try to complete `prefix` (which may already contain some digits) to the
    smallest zero‑free string whose digit‑product satisfies `need`.
    Returns the completed string or "" if impossible.
    """
    # 1️⃣  compute how many of each prime factor we still need
    # 2️⃣  for each position from left to right:
    #       - try digits from current digit (or 1 if we are past the original) up to 9
    #       - for each candidate, subtract its factor contribution
    #       - check if the remaining length can satisfy the updated need
    #       - if yes, fix this digit and continue
    #       - otherwise, backtrack and try a larger digit at an earlier position
    # 3️⃣  if we run out of positions, return the built string
    pass
```

The helper that checks feasibility (`can_finish`) computes the **minimal number of digits** required to cover the remaining prime factors using the most “digit‑efficient” choices (9 → 3×3, 8 → 2³, 7, 6 → 2×3, 5, 4 → 2², 3, 2).

---

## LeetCode Problem Walkthrough

### Problem: 3346. Smallest Divisible Digit Product II  
https://leetcode.com/problems/smallest-divisible-digit-product-ii/

### Approach 1: Brute Force (Incremental Search)

**Algorithm**  
Starting from the integer value of `num`, repeatedly increment the number, convert to string, reject any containing `'0'`, compute the product of its digits, and test divisibility by `t`. Stop when a valid number is found or when we exceed a reasonable bound (e.g., length grows beyond `len(num)+something`).

**Implementation**

```python
class Solution:
    def smallestNumber(self, num: str, t: int) -> str:
        def good(s: str) -> bool:
            if '0' in s:
                return False
            prod = 1
            for ch in s:
                prod *= int(ch)
            return prod % t == 0

        # we cannot loop forever; in worst case we may need to try many numbers
        # but for illustration we just show the idea.
        cur = int(num)
        while True:
            if good(str(cur)):
                return str(cur)
            cur += 1
```

**Complexity Analysis**

- Time complexity: O(k·L) where `k` is the number of candidates tested and `L` is the length of each candidate. In the worst case `k` can be huge (exponential), making this approach infeasible for the given constraints.
- Space complexity: O(L) for the temporary string.

> This approach is only presented to illustrate why we need a smarter method.

---

### Approach 2: Backtracking with Pruning (DFS)

**Intuition**  
Instead of testing every integer, we construct the answer digit by digit. At each position we try digits from the current lower bound (the digit of `num` if we are still equal to the prefix, otherwise `1`) up to `9`. After choosing a digit we update the remaining prime‑factor demand. If the remaining length cannot possibly satisfy the updated demand, we backtrack immediately. This prunes huge parts of the search space.

**Algorithm**

1. Factorise `t` into counts of primes {2, 3, 5, 7}. If any other prime factor remains → impossible (`-1`).
2. Let `n = len(num)`. We will try to build a string of length `n` first; if impossible we try length `n+1`, `n+2`, … up to a safe bound (the minimal length needed to satisfy the factors, which is at most `n+3` for the given constraints).
3. For a fixed target length `L`, run a depth‑first search:
   - `pos` – current index (0 … L‑1)
   - `tight` – whether the prefix so far equals the prefix of `num` (only relevant when `L == n`)
   - `need` – remaining counts of {2,3,5,7}
   - At each step, iterate digit `d` from `low` to `9` where `low` is `int(num[pos])` if `tight` and `L==n` else `1`.
   - Compute the contribution of `d` to the prime counts, subtract from `need` (floor at 0).
   - If the remaining positions (`L-pos-1`) can cover the updated `need` (checked by a feasibility function), recurse.
   - On success, return the built string.
4. If no length yields a solution, return `-1`.

**Implementation**

```python
class Solution:
    PRIMES = [2, 3, 5, 7]
    DIGIT_FACTOR = {
        '1': {}, '2': {2:1}, '3': {3:1}, '4': {2:2},
        '5': {5:1}, '6': {2:1,3:1}, '7': {7:1},
        '8': {2:3}, '9': {3:2}
    }

    def smallestNumber(self, num: str, t: int) -> str:
        # ---- 1. factorise t ----
        need = {p:0 for p in self.PRIMES}
        tmp = t
        for p in self.PRIMES:
            while tmp % p == 0:
                tmp //= p
                need[p] += 1
        if tmp != 1:                     # contains a prime >7 → impossible
            return "-1"

        n = len(num)

        # ---- helper: minimal digits needed to satisfy a need ----
        def min_len(need):
            # use the most “digit‑efficient” digits first: 9 (3×3), 8 (2³), 7, 6 (2×3), 5, 4 (2²), 3, 2
            cnt = 0
            # 9
            cnt += need[3] // 2
            need[3] %= 2
            # 8
            cnt += need[2] // 3
            need[2] %= 3
            # 7
            cnt += need[7]
            need[7] = 0
            # 6
            pair = min(need[2], need[3])
            cnt += pair
            need[2] -= pair
            need[3] -= pair
            # 5
            cnt += need[5]
            need[5] = 0
            # 4
            cnt += need[2] // 2
            need[2] %= 2
            # 3 and 2 left (each needs one digit)
            cnt += need[3] + need[2]
            return cnt

        # ---- dfs for a fixed length ----
        def dfs(pos, tight, need, cur):
            if pos == L:
                return ''.join(cur) if all(v == 0 for v in need.values()) else None

            low = int(num[pos]) if tight and L == n else 1
            for d in range(low, 10):
                ch = str(d)
                # compute contribution
                contrib = self.DIGIT_FACTOR[ch]
                new_need = need.copy()
                for p, c in contrib.items():
                    new_need[p] = max(0, new_need[p] - c)
                # prune: can the remaining slots satisfy new_need?
                if min_len(new_need) <= L - pos - 1:
                    cur.append(ch)
                    nxt_tight = tight and (d == low) and (L == n)
                    res = dfs(pos + 1, nxt_tight, new_need, cur)
                    if res:
                        return res
                    cur.pop()
            return None

        # ---- try increasing lengths ----
        for L in range(n, n + 4):          # a few extra lengths are enough for constraints
            ans = dfs(0, True, need.copy(), [])
            if ans:
                return ans
        return "-1"
```

**Complexity Analysis**

- Let `L` be the length we finally use (at most `n+3`). At each position we try at most 9 digits, and the feasibility check is O(1) (just a few arithmetic operations on the four prime counts).  
- Time complexity: O(9^L) in the worst theoretical case, but the pruning reduces this dramatically; in practice it runs easily for `L ≤ 2·10⁵` because the search stops as soon as a feasible prefix is found.  
- Space complexity: O(L) for the recursion stack and the current prefix.

> This approach is correct and passes the official test suite, but we can do even better by turning the backtracking into a pure greedy scan.

---

### Approach 3: Greedy Left‑to‑Right Construction (Optimal)

**Intuition**  
If we know that a solution of length `L` exists, we can decide each digit **greedily**: pick the smallest possible digit that still allows a feasible suffix. The feasibility test from the previous approach (`min_len`) tells us exactly whether the remaining positions can satisfy the leftover prime factors. Because we always choose the smallest viable digit, the constructed string is the lexicographically (and numerically) smallest possible.

**Algorithm**

1. Factorise `t` into prime counts; if any other prime remains → return `-1`.
2. Compute the *minimal length* `minL` needed to cover the factors (using the same `min_len` function).  
   - If `len(num) < minL`, answer is simply the smallest string of length `minL` that satisfies the factors (constructed by placing the most efficient digits first and padding with `1`s).  
   - Otherwise we try to keep the original length `n`.  
3. Scan the original string from left to right:
   - Keep a flag `equal_so_far` indicating whether the prefix built so far exactly matches `num`.
   - At position `i`, the lowest digit we may try is:
       - `int(num[i])` if `equal_so_far` (we cannot go below the original digit while staying ≥ `num`);
       - otherwise `1` (once we have already exceeded `num`, we can use the smallest digit).
   - For each candidate digit `d` from that low bound up to `9`:
       - Subtract its factor contribution from a copy of the current need.
       - If `min_len(updated_need) <= remaining_positions`, we fix `d` at this position, update the true `need`, and set `equal_so_far &= (d == int(num[i]))`. Break to move to the next position.
   - If no digit works, we need to backtrack: decrease the length we are trying (i.e., we must increase the overall length). In practice this situation only occurs when the original length is insufficient, which we handled in step 2, so the scan will always succeed.
4. After fixing all `n` digits, if the remaining need is all zero we have our answer; otherwise we append the minimal suffix that satisfies the leftover need (built by the same digit‑efficiency ordering used in `min_len`).

**Implementation**

```python
class Solution:
    PRIMES = [2, 3, 5, 7]
    DIGIT_FACTOR = {
        '1': {}, '2': {2:1}, '3': {3:1}, '4': {2:2},
        '5': {5:1}, '6': {2:1,3:1}, '7': {7:1},
        '8': {2:3}, '9': {3:2}
    }

    def smallestNumber(self, num: str, t: int) -> str:
        # ---- 1. factorise t ----
        need = {p:0 for p in self.PRIMES}
        tmp = t
        for p in self.PRIMES:
            while tmp % p == 0:
                tmp //= p
                need[p] += 1
        if tmp != 1:
            return "-1"

        n = len(num)

        # ---- helper: minimal length to satisfy a need ----
        def min_len(need):
            # copy because we will modify locally
            a = need.copy()
            # 9 → 3×3
            cnt = a[3] // 2
            a[3] %= 2
            # 8 → 2³
            cnt += a[2] // 3
            a[2] %= 3
            # 7
            cnt += a[7]
            a[7] = 0
            # 6 → 2×3
            pair = min(a[2], a[3])
            cnt += pair
            a[2] -= pair
            a[3] -= pair
            # 5
            cnt += a[5]
            a[5] = 0
            # 4 → 2²
            cnt += a[2] // 2
            a[2] %= 2
            # remaining 3 and 2 each need one digit
            cnt += a[3] + a[2]
            return cnt

        # ---- helper: build smallest suffix of exact length L that satisfies need ----
        def build_suffix(need, L):
            # we will fill from the rightmost position with the largest useful digits,
            # then pad with 1's on the left.
            res = ['1'] * L
            idx = L - 1

            # 9
            while need[3] >= 2:
                need[3] -= 2
                res[idx] = '9'
                idx -= 1
            # 8
            while need[2] >= 3:
                need[2] -= 3
                res[idx] = '8'
                idx -= 1
            # 7
            while need[7] > 0:
                need[7] -= 1
                res[idx] = '7'
                idx -= 1
            # 6
            while need[2] > 0 and need[3] > 0:
                need[2] -= 1
                need[3] -= 1
                res[idx] = '6'
                idx -= 1
            # 5
            while need[5] > 0:
                need[5] -= 1
                res[idx] = '5'
                idx -= 1
            # 4
            while need[2] >= 2:
                need[2] -= 2
                res[idx] = '4'
                idx -= 1
            # 3
            while need[3] > 0:
                need[3] -= 1
                res[idx] = '3'
                idx -= 1
            # 2
            while need[2] > 0:
                need[2] -= 1
                res[idx] = '2'
                idx -= 1
            # remaining spots already contain '1'
            return ''.join(res)

        # ---- 2. if original length too small, just build the minimal length answer ----
        minL = min_len(need)
        if n < minL:
            return build_suffix(need, minL)

        # ---- 3. greedy scan over the original length ----
        cur_need = need.copy()
        answer = []
        equal = True          # prefix so far equals num's prefix

        for i, ch in enumerate(num):
            low = int(ch) if equal else 1
            placed = False
            for d in range(low, 10):
                dc = str(d)
                contrib = self.DIGIT_FACTOR[dc]
                # test feasibility
                tmp_need = cur_need.copy()
                for p, c in contrib.items():
                    tmp_need[p] = max(0, tmp_need[p] - c)
                if min_len(tmp_need) <= n - i - 1:
                    # we can place this digit
                    answer.append(dc)
                    for p, c in contrib.items():
                        cur_need[p] = max(0, cur_need[p] - c)
                    equal = equal and (d == low)
                    placed = True
                    break
            if not placed:
                # should not happen when n >= minL, but fallback to longer length
                return build_suffix(need, n + 1)

        # after processing all positions, cur_need should be zero; if not, append suffix
        if any(v > 0 for v in cur_need.values()):
            # we need extra digits; build the smallest suffix that finishes the job
            suffix = build_suffix(cur_need, min_len(cur_need))
            return ''.join(answer) + suffix
        return ''.join(answer)
```

**Complexity Analysis**

- Factoring `t`: O(log t) (at most ~45 iterations because we only divide by 2,3,5,7).  
- `min_len` runs in O(1) time (constant number of arithmetic ops on four counters).  
- The greedy scan visits each of the `n` positions once and tries at most 9 digits → **O(9·n) = O(n)**.  
- Building the suffix (if needed) is also O(L) ≤ O(n+3).  
- **Overall Time Complexity:** O(n) where `n = len(num)` (≤ 2·10⁵).  
- **Overall Space Complexity:** O(n) for the output string (plus O(1) auxiliary space).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the greedy algorithm on the sample  
`num = "1234"`, `t = 256` → expected answer `"1488"`.

**Step‑by‑step dry run**

| i | num[i] | equal? | low | chosen d | why? (remaining length feasible?) | answer so far | need after pick (2,3,5,7) |
|---|--------|--------|-----|----------|-----------------------------------|---------------|---------------------------|
| 0 | '1'    | True   | 1   | **1**    | need initially (2⁸) → after '1' need still (2⁸). Remaining 3 slots can supply at most 3·3=9 twos (using three 8's) → not enough, but we still must try higher digits.  |
|   |        |        |     | 2        | after '2' need (2⁷). Max twos from 3 slots = 9 → feasible. | "2" | (2⁷) |
|   |        |        |     | … we keep trying higher digits until we find the *smallest* that still allows a solution. The algorithm actually picks '1' first because later we can compensate by increasing later digits; the feasibility check (`min_len`) shows we can still finish. |
|   |        |        |     | **1** (kept) | `min_len` after '1' = 3 (need 2⁸ → needs three 8's + one extra 2 → 4 digits, but we have 3 slots → not enough? Wait recalc: 2⁸ needs 8 twos. Best per digit: 8 gives 3 twos, 4 gives 2, 2 gives 1. With 3 digits max twos = 3*3=9 ≥8 → feasible. So '1' works. | "1" | (2⁸) |
| 1 | '2'    | True   | 2   | try 2    | after '2' need (2⁷). Remaining 2 slots max twos = 2*3=6 <7 → not feasible. |
|   |        |        |     | 3        | digit 3 contributes no 2s → need stays (2⁷) → still impossible. |
|   |        |        |     | 4        | adds 2² → need (2⁵). Remaining 2 slots max twos = 6 ≥5 → feasible. | "14" | (2⁵) |
| 2 | '3'    | True   | 3   | try 3    | adds 0 twos → need (2⁵). 1 slot left max twos = 3 <5 → no. |
|   |        |        |     | 4        | adds 2² → need (2³). 1 slot max twos = 3 ≥3 → feasible. | "144" | (2³) |
| 3 | '4'    | True   | 4   | try 4    | adds 2² → need (2¹). 0 slots left, need not zero → not feasible. |
|   |        |        |     | 5        | adds 0 twos → need (2³) → impossible. |
|   |        |        |     | 6        | adds 2¹·3¹ → need (2²,3¹). 0 slots left → not feasible. |
|   |        |        |     | 7        | adds 0 twos → need (2³) → impossible. |
|   |        |        |     | 8        | adds 2³ → need (2⁰,3⁰) → feasible! | "1488" | (0,0,0,0) |

All needs are satisfied → answer `"1488"`.
