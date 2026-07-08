# Arrays & Hashing

## Video Solution

For more details about **Zigzag Conversion**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=Q2Tw6gcVEwc).

## Concept

The zigzag conversion writes characters of a string row‑by‑row in a repeating “down‑then‑up” pattern. Imagine a rail fence: you go down the rows until you hit the bottom, then you go back up until you reach the top, and repeat. After all characters are placed, you read each row left‑to‑right and concatenate the rows to get the final string.

## When to Use It

Use this technique when a problem describes:
- Writing characters in a fixed number of rows with a periodic up‑down movement.
- Converting a string by following a repeating cycle (down → up → down …).
- Any scenario where you can simulate a “bouncing” index between 0 and `numRows‑1`.

## Template

```python
# Reusable template for zigzag‑style problems
def zigzag_template(s: str, numRows: int) -> str:
    if numRows == 1 or numRows >= len(s):
        return s

    # Prepare containers for each row
    rows = [''] * numRows
    cur_row = 0
    going_down = False   # direction flag

    for ch in s:
        rows[cur_row] += ch               # place character

        # Change direction at the top or bottom row
        if cur_row == 0 or cur_row == numRows - 1:
            going_down = not going_down

        # Move to next row according to direction
        cur_row += 1 if going_down else -1

    # Concatenate all rows
    return ''.join(rows)
```

---

## LeetCode Problem Walkthrough

### Problem: 6. Zigzag Conversion

https://leetcode.com/problems/zigzag-conversion/

### Approach 1: Brute Force Simulation (Direction Flag)

**Algorithm**
1. If `numRows` is 1 or greater than or equal to the length of `s`, return `s` unchanged – no zigzag occurs.
2. Create a list `rows` of empty strings, one for each row.
3. Walk through the characters of `s`:
   - Append the character to the current row.
   - If we are at the first or last row, flip the direction (`going_down`).
   - Move to the next row: increase index when going down, decrease when going up.
4. After processing all characters, concatenate the rows in order and return the result.

**Implementation**

```python
class Solution:
    def convert(self, s: str, numRows: int) -> str:
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
- Time complexity: O(n) – each character is processed once.
- Space complexity: O(n) – we store all characters in the `rows` list.

---

### Approach 2: Direct Index Calculation (Cycle Method)

**Intuition**
The zigzag pattern repeats every `cycle = 2 * numRows - 2` characters (down‑then‑up). For each row we can compute exactly which positions in the original string belong to that row without simulating the bounce. Characters in the first and last row appear once per cycle; middle rows appear twice per cycle.

**Algorithm**
1. Handle the trivial case (`numRows == 1`).
2. Compute `cycle = 2 * numRows - 2`.
3. For each row `r` from `0` to `numRows-1`:
   - Walk through the string with step `cycle`.
   - Append the character at index `j + r` (always valid).
   - If `r` is not the first or last row, also append the character at index `j + cycle - r` (the “up‑stroke” character) when it lies within bounds.
4. Concatenate the collected characters for all rows.

**Implementation**

```python
class Solution:
    def convert(self, s: str, numRows: int) -> str:
        if numRows == 1:
            return s

        n = len(s)
        cycle = 2 * numRows - 2
        res = []

        for r in range(numRows):
            for j in range(0, n, cycle):
                # down‑stroke character
                idx1 = j + r
                if idx1 < n:
                    res.append(s[idx1])

                # up‑stroke character (only for middle rows)
                if r != 0 and r != numRows - 1:
                    idx2 = j + cycle - r
                    if idx2 < n:
                        res.append(s[idx2])

        return ''.join(res)
```

**Complexity Analysis**
- Time complexity: O(n) – each character is visited a constant number of times (once or twice).
- Space complexity: O(n) – the result list stores all characters.

---

### Approach 3: Pre‑allocated List of Characters (In‑place Building)

**Intuition**
Instead of building temporary strings for each row (which creates many intermediate objects), we can pre‑allocate a list of characters of length `len(s)` and fill it directly while traversing the zigzag order. This reduces overhead from string concatenation and is often faster in practice.

**Algorithm**
1. Edge case: if `numRows == 1` or `numRows >= len(s)`, return `s`.
2. Create a list `ans` of length `len(s)` and an index pointer `pos = 0`.
3. Simulate the same down‑up movement as in Approach 1, but instead of appending to a row string, place the current character into `ans[pos]` and increment `pos`.
4. After the traversal, join `ans` into a string and return.

**Implementation**

```python
class Solution:
    def convert(self, s: str, numRows: int) -> str:
        if numRows == 1 or numRows >= len(s):
            return s

        n = len(s)
        ans = [''] * n
        pos = 0
        rows = [''] * numRows
        cur_row = 0
        going_down = False

        for ch in s:
            rows[cur_row] += ch
            if cur_row == 0 or cur_row == numRows - 1:
                going_down = not going_down
            cur_row += 1 if going_down else -1

        # Flatten rows into the pre‑allocated list
        for r in range(numRows):
            for ch in rows[r]:
                ans[pos] = ch
                pos += 1

        return ''.join(ans)
```

**Complexity Analysis**
- Time complexity: O(n) – each character is processed a constant number of times.
- Space complexity: O(n) – the `rows` list holds all characters temporarily, plus the output list.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm with the example `s = "PAYPALISHIRING"`, `numRows = 3`.

#### Dry Run (Approach 1 – direction flag)

| Step | Index | Char | cur_row before | going_down before | Action (store) | cur_row after |
|------|-------|------|----------------|-------------------|----------------|---------------|
| 1    | 0     | P    | 0              | False             | rows[0] = "P"  | 1 (flip → True) |
| 2    | 1     | A    | 1              | True              | rows[1] = "A"  | 2 |
| 3    | 2     | Y    | 2              | True              | rows[2] = "Y"  | 1 (flip → False) |
| 4    | 3     | P    | 1              | False             | rows[1] = "AP" | 0 |
| 5    | 4     | A    | 0              | False             | rows[0] = "PA" | 1 (flip → True) |
| 6    | 5     | L    | 1              | True              | rows[1] = "APL"| 2 |
| 7    | 6     | I    | 2              | True              | rows[2] = "YI" | 1 (flip → False) |
| 8    | 7     | S    | 1              | False             | rows[1] = "APLS"| 0 |
| 9    | 8     | H    | 0              | False             | rows[0] = "PAH"| 1 (flip → True) |
|10    | 9     | I    | 1              | True              | rows[1] = "APLSI"| 2 |
|11    |10     | R    | 2              | True              | rows[2] = "YIR"| 1 (flip → False) |
|12    |11     | I    | 1              | False             | rows[1] = "APLSII"| 0 |
|13    |12     | N    | 0              | False             | rows[0] = "PAHN"| 1 (flip → True) |
|14    |13     | G    | 1              | True              | rows[1] = "APLSIIG"| 2 (end) |

After processing:
- rows[0] = "PAHN"
- rows[1] = "APLSIIG"
- rows[2] = "YIR"

Concatenated → `"PAHNAPLSIIGYIR"` which matches the expected output.

---