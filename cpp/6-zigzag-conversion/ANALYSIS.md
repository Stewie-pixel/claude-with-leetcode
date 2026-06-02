# Arrays & Hashing

## Video Solution

Referenced a single url containing the according problem at [resources](https://www.youtube.com/results?search_query=leetcode+solution+zigzag+conversion)

## Concept

The zigzag conversion writes characters row‑by‑row in a repeating “down‑then‑up” pattern.  
Imagine a rail fence with `numRows` rails: you start at the top rail, move down one rail at a time until you hit the bottom, then reverse direction and move up until you reach the top again, and repeat.  
After filling the rails, the final string is obtained by concatenating the characters from each rail in order.

## When to Use It

Use this technique when the problem:
- Describes a periodic up‑down or back‑and‑forth traversal (e.g., “zigzag”, “rail fence”, “snake” pattern).
- Asks to rearrange characters of a string according to a fixed periodic movement.
- Can be simulated by keeping a buffer for each “row” or “level” and toggling a direction flag.

## Template

```python
def convert(s: str, numRows: int) -> str:
    if numRows == 1 or numRows >= len(s):
        return s

    # One buffer per row
    rows = [''] * numRows
    cur_row = 0
    going_down = False   # direction flag

    for ch in s:
        rows[cur_row] += ch
        # Change direction at the top or bottom row
        if cur_row == 0 or cur_row == numRows - 1:
            going_down = not going_down
        cur_row += 1 if going_down else -1

    return ''.join(rows)
```

---

## LeetCode Problem Walkthrough

### Problem: 6. Zigzag Conversion  
https://leetcode.com/problems/zigzag-conversion/

---

### Approach 1: Brute Force – 2D Matrix Simulation

**Algorithm**
1. If `numRows == 1` or the string length ≤ `numRows`, return the original string.
2. Create a 2D character matrix with `numRows` rows and enough columns (worst‑case `len(s)` columns) filled with a placeholder (e.g., `''` or `'\0'`).
3. Walk through the string, placing each character in the matrix while moving down then up (change direction at the first or last row).
4. After filling, concatenate all non‑placeholder characters row‑by‑row to produce the result.

**Implementation**

```python
def convert_brute(s: str, numRows: int) -> str:
    if numRows == 1 or numRows >= len(s):
        return s

    # Allocate matrix: rows x len(s) columns (over‑allocation is fine for brute force)
    matrix = [[''] * len(s) for _ in range(numRows)]
    row, col = 0, 0
    going_down = False

    for ch in s:
        matrix[row][col] = ch
        if row == 0 or row == numRows - 1:
            going_down = not going_down
        row += 1 if going_down else -1
        if not going_down:      # we moved up, so advance column
            col += 1

    # Read row‑wise
    result = []
    for r in range(numRows):
        for c in range(len(s)):
            if matrix[r][c] != '':
                result.append(matrix[r][c])
    return ''.join(result)
```

**Complexity Analysis**
- Time complexity: **O(n · numRows)** – we visit each cell of the allocated matrix (size `numRows × n`) once while filling and once while reading.
- Space complexity: **O(n · numRows)** – the full matrix storage.

---

### Approach 2: Row Buffers (Optimized Simulation)

**Intuition**
Instead of a full 2D matrix, we only need to store the characters that actually belong to each row.  
We can keep an array of strings, one per row, and append characters as we traverse the zigzag.  
When we hit the top or bottom row we reverse direction.

**Algorithm**
1. Handle the trivial case (`numRows == 1` or `numRows ≥ len(s)`).
2. Create a list `rows` of `numRows` empty strings.
3. Initialize `cur_row = 0` and a boolean `going_down = False`.
4. Iterate over each character `ch` in `s`:
   - Append `ch` to `rows[cur_row]`.
   - If `cur_row` is `0` or `numRows‑1`, flip `going_down`.
   - Move `cur_row` one step in the current direction (`+1` if down, `‑1` if up).
5. Concatenate all strings in `rows` and return.

**Implementation**

```python
def convert_rows(s: str, numRows: int) -> str:
    if numRows == 1 or numRows >= len(s):
        return s

    rows = [''] * numRows
    cur_row = 0
    going_down = False

    for ch in s:
        rows[cur_row] += ch
        if cur_row == 0 or cur_row == numRows - 1:
            going_down = not going_down
        cur_row += 1 if going_down else -1

    return ''.join(rows)
```

**Complexity Analysis**
- Time complexity: **O(n)** – each character is processed exactly once.
- Space complexity: **O(numRows)** – we store one string per row (total length `n`).

---

### Approach 3: Direct Index Calculation (Mathematical)

**Intuition**
The zigzag pattern repeats every `cycle = 2·numRows − 2` characters.  
For each row `r` (0‑based):
- Characters in the first and last row appear at positions `r, r+cycle, r+2·cycle, …`.
- Characters in middle rows appear at two positions per cycle:
  - `r + k·cycle` (downward leg)
  - `(cycle − r) + k·cycle` (upward leg)
By iterating over rows and jumping by `cycle`, we can collect the output characters without any extra per‑character direction bookkeeping.

**Algorithm**
1. If `numRows == 1` or `numRows ≥ len(s)`, return `s`.
2. Compute `cycle = 2 * numRows - 2`.
3. For each row `r` from `0` to `numRows‑1`:
   - Walk through the string with index `j = r`; while `j < n`:
        - Append `s[j]`.
        - If `r` is not the first or last row, compute `mid = j + cycle - 2*r`; if `mid < n`, append `s[mid]`.
        - Increment `j` by `cycle`.
4. Return the concatenated result.

**Implementation**

```python
def convert_math(s: str, numRows: int) -> str:
    if numRows == 1 or numRows >= len(s):
        return s

    n = len(s)
    cycle = 2 * numRows - 2
    res = []

    for r in range(numRows):
        j = r
        while j < n:
            res.append(s[j])
            # For middle rows, add the character from the upward leg
            if 0 < r < numRows - 1:
                mid = j + cycle - 2 * r
                if mid < n:
                    res.append(s[mid])
            j += cycle

    return ''.join(res)
```

**Complexity Analysis**
- Time complexity: **O(n)** – each character is appended to the result exactly once.
- Space complexity: **O(1)** extra (excluding the output string).

---

### Summary

| Approach               | Idea                                          | Time   | Extra Space |
|------------------------|-----------------------------------------------|--------|-------------|
| Brute Force (2D matrix)| Fill full matrix, then read row‑wise          | O(n·numRows) | O(n·numRows) |
| Row Buffers            | One string per row, toggle direction flag    | O(n)   | O(numRows)  |
| Direct Index Math      | Jump by cycle, pick characters per row       | O(n)   | O(1)        |

The row‑buffer solution is the most common interview answer: simple, linear time, and low space. The mathematical variant shows how recognizing the repeating cycle can eliminate the direction flag altogether. Both are acceptable; pick the one you find clearer to explain and implement.