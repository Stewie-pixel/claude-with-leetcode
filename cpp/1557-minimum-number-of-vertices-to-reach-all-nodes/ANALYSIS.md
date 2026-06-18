# Graphs - Indegree Zero Nodes

## Video Solution

For more details about **Minimum Number of Vertices to Reach All Nodes**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=IfsiNU7J-6c)

## Concept

In a directed acyclic graph (DAG), a node that has **no incoming edges** (indegree = 0) cannot be reached from any other node. Therefore, every such node must be part of the starting set; otherwise it would stay unreachable. Conversely, if we start from all indegree‑zero nodes, we can reach every other node because every node either is a source itself or has a path that originates at some source (follow edges backward until you hit a node with indegree 0). Hence the smallest set of vertices that can reach all nodes is exactly the set of nodes with indegree 0.

## When to Use It

Use this pattern when you see:
- A **directed** graph (often specified as a DAG)
- A request for the **minimum number of starting vertices** to reach all vertices
- The guarantee that a **unique** solution exists
- The ability to compute **incoming edge counts** (indegree) easily

In short: *“minimum sources in a directed graph → count indegree‑zero nodes.”*

## Template

```python
def find_smallest_set_of_vertices(n, edges):
    """
    Returns the list of vertices with indegree == 0.
    n: number of vertices (0 … n-1)
    edges: list of [from, to] directed edges
    """
    indegree = [0] * n               # step 1: count incoming edges
    for _, to in edges:              # step 2: accumulate indegrees
        indegree[to] += 1

    # step 3: collect all vertices that never received an edge
    return [v for v in range(n) if indegree[v] == 0]
```

## LeetCode Problem Walkthrough

### Problem: 1557. Minimum Number of Vertices to Reach All Nodes
https://leetcode.com/problems/minimum-number-of-vertices-to-reach-all-nodes/

---

### Approach 1: Brute Force (Check All Subsets)

**Algorithm**
1. Enumerate every possible subset of vertices (there are 2ⁿ subsets).  
2. For each subset, run a DFS/BFS from all vertices in the subset and mark visited nodes.  
3. If the visited set equals all vertices, record the subset size.  
4. Return the smallest subset found.

**Implementation**
```python
from collections import deque

def brute_force(n, edges):
    # build adjacency list
    adj = [[] for _ in range(n)]
    for u, v in edges:
        adj[u].append(v)

    def reaches_all(start_mask):
        """Return True if the vertices indicated by start_mask can reach all nodes."""
        visited = [False] * n
        q = deque()
        for i in range(n):
            if start_mask & (1 << i):
                q.append(i)
                visited[i] = True
        while q:
            u = q.popleft()
            for w in adj[u]:
                if not visited[w]:
                    visited[w] = True
                    q.append(w)
        return all(visited)

    best = None
    best_size = n + 1
    for mask in range(1 << n):          # iterate over all subsets
        if reaches_all(mask):
            size = bin(mask).count('1')
            if size < best_size:
                best_size = size
                best = mask

    # convert mask to list of vertices
    return [i for i in range(n) if best & (1 << i)]
```

**Complexity Analysis**
- Time complexity: O(2ⁿ · (n + E)) – we examine every subset and for each run a BFS/DFS.
- Space complexity: O(n + E) – adjacency list plus visited/queue structures.  
*This approach is exponential and only works for tiny n.*

---

### Approach 2: Topological‑Sort Insight (Sources Must Be Chosen)

**Intuition**
In any directed graph, a node with indegree = 0 has no incoming edges, so no other node can reach it. Therefore it **must** be included in any feasible starting set. If we take *all* such nodes, we guarantee reachability to the rest because every other node has at least one incoming edge, and by repeatedly following incoming edges backward we eventually arrive at a node with indegree = 0 (the graph is acyclic, so this process terminates). Hence the set of all indegree‑zero nodes is both necessary and sufficient.

**Algorithm**
1. Compute the indegree of each vertex.  
2. Return all vertices whose indegree is zero.

**Implementation**
```python
def find_smallest_set_of_vertices(n, edges):
    indegree = [0] * n
    for _, to in edges:
        indegree[to] += 1
    return [v for v in range(n) if indegree[v] == 0]
```

**Complexity Analysis**
- Time complexity: O(E) – one pass over the edge list to count indegrees.  
- Space complexity: O(n) – the indegree array.

---

### Approach 3: Optimized Indegree Count (Same as Approach 2, Highlighting Implementation Details)

**Intuition**
The optimal solution does not require any graph traversal beyond counting incoming edges. By focusing solely on indegree, we avoid building an explicit adjacency list or performing DFS/BFS, which reduces constant factors and simplifies the code.

**Algorithm** (identical to Approach 2, presented with explicit variable names for clarity)
1. Initialize an array `incoming` of length `n` with zeros.  
2. For each edge `[u, v]`, increment `incoming[v]`.  
3. Collect all indices `i` where `incoming[i] == 0`.

**Implementation**
```python
class Solution:
    def findSmallestSetOfVertices(self, n: int, edges: List[List[int]]) -> List[int]:
        incoming = [0] * n                     # step 1: indegree counter
        for _, v in edges:                     # step 2: tally incoming edges
            incoming[v] += 1
        # step 3: select vertices with zero incoming edges
        return [i for i, cnt in enumerate(incoming) if cnt == 0]
```

**Complexity Analysis**
- Time complexity: O(E) – single traversal of `edges`.  
- Space complexity: O(n) – the `incoming` array.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the optimal algorithm on Example 1.

**Input**  
`n = 6`  
`edges = [[0,1],[0,2],[2,5],[3,4],[4,2]]`

#### Step‑by‑step indegree calculation

| Edge processed | `to` node | incoming[to] before | incoming[to] after |
|----------------|----------|---------------------|--------------------|
| [0,1]          | 1        | 0                   | 1                  |
| [0,2]          | 2        | 0                   | 1                  |
| [2,5]          | 5        | 0                   | 1                  |
| [3,4]          | 4        | 0                   | 1                  |
| [4,2]          | 2        | 1                   | 2                  |

Final `incoming` array: `[0, 1, 2, 0, 1, 1]`

Vertices with indegree = 0 → indices **0** and **3**.

**Output:** `[0, 3]` (matches the expected answer).

The dry run shows that after counting incoming edges, the two sources are exactly the vertices we need to start from. No further traversal is required.