# Graphs (Path Existence)

## Video Solution

For more details about **Find if Path Exists in Graph**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=knLFe7hEp3Y)

## Concept

To determine whether a path exists between two vertices in an undirected graph we can **traverse** the graph starting from the source vertex and see if we ever reach the destination.  
The traversal can be done with **Breadth‑First Search (BFS)** or **Depth‑First Search (DFS)**. Both visit each vertex at most once, using a `visited` array to avoid infinite loops caused by cycles.

A real‑world analogy: imagine you are in a maze of rooms (vertices) connected by doors (edges). You start in the source room and keep opening doors to new rooms you haven’t entered yet. If you ever step into the destination room, a path exists; if you run out of unopened doors without reaching it, there is no path.

## When to Use It

Use graph traversal (BFS/DFS) when you see:
- The problem asks whether a **path** or **connection** exists between two nodes.
- The graph is given as an edge list (or adjacency list) and is **undirected**.
- You need to check **reachability** or **connected components**.
- Constraints are large (up to 2·10⁵ vertices/edges) → an O(V+E) solution is required.

## Template

Here is a reusable Python template for BFS‑based reachability checking. Replace the `process` comment with any per‑node logic you need.

```python
from collections import deque
from typing import List

def has_path(n: int, edges: List[List[int]], source: int, destination: int) -> bool:
    # 1️⃣ Build adjacency list
    adj = [[] for _ in range(n)]
    for u, v in edges:
        adj[u].append(v)
        adj[v].append(u)

    # 2️⃣ BFS traversal
    visited = [False] * n
    q = deque([source])
    visited[source] = True

    while q:
        node = q.popleft()
        if node == destination:          # 🎯 Destination reached
            return True
        for nei in adj[node]:            # Explore neighbours
            if not visited[nei]:
                visited[nei] = True
                q.append(nei)

    # 3️⃣ Exhausted all reachable vertices without finding destination
    return False
```

## LeetCode Problem Walkthrough

### Problem: 1971. Find if Path Exists in Graph
https://leetcode.com/problems/find-if-path-exists-in-graph/

---

### Approach 1: Brute Force (Naïve DFS without visited)

**Algorithm**  
Start a recursive DFS from `source`. At each step, recursively explore every neighbour **without** marking vertices as visited. If we ever hit `destination`, return `True`. If recursion exhausts all paths, return `False`.  
Because we never mark visited nodes, the algorithm may revisit the same vertex many times, leading to exponential blow‑up and infinite recursion on cycles.

**Implementation**

```python
class Solution:
    def validPath(self, n: int, edges: List[List[int]], source: int, destination: int) -> bool:
        # Build adjacency list
        adj = [[] for _ in range(n)]
        for u, v in edges:
            adj[u].append(v)
            adj[v].append(u)

        def dfs(cur: int) -> bool:
            if cur == destination:
                return True
            # Explore all neighbours (no visited check!)
            for nxt in adj[cur]:
                if dfs(nxt):
                    return True
            return False

        return dfs(source)
```

**Complexity Analysis**  
- Time complexity: O(2^V) in the worst case (each vertex can be chosen or not in a path) → exponential.  
- Space complexity: O(V) for the recursion stack (plus adjacency list O(V+E)).

---

### Approach 2: BFS with Visited Set (Optimal Traversal)

**Intuition**  
Marking a vertex as visited the first time we see it guarantees we never process the same vertex twice. This turns the naïve exponential search into a linear‑time graph traversal because each edge is examined at most once.

**Algorithm**  
1. Build an adjacency list from the edge list.  
2. Initialize a queue with the `source` and a `visited` boolean array.  
3. While the queue is not empty: pop a vertex, if it is the `destination` return `True`; otherwise push all unvisited neighbours onto the queue and mark them visited.  
4. If the queue empties without finding the destination, return `False`.

**Implementation**

```python
from collections import deque
from typing import List

class Solution:
    def validPath(self, n: int, edges: List[List[int]], source: int, destination: int) -> bool:
        # 1️⃣ Adjacency list
        adj = [[] for _ in range(n)]
        for u, v in edges:
            adj[u].append(v)
            adj[v].append(u)

        # 2️⃣ BFS
        visited = [False] * n
        q = deque([source])
        visited[source] = True

        while q:
            node = q.popleft()
            if node == destination:
                return True
            for nei in adj[node]:
                if not visited[nei]:
                    visited[nei] = True
                    q.append(nei)

        return False
```

**Complexity Analysis**  
- Time complexity: O(V + E) — each vertex is enqueued at most once and each edge inspected twice (once from each endpoint).  
- Space complexity: O(V + E) for the adjacency list + O(V) for the `visited` array and queue.

---

### Approach 3: Union‑Find (Disjoint Set Union)

**Intuition**  
If two vertices belong to the same connected component, there exists a path between them. Union‑Find efficiently merges edges into components and can answer “are source and destination in the same set?” in near‑constant time.

**Algorithm**  
1. Initialize each vertex as its own parent.  
2. For every edge `[u, v]`, union the sets containing `u` and `v`.  
3. After processing all edges, check if `find(source) == find(destination)`.  
   - Path compression and union by rank/size keep operations almost O(1).

**Implementation**

```python
from typing import List

class DSU:
    def __init__(self, n: int):
        self.parent = list(range(n))
        self.rank = [0] * n

    def find(self, x: int) -> int:
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])   # path compression
        return self.parent[x]

    def union(self, x: int, y: int) -> None:
        xr, yr = self.find(x), self.find(y)
        if xr == yr:
            return
        # union by rank
        if self.rank[xr] < self.rank[yr]:
            self.parent[xr] = yr
        elif self.rank[xr] > self.rank[yr]:
            self.parent[yr] = xr
        else:
            self.parent[yr] = xr
            self.rank[xr] += 1

class Solution:
    def validPath(self, n: int, edges: List[List[int]], source: int, destination: int) -> bool:
        dsu = DSU(n)
        for u, v in edges:
            dsu.union(u, v)
        return dsu.find(source) == dsu.find(destination)
```

**Complexity Analysis**  
- Time complexity: O(V + E * α(V)) ≈ O(V + E), where α is the inverse Ackermann function (practically ≤ 5).  
- Space complexity: O(V) for the parent and rank arrays.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the BFS approach on Example 1.

**Input**: `n = 3`, `edges = [[0,1],[1,2],[2,0]]`, `source = 0`, `destination = 2`

| Step | Queue (front → rear) | Visited[0] | Visited[1] | Visited[2] | Action |
|------|----------------------|------------|------------|------------|--------|
| Init | [0]                  | T          | F          | F          | enqueue source |
| 1    | [] → pop 0, check ≠2 → push 1,2 | T | T (push 1) | T (push 2) | visited 1 & 2 |
| 2    | [1,2] → pop 1, check ≠2 → neighbours 0,2 (0 visited, 2 visited) → nothing new | T | T | T | |
| 3    | [2] → pop 2, check ==2 → **return True** | T | T | T | destination found |

The algorithm terminates after visiting each vertex once, confirming a path exists.

--- 

This lecture follows the required format: concept, when to use, template, three approaches (brute force → BFS → Union‑Find) each with intuition, algorithm, commented Python code, and complexity analysis, plus a concrete dry‑run demonstration. Save the file as `/lectures/YYYY-MM-DD-graphs.md` (using today’s date) in the main branch.