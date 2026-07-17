# Minimum Spanning Tree

## Video Solution

For more details about **Min Cost to Connect All Points**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=8VPIrqwQ8sQ)

## Concept

A **Minimum Spanning Tree (MST)** of a weighted, undirected graph connects all vertices with the smallest possible total edge weight while containing no cycles.  
In this problem each point is a vertex and the weight of an edge between two points is their Manhattan distance.  
The goal is to find the MST weight.

**Prim’s algorithm** builds the MST incrementally:
1. Start from an arbitrary vertex.
2. Repeatedly add the cheapest edge that connects a vertex already in the tree to a vertex outside the tree.
3. Stop when all vertices are included.

Because the graph is complete (every pair of points has an edge), we can compute distances on the fly without storing all `n*(n‑1)/2` edges.

## When to Use It

Use Prim’s (or Kruskal’s) when you see:
- “Connect all points/nodes with minimum total cost”
- Edge weight defined by a formula (e.g., Manhattan, Euclidean)
- The graph is dense or you can generate edges lazily
- You need exactly one simple path between any pair of vertices (tree property)

## Template

```python
import heapq
from typing import List

def prim_mst(points: List[List[int]]) -> int:
    """
    Prim's algorithm O(n^2) for dense graphs.
    Returns the total weight of the MST.
    """
    n = len(points)
    in_mst = [False] * n          # whether vertex i is already in the MST
    min_edge = [float('inf')] * n # cheapest edge that connects i to the MST
    min_edge[0] = 0               # start from vertex 0
    total_cost = 0

    for _ in range(n):
        # pick the vertex with the smallest connecting edge that is not yet in MST
        u = -1
        for v in range(n):
            if not in_mst[v] and (u == -1 or min_edge[v] < min_edge[u]):
                u = v

        in_mst[u] = True
        total_cost += min_edge[u]

        # update the cheapest connection for the remaining vertices
        for v in range(n):
            if not in_mst[v]:
                dist = abs(points[u][0] - points[v][0]) + abs(points[u][1] - points[v][1])
                if dist < min_edge[v]:
                    min_edge[v] = dist

    return total_cost
```

## LeetCode Problem Walkthrough

### Problem: 1584. Min Cost to Connect All Points  
https://leetcode.com/problems/min-cost-to-connect-all-points/

---

### Approach 1: Brute Force (Conceptual)

**Algorithm**  
Enumerate every possible spanning tree (there are `n^(n‑2)` of them by Cayley’s formula), compute its total Manhattan cost, and keep the minimum.  
This is exponential and infeasible for `n ≤ 1000`.

**Implementation**  
*(Not provided – would explode in time and memory.)*

**Complexity Analysis**  
- Time complexity: O(n^(n‑2)) — grows super‑exponentially.  
- Space complexity: O(n) — to store a single tree while evaluating.

*We only mention this to emphasize why a smarter algorithm is necessary.*

---

### Approach 2: Prim’s Algorithm – O(n²) (Array‑based)

**Intuition**  
Since the graph is complete, we can avoid building the full edge list.  
At each step we keep, for every vertex not yet in the MST, the cheapest edge that connects it to any vertex already in the MST.  
Picking the minimum of these values gives the next vertex to add—exactly what Prim’s algorithm does.

**Algorithm**  
1. Initialize `min_edge[i] = ∞` for all vertices, except the start vertex (`0`) where `min_edge[0] = 0`.  
2. Repeat `n` times:  
   a. Select the vertex `u` not in the MST with the smallest `min_edge[u]`.  
   b. Add its cost to the answer and mark `u` as inside the MST.  
   c. For each vertex `v` still outside the MST, compute the Manhattan distance `dist(u, v)` and relax `min_edge[v] = min(min_edge[v], dist)`.  
3. After all vertices are taken, the accumulated cost is the MST weight.

**Implementation**

```python
class Solution:
    def minCostConnectPoints(self, points: List[List[int]]) -> int:
        n = len(points)
        in_mst = [False] * n
        min_edge = [float('inf')] * n
        min_edge[0] = 0          # start from point 0
        ans = 0

        for _ in range(n):
            # pick the closest vertex not yet in the MST
            u = -1
            for v in range(n):
                if not in_mst[v] and (u == -1 or min_edge[v] < min_edge[u]):
                    u = v

            in_mst[u] = True
            ans += min_edge[u]

            # update distances to the remaining vertices
            for v in range(n):
                if not in_mst[v]:
                    dist = abs(points[u][0] - points[v][0]) + abs(points[u][1] - points[v][1])
                    if dist < min_edge[v]:
                        min_edge[v] = dist

        return ans
```

**Complexity Analysis**  
- Time complexity: O(n²) — we scan all vertices to pick the minimum (`O(n)`) and update distances (`O(n)`) for each of the `n` iterations.  
- Space complexity: O(n) — two boolean/float arrays of size `n`.

---

### Approach 3: Prim’s Algorithm – O(E log V) with Heap

**Intuition**  
When the graph is sparse, a priority queue (min‑heap) gives a faster way to repeatedly extract the vertex with the smallest connecting edge.  
Even though our graph is dense (`E = n²`), the heap version is still correct and often easier to extend to adjacency‑list representations.

**Algorithm**  
1. Start from vertex 0, push all edges from 0 to the heap as `(distance, vertex)`.  
2. While the heap is not empty and we haven’t added `n` vertices:  
   a. Pop the smallest `(d, v)`. If `v` is already in the MST, skip it.  
   b. Otherwise, add `d` to the answer, mark `v` as visited, and push all edges from `v` to unvisited vertices.  
3. The answer after `n` vertices have been taken is the MST weight.

**Implementation**

```python
import heapq
from typing import List

class Solution:
    def minCostConnectPoints(self, points: List[List[int]]) -> int:
        n = len(points)
        visited = [False] * n
        # heap stores (distance_to_mst, vertex)
        heap = [(0, 0)]          # start from vertex 0 with cost 0
        total = 0
        used = 0

        while heap and used < n:
            dist, u = heapq.heappop(heap)
            if visited[u]:
                continue
            visited[u] = True
            total += dist
            used += 1

            for v in range(n):
                if not visited[v]:
                    d = abs(points[u][0] - points[v][0]) + abs(points[u][1] - points[v][1])
                    heapq.heappush(heap, (d, v))

        return total
```

**Complexity Analysis**  
- Time complexity: O(E log V) = O(n² log n) — each of the `n²` possible edges may be pushed once, each heap operation costs `log n`.  
- Space complexity: O(E) = O(n²) in the worst case for the heap, but in practice we only keep O(n) entries because we discard visited vertices; the auxiliary arrays are O(n).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the array‑based Prim’s algorithm on the first example:

**Input:** `points = [[0,0],[2,2],[3,10],[5,2],[7,0]]`  
We will compute the minimum cost to connect all points.

```
| Step | Added vertex (u) | min_edge before pick | Cost added | Updated min_edge for remaining vertices |
|------|------------------|----------------------|------------|------------------------------------------|
| 0    | –                | [0, ∞, ∞, ∞, ∞]      | 0          | (start)                                  |
| 1    | 0                | [0, 4, 13, 7, 7]     | 0          | after relaxing from 0:                  |
|      |                  |                      |            |   to 1: |0-2|+|0-2| = 4                     |
|      |                  |                      |            |   to 2: |0-3|+|0-10| = 13                   |
|      |                  |                      |            |   to 3: |0-5|+|0-2| = 7                     |
|      |                  |                      |            |   to 4: |0-7|+|0-0| = 7                     |
| 2    | 1 (cost 4)       | [0, 4, 13, 7, 7]     | 4          | relax from 1:                           |
|      |                  |                      |            |   to 2: min(13, |2-3|+|2-10| = 9) → 9 |
|      |                  |                      |            |   to 3: min(7,  |2-5|+|2-2| = 3) → 3 |
|      |                  |                      |            |   to 4: min(7,  |2-7|+|2-0| = 9) → 7 |
| 3    | 3 (cost 3)       | [0,4,9,3,7]          | 3          | relax from 3:                           |
|      |                  |                      |            |   to 2: min(9, |5-3|+|2-10| =10) →9   |
|      |                  |                      |            |   to 4: min(7, |5-7|+|2-0| =4)  →4   |
| 4    | 4 (cost 4)       | [0,4,9,3,4]          | 4          | relax from 4:                           |
|      |                  |                      |            |   to 2: min(9,|7-3|+|0-10| =14) →9   |
| 5    | 2 (cost 9)       | [0,4,9,3,4]          | 9          | (all vertices visited)                  |
```

**Total cost** = 0 + 4 + 3 + 4 + 9 = **20**, which matches the expected output.

---

**Summary**  
- The problem is a classic Minimum Spanning Tree with Manhattan edge weights.  
- Prim’s algorithm (either O(n²) array version or O(n² log n) heap version) yields the optimal solution efficiently for `n ≤ 1000`.  
- Always start with a brute‑force mindset to appreciate why the greedy Prim’s approach works, then present the optimized versions with clear intuition, correctness argument, and complexity analysis.
