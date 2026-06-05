**Lecture File Path:** `/lectures/matrix-traversal/2025-09-24-matrix-traversal.md`

```markdown
# Matrix Traversal

## Video Solution

For more details about **Equal Row and Column Pairs**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=R3G8lUfrY2M).

## Concept

When we need to compare every row with every column in a square matrix, a naïve double‑loop comparison leads to O(n³) time.  
Instead, we can **hash each row** (or column) into a immutable key (e.g., a string or tuple) and store how many times each key appears.  
Then, while iterating over columns, we simply look up the column’s key in the hash map and add its frequency to the answer.  
This reduces the problem to O(n²) time and O(n²) extra space (for the hash map).

## When to Use It

Use this pattern when you see:
- A square matrix (`n × n`).
- The need to test equality of entire rows **and** columns (or any two sequences that share the same length).
- Counting how many pairs match, rather than just detecting existence.
- Constraints where `n ≤ 200‑400`, making O(n²) feasible but O(n³) too slow.

## Template

```python
from collections import defaultdict
from typing import List

def count_matching_rows_and_columns(grid: List[List[int]]) -> int:
    n = len(grid)
    freq = defaultdict(int)          # hash map: row representation -> count

    # 1️⃣ Hash all rows
    for r in range(n):
        key = tuple(grid[r])         # tuple is hashable and preserves order
        freq[key] += 1

    # 2️⃣ Scan columns and accumulate matches
    ans = 0
    for c in range(n):
        col_key = tuple(grid[r][c] for r in range(n))
        ans += freq.get(col_key, 0)

    return ans
```

## LeetCode Problem Walkthrough

### Problem: 2352. Equal Row and Column Pairs

https://leetcode.com/problems/equal-row-and-column-pairs/

---

### Approach 1: Brute Force

**Algorithm**  
For each row index `i`, compare it element‑by‑element with every column index `j`.  
If all `n` elements match, increment the answer.  
This checks every possible `(row, column)` pair directly.

**Implementation**

```python
class Solution:
    def equalPairs(self, grid: List[List[int]]) -> int:
        n = len(grid)
        ans = 0
        for i in range(n):                 # each row
            for j in range(n):             # each column
                match = True
                for k in range(n):
                    if grid[i][k] != grid[k][j]:
                        match = False
                        break
                if match:
                    ans += 1
        return ans
```

**Complexity Analysis**

- Time complexity: **O(n³)** — three nested loops (rows × columns × element‑wise compare).  
- Space complexity: **O(1)** — only a few integer variables.

---

### Approach 2: Hash Rows with Tuple Keys (Recommended)

**Intuition**  
If we can represent a row (or column) as a hashable object, we can count how many times each distinct row appears.  
Then, for each column we just need to know how many rows are identical to it — obtained in O(1) lookup from the hash map.

**Algorithm**

1. Iterate over all rows, convert each row to a `tuple`, and store its frequency in a hash map.  
2. Iterate over all columns, build the column’s tuple, and add `freq[col_tuple]` to the answer.

**Implementation**

```python
class Solution:
    def equalPairs(self, grid: List[List[int]]) -> int:
        from collections import defaultdict
        n = len(grid)
        freq = defaultdict(int)

        # count each row
        for r in range(n):
            freq[tuple(grid[r])] += 1

        ans = 0
        # match each column
        for c in range(n):
            col_key = tuple(grid[r][c] for r in range(n))
            ans += freq.get(col_key, 0)

        return ans
```

**Complexity Analysis**

- Time complexity: **O(n²)** — we touch each matrix element twice (once while hashing rows, once while building columns).  
- Space complexity: **O(n²)** in the worst case — each of the `n` rows could be distinct, storing `n`‑length tuples.

---

### Approach 3: Hash Rows as Encoded Strings (Alternative)

**Intuition**  
Instead of tuples we can encode a row into a single string (e.g., `"3,2,1,"`).  
String keys are also hashable and sometimes slightly faster in practice because they avoid the overhead of tuple allocation.

**Algorithm**  
Same as Approach 2, but the key is a comma‑separated string with a trailing delimiter to avoid ambiguity like `[1,11]` vs `[11,1]`.

**Implementation**

```python
class Solution:
    def equalPairs(self, grid: List[List[int]]) -> int:
        n = len(grid)
        freq = {}

        # hash rows as strings
        for r in range(n):
            key = ','.join(str(num) for num in grid[r]) + ','
            freq[key] = freq.get(key, 0) + 1

        ans = 0
        # hash columns and look up
        for c in range(n):
            key = ','.join(str(grid[r][c]) for r in range(n)) + ','
            ans += freq.get(key, 0)

        return ans
```

**Complexity Analysis**

- Time complexity: **O(n²)** — same reasoning as Approach 2.  
- Space complexity: **O(n²)** — we store up to `n` distinct string keys, each of length O(n).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the optimized algorithm on Example 1.

**Input**

```
grid = [[3,2,1],
        [1,7,6],
        [2,7,7]]
```

#### Step 1: Hash rows

| Row index | Row as tuple          | freq after insertion |
|-----------|-----------------------|----------------------|
| 0         | (3, 2, 1)             | {(3,2,1):1}          |
| 1         | (1, 7, 6)             | {(3,2,1):1, (1,7,6):1} |
| 2         | (2, 7, 7)             | {(3,2,1):1, (1,7,6):1, (2,7,7):1} |

#### Step 2: Scan columns and accumulate matches

| Column index | Column as tuple   | freq[column] | ans after addition |
|--------------|-------------------|--------------|--------------------|
| 0            | (3, 1, 2)         | 0            | 0 |
| 1            | (2, 7, 7)         | 1 (from row 2) | 1 |
| 2            | (1, 6, 7)         | 0            | 1 |

**Result:** `ans = 1`, matching the expected output.

---

**Key Takeaways**

- Transforming rows/columns into hashable keys lets us reduce an O(n³) brute force to O(n²).  
- Both tuple and string keys work; pick whichever is more idiomatic in your language.  
- Always state the complexity for each approach — this makes the trade‑off crystal‑clear.  

--- 

*End of lecture.*