# 0‑1 BFS  

## Video Solution  

For more details about **Find a Safe Walk Through a Grid**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=pf0mqwHOFx4).

## Concept  

0‑1 BFS is a specialization of Breadth‑First Search for graphs whose edge weights are only **0** or **1**.  
Instead of using a priority queue (as in Dijkstra), we store vertices in a double‑ended queue (`deque`).  
When we traverse an edge of weight 0 we push the neighbor to the **front** of the deque (because it does not increase the distance), and for weight 1 we push it to the **back**.  
This guarantees that vertices are processed in non‑decreasing order of their distance from the source, giving us the shortest‑path distances in **O(V + E)** time.

A real‑world analogy: imagine you are walking through a hallway where some tiles are slippery (cost 0) and others are sticky (cost 1). You always prefer to step on slippery tiles first, so you keep a list of “to‑visit” tiles where slippery steps go to the front of the list and sticky steps go to the back. You always take the next tile from the front, ensuring you never walk a sticky tile before all reachable slippery ones are exhausted.

## When to Use It  

- The problem can be modeled as a graph where each move has cost **0** or **1**.  
- You need the **minimum total cost** (shortest path) from a source to a target.  
- The graph is unweighted except for these two possible weights.  
- Typical LeetCode clues:  
  - “grid with cells that cost 0 or 1”  
  - “you lose/gain health when stepping on certain cells”  
  - “find if you can reach the end with at most K cost” (often transformed to “is min cost ≤ K?”)  
  - “binary matrix, moving up/down/left/right”  

If you see any of the above, think 0‑1 BFS before reaching for a full Dijkstra implementation.

## Template  

```python
from collections import deque
from typing import List

def zero_one_bfs(grid: List[List[int]], start: tuple[int, int], end: tuple[int, int]) -> int:
    """
    Returns the minimum cost to go from start to end in a grid where
    moving onto a cell costs grid[ni][nj] (0 or 1). If unreachable, returns -1.
    """
    m, n = len(grid), len(grid[0])
    INF = 10**9
    dist = [[INF] * n for _ in range(m)]
    sr, sc = start
    tr, tc = end

    dist[sr][sc] = grid[sr][sc]          # cost to occupy the start cell
    dq = deque()
    dq.append((sr, sc))

    # four directional moves
    dirs = [(1,0), (-1,0), (0,1), (0,-1)]

    while dq:
        r, c = dq.popleft()
        d = dist[r][c]

        for dr, dc in dirs:
            nr, nc = r + dr, c + dc
            if 0 <= nr < m and 0 <= nc < n:
                w = grid[nr][nc]          # cost to step into neighbor (0 or 1)
                nd = d + w
                if nd < dist[nr][nc]:
                    dist[nr][nc] = nd
                    if w == 0:
                        dq.appendleft((nr, nc))   # zero cost → process sooner
                    else:
                        dq.append((nr, nc))       # one cost → process later

    return dist[tr][tc] if dist[tr][tc] != INF else -1
```

> **Note:** The template treats the cost of entering a cell as the cell’s value (0 or 1). Adjust the initialization (`dist[start]`) if the problem defines cost differently (e.g., cost of leaving a cell).

---

## LeetCode Problem Walkthrough  

### Problem: 3286. Find a Safe Walk Through a Grid  

https://leetcode.com/problems/find-a-safe-walk-through-a-grid/

You are given a binary matrix `grid` (0 = safe, 1 = unsafe) and an initial `health`.  
Moving onto an unsafe cell reduces health by 1.  
You start at `(0,0)` and must reach `(m‑1,n‑1)` while health stays **> 0**.  
Return `True` iff you can finish with health ≥ 1.

--------------------------------------------------------------------
### Approach 1: Brute Force (DFS with pruning)

**Algorithm**  
- Perform a depth‑first search from the start cell.  
- Keep track of remaining health; if it drops to 0 or below, abort that branch.  
- Use a visited set that also stores the health left when we arrived at a cell, because reaching the same cell with more health is strictly better.  
- If we reach the target with health ≥ 1, return `True`.  
- Exhaust all possibilities → return `False`.

**Implementation**

```python
class Solution:
    def findSafeWalk(self, grid: List[List[int]], health: int) -> bool:
        m, n = len(grid), len(grid[0])
        from functools import lru_cache

        @lru_cache(None)
        def dfs(r: int, c: int, h: int) -> bool:
            if not (0 <= r < m and 0 <= c < n):
                return False
            h -= grid[r][c]               # pay cost of entering this cell
            if h <= 0:                    # health not positive
                return False
            if r == m-1 and c == n-1:     # reached destination
                return True
            # explore neighbours
            return (dfs(r+1, c, h) or dfs(r-1, c, h) or
                    dfs(r, c+1, h) or dfs(r, c-1, h))

        return dfs(0, 0, health)
```

**Complexity Analysis**  
- Time complexity: O(4^(m*n)) in the worst case (every cell visited with many different health values).  
- Space complexity: O(m*n*H) for the memoization table, where H ≤ health ≤ m+n.

--------------------------------------------------------------------
### Approach 2: Dijkstra’s Algorithm (Priority Queue)

**Intuition**  
The grid can be seen as a graph where each cell is a node and edges connect 4‑neighbour cells.  
Moving into a cell costs `grid[ni][nj]` (0 or 1).  
We need the **minimum total cost** to reach the target; if that cost `< health` we succeed.  
Dijkstra’s algorithm works for any non‑negative edge weights, giving us the exact shortest‑path cost.

**Algorithm**  
- Initialize a distance matrix with ∞, set distance of start cell to its cost (`grid[0][0]`).  
- Use a min‑heap priority queue storing `(dist, r, c)`.  
- Pop the cell with smallest current distance, relax its 4 neighbours:  
  `newDist = dist[r][c] + grid[nr][nc]`.  
  If `newDist < dist[nr][nc]`, update and push to heap.  
- After the loop, compare `dist[target]` with `health`.

**Implementation**

```python
import heapq

class Solution:
    def findSafeWalk(self, grid: List[List[int]], health: int) -> bool:
        m, n = len(grid), len(grid[0])
        INF = 10**9
        dist = [[INF] * n for _ in range(m)]
        dist[0][0] = grid[0][0]
        heap = [(dist[0][0], 0, 0)]

        dirs = [(1,0), (-1,0), (0,1), (0,-1)]

        while heap:
            d, r, c = heapq.heappop(heap)
            if d != dist[r][c]:
                continue          # stale entry
            if r == m-1 and c == n-1:
                break
            for dr, dc in dirs:
                nr, nc = r + dr, c + dc
                if 0 <= nr < m and 0 <= nc < n:
                    nd = d + grid[nr][nc]
                    if nd < dist[nr][nc]:
                        dist[nr][nc] = nd
                        heapq.heappush(heap, (nd, nr, nc))

        return dist[m-1][n-1] < health
```

**Complexity Analysis**  
- Time complexity: O((m*n) log(m*n)) – each cell may be pushed into the heap several times, but heap operations are log‑size.  
- Space complexity: O(m*n) for the distance matrix and heap.

--------------------------------------------------------------------
### Approach 3: 0‑1 BFS (Deque) – Most Optimal

**Intuition**  
Because every edge weight is either 0 or 1, we can improve Dijkstra’s O(log V) factor to O(1) per edge by using a deque:  
- When we traverse a 0‑cost edge, the resulting distance is **not larger** than the current node’s distance, so the neighbor should be processed **before** any nodes at the current distance → push to the **front** of the deque.  
- When we traverse a 1‑cost edge, the distance increases by exactly 1, so the neighbor belongs to the **next** layer → push to the **back**.  
Processing nodes in this order yields distances in non‑decreasing order, exactly what Dijkstra guarantees, but with linear time.

**Algorithm**  
- Same distance matrix initialization as Dijkstra.  
- Use a `deque`. Push the start cell to the front.  
- While deque not empty: pop from the front, examine 4 neighbours.  
- Compute `newDist = dist[r][c] + grid[nr][nc]`.  
- If `newDist < dist[nr][nc]`: update distance and  
  - push neighbor to **front** if `grid[nr][nc] == 0`  
  - else push to **back**.  
- After traversal, check `dist[target] < health`.

**Implementation**

```python
from collections import deque
from typing import List

class Solution:
    def findSafeWalk(self, grid: List[List[int]], health: int) -> bool:
        m, n = len(grid), len(grid[0])
        INF = 10**9
        dist = [[INF] * n for _ in range(m)]
        dist[0][0] = grid[0][0]               # cost to occupy start cell
        dq = deque()
        dq.append((0, 0))

        dirs = [(1,0), (-1,0), (0,1), (0,-1)]

        while dq:
            r, c = dq.popleft()
            d = dist[r][c]

            for dr, dc in dirs:
                nr, nc = r + dr, c + dc
                if 0 <= nr < m and 0 <= nc < n:
                    w = grid[nr][nc]          # 0 or 1
                    nd = d + w
                    if nd < dist[nr][nc]:
                        dist[nr][nc] = nd
                        if w == 0:
                            dq.appendleft((nr, nc))   # zero‑cost edge → higher priority
                        else:
                            dq.append((nr, nc))       # one‑cost edge → later

        return dist[m-1][n-1] < health
```

**Complexity Analysis**  
- Time complexity: O(m * n) – each edge is relaxed at most once because each time we improve a distance we push the vertex, and distances only decrease.  
- Space complexity: O(m * n) for the distance matrix and the deque (worst‑case holds all vertices).

--------------------------------------------------------------------
### Provide a Visual Demonstration  

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example  

We dry‑run the 0‑1 BFS on the first example:

```
grid = [[0,1,0,0,0],
        [0,1,0,1,0],
        [0,0,0,1,0]]
health = 1
```

We start at (0,0) with cost = grid[0][0] = 0.

| Step | Cell popped (r,c) | dist[r][c] | Action (neighbour, w) | newDist | Updated? | Deque after push (front← … →back) |
|------|-------------------|------------|-----------------------|---------|----------|-----------------------------------|
| 1    | (0,0)             | 0          | (0,1) w=1 → nd=1      | 1       | yes (was INF) | push_back → [(0,1)] |
|      |                   |            | (1,0) w=0 → nd=0      | 0       | yes      | push_front → [(1,0),(0,1)] |
| 2    | (1,0) (front)     | 0          | (0,0) w=0 → nd=0 (no) | — | no | [(0,1)] |
|      |                   |            | (2,0) w=0 → nd=0      | 0       | yes      | push_front → [(2,0),(0,1)] |
|      |                   |            | (1,1) w=1 → nd=1      | 1       | yes      | push_back → [(2,0),(0,1),(1,1)] |
| 3    | (2,0) (front)     | 0          | (1,0) w=0 → nd=0 (no) | — | no | [(0,1),(1,1)] |
|      |                   |            | (2,1) w=0 → nd=0      | 0       | yes      | push_front → [(2,1),(0,1),(1,1)] |
| 4    | (2,1) (front)     | 0          | (2,0) w=0 (no)        | — | no | [(0,1),(1,1)] |
|      |                   |            | (2,2) w=0 → nd=0      | 0       | yes      | push_front → [(2,2),(0,1),(1,1)] |
|      |                   |            | (1,1) w=1 → nd=1 (no) | — | no | [(2,2),(0,1),(1,1)] |
| 5    | (2,2) (front)     | 0          | (2,1) w=0 (no)        | — | no | [(0,1),(1,1)] |
|      |                   |            | (2,3) w=1 → nd=1      | 1       | yes      | push_back → [(0,1),(1,1),(2,3)] |
|      |                   |            | (1,2) w=0 → nd=0      | 0       | yes      | push_front → [(1,2),(0,1),(1,1),(2,3)] |
| 6    | (1,2) (front)     | 0          | (0,2) w=0 → nd=0      | 0       | yes      | push_front → [(0,2),(0,1),(1,1),(2,3)] |
|      |                   |            | (2,2) w=0 → nd=0 (no) | — | no | [(0,2),(0,1),(1,1),(2,3)] |
|      |                   |            | (1,3) w=1 → nd=1      | 1       | yes      | push_back → [(0,2),(0,1),(1,1),(2,3),(1,3)] |
| 7    | (0,2) (front)     | 0          | (0,1) w=1 → nd=1 (no) | — | no | [(0,1),(1,1),(2,3),(1,3)] |
|      |                   |            | (0,3) w=0 → nd=0      | 0       | yes      | push_front → [(0,3),(0,1),(1,1),(2,3),(1,3)] |
| 8    | (0,3) (front)     | 0          | (0,2) w=0 (no)        | — | no | [(0,1),(1,1),(2,3),(1,3)] |
|      |                   |            | (0,4) w=0 → nd=0      | 0       | yes      | push_front → [(0,4),(0,1),(1,1),(2,3),(1,3)] |
|      |                   |            | (1,3) w=1 → nd=1 (no) | — | no | [(0,4),(0,1),(1,1),(2,3),(1,3)] |
| 9    | (0,4) (front)     | 0          | (0,3) w=0 (no)        | — | no | [(0,1),(1,1),(2,3),(1,3)] |
|      |                   |            | (1,4) w=0 → nd=0      | 0       | yes      | push_front → [(1,4),(0,1),(1,1),(2,3),(1,3)] |
|10    | (1,4) (front)     | 0          | (0,4) w=0 (no)        | — | no | [(0,1),(1,1),(2,3),(1,3)] |
|      |                   |            | (2,4) w=0 → nd=0      | 0       | yes      | push_front → [(2,4),(0,1),(1,1),(2,3),(1,3)] |
|11    | (2,4) (front)     | 0          | (1,4) w=0 (no)        | — | no | [(0,1),(1,1),(2,3),(1,3)] |
|      |                   |            | (2,3) w=1 → nd=1      | 1       | yes (was INF) | push_back → [(0,1),(1,1),(2,3),(1,3),(2,3)] |
|12    | (0,1) (front)     | 1          | (0,0) w=0 → nd=1 (no) | — | no | [(1,1),(2,3),(1,3),(2,3)] |
|      |                   |            | (0,2) w=0 → nd=1 (no) | — | no | [(1,1),(2,3),(1,3),(2,3)] |
|      |                   |            | (1,1) w=1 → nd=2 (no) | — | no | [(1,1),(2,3),(1,3),(2,3)] |
|13    | (1,1) (front)     | 1          | (0,1) w=1 → nd=2 (no) | — | no | [(2,3),(1,3),(2,3)] |
|      |                   |            | (2,1) w=0 → nd=1      | 1       | yes (was INF) | push_front → [(2,1),(2,3),(1,3),(2,3)] |
|      |                   |            | (1,0) w=0 → nd=1 (no) | — | no | [(2,1),(2,3),(1,3),(2,3)] |
|      |                   |            | (1,2) w=0 → nd=1 (no) | — | no | [(2,1),(2,3),(1,3),(2,3)] |
|14    | (2,1) (front)     | 1          | (2,0) w=0 → nd=1 (no) | — | no | [(2,3),(1,3),(2,3)] |
|      |                   |            | (2,2) w=0 → nd=1 (no) | — | no | [(2,3),(1,3),(2,3)] |
|      |                   |            | (1,1) w=1 → nd=2 (no) | — | no | [(2,3),(1,3),(2,3)] |
|      |                   |            | (3,1) out of bounds   | — | — | [(2,3),(1,3),(2,3)] |
|15    | (2,3) (front)     | 1          | (2,2) w=0 → nd=1 (no) | — | no | [(1,3),(2,3)] |
|      |                   |            | (2,4) w=0 → nd=1 (no) | — | no | [(1,3),(2,3)] |
|      |                   |            | (1,3) w=1 → nd=2 (no) | — | no | [(1,3),(2,3)] |
|      |                   |            | (3,3) out of bounds   | — | — | [(1,3),(2,3)] |
|16    | (1,3) (front)     | 1          | (0,3) w=0 → nd=1 (no) | — | no | [(2,3)] |
|      |                   |            | (2,3) w=1 → nd=2 (no) | — | no | [(2,3)] |
|      |                   |            | (1,2) w=0 → nd=1 (no) | — | no | [(2,3)] |
|      |                   |            | (1,4) w=0 → nd=1 (no) | — | no | [(2,3)] |
|17    | (2,3) (front)     | 1          | … (no further improvements) | | | [] (deque empty) |

At the end, `dist[2][4] = 0`.  
Since `0 < health (1)`, we return **True** – a safe walk exists.

--------------------------------------------------------------------

**Summary of the three approaches**

| Approach | When to use | Time | Space |
|----------|-------------|------|-------|
| Brute‑force DFS | Tiny grids, need all paths | Exponential | O(m*n*health) |
| Dijkstra (PQ) | General 0/1 weighted graph, easy to implement | O(V log V) | O(V) |
| **0‑1 BFS** (optimal) | Edge weights are only 0 or 1 – the case here | **O(V + E)** | O(V) |

Because the grid only contains 0/1 costs, 0‑1 BFS gives the fastest linear‑time solution while staying simple to code. Use it whenever you see a “cost is 0 or 1” pattern in a grid or graph problem.