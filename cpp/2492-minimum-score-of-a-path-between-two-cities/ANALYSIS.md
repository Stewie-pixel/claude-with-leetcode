# Graphs

## Video Solution

For more details about **Minimum Score of a Path Between Two Cities**, watch the walkthrough at [https://www.youtube.com/watch?v=K7-mXA0irhY](https://www.youtube.com/watch?v=K7-mXA0irhY).

## Concept

The score of a path is the *minimum* edge weight along that path.  
Because we are allowed to travel roads any number of times and revisit cities, we can always include **any** edge that lies in the same connected component as city 1 (and city n) in our walk. Therefore, the best possible score is simply the smallest edge weight that belongs to the component containing both city 1 and city n.

So the problem reduces to:

1. Find the connected component that contains node 1 (which, by the guarantee, also contains node n).
2. Return the minimum edge weight among all edges inside that component.

We can discover the component with a simple BFS/DFS and keep track of the smallest weight seen.

## When to Use It

Use this pattern when you see:

- A graph (undirected) with weighted edges.
- The task asks for a *minimum* (or maximum) value along a path **between two specific nodes**.
- You are allowed to reuse edges/nodes arbitrarily.
- The answer ends up being an extreme value (min/max) inside the *connected component* of the source/destination.

In short: if the graph’s connectivity matters more than the exact sequence of nodes, look for a component‑based solution.

## Template

```python
from collections import deque
from typing import List

def min_score_in_component(n: int, roads: List[List[int]]) -> int:
    # Build adjacency list: node -> list of (neighbor, weight)
    adj = [[] for _ in range(n + 1)]
    for u, v, w in roads:
        adj[u].append((v, w))
        adj[v].append((u, w))

    visited = [False] * (n + 1)
    q = deque([1])
    visited[1] = True

    min_edge = float('inf')

    while q:
        cur = q.popleft()
        for nxt, w in adj[cur]:
            # Every edge we traverse belongs to the component of node 1
            min_edge = min(min_edge, w)
            if not visited[nxt]:
                visited[nxt] = True
                q.append(nxt)

    return min_edge
```

---

## LeetCode Problem Walkthrough

### Problem: 2492. Minimum Score of a Path Between Two Cities  
https://leetcode.com/problems/minimum-score-of-a-path-between-two-cities/

### Approach 1: Brute Force – Check Every Edge’s Feasibility

**Algorithm**  
For each edge `(u, v, w)` we test whether there exists *any* path from city 1 to city n that can use this edge.  
Because we may revisit nodes, an edge can be used iff both its endpoints lie in the same component as city 1 (which also contains city n).  
So for each edge we run a BFS/DFS from city 1 to see if we can reach `u` **and** `v`.  
If both are reachable, the edge belongs to the component and we consider its weight for the answer.  
The minimum over all such feasible edges is the result.

**Implementation**

```python
from collections import deque
from typing import List

class Solution:
    def minScore(self, n: int, roads: List[List[int]]) -> int:
        # adjacency for reachability checks
        adj = [[] for _ in range(n + 1)]
        for u, v, w in roads:
            adj[u].append(v)
            adj[v].append(u)

        def reachable(src: int, target: int) -> bool:
            """Simple BFS to see if target can be reached from src."""
            if src == target:
                return True
            q = deque([src])
            seen = [False] * (n + 1)
            seen[src] = True
            while q:
                cur = q.popleft()
                for nxt in adj[cur]:
                    if not seen[nxt]:
                        if nxt == target:
                            return True
                        seen[nxt] = True
                        q.append(nxt)
            return False

        answer = float('inf')
        for u, v, w in roads:
            if reachable(1, u) and reachable(1, v):   # both ends in 1's component
                answer = min(answer, w)
        return answer
```

**Complexity Analysis**  
- Time complexity: O(E·(V+E)) – for each of the E edges we run a BFS that may touch all V vertices and E edges.  
- Space complexity: O(V+E) – adjacency list plus BFS queue/visited array.

> This approach is correct but far too slow for the constraints (E up to 10⁵).

---

### Approach 2: BFS/DFS to Find Component & Track Minimum Edge

**Intuition**  
Since we can travel roads arbitrarily, any edge inside the component of city 1 can be incorporated into a walk from 1 to n. Thus we only need to find that component once and record the smallest weight we encounter.

**Algorithm**  
1. Build an adjacency list storing `(neighbor, weight)`.  
2. Run a BFS starting from node 1, marking visited nodes.  
3. Whenever we traverse an edge, update `min_edge = min(min_edge, weight)`.  
4. After the BFS finishes, `min_edge` holds the answer.

**Implementation**

```python
from collections import deque
from typing import List

class Solution:
    def minScore(self, n: int, roads: List[List[int]]) -> int:
        adj = [[] for _ in range(n + 1)]
        for u, v, w in roads:
            adj[u].append((v, w))
            adj[v].append((u, w))

        visited = [False] * (n + 1)
        q = deque([1])
        visited[1] = True

        min_edge = float('inf')
        while q:
            cur = q.popleft()
            for nxt, w in adj[cur]:
                min_edge = min(min_edge, w)          # every seen edge belongs to the component
                if not visited[nxt]:
                    visited[nxt] = True
                    q.append(nxt)

        return min_edge
```

**Complexity Analysis**  
- Time complexity: O(V + E) – each vertex and edge is processed at most once.  
- Space complexity: O(V + E) – adjacency list plus visited array and queue.

---

### Approach 3: Union‑Find (Disjoint Set Union) – Track Minimum Weight per Component

**Intuition**  
If we union all edges, each connected component gets a representative root.  
While performing the unions we can keep, for each root, the minimum edge weight seen inside that component.  
After processing all edges, the answer is the minimum weight stored in the component that contains node 1 (which also contains node n).

**Algorithm**  
1. Initialise `parent[i] = i` and `min_weight[i] = +∞`.  
2. For each edge `(u, v, w)`:
   - Find roots `ru`, `rv`.
   - If they differ, attach one to the other (union by size/rank) and set the new root’s `min_weight` to `min(min_weight[ru], min_weight[rv], w)`.
   - If they are the same, just update that root’s `min_weight` with `w` (the edge lies inside the component).  
3. After all unions, find the root of node 1 and return its `min_weight`.

**Implementation**

```python
from typing import List

class Solution:
    def minScore(self, n: int, roads: List[List[int]]) -> int:
        parent = list(range(n + 1))
        size   = [1] * (n + 1)
        min_w  = [float('inf')] * (n + 1)

        def find(x: int) -> int:
            while parent[x] != x:
                parent[x] = parent[parent[x]]
                x = parent[x]
            return x

        def union(a: int, b: int, w: int) -> None:
            ra, rb = find(a), find(b)
            if ra == rb:
                min_w[ra] = min(min_w[ra], w)
                return
            # union by size
            if size[ra] < size[rb]:
                ra, rb = rb, ra
            parent[rb] = ra
            size[ra] += size[rb]
            min_w[ra] = min(min_w[ra], min_w[rb], w)

        for u, v, w in roads:
            union(u, v, w)

        root1 = find(1)
        return min_w[root1]
```

**Complexity Analysis**  
- Time complexity: O(E α(V)) – essentially linear, where α is the inverse Ackermann function (practically ≤ 5).  
- Space complexity: O(V) – parent, size, and min_weight arrays.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the BFS approach on **Example 1**:

```
n = 4
roads = [[1,2,9], [2,3,6], [2,4,5], [1,4,7]]
```

Adjacency list:
```
1: (2,9), (4,7)
2: (1,9), (3,6), (4,5)
3: (2,6)
4: (2,5), (1,7)
```

**BFS trace**

| Step | Queue (front→back) | Visited set | Edge examined (cur → nxt, w) | min_edge so far |
|------|--------------------|-------------|------------------------------|-----------------|
| 0    | [1]                | {1}         | –                            | ∞               |
| 1    | [2,4]              | {1,2,4}     | 1→2 (9)                      | 9               |
|      |                    |             | 1→4 (7)                      | 7               |
| 2    | [4,3]              | {1,2,3,4}   | 2→1 (9) – already visited    | 7               |
|      |                    |             | 2→3 (6)                      | 6               |
|      |                    |             | 2→4 (5) – already visited    | 5               |
| 3    | [3]                | {1,2,3,4}   | 4→2 (5) – visited            | 5               |
|      |                    |             | 4→1 (7) – visited            | 5               |
| 4    | []                 | {1,2,3,4}   | 3→2 (6) – visited            | 5               |

When the queue empties, `min_edge = 5`, which matches the expected output.

---