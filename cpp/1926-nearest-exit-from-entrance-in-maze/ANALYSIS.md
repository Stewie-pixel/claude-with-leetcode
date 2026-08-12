# Tree BFS

## Video Solution

For more details about **Nearest Exit from Entrance in Maze**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=9a1QRrLICQ0)

## Concept

Breadth‑First Search (BFS) explores a graph level by level. Imagine dropping a drop of ink at the entrance cell and watching it spread simultaneously to all neighboring empty cells each second. The first time the ink reaches any border cell (that isn’t the entrance) we have found the shortest path, because all paths of length k are examined before any path of length k+1.

## When to Use It

Use BFS when you see:
- “shortest number of steps” or “minimum distance” in an **unweighted** grid/graph
- A maze, board, or grid where movement is allowed in the four cardinal directions
- The need to find the nearest target (exit, gate, etc.) from a start point
- All edges have equal cost (here each move costs 1)

## Template

```python
from collections import deque

def bfs(grid, start):
    """
    Generic BFS template for an m x n grid.
    Returns the distance to the first cell that satisfies `is_target`.
    """
    rows, cols = len(grid), len(grid[0])
    q = deque([start])
    visited = set([start])          # or modify grid in‑place if allowed
    steps = 0                       # distance from start to current layer

    while q:
        for _ in range(len(q)):     # process current BFS layer
            r, c = q.popleft()
            if is_target(r, c):     # problem‑specific condition
                return steps
            for dr, dc in [(1,0),(-1,0),(0,1),(0,-1)]:
                nr, nc = r+dr, c+dc
                if 0 <= nr < rows and 0 <= nc < cols \
                   and grid[nr][nc] == '.' and (nr, nc) not in visited:
                    visited.add((nr, nc))
                    q.append((nr, nc))
        steps += 1                  # finished one layer → increase distance
    return -1                       # no target reachable
```

*Replace `is_target` with the problem’s exit condition (border cell ≠ entrance).*

## LeetCode Problem Walkthrough

### Problem: 1926. Nearest Exit from Entrance in Maze
https://leetcode.com/problems/nearest-exit-from-entrance-in-maze/

---

### Approach 1: Brute Force (DFS / Exhaustive Search)

**Algorithm**
1. Run a depth‑first search from the entrance.
2. Keep track of the current path length and a `visited` set to avoid cycles.
3. Whenever we step onto a border cell that is **not** the entrance, update the global answer with the minimum path length seen so far.
4. Backtrack, unmarking the cell so other paths can reuse it.
5. After exploring all possibilities, return the smallest distance found, or `-1` if none.

**Implementation**

```python
class Solution:
    def nearestExit(self, maze: List[List[str]], entrance: List[int]) -> int:
        m, n = len(maze), len(maze[0])
        er, ec = entrance
        self.best = float('inf')
        visited = set()
        visited.add((er, ec))

        def dfs(r: int, c: int, dist: int):
            # prune if already worse than best found
            if dist >= self.best:
                return
            # check if current cell is an exit (border & not entrance)
            if (r == 0 or r == m-1 or c == 0 or c == n-1) and not (r == er and c == ec):
                self.best = min(self.best, dist)
                return
            for dr, dc in [(1,0),(-1,0),(0,1),(0,-1)]:
                nr, nc = r+dr, c+dc
                if 0 <= nr < m and 0 <= nc < n and maze[nr][nc] == '.' and (nr, nc) not in visited:
                    visited.add((nr, nc))
                    dfs(nr, nc, dist+1)
                    visited.remove((nr, nc))   # backtrack

        dfs(er, ec, 0)
        return self.best if self.best != float('inf') else -1
```

**Complexity Analysis**
- Time complexity: O(4^{m*n}) in the worst case – each cell can branch into up to 4 directions and we may explore every simple path.
- Space complexity: O(m*n) for the recursion stack and `visited` set.

---

### Approach 2: Breadth‑First Search (Standard)

**Intuition**
BFS expands uniformly from the start. The first time we reach any border cell (excluding the entrance) we have used the minimal number of steps, because all paths of length `k` are processed before any path of length `k+1`.

**Algorithm**
1. Initialize a queue with the entrance cell and distance `0`.
2. Mark the entrance as visited (we can change `maze[r][c]` to `'+'` to avoid extra space).
3. While the queue is not empty:
   - Process all nodes at the current distance (layer‑by‑layer).
   - For each cell, try the four neighbours.
   - If a neighbour is inside the grid, empty (`'.'`), and not visited:
        - If it lies on the border and is **not** the entrance → return `dist+1`.
        - Otherwise, mark it visited and push it into the queue with distance `dist+1`.
4. If the queue empties without finding an exit, return `-1`.

**Implementation**

```python
from collections import deque

class Solution:
    def nearestExit(self, maze: List[List[str]], entrance: List[int]) -> int:
        m, n = len(maze), len(maze[0])
        er, ec = entrance
        q = deque()
        q.append((er, ec, 0))          # (row, col, distance)
        maze[er][ec] = '+'             # mark entrance as visited

        while q:
            r, c, dist = q.popleft()
            for dr, dc in [(1,0),(-1,0),(0,1),(0,-1)]:
                nr, nc = r+dr, c+dc
                if 0 <= nr < m and 0 <= nc < n and maze[nr][nc] == '.':
                    # Check if this neighbour is an exit (border & not entrance)
                    if nr == 0 or nr == m-1 or nc == 0 or nc == n-1:
                        return dist + 1
                    maze[nr][nc] = '+'  # mark visited
                    q.append((nr, nc, dist+1))
        return -1
```

**Complexity Analysis**
- Time complexity: O(m*n) – each cell is enqueued at most once.
- Space complexity: O(m*n) in the worst case for the queue (holds a frontier of cells).

---

### Approach 3: BFS with Early Exit & In‑Place Visiting (Micro‑optimized)

**Intuition**
The previous BFS already yields optimal O(mn) time. We can shave a constant factor by:
- Using a simple list as a queue with two pointers (avoids `collections.deque` overhead).
- Checking the exit condition **before** enqueuing a neighbour, so we return immediately without an extra loop iteration.
- Reusing the input `maze` for visited marks, eliminating the `visited` set.

These tweaks do not change asymptotic complexity but often run faster in practice.

**Implementation**

```python
class Solution:
    def nearestExit(self, maze: List[List[str]], entrance: List[int]) -> int:
        m, n = len(maze), len(maze[0])
        er, ec = entrance
        # queue implemented as list with head/tail indices for O(1) pops
        q = [(er, ec)]
        head = 0
        maze[er][ec] = '+'          # visited
        steps = 0

        while head < len(q):
            # process current layer
            layer_size = len(q) - head
            for _ in range(layer_size):
                r, c = q[head]
                head += 1
                for dr, dc in [(1,0),(-1,0),(0,1),(0,-1)]:
                    nr, nc = r+dr, c+dc
                    if 0 <= nr < m and 0 <= nc < n and maze[nr][nc] == '.':
                        # exit check before pushing
                        if nr == 0 or nr == m-1 or nc == 0 or nc == n-1:
                            return steps + 1
                        maze[nr][nc] = '+'
                        q.append((nr, nc))
            steps += 1
        return -1
```

**Complexity Analysis**
- Time complexity: O(m*n) – each cell processed once.
- Space complexity: O(m*n) for the queue (same as Approach 2).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace Approach 2 on Example 1.

**Input**
```
maze = [
    ['+', '+', '.', '+'],
    ['.', '.', '.', '+'],
    ['+', '+', '+', '.']
]
entrance = [1, 2]   # cell with '.' at row1,col2
```

**Dry Run**

| Step | Queue (front → rear)                | Steps so far | Action / Note |
|------|-------------------------------------|--------------|---------------|
| 0    | [(1,2,0)]                           | 0            | Start at entrance, marked visited |
| 1    | [(1,1,1), (0,2,1), (2,2,1)]        | 1            | Neighbours: left (1,1), up (0,2), down (2,2). <br>**Up cell (0,2)** is border → **return 1** |

The algorithm stops as soon as it discovers the border cell (0,2), reporting the correct answer = 1.
