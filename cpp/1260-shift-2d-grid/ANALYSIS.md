# Arrays & Hashing

## Video Solution

For more details about **Shift 2D Grid**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=nJYFh4Dl-as)

## Concept

Shifting a 2D grid as described is equivalent to treating the grid as a single 1‑D array (row‑major order) and rotating that array to the right by `k` positions. After the rotation we reshape the 1‑D array back into `m × n` dimensions. This insight lets us avoid simulating each shift step‑by‑step.

## When to Use It

Use this technique when you see:
- A matrix that is being shifted or rotated with wrap‑around at the borders.
- The operation can be expressed as a linear index transformation (`(i·n + j)`).
- The problem asks for the state after `k` repetitions of a simple local move.

## Template

```python
def shift_2d_grid(grid, k):
    """
    Template for solving shift‑2D‑grid‑like problems.
    1. Flatten the matrix into a list.
    2. Compute effective shift = k % total_elements.
    3. Rotate the list (right shift) using slicing or index math.
    4. Reshape the list back to m × n.
    """
    m, n = len(grid), len(grid[0])
    total = m * n
    k %= total                     # effective shift

    # 1. flatten
    flat = [grid[i][j] for i in range(m) for j in range(n)]

    # 2. rotate (right shift) – example using slicing
    rotated = flat[-k:] + flat[:-k] if k else flat

    # 3. reshape
    res = [[rotated[i * n + j] for j in range(n)] for i in range(m)]
    return res
```

---

## LeetCode Problem Walkthrough

### Problem: 1260. Shift 2D Grid

https://leetcode.com/problems/shift-2d-grid/

### Approach 1: Brute Force (simulate each shift)

**Algorithm**
- Repeat `k` times:
  - Remember the last element (`grid[m‑1][n‑1]`).
  - Move every element to its right/down neighbor by iterating the matrix in reverse order.
  - Place the saved last element at `grid[0][0]`.
- Return the mutated grid.

**Implementation**

```python
class Solution:
    def shiftGrid(self, grid, k):
        m, n = len(grid), len(grid[0])
        total = m * n
        k %= total                     # unnecessary work if k >= total

        for _ in range(k):
            last = grid[m - 1][n - 1]  # element that will wrap around
            for i in range(m - 1, -1, -1):
                for j in range(n - 1, -1, -1):
                    if i == 0 and j == 0:
                        continue
                    if j == 0:
                        grid[i][j] = grid[i - 1][n - 1]
                    else:
                        grid[i][j] = grid[i][j - 1]
            grid[0][0] = last
        return grid
```

**Complexity Analysis**
- Time complexity: O(k·m·n) — we touch every cell for each of the `k` shifts.
- Space complexity: O(1) — the grid is modified in place (ignoring input/output).

---

### Approach 2: Flatten → rotate → reshape

**Intuition**
If we write the grid row‑by‑row into a one‑dimensional list, a single shift corresponds to moving the last element to the front. Performing `k` shifts is therefore a right rotation of that list by `k` positions. After rotating we simply cut the list back into rows.

**Algorithm**
1. Flatten the matrix into `flat`.
2. Compute effective shift `k = k % (m*n)`.
3. Rotate `flat` to the right by `k` using slicing: `flat[-k:] + flat[:-k]`.
4. Rebuild the matrix from the rotated list.

**Implementation**

```python
class Solution:
    def shiftGrid(self, grid, k):
        m, n = len(grid), len(grid[0])
        total = m * n
        k %= total                     # handle large k

        # 1. flatten
        flat = [grid[i][j] for i in range(m) for j in range(n)]

        # 2. rotate right by k
        rotated = flat[-k:] + flat[:-k] if k else flat

        # 3. reshape
        return [[rotated[i * n + j] for j in range(n)] for i in range(m)]
```

**Complexity Analysis**
- Time complexity: O(m·n) — one pass to flatten, one pass to reshape.
- Space complexity: O(m·n) — we store the flattened list (output excluded).

---

### Approach 3: Direct index mapping (O(1) extra space)

**Intuition**
Instead of building an intermediate list, we can compute where each original element ends up after the rotation. The element at linear index `p = i·n + j` moves to index `(p + k) % total`. We fill a new result matrix using this formula.

**Algorithm**
1. Compute `total = m*n` and `k = k % total`.
2. Allocate an empty `m × n` matrix `res`.
3. For each cell `(i, j)` calculate `src = (i·n + j - k) % total` (the element that will occupy this position after the shift) and copy `grid[src // n][src % n]` into `res[i][j]`.
4. Return `res`.

**Implementation**

```python
class Solution:
    def shiftGrid(self, grid, k):
        m, n = len(grid), len(grid[0])
        total = m * n
        k %= total

        res = [[0] * n for _ in range(m)]
        for i in range(m):
            for j in range(n):
                # index of the element that ends up at (i, j) after k right shifts
                src = (i * n + j - k) % total
                res[i][j] = grid[src // n][src % n]
        return res
```

**Complexity Analysis**
- Time complexity: O(m·n) — we visit each cell once.
- Space complexity: O(1) extra (excluding the output matrix).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the optimal flatten‑rotate approach on the first example.

**Input:** `grid = [[1,2,3],[4,5,6],[7,8,9]]`, `k = 1`

**Flattened list:** `[1, 2, 3, 4, 5, 6, 7, 8, 9]` (length = 9)  
**Effective shift:** `k = 1 % 9 = 1`  
**Rotated list (right by 1):** `[9, 1, 2, 3, 4, 5, 6, 7, 8]`  
**Reshaped 3×3:**  

| Row 0 | Row 1 | Row 2 |
|-------|-------|-------|
| 9,1,2 | 3,4,5 | 6,7,8 |

**Dry‑run table (showing mapping from original index → new index):**

| Original (i,j) | Original index p = i·n + j | New index (p + k) % 9 | New (i',j') |
|----------------|----------------------------|-----------------------|-------------|
| (0,0)          | 0                          | 1                     | (0,1)       |
| (0,1)          | 1                          | 2                     | (0,2)       |
| (0,2)          | 2                          | 3                     | (1,0)       |
| (1,0)          | 3                          | 4                     | (1,1)       |
| (1,1)          | 4                          | 5                     | (1,2)       |
| (1,2)          | 5                          | 6                     | (2,0)       |
| (2,0)          | 6                          | 7                     | (2,1)       |
| (2,1)          | 7                          | 8                     | (2,2)       |
| (2,2)          | 8                          | 0                     | (0,0)       |

Placing each original value at its new coordinates yields exactly the expected output `[[9,1,2],[3,4,5],[6,7,8]]`. This confirms the correctness of the rotation‑based solution.