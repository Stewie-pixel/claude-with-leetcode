# Bit Manipulation

## Video Solution

For more details about **Counting Bits**, watch the walkthrough at [https://www.youtube.com/watch?v=RyBM56RIWrM](https://www.youtube.com/watch?v=RyBM56RIWrM)

## Concept

Counting the number of `1` bits in the binary representation of numbers is a classic bit‑manipulation task.  
Instead of converting each number to a string and counting characters, we can exploit the relationship between a number and its half (right‑shifted) value: the binary form of `i` is the same as `i >> 1` with an extra least‑significant bit equal to `i & 1`. This lets us build the answer for all `0 … n` in a single linear pass.

## When to Use It

Use bit‑manipulation tricks like this when you see:
- Problems asking for the number of set bits, parity, or binary patterns.
- Constraints up to `10^5` or `10^6` where an `O(n log n)` per‑element bit scan would be too slow.
- A recurrence that relates `f(i)` to `f(i>>1)` or `f(i & (i-1))`.

## Template

```python
def count_bits(n: int) -> list[int]:
    # dp[i] will hold the answer for i
    dp = [0] * (n + 1)
    dp[0] = 0                     # base case

    for i in range(1, n + 1):
        # Relation: dp[i] = dp[i >> 1] + (i & 1)
        dp[i] = dp[i >> 1] + (i & 1)

    return dp
```

---

## LeetCode Problem Walkthrough

### Problem: 338. Counting Bits

https://leetcode.com/problems/counting-bits/

### Approach 1: Brute Force

**Algorithm**  
For each number `i` from `0` to `n`, repeatedly examine its least‑significant bit, add it to a running count, and right‑shift `i` until it becomes zero. Store the count in the result array.

**Implementation**

```python
class Solution:
    def countBits(self, n: int) -> List[int]:
        ans = []
        for i in range(n + 1):
            cnt = 0
            x = i
            while x:
                cnt += x & 1      # add 1 if LSB is set
                x >>= 1           # shift right
            ans.append(cnt)
        return ans
```

**Complexity Analysis**

- Time complexity: O(n log n) — each number costs at most `log2(i)` iterations.  
- Space complexity: O(1) extra (output array not counted).

### Approach 2: DP using `i >> 1` and `i & 1`

**Intuition**  
The binary representation of `i` is identical to that of `i >> 1` (i divided by 2) with an extra bit equal to `i & 1` at the least‑significant position. Therefore the number of `1`s in `i` equals the number of `1`s in `i >> 1` plus that extra bit.

**Algorithm**  
Initialize `dp[0] = 0`. For each `i` from `1` to `n`, compute `dp[i] = dp[i >> 1] + (i & 1)` and store it.

**Implementation**

```python
class Solution:
    def countBits(self, n: int) -> List[int]:
        dp = [0] * (n + 1)
        dp[0] = 0
        for i in range(1, n + 1):
            dp[i] = dp[i >> 1] + (i & 1)
        return dp
```

**Complexity Analysis**

- Time complexity: O(n) — one constant‑time operation per i.  
- Space complexity: O(n) — the dp array of size n+1.

### Approach 3: DP using `i & (i-1)` (Brian Kernighan’s trick)

**Intuition**  
The operation `i & (i-1)` clears the lowest set bit of `i`. Thus `i` has exactly one more set bit than `i & (i-1)`. This yields another O(1) recurrence: `dp[i] = dp[i & (i-1)] + 1`.

**Algorithm**  
Set `dp[0] = 0`. For each `i` from `1` to `n`, compute `dp[i] = dp[i & (i-1)] + 1`.

**Implementation**

```python
class Solution:
    def countBits(self, n: int) -> List[int]:
        dp = [0] * (n + 1)
        for i in range(1, n + 1):
            dp[i] = dp[i & (i - 1)] + 1
        return dp
```

**Complexity Analysis**

- Time complexity: O(n) — each step does O(1) work.  
- Space complexity: O(n) — output array.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: n = 5  
We'll trace Approach 2 (`dp[i] = dp[i >> 1] + (i & 1)`).

| i  | i >> 1 | i & 1 | dp[i >> 1] | dp[i] = dp[i>>1] + (i&1) |
|----|--------|-------|------------|--------------------------|
| 0  |   0    |   0   |    0       | 0 (base)                |
| 1  |   0    |   1   |    0       | 0 + 1 = 1                |
| 2  |   1    |   0   |    1       | 1 + 0 = 1                |
| 3  |   1    |   1   |    1       | 1 + 1 = 2                |
| 4  |   2    |   0   |    1       | 1 + 0 = 1                |
| 5  |   2    |   1   |    1       | 1 + 1 = 2                |

Result: `[0, 1, 1, 2, 1, 2]` matches the expected output.

---