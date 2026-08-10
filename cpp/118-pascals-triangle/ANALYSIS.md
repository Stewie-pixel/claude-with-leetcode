# Dynamic Programming

## Video Solution

For more details about **Pascal's Triangle**, watch the walkthrough at [https://www.youtube.com/watch?v=nPVEaB3AjUM](https://www.youtube.com/watch?v=nPVEaB3AjUM).

## Concept

Pascal's Triangle is a triangular array where each entry is the sum of the two entries directly above it.  
The value at row `r` and column `c` (0‑indexed) equals the binomial coefficient **C(r, c)**.  
Because each row only depends on the previous row, we can build the triangle iteratively – a classic **bottom‑up dynamic programming** pattern.

## When to Use It

Use this technique when you see:
- A problem that asks for rows of Pascal's Triangle or binomial coefficients.
- A recurrence where `dp[i][j] = dp[i‑1][j‑1] + dp[i‑1][j]`.
- Constraints that allow O(numRows²) time and O(numRows) or O(numRows²) space.

## Template

```python
def generate_pascal(numRows: int) -> List[List[int]]:
    # dp will hold the triangle we are building
    dp = []

    for r in range(numRows):
        # start each row with a list of the correct length filled with 1s
        row = [1] * (r + 1)

        # inner elements (if any) are sum of two elements from the previous row
        for c in range(1, r):
            row[c] = dp[r-1][c-1] + dp[r-1][c]

        dp.append(row)

    return dp
```

---

## LeetCode Problem Walkthrough

### Problem: 118. Pascal's Triangle
https://leetcode.com/problems/pascals-triangle/

### Approach 1: Brute Force (Recursive without memoization)

**Algorithm**
- Define a recursive function `value(r, c)` that returns the element at row `r`, column `c`.
- Base cases: `c == 0` or `c == r` → return 1.
- Recurrence: `value(r, c) = value(r-1, c-1) + value(r-1, c)`.
- Build each row by calling `value(r, c)` for every column `c`.
- No memoization → the same sub‑problems are recomputed many times.

**Implementation**

```python
from typing import List

class Solution:
    def generate(self, numRows: int) -> List[List[int]]:
        def val(r: int, c: int) -> int:
            if c == 0 or c == r:
                return 1
            return val(r-1, c-1) + val(r-1, c)

        triangle = []
        for r in range(numRows):
            row = [val(r, c) for c in range(r+1)]
            triangle.append(row)
        return triangle
```

**Complexity Analysis**
- Time complexity: O(2^numRows) — each call branches into two, leading to exponential recomputation.
- Space complexity: O(numRows) — recursion depth (call stack) plus output storage.

---

### Approach 2: Bottom‑Up DP using previous row

**Intuition**
Since each entry only needs the two entries above it, we can compute a row directly from the preceding row, storing only the last row at any time. This removes the exponential recomputation of the naïve recursion.

**Algorithm**
- Initialise an empty list `triangle`.
- For each row index `r` from 0 to `numRows‑1`:
  - Create a row of length `r+1` filled with 1s.
  - For each inner column `c` from 1 to `r‑1`, set `row[c] = prev_row[c‑1] + prev_row[c]`.
  - Append the completed row to `triangle` and set `prev_row = row` for the next iteration.

**Implementation**

```python
from typing import List

class Solution:
    def generate(self, numRows: int) -> List[List[int]]:
        triangle = []
        prev_row = []

        for r in range(numRows):
            row = [1] * (r + 1)          # start and end with 1
            for c in range(1, r):        # compute inner values
                row[c] = prev_row[c-1] + prev_row[c]
            triangle.append(row)
            prev_row = row               # current row becomes previous for next step
        return triangle
```

**Complexity Analysis**
- Time complexity: O(numRows²) — we visit each element exactly once.
- Space complexity: O(numRows²) for the output (required) plus O(numRows) auxiliary for `prev_row`.

---

### Approach 3: In‑place O(k) Space DP

**Intuition**
We can reuse a single list to build each row by updating it from the end towards the start. When we iterate backwards, the values we need from the previous row are still intact because we haven’t overwritten them yet.

**Algorithm**
- Initialise `row = [1]` (the first row).
- For each subsequent row index `r` from 1 to `numRows‑1`:
  - Append a `1` to the end of `row` (new row now length `r+1`).
  - Walk backwards from index `r‑1` down to 1, updating `row[j] = row[j] + row[j‑1]`.
  - After the inner loop, `row` holds the r‑th row; append a copy to the result.
- The first row `[1]` is added separately before the loop.

**Implementation**

```python
from typing import List

class Solution:
    def generate(self, numRows: int) -> List[List[int]]:
        result = []
        row = [1]                     # first row

        for _ in range(numRows):
            result.append(row.copy()) # store a copy of the current row
            # update row in‑place for next iteration
            row.append(1)             # new element at the end
            for j in range(len(row)-2, 0, -1):
                row[j] = row[j] + row[j-1]
        return result
```

**Complexity Analysis**
- Time complexity: O(numRows²) — same number of updates as Approach 2.
- Space complexity: O(numRows) auxiliary (the single `row` list) plus O(numRows²) for the output.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: `numRows = 5`

We'll trace Approach 3 (in‑place) because it shows the evolution of a single list.

```
Initially: row = [1], result = []

Iteration 0:
  result.append([1])               -> result = [[1]]
  row.append(1)                    -> row = [1, 1]
  inner loop: none (len(row)-2 = 0)
  row now = [1, 1]                 (row for next iteration)

Iteration 1:
  result.append([1,1])             -> result = [[1], [1,1]]
  row.append(1)                    -> row = [1, 1, 1]
  j = 1: row[1] = row[1] + row[0]  -> row = [1, 2, 1]
  row now = [1, 2, 1]

Iteration 2:
  result.append([1,2,1])           -> result = [[1], [1,1], [1,2,1]]
  row.append(1)                    -> row = [1,2,1,1]
  j = 2: row[2] = row[2] + row[1]  -> [1,2,3,1]
  j = 1: row[1] = row[1] + row[0]  -> [1,3,3,1]
  row now = [1,3,3,1]

Iteration 3:
  result.append([1,3,3,1])         -> result = [[1],[1,1],[1,2,1],[1,3,3,1]]
  row.append(1)                    -> row = [1,3,3,1,1]
  j = 3: row[3] = 1+3 = 4          -> [1,3,3,4,1]
  j = 2: row[2] = 3+3 = 6          -> [1,3,6,4,1]
  j = 1: row[1] = 3+1 = 4          -> [1,4,6,4,1]
  row now = [1,4,6,4,1]

Iteration 4:
  result.append([1,4,6,4,1])       -> final result
  (no further rows needed)
```

The final `result` matches the expected output:
```
[[1],
 [1,1],
 [1,2,1],
 [1,3,3,1],
 [1,4,6,4,1]]
```

--- 

**Summary**
- Brute force recursion shows the natural definition but is exponential.
- Bottom‑up DP using the previous row gives the optimal O(numRows²) time with clear, readable code.
- The in‑place variant reduces auxiliary space to O(numRows) while preserving the same time complexity.  
All approaches include full complexity analysis, and the dry run validates the logic step‑by‑step.