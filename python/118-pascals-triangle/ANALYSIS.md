# 1D Dynamic Programming

## Video Solution

For more details about **Pascal's Triangle**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=nPVEaB3AjUM)

## Concept

Pascal’s triangle is a classic example of **1‑D Dynamic Programming**: each entry depends only on the two entries directly above it from the previous row.  
If we already know row `i‑1`, we can compute row `i` in linear time by summing adjacent pairs. The problem therefore reduces to iteratively building each row from the previous one.

## When to Use It

Use this pattern when you see:
- A triangular or layered structure where each level is built from the previous level.
- Recurrence of the form `dp[i][j] = dp[i‑1][j‑1] + dp[i‑1][j]`.
- Problems asking for combinatorial numbers (n choose k) or paths in a grid.

## Template

```python
# 1‑D DP template for problems like Pascal's Triangle
def build_triangle(rows):
    triangle = []                     # will hold all rows
    prev = []                         # previous row (initially empty)

    for i in range(rows):
        cur = [1] * (i + 1)           # start with all 1s
        # update inner cells using the previous row
        for j in range(1, i):
            cur[j] = prev[j-1] + prev[j]
        triangle.append(cur)          # store a copy of the current row
        prev = cur                    # move forward

    return triangle
```

---

## LeetCode Problem Walkthrough

### Problem: 118. Pascal's Triangle

https://leetcode.com/problems/pascals-triangle/

---

### Approach 1: Brute Force – Combination Formula

**Algorithm**  
Each element of Pascal’s triangle is a binomial coefficient:  
`C(n, k) = n! / (k! * (n‑k)!)`.  
For every row `n` (0‑based) and each column `k` we compute the factorial‑based value directly.

**Implementation**

```python
class Solution:
    def generate(self, numRows):
        """
        :type numRows: int
        :rtype: List[List[int]]
        """
        def factorial(x):
            res = 1
            for i in range(2, x+1):
                res *= i
            return res

        triangle = []
        for n in range(numRows):
            row = []
            for k in range(n+1):
                val = factorial(n) // (factorial(k) * factorial(n-k))
                row.append(val)
            triangle.append(row)
        return triangle
```

**Complexity Analysis**

- Time complexity: O(numRows³) — we compute a factorial for each element; each factorial is O(numRows) and there are O(numRows²) elements.  
- Space complexity: O(numRows²) — output triangle stores all values.

---

### Approach 2: DP – Build Each Row from the Previous One

**Intuition**Intuition**  
Instead of recomputing factorials, we reuse the already computed row[i][j] = row[i‑1][j‑1] + row[i‑1][j].  
We keep only the previous row to generate the next one, then store a copy for the final answer.

**Algorithm**

1. Initialise an empty list `triangle`.
2. Keep a variable `prev` for the last generated row.
3. For each row index `i` from `0` to `numRows‑1`:
   - Create a list `cur` of length `i+1` filled with `1`s.
   - For each inner position `j` from `1` to `i‑1`, set `cur[j] = prev[j‑1] + prev[j]`.
   - Append `cur` to `triangle` and set `prev = cur`.
4. Return `triangle`.

**Implementation**

```python
class Solution:
    def generate(self, numRows):
        """
        :type numRows: int
        :rtype: List[List[int]]
        """
        triangle = []
        prev = []                     # no previous row yet

        for i in range(numRows):
            cur = [1] * (i + 1)       # first and last are always 1
            for j in range(1, i):    # update interior cells
                cur[j] = prev[j-1] + prev[j]
            triangle.append(cur)
            prev = cur                # move forward

        return triangle
```

**Complexity Analysis**

- Time complexity: O(numRows²) — each element is computed exactly once.  
- Space complexity: O(numRows²) — the output triangle stores all rows (auxiliary space O(numRows) for `prev`).

---

### Approach 3: Multiplicative Formula – O(1) Extra Space per Row

**Intuition**  
The binomial coefficient can be updated iteratively:  
`C(n, k) = C(n, k‑1) * (n‑k+1) // k`.  
Starting with `C(n,0)=1`, we can compute each subsequent value in O(1) time, avoiding factorials and extra storage beyond the current row.

**Algorithm**

For each row `n`:
- Start with `val = 1` (this is `C(n,0)`).
- For `k` from `1` to `n`:
  - Update `val = val * (n - k + 1) // k`.
  - Append `val` to the current row.
- Store the row.

**Implementation**

```python
class Solution:
    def generate(self, numRows):
        """
        :type numRows: int
        :rtype: List[List[int]]
        """
        triangle = []
        for n in range(numRows):
            row = []
            val = 1                     # C(n,0)
            for k in range(n+1):
                row.append(val)
                # prepare next binomial coefficient
                val = val * (n - k) // (k + 1)
            triangle.append(row)
        return triangle
```

**Complexity Analysis**

- Time complexity: O(numRows²) — each element computed once with O(1) work.  
- Space complexity: O(numRows²) — output dominates; auxiliary space O(1) besides the output.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

**Dry Run** (numRows = 5)

We will trace Approach 2 (DP from previous row).

```
Step | i (row) | prev (row i-1)       | cur (being built)                     | Action
-----|---------|----------------------|----------------------------------------|---------------------------------------------------
  1  |   0     | []                   | [1]                                    | cur[0]=1 (edges)
  2  |   1     | [1]                  | [1,1]                                  | cur[0]=cur[2]=1; no inner cells
  3  |   2     | [1,1]                | [1, ?,1]                               | cur[1]=prev[0]+prev[1]=1+1=2 → [1,2,1]
  4  |   3     | [1,2,1]              | [1, ?, ?,1]                            | cur[1]=prev[0]+prev[1]=1+2=3
                                   | cur[2]=prev[1]+prev[2]=2+1=3 → [1,3,3,1]
  5  |   4     | [1,3,3,1]            | [1, ?, ?, ?,1]                         | cur[1]=1+3=4
                                   | cur[2]=3+3=6
                                   | cur[3]=3+1=4 → [1,4,6,4,1]
```

After processing all rows we obtain:

```
[[1],
 [1,1],
 [1,2,1],
 [1,3,3,1],
 [1,4,6,4,1]]
```

which matches the expected output.