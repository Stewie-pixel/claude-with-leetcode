# Two Pointers

## Video Solution

For more details about **Minimum Operations to Make a Rotated Palindrome I**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=2Vcdjb-H8yA).

## Concept

The two‑pointer technique scans a sequence from both ends toward the centre.  
For a palindrome we only need to make the characters at symmetric positions equal.  
When we also allow left rotations, we can try every possible rotation (0 … n‑1) and, for each rotation, use two pointers to compute the cost of turning the rotated string into a palindrome by incrementing letters (circular distance). The minimum cost over all rotations is the answer.

## When to Use It

Use two pointers when you see:
- Palindrome checks or making a string a palindrome.
- Pairwise comparisons from the start and end moving inward.
- Problems that can be decomposed into independent left/right contributions (e.g., cost to match s[i] with s[j]).

## Template

```python
def two_pointer_palindrome_cost(s: str) -> int:
    n = len(s)
    best = float('inf')
    for rot in range(n):                     # try every left rotation
        cost = rot                           # rot left‑rotations needed
        i, j = 0, n - 1
        while i < j:                         # two‑pointer scan
            a = s[(rot + i) % n]             # character at left after rotation
            b = s[(rot + j) % n]             # character at right after rotation
            diff = abs(ord(a) - ord(b))
            cost += min(diff, 26 - diff)    # circular alphabet distance
            i += 1
            j -= 1
        best = min(best, cost)
    return best
```

## LeetCode Problem Walkthrough

### Problem: 2476. Minimum Operations to Make a Rotated Palindrome I

https://leetcode.com/problems/minimum-operations-to-make-a-rotated-palindrome-i/

### Approach 1: Brute Force over Rotations + Two Pointers

**Algorithm**
1. Let `n = len(s)`. Initialise answer to infinity.
2. For each possible left‑rotation amount `k` from `0` to `n‑1`:
   * The rotation itself costs `k` operations (each left rotation counts as one).
   * Use two pointers `i = 0`, `j = n‑1`. While `i < j`:
     * Fetch the characters after rotation: `a = s[(k+i) % n]`, `b = s[(k+j) % n]`.
     * The cheapest way to make `a` equal `b` using only increments is the circular distance:
       `cost_pair = min(|a-b|, 26 - |a-b|)`.
     * Add `cost_pair` to the running cost and move pointers inward.
   * Update the global answer with the minimum cost seen.
3. Return the answer.

**Implementation**

```python
class Solution:
    def minOperations(self, s: str) -> int:
        n = len(s)
        ans = float('inf')
        for k in range(n):                     # try each rotation
            cost = k                           # k left rotations
            i, j = 0, n - 1
            while i < j:
                a = s[(k + i) % n]
                b = s[(k + j) % n]
                diff = abs(ord(a) - ord(b))
                cost += min(diff, 26 - diff)   # cheapest way to match a,b
                i += 1
                j -= 1
            ans = min(ans, cost)
        return ans
```

**Complexity Analysis**

- Time complexity: O(n²) — we iterate over n rotations and each rotation scans the string with two pointers (≈ n/2 steps).
- Space complexity: O(1) — only a handful of integer variables are used.

### Approach 2: Pre‑compute Pair Costs (Optional Optimisation)

**Intuition**
The inner two‑pointer loop recomputes the cost for each pair `(i, j)` many times across different rotations. Notice that after a rotation by `k`, the characters compared are `s[(k+i)%n]` and `s[(k+j)%n]`. If we pre‑compute the cost to make any two characters equal, we can reuse it, but we still need to sum over the appropriate pairs for each k. This yields the same O(n²) bound but with a smaller constant factor.

**Algorithm**
1. Build a 26×26 matrix `pair_cost[c1][c2] = min(|c1-c2|, 26-|c1-c2|)`.
2. For each rotation `k`, compute the total cost as `k + Σ_{i<j} pair_cost[s[(k+i)%n]][s[(k+j)%n]]`.
3. Keep the minimum.

**Implementation**

```python
class Solution:
    def minOperations(self, s: str) -> int:
        n = len(s)
        # pre‑compute cost to turn any letter into any other
        pair_cost = [[0]*26 for _ in range(26)]
        for a in range(26):
            for b in range(26):
                diff = abs(a - b)
                pair_cost[a][b] = min(diff, 26 - diff)

        ans = float('inf')
        s_vals = [ord(ch) - 97 for ch in s]   # convert to 0‑25 ints
        for k in range(n):
            cost = k
            i, j = 0, n - 1
            while i < j:
                a = s_vals[(k + i) % n]
                b = s_vals[(k + j) % n]
                cost += pair_cost[a][b]
                i += 1
                j -= 1
            ans = min(ans, cost)
        return ans
```

**Complexity Analysis**

- Time complexity: O(n² + 26²) → O(n²) — the dominant term is the double loop over rotations and pairs.
- Space complexity: O(26²) = O(1) — constant extra space for the lookup table.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: `s = "abc"` (n = 3)

We try rotations k = 0, 1, 2.

| k (rotations) | rotated string | i/j pairs examined | pair cost (circular) | running cost (rotations + pairs) |
|---------------|----------------|--------------------|----------------------|-----------------------------------|
| 0             | "abc"          | (a,c) → diff=2 → min(2,24)=2 | 0 + 2 = 2 |
| 1             | "bca"          | (b,a) → diff=1 → min(1,25)=1 | 1 + 1 = 2 |
| 2             | "cab"          | (c,b) → diff=1 → min(1,25)=1 | 2 + 1 = 3 |

Minimum cost across rotations = **2**, matching the example.

---