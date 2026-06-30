# Dijkstra's Algorithm

## Video Solution

For more details about **Network Delay Time**, watch the walkthrough at [https://www.youtube.com/watch?v=EaphyqKU4PQ](https://www.youtube.com/watch?v=EaphyqKU4PQ)

## Concept

Dijkstra's algorithm finds the shortest path from a single source node to all other nodes in a graph with **non‑negative** edge weights. It repeatedly picks the unvisited node with the smallest known distance, relaxes its outgoing edges, and marks it as visited. When the algorithm finishes, the distance array holds the minimum travel time from the source to every node. The answer for Network Delay Time is the maximum value in this distance array (the time when the last node receives the signal). If any node remains unreachable (distance = ∞), we return -1.

## When to Use It

Use Dijkstra's algorithm when you see:
- A directed or undirected graph with **non‑negative** weights.
- A request for the **shortest path** from a source to all nodes (or to a specific target).
- Problems that ask for the **minimum time/cost** to reach every node (e.g., network delay, routing, cost propagation).

If edges can have negative weights, Bellman‑Ford or SPFA is needed instead.

## Template

```python
import heapq
from typing import List, Tuple

def dijkstra(n: int, graph: List[List[Tuple[int, int]]], src: int) -> List[int]:
    """
    Returns a list `dist` where dist[v] is the shortest distance from src to v.
    Unreachable nodes have distance INF.
    """
    INF = float('inf')
    dist = [INF] * (n + 1)          # 1‑based indexing
    dist[src] = 0
    min_heap = [(0, src)]           # (distance, node)

    while min_heap:
        d, u = heapq.heappop(min_heap)
        if d > dist[u]:             # stale entry
            continue
        for v, w in graph[u]:       # relax edges u -> v
            nd = d + w
            if nd < dist[v]:
                dist[v] = nd
                heapq.heappush(min_heap, (nd, v))
    return dist
```

---

# LeetCode Problem Walkthrough

### Problem: 743. Network Delay Time

https://leetcode.com/problems/network-delay-time/

We are given a directed graph with travel times as edge weights. Starting from node `k`, we need the earliest time when **all** `n` nodes have received the signal, i.e., the maximum shortest‑path distance from `k` to any node. If any node cannot be reached, return `-1`.

---

## Approach 1: Brute Force – Exhaustive Path Search (DFS)

**Algorithm**
- Perform a depth‑first search from the source node `k`.
- Keep track of the accumulated time along the current path.
- When we reach a node, update the best (minimum) time seen so far for that node.
- Explore every possible path (allowing revisits only if it leads to a strictly smaller time, otherwise prune to avoid infinite loops).
- After the search, the answer is the maximum of the best times for all nodes; if any node stays at infinity, return `-1`.

This approach explores an exponential number of paths in the worst case (e.g., a dense graph) because it does not leverage the optimal‑substructure property efficiently.

**Implementation**

```python
from typing import List

class Solution:
    def networkDelayTime(self, times: List[List[int]], n: int, k: int) -> int:
        # build adjacency list
        graph = [[] for _ in range(n + 1)]
        for u, v, w in times:
            graph[u].append((v, w))

        INF = float('inf')
        best = [INF] * (n + 1)      # best time to reach each node

        def dfs(node: int, elapsed: int) -> None:
            # prune if we already have a better time for this node
            if elapsed >= best[node]:
                return
            best[node] = elapsed
            for nxt, w in graph[node]:
                dfs(nxt, elapsed + w)

        dfs(k, 0)
        ans = max(best[1:])         # ignore index 0
        return -1 if ans == INF else ans
```

**Complexity Analysis**
- Time complexity: O(N! ) in the worst case (exponential) – each recursive call may branch to many neighbors.
- Space complexity: O(N) for recursion stack + `best` array.

---

## Approach 2: Bellman‑Ford (Dynamic Programming)

**Intuition**
Instead of exploring every path, we can iteratively relax all edges `n‑1` times. After `i` iterations, we know the shortest path that uses at most `i` edges. Because any shortest path in a graph with `n` nodes has at most `n‑1` edges, after `n‑1` passes the distances are optimal. This works for any edge weights (including negative) but is slower than Dijkstra when all weights are non‑negative.

**Algorithm**
1. Initialize `dist[src] = 0`, all others = ∞.
2. Repeat `n‑1` times:
   - For each edge `(u, v, w)`, if `dist[u] + w < dist[v]`, update `dist[v]`.
3. After the loops, the answer is `max(dist[1..n])`; return `-1` if any distance remains ∞.

**Implementation**

```python
from typing import List

class Solution:
    def networkDelayTime(self, times: List[List[int]], n: int, k: int) -> int:
        INF = float('inf')
        dist = [INF] * (n + 1)
        dist[k] = 0

        # relax edges n-1 times
        for _ in range(n - 1):
            updated = False
            for u, v, w in times:
                if dist[u] != INF and dist[u] + w < dist[v]:
                    dist[v] = dist[u] + w
                    updated = True
            if not updated:   # early exit if no change
                break

        ans = max(dist[1:])
        return -1 if ans == INF else ans
```

**Complexity Analysis**
- Time complexity: O(N * E) – we scan all `E` edges `n‑1` times.
- Space complexity: O(N) – distance array.

---

## Approach 3: Dijkstra’s Algorithm with Min‑Heap (Optimal)

**Intuition**
Since all travel times are non‑negative, we can greedily always expand the node with the currently smallest known distance. A min‑heap (priority queue) lets us retrieve that node in logarithmic time. Once a node is popped from the heap, its distance is finalized because any other path would have to go through a node with equal or larger distance, which cannot improve it. This yields the classic O((V+E) log V) solution.

**Algorithm**
- Build an adjacency list from `times`.
- Use a min‑heap storing `(distance, node)`.
- Repeatedly extract the node with smallest distance, relax its outgoing edges, and push any improvements back into the heap.
- After the heap is empty, compute the maximum distance; return `-1` if any node is unreachable.

**Implementation**

```python
import heapq
from typing import List, Tuple

class Solution:
    def networkDelayTime(self, times: List[List[int]], n: int, k: int) -> int:
        # 1. Build graph
        graph = [[] for _ in range(n + 1)]
        for u, v, w in times:
            graph[u].append((v, w))

        # 2. Dijkstra
        INF = float('inf')
        dist = [INF] * (n + 1)
        dist[k] = 0
        heap = [(0, k)]                     # (distance, node)

        while heap:
            d, u = heapq.heappop(heap)
            if d > dist[u]:                 # stale entry
                continue
            for v, w in graph[u]:
                nd = d + w
                if nd < dist[v]:
                    dist[v] = nd
                    heapq.heappush(heap, (nd, v))

        # 3. Extract answer
        ans = max(dist[1:])                 # ignore index 0
        return -1 if ans == INF else ans
```

**Complexity Analysis**
- Time complexity: O((V + E) log V) – each edge may cause a heap push, and each pop is log V.
- Space complexity: O(V + E) for the graph + O(V) for distance array + heap.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run **Approach 3 (Dijkstra)** on the first example:

**Input**: `times = [[2,1,1],[2,3,1],[3,4,1]]`, `n = 4`, `k = 2`  
**Graph**:
- 2 → (1,1), (3,1)
- 3 → (4,1)

```
| Step | Popped (dist, node) | Edges examined                | Updates (node, newDist)                     | Dist array [1..4]                |
|------|----------------------|------------------------------|---------------------------------------------|----------------------------------|
| Init | –                    | –                            | –                                           | [∞, 0, ∞, ∞, ∞]                  |
| 1    | (0, 2)               | 2→1 (w=1) → dist[1]=1       | (1,1)                                       | [∞, 0, 1, ∞, ∞]                  |
|      |                      | 2→3 (w=1) → dist[3]=1       | (3,1)                                       | [∞, 0, 1, 1, ∞]                  |
| 2    | (1, 1)               | (no outgoing edges)          | –                                           | [∞, 0, 1, 1, ∞]                  |
| 3    | (1, 3)               | 3→4 (w=1) → dist[4]=2       | (4,2)                                       | [∞, 0, 1, 1, 2]                  |
| 4    | (2, 4)               | (no outgoing edges)          | –                                           | [∞, 0, 1, 1, 2]                  |
| End  | heap empty           | –                            | –                                           | max = 2 → answer = 2             |
```

All nodes are reachable; the largest distance is 2, matching the expected output.  

If we run the same trace on Example 3 (`k = 2` but only edge `1→2`), node 3 and 4 stay at ∞, so we return `-1`.