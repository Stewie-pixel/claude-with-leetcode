# Graphs – BFS with State (Shortest Path with Alternating Colors)

## Video Solution

For more details about **Shortest Path with Alternating Colors**, watch the walkthrough at [https://www.youtube.com/watch?v=69rcy6lb-HQ](https://www.youtube.com/watch?v=69rcy6lb-HQ)

## Concept

We need the shortest path from node 0 to every other node where consecutive edges must alternate colors (red ↔ blue).  
A plain BFS that only tracks the current node cannot enforce the alternation rule because it forgets what color was used to arrive at the node.  
Instead, we augment the BFS state with the **color of the last edge used** (or a sentinel for “no color yet”).  
Each state is `(node, lastColor, distance)`. From a state we can only traverse edges of the opposite color, guaranteeing alternation.  
The first time we reach a node (with any lastColor) we have found the shortest alternating‑color path to it because BFS explores states in increasing distance order.

## When to Use It

Use this technique when you see:
- A graph problem where edges have **types/colors/labels** and a constraint depends on the **previous edge type**.
- The requirement is to find a **shortest path** under such a constraint.
- The constraint only depends on the **immediate previous step** (Markov‑like), allowing us to store that information in the BFS/DFS state.

## Template

```python
from collections import deque
from typing import List

def bfs_alternating(n: int, red_adj: List[List[int]], blue_adj: List[List[int]]) -> List[int]:
    # visited[color][node] -> have we visited `node` arriving via an edge of `color`?
    # color: 0 = red, 1 = blue, we also allow a start state with lastColor = -1 (no restriction)
    visited = [[False] * n for _ in range(2)]
    dist = [-1] * n
    q = deque()

    # (node, lastColor, distance) ; lastColor = -1 means we have not used any edge yet
    q.append((0, -1, 0))
    dist[0] = 0                     # distance to start is 0

    while q:
        node, lastColor, d = q.popleft()

        # Try blue edges if the last edge was NOT blue (i.e., we can alternate)
        if lastColor != 1:          # we may take a blue edge now
            for nxt in blue_adj[node]:
                if not visited[1][nxt]:
                    visited[1][nxt] = True
                    if dist[nxt] == -1:          # first time we reach nxt -> shortest
                        dist[nxt] = d + 1
                    q.append((nxt, 1, d + 1))

        # Try red edges if the last edge was NOT red
        if lastColor != 0:          # we may take a red edge now
            for nxt in red_adj[node]:
                if not visited[0][nxt]:
                    visited[0][nxt] = True
                    if dist[nxt] == -1:
                        dist[nxt] = d + 1
                    q.append((nxt, 0, d + 1))

    return dist
```

---

## LeetCode Problem Walkthrough

### Problem: 1129. Shortest Path with Alternating Colors
https://leetcode.com/problems/shortest-path-with-alternating-colors/

### Approach 1: Brute Force – DFS with Path Enumeration

**Algorithm**
1. From node 0, perform a depth‑first search that explores every possible path, keeping track of the last edge color to enforce alternation.
2. Whenever we reach a node, record the path length if it is smaller than the current best for that node.
3. Because the graph may contain cycles, we limit recursion depth to a safe bound (e.g., `2 * n`) to avoid infinite loops.
4. After exploring all paths, the recorded lengths are the answers (‑1 if never reached).

**Implementation**

```python
class Solution:
    def shortestAlternatingPaths(self, n: int, redEdges: List[List[int]], blueEdges: List[List[int]]) -> List[int]:
        red_adj = [[] for _ in range(n)]
        blue_adj = [[] for _ in range(n)]
        for u, v in redEdges:
            red_adj[u].append(v)
        for u, v in blueEdges:
            blue_adj[u].append(v)

        INF = 10 ** 9
        best = [INF] * n
        best[0] = 0

        def dfs(node: int, last_color: int, length: int) -> None:
            # prune if we already exceed known best for this node
            if length >= best[node]:
                return
            best[node] = length
            # stop exploring overly long paths (graph size ≤ 100, edges ≤ 400)
            if length > 2 * n:          # safe cutoff
                return
            # choose opposite color edges
            if last_color != 1:         # can take blue
                for nxt in blue_adj[node]:
                    dfs(nxt, 1, length + 1)
            if last_color != 0:         # can take red
                for nxt in red_adj[node]:
                    dfs(nxt, 0, length + 1)

        dfs(0, -1, 0)                   # start with no previous color
        return [x if x != INF else -1 for x in best]
```

**Complexity Analysis**
- Time complexity: O( E · L ) where L is the depth cutoff (≈ 2n). In worst case we may explore many paths → exponential, but with the cutoff it becomes O(E·n).  
- Space complexity: O( n ) for recursion stack + adjacency lists.

> This approach is correct but inefficient; it repeatedly revisits the same state (node, last_color) via different paths, leading to repeated work.

### Approach 2: BFS with Color‑Specific Visited (Intermediate Optimization)

**Intuition**
Instead of enumerating every path, we notice that the only information needed to decide future moves is:
- the current node,
- the color of the edge we used to arrive (or none for the start).

If we have already visited a node **with a given last color**, any later arrival with the same last color cannot yield a shorter path because BFS processes states in non‑decreasing distance order. Hence we can skip re‑processing that state.

**Algorithm**
1. Build adjacency lists for red and blue edges.
2. Maintain a `visited[2][n]` boolean table (`0` = arrived via red, `1` = arrived via blue).
3. Initialize a queue with the start state `(node=0, lastColor=-1, dist=0)`. Treat `-1` as allowing both colors.
4. Pop a state, try to traverse edges of the opposite color:
   - If `lastColor != 1`, we may follow blue edges.
   - If `lastColor != 0`, we may follow red edges.
5. For each neighbor not yet visited with that edge’s color, mark visited, set its distance (if first time), and enqueue `(neighbor, thisColor, dist+1)`.
6. After BFS finishes, `dist[i]` holds the shortest alternating‑color distance to node i (or -1 if unreachable).

**Implementation**

```python
from collections import deque
from typing import List

class Solution:
    def shortestAlternatingPaths(self, n: int,
                                 redEdges: List[List[int]],
                                 blueEdges: List[List[int]]) -> List[int]:
        red_adj = [[] for _ in range(n)]
        blue_adj = [[] for _ in range(n)]
        for u, v in redEdges:
            red_adj[u].append(v)
        for u, v in blueEdges:
            blue_adj[u].append(v)

        # visited[color][node]; color 0 = red, 1 = blue
        visited = [[False] * n for _ in range(2)]
        dist = [-1] * n
        q = deque()

        # (node, lastColor, distance); lastColor = -1 means no previous edge
        q.append((0, -1, 0))
        dist[0] = 0

        while q:
            node, last_color, d = q.popleft()

            # try blue edges if last edge wasn't blue
            if last_color != 1:
                for nxt in blue_adj[node]:
                    if not visited[1][nxt]:
                        visited[1][nxt] = True
                        if dist[nxt] == -1:
                            dist[nxt] = d + 1
                        q.append((nxt, 1, d + 1))

            # try red edges if last edge wasn't red
            if last_color != 0:
                for nxt in red_adj[node]:
                    if not visited[0][nxt]:
                        visited[0][nxt] = True
                        if dist[nxt] == -1:
                            dist[nxt] = d + 1
                        q.append((nxt, 0, d + 1))

        return dist
```

**Complexity Analysis**
- Time complexity: O( V + E ). Each state `(node, color)` is processed at most once; there are at most `2·n` states, and each edge is examined at most twice (once from each color perspective).
- Space complexity: O( V + E ) for adjacency lists + O( V ) for visited and distance arrays.

### Approach 3: BFS with Combined Distance Array (Most Optimal)

**Intuition**
The intermediate solution already visits each `(node, color)` state once, which is optimal for this problem because the state space is exactly that small.  
We can slightly tidy the code by storing distances in a 2‑D array `dist[color][node]` and eliminating the separate `visited` table (the distance array itself serves as visited when initialized to INF).  
The logic stays identical, but the implementation is a bit cleaner and avoids an extra boolean array.

**Algorithm**
1. Build red and blue adjacency lists.
2. Initialize `dist[2][n]` with INF; set `dist[0][0] = dist[1][0] = 0` (we can start with either color because no edge has been taken yet).
3. Queue holds `(node, lastColor)`. Start with both `(0,0)` and `(0,1)` (or a single `(0,-1)` variant – we’ll use the two‑seed approach for symmetry).
4. While queue not empty:
   - Pop `(node, lastColor)`.
   - Determine the next color we are allowed to use: `nextColor = 1 - lastColor`.
   - For each neighbor in the adjacency list of `nextColor`:
        - If `dist[nextColor][neighbor] > dist[lastColor][node] + 1`:
            - Update distance and push `(neighbor, nextColor)`.
5. After BFS, answer for each node is `min(dist[0][i], dist[1][i])`; if both are INF, answer is -1.

**Implementation**

```python
from collections import deque
from typing import List

class Solution:
    def shortestAlternatingPaths(self, n: int,
                                 redEdges: List[List[int]],
                                 blueEdges: List[List[int]]) -> List[int]:
        red_adj = [[] for _ in range(n)]
        blue_adj = [[] for _ in range(n)]
        for u, v in redEdges:
            red_adj[u].append(v)
        for u, v in blueEdges:
            blue_adj[u].append(v)

        INF = 10 ** 9
        # dist[color][node] = shortest distance to `node` ending with an edge of `color`
        dist = [[INF] * n for _ in range(2)]
        q = deque()

        # we can start as if the previous edge was red or blue (no restriction)
        dist[0][0] = dist[1][0] = 0
        q.append((0, 0))   # pretend last edge was red
        q.append((0, 1))   # pretend last edge was blue

        while q:
            node, last_color = q.popleft()
            next_color = 1 - last_color          # must alternate
            adj = blue_adj[node] if next_color == 1 else red_adj[node]
            for nxt in adj:
                if dist[next_color][nxt] > dist[last_color][node] + 1:
                    dist[next_color][nxt] = dist[last_color][node] + 1
                    q.append((nxt, next_color))

        answer = []
        for i in range(n):
            best = min(dist[0][i], dist[1][i])
            answer.append(best if best != INF else -1)
        return answer
```

**Complexity Analysis**
- Time complexity: O( V + E ). Each directed edge is relaxed at most twice (once from each color state), and each state is enqueued at most once.
- Space complexity: O( V + E ) for adjacency lists + O( V ) for the distance table.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the BFS on the sample:
```
n = 3
redEdges = [[0,1],[1,2]]
blueEdges = []
```
Adjacency:
- red_adj: 0→[1], 1→[2], 2→[]
- blue_adj: all empty

We use the combined-distance version (Approach 3) and show the queue evolution.

**Initial state**
```
dist[0] = [0, INF, INF]
dist[1] = [0, INF, INF]
queue = [(0,0), (0,1)]
```

**Step 1 – pop (0,0)**  
last_color = 0 (red) → next_color = 1 (blue)  
blue_adj[0] = [] → nothing to push.

**Step 2 – pop (0,1)**  
last_color = 1 (blue) → next_color = 0 (red)  
red_adj[0] = [1]  
For neighbor 1:
- dist[0][1] > dist[1][0] + 1 → INF > 0+1 → true
- Set dist[0][1] = 1
- Push (1,0)

Queue now: [(1,0)]

**Step 3 – pop (1,0)**  
last_color = 0 (red) → next_color = 1 (blue)  
blue_adj[1] = [] → nothing.

Queue becomes empty.

**Final distances**
```
dist[0] = [0, 1, INF]
dist[1] = [0, INF, INF]
answer[i] = min(dist[0][i], dist[1][i]) → [0, 1, -1]
```
Matches the expected output `[0,1,-1]`.

---

### Summary

- **Brute force** (DFS with path enumeration) works but repeats states and is inefficient.
- **BFS with color‑specific visited** (Approach 2) guarantees each `(node, color)` state is processed once, giving linear time.
- **BFS with combined distance array** (Approach 3) is the most polished version, still O(V+W) time and O(V+E) space, and is the reference solution.

Use the BFS‑state pattern whenever a shortest‑path problem carries a **local memory constraint** (like edge color, token type, or step parity) that only depends on the immediate previous step. Store that information in the BFS/DFS state to keep the algorithm optimal.