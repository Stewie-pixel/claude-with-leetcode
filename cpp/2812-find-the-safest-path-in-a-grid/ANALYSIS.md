# Binary Search

## Video Solution

For more details about **Find the Safest Path in a Grid**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=-5mQcNiVWTs).

## Concept

Sometimes the answer we seek is not a direct computation but a value we can **test** for feasibility.  
If we can answer the question “Is there a path whose safeness factor is at least **X**?” efficiently, then the original problem becomes: find the largest X for which the answer is *yes*.  
When the feasibility predicate is **monotonic** (if X works, any smaller value also works), we can binary‑search on X instead of scanning every possibility.

In this problem:

1. First compute, for every cell, its Manhattan distance to the nearest thief (multi‑source BFS).  
   This gives us a `dist[r][c]` = safeness contributed by that cell alone.
2. A path’s safeness factor is the minimum `dist` among its cells.  
   So a path has safeness ≥ v **iff** every cell on the path has `dist ≥ v`.
3. The predicate “exists a path from (0,0) to (n‑1,n‑1) using only cells with `dist ≥ v`” is monotonic in v.  
   Hence we binary‑search the maximum v.

## When to Use It

Use binary search on answer when you notice:

- The objective is to **maximize** or **minimize** some value.
- You can efficiently check **feasibility** for a candidate value (usually via BFS/DFS, DP, greedy, etc.).
- The feasibility function is **monotonic** with respect to the candidate value.

Common clues: “maximum minimum distance”, “largest possible … such that …”, “minimum threshold to make something possible”.

## Template

```python
def possible(value: int) -> bool:
    """Return True if we can achieve the goal with at least `value`."""
    # implement feasibility test (BFS/DFS, greedy, etc.)
    ...

low, high = 0, max_possible   # inclusive bounds
answer = 0
while low <= high:
    mid = (low + high) // 2
    if possible(mid):
        answer = mid          # mid works, try to go higher
        low = mid + 1
    else:
        high = mid - 1        # mid fails, go lower
return answer
```

## LeetCode Problem Walkthrough

### Problem: 2812. Find the Safest Path in a Grid

https://leetcode.com/problems/find-the-safest-path-in-a-grid/

### Approach 1: Brute Force Linear Scan

**Algorithm**

1. Run multi‑source BFS from all thieves to compute `dist[r][c]` = distance to nearest thief for every cell.  
2. Determine the maximum possible safeness (`maxDist`) – the largest value in `dist`.  
3. For each candidate safeness `v` from `0` to `maxDist` (inclusive):
   - Run a BFS/DFS that only steps onto cells with `dist ≥ v`.  
   - If we can reach `(n‑1, n‑1)`, record `v` as a feasible answer.
4. Return the largest feasible `v`.

**Implementation**

```python
from collections import deque
from typing import List

class Solution:
    def maximumSafenessFactor(self, grid: List[List[int]]) -> int:
        n = len(grid)
        # ---------- multi-source BFS for distances ----------
        dist = [[float('inf')] * n for _ in range(n)]
        q = deque()
        for r in range(n):
            for c in range(n):
                if grid[r][c] == 1:
                    dist[r][c] = 0
                    q.append((r, c))

        dirs = [(1,0),(-1,0),(0,1),(0,-1)]
        while q:
            r, c = q.popleft()
            for dr, dc in dirs:
                nr, nc = r + dr, c + dc
                if 0 <= nr < n and 0 <= nc < n and dist[nr][nc] > dist[r][c] + 1:
                    dist[nr][nc] = dist[r][c] + 1
                    q.append((nr, nc))

        # ---------- brute force scan ----------
        max_dist = max(max(row) for row in dist)   # upper bound
        def can_reach(v: int) -> bool:
            if dist[0][0] < v or dist[n-1][n-1] < v:
                return False
            visited = [[False]*n for _ in range(n)]
            dq = deque([(0,0)])
            visited[0][0] = True
            while dq:
                r, c = dq.popleft()
                if r == n-1 and c == n-1:
                    return True
                for dr, dc in dirs:
                    nr, nc = r+dr, c+dc
                    if 0 <= nr < n and 0 <= nc < n and not visited[nr][nc] and dist[nr][nc] >= v:
                        visited[nr][nc] = True
                        dq.append((nr,nc))
            return False

        answer = 0
        for v in range(max_dist + 1):
            if can_reach(v):
                answer = v
        return answer
```

**Complexity Analysis**

- Time complexity: O(maxDist · n²) – we run a BFS (O(n²)) for every possible safeness value (maxDist ≤ 2n).  
- Space complexity: O(n²) for the distance matrix and visited array.

### Approach 2: Binary Search on Answer (Optimal)

**Intuition**  
The feasibility test from the brute force (`can_reach(v)`) is monotonic: if a path exists with safeness ≥ v, it also exists for any smaller threshold. Therefore we can binary‑search the largest v instead of scanning linearly.

**Algorithm**

1. Compute `dist` with multi‑source BFS (same as before).  
2. Set search interval `[low, high]` where `low = 0` and `high = max(dist)`.  
3. While `low ≤ high`:
   - `mid = (low + high) // 2`.
   - If `can_reach(mid)` is true, store `mid` as a candidate answer and search higher (`low = mid + 1`).  
   - Otherwise search lower (`high = mid - 1`).  
4. Return the best answer found.

**Implementation**

```python
from collections import deque
from typing import List

class Solution:
    def maximumSafenessFactor(self, grid: List[List[int]]) -> int:
        n = len(grid)
        # ---------- 1. Multi-source BFS: distance to nearest thief ----------
        dist = [[float('inf')] * n for _ in range(n)]
        q = deque()
        for r in range(n):
            for c in range(n):
                if grid[r][c] == 1:
                    dist[r][c] = 0
                    q.append((r, c))

        dirs = [(1,0),(-1,0),(0,1),(0,-1)]
        while q:
            r, c = q.popleft()
            for dr, dc in dirs:
                nr, nc = r + dr, c + dc
                if 0 <= nr < n and 0 <= nc < n and dist[nr][nc] > dist[r][c] + 1:
                    dist[nr][nc] = dist[r][c] + 1
                    q.append((nr, nc))

        # ---------- 2. Feasibility test (BFS) ----------
        def can_reach(threshold: int) -> bool:
            if dist[0][0] < threshold or dist[n-1][n-1] < threshold:
                return False
            visited = [[False]*n for _ in range(n)]
            dq = deque([(0,0)])
            visited[0][0] = True
            while dq:
                r, c = dq.popleft()
                if r == n-1 and c == n-1:
                    return True
                for dr, dc in dirs:
                    nr, nc = r+dr, c+dc
                    if 0 <= nr < n and 0 <= nc < n and not visited[nr][nc] and dist[nr][nc] >= threshold:
                        visited[nr][nc] = True
                        dq.append((nr,nc))
            return False

        # ---------- 3. Binary search on answer ----------
        low, high = 0, max(max(row) for row in dist)
        ans = 0
        while low <= high:
            mid = (low + high) // 2
            if can_reach(mid):
                ans = mid
                low = mid + 1          # try to increase safeness
            else:
                high = mid - 1         # need lower threshold
        return ans
```

**Complexity Analysis**

- Time complexity: O(n² · log (maxDist)) – one multi‑source BFS (O(n²)) + O(log (maxDist)) feasibility BFS calls, each O(n²).  
- Space complexity: O(n²) for `dist` and the visited matrix.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the algorithm on Example 2:

```
grid = [[0,0,1],
        [0,0,0],
        [0,0,0]]
```

**Step 1 – Multi‑source BFS distances**

```
Initial thieves at (0,2) → distance 0.
BFS spreads outward:

dist =
[[2,1,0],
 [1,2,1],
 [2,3,2]]
```

**Step 2 – Binary search**

- `low = 0`, `high = max(dist) = 3`.

| Iteration | mid | can_reach(mid)? | Action                | low | high | ans |
|-----------|-----|-----------------|-----------------------|-----|------|-----|
| 1         | 1   | True            | store ans=1, go higher| 2   | 3    | 1   |
| 2         | 2   | True            | store ans=2, go higher| 3   | 3    | 2   |
| 3         | 3   | False           | go lower              | 3   | 2    | 2   |
| 4         |     | low > high → stop|                       |     |      | 2   |

Result `ans = 2`, matching the expected output.

The dry run shows how each mid‑value is tested with a simple BFS that only walks through cells whose `dist ≥ mid`. The monotonic nature lets us halve the search space each step.
