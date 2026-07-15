# Dynamic Programming (2D Grid)

## Video Solution

For more details about **Number of Paths with Max Score**, watch the walkthrough at [https://www.youtube.com/watch?v=ySMrJ4N94JM](https://www.youtube.com/watch?v=ySMrJ4N94JM)

## Concept

Dynamic Programming on a grid solves problems where you need to compute an optimal value (maximum/minimum sum, shortest path, etc.) while moving only in allowed directions.  
We store two pieces of information for each cell:

* `dp[i][j]` – the best score achievable **from** cell `(i,j)` to the destination.
* `cnt[i][j]` – the number of ways to achieve that best score (mod `1e9+7`).

By processing cells in reverse topological order (bottom‑right → top‑left) we guarantee that when we compute a cell, all cells we can move to have already been solved.

## When to Use It

Use this pattern when you see:

* A 2D board/grid with movement restricted to a set of directions (e.g., 
* A goal to **optimize** a value (sum, cost, length) **and** count how many ways achieve that optimum. 
* Obstacles that block certain cells. 
* The grid size is up to ~100, making an O(N²) DP feasible.

## Template

```python
MOD = 10**9 + 7
n = len(board)

# dp[i][j] = max score from (i,j) to destination, -1 means unreachable
dp = [[-1] * n for _ in range(n)]
# cnt[i][j] = number of ways to achieve dp[i][j]
cnt = [[0] * n for _ in range(n)]

# initialise destination
dp[si][sj] = 0          # score contributed by the start cell itself
cnt[si][sj] = 1

# iterate in reverse topological order
for i in range(...):
    for j in range(...):
        if board[i][j] == 'X': continue
        best = -1
        ways = 0
        for each allowed move (ni, nj) from (i,j):
            if dp[ni][nj] == -1: continue   # unreachable neighbour
            if dp[ni][nj] > best:
                best = dp[ni][nj]
                ways = cnt[ni][nj]
            elif dp[ni][nj] == best:
                ways = (ways + cnt[ni][nj]) % MOD

        if best == -1:       # no reachable neighbour
            continue
        cell_val = 0 if board[i][j] in ('S','E') else int(board[i][j])
        dp[i][j] = best + cell_val
        cnt[i][j] = ways
```

Replace the start/end coordinates (`si`,`sj`) and the move set according to the problem.

## LeetCode Problem Walkthrough

### Problem: 1301. Number of Paths with Max Score

https://leetcode.com/problems/number-of-paths-with-max-score/

---

### Approach 1: Brute Force (DFS)

**Algorithm**  
Explore every possible path from the start `'S'` (bottom‑right) to the end `'E'` (top‑left) using recursion. At each step try the three allowed moves (up, left, up‑left) if the cell is inside the board and not an obstacle `'X'`. Keep track of the current sum; when reaching `'E'` update the global maximum and count how many paths achieve it.

**Implementation**

```python
class Solution:
    def pathsWithMaxScore(self, board: List[str]) -> List[int]:
        self.n = len(board)
        self.mod = 10**9 + 7
        self.best = -1          # maximal sum found
        self.ways = 0           # number of paths attaining best

        def dfs(r: int, c: int, cur_sum: int):
            # out of bounds or obstacle
            if not (0 <= r < self.n and 0 <= c < self.n) or board[r][c] == 'X':
                return
            ch = board[r][c]
            if ch not in ('S', 'E'):
                cur_sum += int(ch)

            # reached destination
            if r == 0 and c == 0:
                if cur_sum > self.best:
                    self.best = cur_sum
                    self.ways = 1
                elif cur_sum == self.best:
                    self.ways = (self.ways + 1) % self.mod
                return

            # move up, left, up‑left
            for dr, dc in [(-1,0), (0,-1), (-1,-1)]:
                dfs(r+dr, c+dc, cur_sum)

        # start from bottom‑right
        dfs(self.n-1, self.n-1, 0)

        if self.best == -1:          # no path found
            return [0, 0]
        return [self.best, self.ways]
```

**Complexity Analysis**

- Time complexity: O(3^(N²)) in the worst case – each cell can branch into three moves, exponential.
- Space complexity: O(N²) recursion stack depth (worst‑case path length) plus O(1) extra.

---

### Approach 2: Top‑Down DP with Memoization

**Intuition**  
The brute force repeats the same sub‑problems many times: the optimal score from a cell to the end does not depend on how we arrived there. By caching results (`dp`, `cnt`) we turn the exponential DFS into a polynomial solution.

**Algorithm**  
Define a recursive function `solve(r,c)` that returns a tuple `(maxScore, ways)` for the sub‑grid starting at `(r,c)` and ending at the top‑left. Memoise results in two tables. For each cell, look at the three reachable neighbours, pick the maximum score among them, sum the ways of those neighbours that achieve that maximum, then add the current cell’s numeric value (0 for `'S'`/`'E'`).

**Implementation**

```python
class Solution:
    def pathsWithMaxScore(self, board: List[str]) -> List[int]:
        self.n = len(board)
        self.mod = 10**9 + 7
        from functools import lru_cache

        @lru_cache(None)
        def solve(r: int, c: int):
            # outside or obstacle -> unreachable
            if not (0 <= r < self.n and 0 <= c < self.n) or board[r][c] == 'X':
                return (-1, 0)

            # reached destination
            if r == 0 and c == 0:
                return (0, 1)          # score 0, one way (stay)

            best = -1
            ways = 0
            for dr, dc in [(-1,0), (0,-1), (-1,-1)]:
                sc, wt = solve(r+dr, c+dc)
                if sc == -1:
                    continue
                if sc > best:
                    best = sc
                    ways = wt
                elif sc == best:
                    ways = (ways + wt) % self.mod

            if best == -1:            # no reachable neighbour
                return (-1, 0)

            add = 0 if board[r][c] in ('S','E') else int(board[r][c])
            return (best + add, ways)

        score, cnt = solve(self.n-1, self.n-1)
        if score == -1:
            return [0, 0]
        return [score, cnt % self.mod]
```

**Complexity Analysis**

- Time complexity: O(N²) – each cell is computed once, each does O(1) work (3 neighbours).
- Space complexity: O(N²) for memoization tables + recursion stack O(N²) in worst case (though depth ≤ 2N).

---

### Approach 3: Bottom‑Down DP (Iterative)

**Intuition**  
Instead of recursion we can fill the DP tables iteratively. Since moves are only up, left, or up‑left, the destination cell `(0,0)` depends on cells that are *below*, *right*, or *bottom‑right* of it. Therefore processing the board from bottom‑right to top‑left guarantees that when we compute a cell, all cells we can move to have already been solved.

**Algorithm**  
1. Initialise `dp` with `-1` (unreachable) and `cnt` with `0`.  
2. Set the start cell `(n‑1,n‑1)` to score `0` and count `1`.  
3. Loop `i` from `n‑1` down to `0`, inside loop `j` from `n‑1` down to `0`.  
4. Skip the start cell and any obstacle.  
5. Examine the three neighbours `(i+1,j)`, `(i,j+1)`, `(i+1,j+1)`.  
   - Keep the largest neighbour score (`best`).  
   - Sum the counts of neighbours that achieve `best`.  
6. If no neighbour is reachable, leave the cell as unreachable.  
7. Otherwise add the current cell’s value (`0` for `'S'`/`'E'`, else digit) to `best` and store the summed ways.  
8. Answer is `dp[0][0]` and `cnt[0][0]`; if unreachable return `[0,0]`.

**Implementation**

```python
class Solution:
    def pathsWithMaxScore(self, board: List[str]) -> List[int]:
        n = len(board)
        MOD = 10**9 + 7

        # dp[i][j] = max sum reachable from (i,j) to (0,0)
        dp = [[-1] * n for _ in range(n)]
        # cnt[i][j] = number of ways to achieve dp[i][j]
        cnt = [[0] * n for _ in range(n)]

        # start cell (bottom‑right)
        dp[n-1][n-1] = 0
        cnt[n-1][n-1] = 1

        for i in range(n-1, -1, -1):
            for j in range(n-1, -1, -1):
                if i == n-1 and j == n-1:
                    continue
                if board[i][j] == 'X':
                    continue

                best = -1
                ways = 0
                # three possible moves: down, right, down‑right (reverse of up,left,up‑left)
                for di, dj in [(1,0), (0,1), (1,1)]:
                    ni, nj = i + di, j + dj
                    if ni >= n or nj >= n:
                        continue
                    if dp[ni][nj] == -  ] == -1:   # neighbour unreachable
                        continue
                    if dp[ni][nj] > best:
                        best = dp[ni][nj]
                        ways = cnt[ni][nj]
                    elif dp[ni][nj] == best:
                        ways = (ways + cnt[ni][nj]) % MOD

                if best == -1:   # no way to reach the end from here
                    continue

                cell_val = 0 if board[i][j] in ('S','E') else int(board[i][j])
                dp[i][j] = best + cell_val
                cnt[i][j] = ways

        if dp[0][0] == -1:
            return [0, 0]
        return [dp[0][0], cnt[0][0] % MOD]
```

**Complexity Analysis**

- Time complexity: O(N²) – two nested loops, each cell does constant work (3 neighbours).  
- Space complexity: O(N²) for the two DP tables.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the bottom‑up DP on the example  

`board = ["E23","2X2","12S"]`  

Indices: `(row, col)` with `(0,0)` = top‑left `'E'`, `(2,2)` = bottom‑right `'S'`.

```
Initial dp ( -1 = unreachable ), cnt (0)

        col0  col1  col2
row0    E(0)  2     3
row1    2     X     2
row2    1     2     S(0)
```

We fill from bottom‑right upwards.

#### Step‑by‑step table

| (i,j) | cell | best neighbour score | ways from neighbours | dp[i][j] = best + val | cnt[i][j] |
|-------|------|----------------------|----------------------|----------------------|-----------|
| (2,2) | S    | — (start)            | —                    | 0                    | 1 |
| (2,1) | 2    | neighbours: (3,1) OOB, (2,2)=0, (3,2) OOB → best=0, ways=1 | 0+2=2 | 1 |
| (2,0) | 1    | neighbours: (3,0) OOB, (2,1)=2, (3,1) OOB → best=2, ways=1 | 2+1=3 | 1 |
| (1,2) | 2    | neighbours: (2,2)=0, (1,3) OOB, (2,3) OOB → best=0, ways=1 | 0+2=2 | 1 |
| (1,1) | X    | obstacle → skip      | –                    | –                    | – |
| (1,0) | 2    | neighbours: (2,0)=3, (1,1)=X, (2,1)=2 → best=3 (from (2,0)), ways=1 | 3+2=5 | 1 |
| (0,2) | 3    | neighbours: (1,2)=2, (0,3) OOB, (1,3) OOB → best=2, ways=1 | 2+3=5 | 1 |
| (0,1) | 2    | neighbours: (1,1)=X, (0,2)=5, (1,2)=2 → best=5 (from (0,2)), ways=1 | 5+2=7 | 1 |
| (0,0) | E    | neighbours: (1,0)=5, (0,1)=7, (1,1)=X → best=7 (from (0,1)), ways=1 | 7+0=7 | 1 |

Result at `(0,0)`: `dp = 7`, `cnt = 1` → `[7,1]`, matching the expected output.

---

**Summary**

* We transformed an exponential brute‑force search into an O(N²) DP by realizing the optimal sub‑structure and overlapping sub‑problems.  
* Two tables (`dp` for best score, `cnt` for number of optimal ways) capture all needed information.  
* Processing the board in reverse topological order guarantees that dependencies are already solved.  

This lecture equips you to tackle any grid‑based optimisation‑plus‑counting problem (e.g., unique paths with maximum sum, minimum‑cost path with number of ways, etc.). Happy coding!