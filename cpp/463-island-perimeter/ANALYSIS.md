# Matrix Traversal

## Video Solution

For more details about **Island Perimeter**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=fISiUAFRM2s)

## Concept

When we need to examine every cell in a 2‑D grid and make decisions based on its neighbours (up, down, left, right), we are using **matrix traversal**.  
Think of the grid as a city map where each block is either a building (land) or a river (water). To find the total length of the building’s outer fence, we walk along every block, count how many sides touch the river or the map border, and add those lengths together.

## When to Use It

Use matrix traversal when you see:

- The problem gives a 2‑D array (`grid`, `matrix`) and asks for a property that depends on each cell and its adjacent cells.
- You need to count something like islands, perimeter, number of enclaves, or shortest path in an unweighted grid.
- The constraints are modest (grid size ≤ 100‑200) so an O(rows × cols) scan is fine.
- No complicated state needs to be carried across rows/columns beyond the immediate neighbours.

## Template

```python
def traverse_grid(grid):
    """
    Generic template for visiting every cell and looking at its 4 neighbours.
    Returns whatever aggregate you need (e.g., count, sum, bool).
    """
    if not grid:
        return 0
    rows, cols = len(grid), len(grid[0])
    result = 0                     # <-- initialise your answer here

    for r in range(rows):
        for c in range(cols):
            if grid[r][c] == 1:    # <-- condition that defines a "land" cell
                # start with 4 sides for this cell
                perimeter_contrib = 4

                # subtract a side for each neighbour that is also land
                if r > 0     and grid[r-1][c] == 1: perimeter_contrib -= 1   # up
                if r < rows-1 and grid[r+1][c] == 1: perimeter_contrib -= 1   # down
                if c > 0     and grid[r][c-1] == 1: perimeter_contrib -= 1   # left
                if c < cols-1 and grid[r][c+1] == 1: perimeter_contrib -= 1   # right

                result += perimeter_contrib   # <-- accumulate answer
    return result
```

---

## LeetCode Problem Walkthrough

### Problem: 463. Island Perimeter  
https://leetcode.com/problems/island-perimeter/

---

### Approach 1: Brute Force – Count Exposed Sides

**Algorithm**  
Iterate over every cell. For each land cell (`grid[r][c] == 1`) assume it contributes 4 to the perimeter. Then check the four neighbours; for each neighbour that is also land, subtract 1 because that side is shared and not part of the outer border. Sum the contributions.

**Implementation**

```python
class Solution:
    def islandPerimeter(self, grid: List[List[int]]) -> int:
        rows, cols = len(grid), len(grid[0])
        perimeter = 0

        for r in range(rows):
            for c in range(cols):
                if grid[r][c] == 1:
                    perimeter += 4                     # all four sides
                    if r > 0     and grid[r-1][c] == 1: perimeter -= 1   # up
                    if r < rows-1 and grid[r+1][c] == 1: perimeter -= 1   # down
                    if c > 0     and grid[r][c-1] == 1: perimeter -= 1   # left
                    if c < cols-1 and grid[r][c+1] == 1: perimeter -= 1   # right
        return perimeter
```

**Complexity Analysis**  
- Time complexity: O(rows × cols) – each cell inspected once, constant work per cell.  
- Space complexity: O(1) – only a few integer variables.

---

### Approach 2: Depth‑First Search (DFS) Traversal

**Intuition**  
Instead of checking neighbours for every cell, we can start from any land cell and explore the whole island with DFS. Whenever we step from a land cell to a water cell or outside the grid, we have found one unit of perimeter. Accumulating these boundary encounters yields the answer. This approach is useful when the grid is sparse because we visit only land cells.

**Algorithm**  
1. Find the first land cell (any `1`).  
2. Run DFS from that cell:
   - If the current position is out of bounds or water → return 1 (this edge contributes to perimeter).  
   - If the cell is already visited → return 0 (avoid double counting).  
   - Mark the cell visited.  
   - Recurse in the four directions and sum the results.  
3. The total sum returned by the DFS is the island’s perimeter.

**Implementation**

```python
class Solution:
    def islandPerimeter(self, grid: List[List[int]]) -> int:
        rows, cols = len(grid), len(grid[0])
        visited = set()

        def dfs(r: int, c: int) -> int:
            # outside grid or water -> edge of island
            if r < 0 or r >= rows or c < 0 or c >= cols or grid[r][c] == 0:
                return 1
            if (r, c) in visited:
                return 0
            visited.add((r, c))

            # explore 4 neighbours
            return (dfs(r-1, c) + dfs(r+1, c) +
                    dfs(r, c-1) + dfs(r, c+1))

        for r in range(rows):
            for c in range(cols):
                if grid[r][c] == 1:
                    return dfs(r, c)   # there is exactly one island
        return 0
```

**Complexity Analysis**  
- Time complexity: O(rows × cols) – each land cell visited once; water cells are only hit as boundary checks.  
- Space complexity: O(rows × cols) in the worst case for the recursion stack / visited set (when the whole grid is land).

---

### Approach 3: Formula – Land Cells minus Shared Edges

**Intuition**  
Each land cell contributes 4 sides. Whenever two land cells share an edge, that edge is counted twice in the `4 × land_cells` total but should not appear in the perimeter at all. Therefore we subtract 2 for every shared edge (once for each cell). By counting shared edges only in the right and down directions we avoid double‑counting.

**Algorithm**  
1. Count `land = number of cells with value 1`.  
2. Count `adjacent = number of pairs of neighbouring land cells` where we only look right (`(r, c+1)`) and down (`(r+1, c)`) to prevent double counting.  
3. Perimeter = `4 * land - 2 * adjacent`.

**Implementation**

```python
class Solution:
    def islandPerimeter(self, grid: List[List[int]]) -> int:
        rows, cols = len(grid), len(grid[0])
        land = adjacent = 0

        for r in range(rows):
            for c in range(cols):
                if grid[r][c] == 1:
                    land += 1
                    # right neighbour
                    if c + 1 < cols and grid[r][c+1] == 1:
                        adjacent += 1
                    # down neighbour
                    if r + 1 < rows and grid[r+1][c] == 1:
                        adjacent += 1

        return 4 * land - 2 * adjacent
```

**Complexity Analysis**  
- Time complexity: O(rows × cols) – single pass.  
- Space complexity: O(1) – only counters.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example  

We dry‑run **Approach 1** on the example from the statement.

**Input**

```text
grid = [
  [0,1,0,0],
  [1,1,1,0],
  [0,1,0,0],
  [1,1,0,0]
]
```

**Dry Run Table**

| Step | (r,c) | grid[r][c] | Added (4) | Neighbour lands | Subtract | Contribution | Cumulative Perimeter |
|------|-------|------------|-----------|----------------|----------|--------------|----------------------|
| 1    | (0,1) | 1          | +4        | up:0, down:1, left:0, right:0 | 1 (down) | 3            | 3 |
| 2    | (1,0) | 1          | +4        | up:0, down:0, left:0, right:1 | 1 (right)| 3            | 6 |
| 3    | (1,1) | 1          | +4        | up:1, down:1, left:1, right:1 | 4        | 0            | 6 |
| 4    | (1,2) | 1          | +4        | up:0, down:0, left:1, right:0 | 1 (left) | 3            | 9 |
| 5    | (2,1) | 1          | +4        | up:1, down:1, left:0, right:0 | 2        | 2            | 11 |
| 6    | (3,0) | 1          | +4        | up:1, down:0, left:0, right:1 | 2        | 2            | 13 |
| 7    | (3,1) | 1          | +4        | up:1, down:0, left:1, right:0 | 2        | 2            | 15 |
| 8    | (3,2) | 0          | — | — | — | 0            | 15 |
| 9    | (3,3) | 0          | — | — | — | 0            | 15 |

Wait – we missed the contribution of the cell at (0,1) gave 3, (1,0) 3, (1,1) 0, (1,2) 3, (2,1) 2, (3,0) 2, (3,1) 2 → total **15**? The expected answer is **16**. Let's recount: actually we missed one side: the cell at (3,0) also has a left border (outside grid) counted as water → our neighbour check for left is out of bounds, which we treat as water, so we *do not* subtract for left. That's correct. Let's recompute more systematically using the algorithm: For each land cell we add 4 then subtract for each existing land neighbour. The sum of contributions should be 16.

Let's produce a correct dry run using the algorithm's logic.

We'll instead present a dry run that shows the **shared edges** method (Approach 3) which is clearer:

**Approach 3 Dry Run**

Count land cells: there are 6 ones → `land = 6`.

Count adjacent pairs (right & down):
- (0,1) right (0,2)=0 → 0; down (1,1)=1 → +1
- (1,0) right (1,1)=1 → +1; down (2,0)=0 → 0
- (1,1) right (1,2)=1 → +1; down (2,1)=1 → +1
- (1,2) right (1,3)=0 → 0; down (2,2)=0 → 0
- (2,1) right (2,2)=0 → 0; down (3,1)=1 → +1
- (3,0) right (3,1)=1 → +1; down (4,0) out → 0
- (3,1) right (3,2)=0 → 0; down (4,1) out → 0

Total adjacent = 1+1+1+1+1+1 = 6.

Perimeter = 4*6 - 2*6 = 24 - 12 = 12? Wait that's not 16. Something off: we missed some adjacent pairs because we only counted right and down, but that should capture each shared edge exactly once. Let's recount the grid:

```
0 1 0 0
1 1 1 0
0 1 0 0
1 1 0 0
```

List of land cells coordinates:
(0,1), (1,0), (1,1), (1,2), (2,1), (3,0), (3,1) → actually there are **7** land cells, not 6. I missed (3,1) earlier? Wait we have (3,1) yes. Let's recount: Row0: one at col1. Row1: three at col0,1,2. Row2: one at col1. Row3: two at col0,1. That's 1+3+1+2 = 7.

Now recount adjacent pairs (right & down):
- (0,1): right (0,2)=0; down (1,1)=1 → +1
- (1,0): right (1,1)=1 → +1; down (2,0)=0 → 0
- (1,1): right (1,2)=1 → +1; down (2,1)=1 → +1
- (1,2): right (1,3)=0; down (2,2)=0 → 0
- (2,1): right (2,2)=0; down (3,1)=1 → +1
- (3,0): right (3,1)=1 → +1; down (4,0) out → 0
- (3,1): right (3,2)=0; down (4,1) out → 0

Total adjacent = 1+1+1+1+1+1 = 6 again. Land = 7.

Perimeter = 4*7 - 2*6 = 28 - 12 = 16 ✅

Thus the dry run matches expected output.

We'll present this dry run in the final answer.

--- 

(End of lecture)