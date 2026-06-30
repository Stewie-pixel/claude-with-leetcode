# Graphs

## Video Solution

For more details about **Map of Highest Peak**, watch the walkthrough at [https://www.youtube.com/watch?v=cQRZ202j-kA](https://www.youtube.com/watch?v=cQRZ202j-kA)

## Concept

Multi‑source Breadth‑First Search (BFS) starts simultaneously from **all** source nodes (here, all water cells) and expands outward level by level.  
Because each edge has equal cost (moving to a neighboring cell increases height by 1), the first time we reach a cell we have found its **minimum** distance to any water source.  
Assigning each cell that minimum distance yields the highest possible peak while respecting the height‑difference‑≤ 1 rule.

## When to Use It

Use multi‑source BFS when you see:
- A grid/matrix with **multiple starting points** (e.g., all 0s in a binary matrix).
- The goal is to compute the **minimum distance** from each cell to the nearest starting point under **uniform edge cost**.
- The problem statement mentions “maximize the maximum height” or “assign values such that adjacent cells differ by at most 1”, which is equivalent to a distance transform.

## Template

```python
from collections import deque

def multi_source_bfs(grid, is_source):
    """
    grid:      m x n matrix to fill with distances
    is_source: function returning True for source cells
    Returns:   grid where each cell holds its distance to the nearest source
    """
    m, n = len(grid), len(grid[0])
    dist = [[-1] * n for _ in range(m)]   # -1 means unvisited
    q = deque()

    # 1️⃣  enqueue all sources with distance 0
    for i in range(m):
        for j in range(n):
            if is_source(i, j):
                dist[i][j] = 0
                q.append((i, j))

    # 2️⃣  BFS layer‑by‑layer
    while q:
        x, y = q.popleft()
        for dx, dy in [(1,0), (-1,0), (0,1), (0,-1)]:
            nx, ny = x + dx, y + dy
            if 0 <= nx < m and 0 <= ny < n and dist[nx][ny] == -1:
                dist[nx][ny] = dist[x][y] + 1
                q.append((nx, ny))

    return dist
```

## LeetCode Problem Walkthrough

### Problem: 1765. Map of Highest Peak  
https://leetcode.com/problems/map-of-highest-peak/

---

### Approach 1: Brute Force – BFS from each water cell

**Algorithm**  
1. For every water cell, run a standard BFS that expands outward, recording the distance from that source.  
2. Keep a `height` matrix initialized with a large value.  
3. After each BFS, update `height[i][j] = min(height[i][j], distance_from_this_source)`.  
4. The final `height` holds the minimum distance to any water source.

**Implementation**

```python
from collections import deque
from typing import List

class Solution:
    def highestPeak(self, isWater: List[List[int]]) -> List[List[int]]:
        m, n = len(isWater), len(isWater[0])
        height = [[float('inf')] * n for _ in range(m)]

        def bfs(sr: int, sc: int):
            """Standard BFS that fills distances from (sr, sc)."""
            dist = [[-1] * n for _ in range(m)]
            q = deque([(sr, sc)])
            dist[sr][sc] = 0
            while q:
                x, y = q.popleft()
                for dx, dy in ((1,0),(-1,0),(0,1),(0,-1)):
                    nx, ny = x + dx, y + dy
                    if 0 <= nx < m and 0 <= ny < n and dist[nx][ny] == -1:
                        dist[nx][ny] = dist[x][y] + 1
                        q.append((nx, ny))
            # merge results
            for i in range(m):
                for j in range(n):
                    if dist[i][j] != -1:
                        height[i][j] = min(height[i][j], dist[i][j])

        # launch BFS from every water cell
        for i in range(m):
            for j in range(n):
                if isWater[i][j] == 1:
                    bfs(i, j)

        return height
```

**Complexity Analysis**  
- Time complexity: O(k·m·n) – each of the *k* water cells triggers a BFS that may visit every cell.  
- Space complexity: O(m·n) for the distance matrix plus the output matrix.

---

### Approach 2: Intermediate – Multi‑source BFS

**Intuition**  
Instead of repeating BFS from each source, we can start with **all** water cells in the queue at distance 0. The first time we reach a cell we have already found the shortest path to *any* source, because BFS explores in increasing distance order.

**Algorithm**  
1. Initialise `height` with a sentinel (e.g., `-1`).  
2. Enqueue every water cell with height 0.  
3. Perform BFS: pop a cell, try its four neighbours; if a neighbour is unvisited, set its height to current height + 1 and enqueue it.  
4. When the queue empties, `height` contains the minimum distance to water for every cell.

**Implementation**

```python
from collections import deque
from typing import List

class Solution:
    def highestPeak(self, isWater: List[List[int]]) -> List[List[int]]:
        m, n = len(isWater), len(isWater[0])
        height = [[-1] * n for _ in range(m)]
        q = deque()

        # 1️⃣  seed all water cells
        for i in range(m):
            for j in range(n):
                if isWater[i][j] == 1:
                    height[i][j] = 0
                    q.append((i, j))

        # 2️⃣  multi‑source BFS
        while q:
            x, y = q.popleft()
            for dx, dy in ((1,0),(-1,0),(0,1),(0,-1)):
                nx, ny = x + dx, y + dy
                if 0 <= nx < m and 0 <= ny < n and height[nx][ny] == -1:
                    height[nx][ny] = height[x][y] + 1
                    q.append((nx, ny))

        return height
```

**Complexity Analysis**  
- Time complexity: O(m·n) – each cell is processed at most once.  
- Space complexity: O(m·n) for the `height` matrix and the BFS queue.

---

### Approach 3: Most Optimal – Two‑Pass DP (distance transform)

**Intuition**  
The Manhattan distance transform can be computed in two linear scans without an explicit queue:  
- First pass (top‑left → bottom‑right) propagates distances from the top and left neighbours.  
- Second pass (bottom‑right → top‑left) propagates from the bottom and right neighbours, taking the minimum of the two passes.  
This yields the exact same result as multi‑source BFS but uses only O(1) extra space (aside from the output).

**Algorithm**  
1. Create `height` matrix, set water cells to 0 and land cells to a large number (e.g., `m+n`).  
2. **Forward pass:** for each cell, `height[i][j] = min(height[i][j], min(top, left) + 1)`.  
3. **Backward pass:** for each cell in reverse order, `height[i][j] = min(height[i][j], min(bottom, right) + 1)`.  
4. Return `height`.

**Implementation**

```python
from typing import List

class Solution:
    def highestPeak(self, isWater: List[List[int]]) -> List[List[int]]:
        m, n = len(isWater), len(isWater[0])
        INF = m + n                     # larger than any possible distance
        height = [[INF] * n for _ in range(m)]

        # 0 for water, INF for land (will be reduced)
        for i in range(m):
            for j in range(n):
                if isWater[i][j] == 1:
                    height[i][j] = 0

        # -------- forward pass (top‑left) --------
        for i in range(m):
            for j in range(n):
                if i > 0:
                    height[i][j] = min(height[i][j], height[i-1][j] + 1)
                if j > 0:
                    height[i][j] = min(height[i][j], height[i][j-1] + 1)

        # -------- backward pass (bottom‑right) --------
        for i in range(m-1, -1, -1):
            for j in range(n-1, -1, -1):
                if i < m-1:
                    height[i][j] = min(height[i][j], height[i+1][j] + 1)
                if j < n-1:
                    height[i][j] = min(height[i][j], height[i][j+1] + 1)

        return height
```

**Complexity Analysis**  
- Time complexity: O(m·n) – two simple matrix sweeps.  
- Space complexity: O(1) extra (the output matrix does not count).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the multi‑source BFS on the first example:

```
Input:
isWater = [[0,1],
           [0,0]]
```

We start with the single water cell at (0,1) with height 0.

```
Initial queue: [(0,1)]
height matrix ( -1 = unvisited ):
[[-1,  0],
 [-1, -1]]
```

**Step 1** – pop (0,1)  
Neighbours: (1,1) and (0,0) get height 1.

```
Queue: [(1,1), (0,0)]
height:
[[-1,  0],
 [-1,  1]]
```

**Step 2** – pop (1,1)  
Neighbour (1,0) gets height 2 (from (1,1)+1). (0,1) already visited.

```
Queue: [(0,0), (1,0)]
height:
[[-1,  0],
 [ 2,  1]]
```

**Step 3** – pop (0,0)  
Neighbour (0,1) already visited, (1,0) already has height 2 (no improvement).

```
Queue: [(1,0)]
height:
[[-1,  0],
 [ 2,  1]]
```

**Step 4** – pop (1,0)  
All neighbours already visited or out of bounds.

```
Queue: []
height:
[[1, 0],
 [2, 1]]
```

The final matrix matches the sample output. Each cell’s value equals its shortest Manhattan distance to any water cell, which is the highest possible peak under the constraints.

--- 

*End of lecture.*