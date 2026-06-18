# Graphs – Count the Number of Complete Components  

## Video Solution  

For more details about **Count the Number of Complete Components**, watch the walkthrough at  
[https://www.youtube.com/watch?v=FjLirf3k9ao](https://www.youtube.com/watch?v=FjLirf3k9ao)  

## Concept  

A **complete component** (also called a *clique*) is a connected subgraph where every pair of vertices is directly connected by an edge.  
If a component has `k` vertices, it must contain exactly `k·(k‑1)/2` edges.  
Thus, to count complete components we can:  

1. Find each connected component (via BFS/DFS or Union‑Find).  
2. Count the number of vertices `k` and edges `e` inside that component.  
3. The component is complete iff `e == k·(k‑1)/2`.  

The real‑world analogy: think of a group of people where everyone knows everyone else. If you can list all members of a group and count the distinct friendships, the group is a “complete component” only when the friendship count matches the number of possible pairs.

## When to Use It  

Use this technique when a problem asks you to:  

- Count or identify subgraphs that are internally fully connected.  
- Verify whether each connected component of an undirected graph is a clique.  
- Work with small‑to‑moderate sized graphs where you can afford to traverse components and count edges.  

Typical clues in the statement: “complete connected component”, “every pair of vertices has an edge”, “clique”, or “fully connected subgraph”.

## Template  

Below is a reusable Python template for exploring components and checking completeness. Replace the `process_component` function with the specific logic you need (here we will fill it in).

```python
from collections import deque
from typing import List, Set, Tuple

def count_complete_components(n: int, edges: List[List[int]]) -> int:
    # Build adjacency list
    adj = [[] for _ in range(n)]
    for u, v in edges:
        adj[u].append(v)
        adj[v].append(u)

    visited = [False] * n
    complete = 0

    for start in range(n):
        if not visited[start]:
            # -------- explore one component ----------
            q = deque([start])
            visited[start] = True
            nodes: List[int] = []          # vertices in this component
            edge_sum = 0                   # sum of degrees (will be halved)

            while q:
                u = q.popleft()
                nodes.append(u)
                edge_sum += len(adj[u])    # each incident edge counted once per endpoint
                for v in adj[u]:
                    if not visited[v]:
                        visited[v] = True
                        q.append(v)
            # -----------------------------------------

            # Process the gathered component
            if process_component(nodes, edge_sum):
                complete += 1

    return complete

def process_component(nodes: List[int], edge_sum: int) -> bool:
    """
    Return True if the component defined by `nodes` and `edge_sum`
    (sum of degrees) is a complete graph.
    """
    k = len(nodes)
    edges = edge_sum // 2          # each edge counted twice in edge_sum
    return edges == k * (k - 1) // 2
```

You can swap `process_component` for a Union‑Find based version if you prefer.

---

## LeetCode Problem Walkthrough  

### Problem: 2685. Count the Number of Complete Components  

https://leetcode.com/problems/count-the-number-of-complete-components/  

---

### Approach 1: Brute Force – Check All Pairs Inside Each Component  

**Algorithm**  
1. Build an adjacency matrix `mat[n][n]` (or a set of edges) for O(1) edge lookup.  
2. Run BFS/DFS to collect the vertices of each component.  
3. For the collected list `nodes`, iterate over every unordered pair `(i, j)` and verify that `mat[i][j]` is present.  
4. If all pairs exist, increment the answer.  

**Implementation**  

```python
class Solution:
    def countCompleteComponents(self, n: int, edges: List[List[int]]) -> int:
        # adjacency matrix for O(1) edge existence check
        mat = [[False] * n for _ in range(n)]
        for u, v in edges:
            mat[u][v] = mat[v][u] = True

        visited = [False] * n
        ans = 0

        for i in range(n):
            if not visited[i]:
                # BFS to get component vertices
                q = deque([i])
                visited[i] = True
                comp = []
                while q:
                    u = q.popleft()
                    comp.append(u)
                    for v in range(n):
                        if mat[u][v] and not visited[v]:
                            visited[v] = True
                            q.append(v)

                # brute‑force check: every pair must be connected
                ok = True
                for a in range(len(comp)):
                    for b in range(a + 1, len(comp)):
                        if not mat[comp[a]][comp[b]]:
                            ok = False
                            break
                    if not ok:
                        break
                if ok:
                    ans += 1
        return ans
```

**Complexity Analysis**  

- Time complexity: O(n² + n·α) → building the matrix O(n²) + for each component we may check up to k² pairs; in the worst case (one big component) this is O(n²).  
- Space complexity: O(n²) for the adjacency matrix + O(n) for visited/queue.  

---

### Approach 2: BFS/DFS + Edge Count (Optimized)  

**Intuition**  
Instead of checking every pair, we can use the mathematical property of a complete graph: a component with `k` vertices is complete **iff** it contains exactly `k·(k‑1)/2` edges. While traversing a component we can sum the degrees of its vertices; the total degree sum equals `2·E`. Halving it gives the actual edge count `E`. Comparing `E` with `k·(k‑1)/2` tells us completeness in linear time per component.  

**Algorithm**  
1. Build adjacency list.  
2. For each unvisited vertex, run BFS/DFS:  
   - Collect all vertices (`k`).  
   - Accumulate `deg_sum = Σ degree(v)`.  
3. After traversal, compute `edges = deg_sum // 2`.  
4. If `edges == k·(k‑1)/2`, increment answer.  

**Implementation**  

```python
class Solution:
    def countCompleteComponents(self, n: int, edges: List[List[int]]) -> int:
        adj = [[] for _ in range(n)]
        for u, v in edges:
            adj[u].append(v)
            adj[v].append(u)

        visited = [False] * n
        complete = 0

        for start in range(n):
            if not visited[start]:
                q = deque([start])
                visited[start] = True
                nodes = []
                deg_sum = 0

                while q:
                    u = q.popleft()
                    nodes.append(u)
                    deg_sum += len(adj[u])      # each incident edge counted once per endpoint
                    for v in adj[u]:
                        if not visited[v]:
                            visited[v] = True
                            q.append(v)

                k = len(nodes)
                e = deg_sum // 2
                if e == k * (k - 1) // 2:
                    complete += 1
        return complete
```

**Complexity Analysis**  

- Time complexity: O(n + m) – each vertex and edge is processed once during BFS/DFS.  
- Space complexity: O(n + m) for adjacency list + O(n) for visited/queue.  

---

### Approach 3: Union‑Find with Edge Count per Component  

**Intuition**  
Union‑Find (Disjoint Set Union) can merge vertices while tracking the size of each set. If we also maintain the number of edges contributed to each set, we can decide completeness after all unions: a set of size `k` is complete iff its edge count equals `k·(k‑1)/2`. This avoids an explicit BFS/DFS pass and works well when the graph is given as a list of edges.  

**Algorithm**  
1. Initialize DSU with `parent[i] = i`, `size[i] = 1`, `edge_cnt[i] = 0`.  
2. For each edge `(u, v)`:  
   - Find roots `ru`, `rv`.  
   - If `ru != rv`: union the two sets, and set the new edge count to `edge_cnt[ru] + edge_cnt[rv] + 1` (the current edge).  
   - If `ru == rv`: the edge lies inside an existing component; simply increment `edge_cnt[ru]` by 1.  
3. After processing all edges, iterate over all roots. For each root `r` with size `k = size[r]` and edge count `e = edge_cnt[r]`, check if `e == k·(k‑1)/2`. Count those that satisfy the condition.  

**Implementation**  

```python
class DSU:
    def __init__(self, n: int):
        self.parent = list(range(n))
        self.size = [1] * n
        self.edges = [0] * n          # number of edges inside the component

    def find(self, x: int) -> int:
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]
            x = self.parent[x]
        return x

    def union(self, x: int, y: int) -> None:
        xr, yr = self.find(x), self.find(y)
        if xr == yr:
            self.edges[xr] += 1       # edge inside same component
            return
        # union by size
        if self.size[xr] < self.size[yr]:
            xr, yr = yr, xr
        self.parent[yr] = xr
        self.size[xr] += self.size[yr]
        self.edges[xr] += self.edges[yr] + 1   # add edges from both + current edge

class Solution:
    def countCompleteComponents(self, n: int, edges: List[List[int]]) -> int:
        dsu = DSU(n)
        for u, v in edges:
            dsu.union(u, v)

        complete = 0
        for i in range(n):
            if dsu.parent[i] == i:          # i is a root
                k = dsu.size[i]
                e = dsu.edges[i]
                if e == k * (k - 1) // 2:
                    complete += 1
        return complete
```

**Complexity Analysis**  

- Time complexity: O(n + m·α(n)) – each `find/union` is almost constant (inverse Ackermann).  
- Space complexity: O(n) for parent, size, and edge arrays.  

---

## Visual Demonstration (Dry Run)  

We will trace **Approach 2 (BFS + edge count)** on the first example:

**Input**: `n = 6`, `edges = [[0,1],[0,2],[1,2],[3,4]]`  

Adjacency list:  
```
0: [1,2]
1: [0,2]
2: [0,1]
3: [4]
4: [3]
5: []
```

### Step‑by‑step BFS

| Step | Queue start | Visited set | Nodes collected | Degree sum so far |
|------|-------------|-------------|-----------------|-------------------|
| 1    | [0]         | {0}         | [0]             | 2 (deg of 0)      |
| 2    | [1,2]       | {0,1}       | [0,1]           | 2 + 2 = 4         |
| 3    | [2]         | {0,1,2}     | [0,1,2]         | 4 + 2 = 6         |
| 4    | []          | {0,1,2}     | [0,1,2]         | 6 (done)          |

Component `{0,1,2}`:  
- `k = 3`  
- `edge_sum = 6` → `E = 6/2 = 3`  
- Required edges for a clique: `3·2/2 = 3` → **complete** → count = 1  

Next start at vertex 3:

| Step | Queue start | Visited set | Nodes collected | Degree sum |
|------|-------------|-------------|-----------------|------------|
| 1    | [3]         | {0,1,2,3}   | [3]             | 1 (deg of 3) |
| 2    | [4]         | {0,1,2,3,4}| [3,4]           | 1 + 1 = 2   |
| 3    | []          | {0,1,2,3,4}| [3,4]           | 2 (done)    |

Component `{3,4}`:  
- `k = 2`  
- `edge_sum = 2` → `E = 1`  
- Required edges: `2·1/2 = 1` → **complete** → count = 2  

Vertex 5 is isolated:

- `k = 1`, `edge_sum = 0` → `E = 0`  
- Required edges: `1·0/2 = 0` → **complete** → count = 3  

Final answer = 3, matching the example.

---  

**Summary**  
- The brute‑force method works but is wasteful (O(n²) time, O(n²) space).  
- Counting vertices and edges while traversing each component gives an optimal O(n+m) solution.  
- Union‑Find offers an alternative with similar asymptotic performance and can be preferable when edges are given as a list.  

Use the BFS/DFS + edge count approach as the default template for “count complete components” problems; it is simple, efficient, and easy to extend.