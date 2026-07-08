# Binary Search

## Video Solution

For more details about **Network Recovery Pathways**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=6Uait6ODg5Q).

## Concept

Binary search on the answer is a technique used when we can monotonic‑ly test a predicate `P(x)`: if `P(x)` is true then `P(y)` is true for all `y ≤ x` (or the opposite).  
Here the predicate is: “Is there a valid 0 → n‑1 path whose **minimum edge cost** is at least `T` and whose total cost ≤ k?”  
If a path exists with minimum edge ≥ T, then any smaller threshold `T' < T` also admits a path (we can keep the same path). Hence the predicate is monotonic and we can binary‑search the largest feasible `T`.  
For a fixed `T` we keep only edges with cost ≥ T and only online intermediate nodes, then we need to know whether the **shortest** total‑cost path from 0 to n‑1 fits within `k`. Because the graph is a DAG, the shortest path can be found in linear time using a topological order DP.

## When to Use It

Use binary search on the answer when you notice:

- The problem asks to **maximize** (or minimize) a value that appears as a threshold on edges/nodes (e.g., maximize the minimum edge weight, minimize the maximum load, etc.).
- For a given threshold you can decide feasibility in polynomial time (often via a shortest‑path, DP, or greedy check).
- The feasibility predicate is monotonic with respect to the threshold.

Typical clues: “maximum minimum”, “largest value such that …”, “minimum possible maximum”, together with a constraint like total cost ≤ k, path length ≤ L, etc.

## Template

```python
def feasible(threshold: int) -> bool:
    # Return True iff a path satisfying the threshold exists.
    ...

# Binary search over sorted unique edge costs (or over a numeric range)
lo, hi = 0, len(values) - 1
answer = -1
while lo <= hi:
    mid = (lo + hi) // 2
    if feasible(values[mid]):
        answer = values[mid]          # threshold works → try higher
        lo = mid + 1
    else:
        hi = mid - 1                  # too high → lower threshold
return answer
```

## LeetCode Problem Walkthrough

### Problem: 3620. Network Recovery Pathways

https://leetcode.com/problems/network-recovery-pathways/

### Approach 1: Brute Force (DFS over all paths)

**Algorithm**  
Explore every possible path from node 0 to node n‑1 using depth‑first search.  
While recursing, keep:
- `cost_sofar`: sum of edge costs on the current path,
- `min_edge`: minimum edge cost seen so far.  
If we reach the target and `cost_sofar ≤ k`, update the answer with `min_edge`.  
Prune the search when `cost_sofar > k` or when the next node is offline (except the target).

**Implementation**

```python
class Solution:
    def maxMinPath(self, edges, online, k):
        n = len(online)
        g = [[] for _ in range(n)]
        for u, v, w in edges:
            g[u].append((v, w))

        self.best = -1
        visited = [False] * n          # not needed for DAG but keeps symmetry

        def dfs(u, cost_sofar, min_edge):
            if cost_sofar > k:               # cost already too large
                return
            if u == n - 1:                   # reached destination
                self.best = max(self.best, min_edge)
                return
            for v, w in g[u]:
                if not online[v] and v != n - 1:
                    continue                  # cannot step onto offline node
                dfs(v, cost_sofar + w, min(min_edge, w))

        dfs(0, 0, float('inf'))
        return self.best if self.best != float('inf') else -1
```

**Complexity Analysis**

- Time: O(number of different paths) – in the worst case exponential, O(2ⁿ) for a dense DAG.  
- Space: O(n) recursion stack depth + recursion stack.

### Approach 2: Modified Dijkstra (state = node, current minimum edge)

**Intuition**  
Instead of trying every path, we can propagate the *best* (largest) minimum edge we can achieve for each node while keeping track of the smallest total cost needed to achieve that minimum.  
We use a max‑heap priority queue keyed by the current minimum edge (so we always expand the path with the highest bottleneck first). For each state we store the minimal total cost required to reach that node with at least that bottleneck.  
When we pop a state `(node, bottleneck, cost)` we try outgoing edges: the new bottleneck becomes `min(bottleneck, edge_cost)`. If the new total cost `cost + edge_cost` ≤ k and is better than any previously recorded cost for `(neighbor, new_bottleneck)`, we push it.

**Algorithm**  
1. Build adjacency list.  
2. `best_cost[node][bottleneck]` – we store the smallest cost to reach `node` with at least `bottleneck`. Because edge costs are up to 1e9 we cannot keep a full table; instead we keep a dictionary per node that maps bottleneck → best cost, pruning dominated states (if a state has both ≤ cost and ≥ bottleneck it dominates another).  
3. Initialize with `(0, +∞, 0)`.  
4. Pop from max‑heap; if node is n‑1 return its bottleneck (first time we pop the target we have the maximal possible bottleneck because of heap ordering).  
5. Otherwise relax outgoing edges as described.  
6. If the heap empties, return -1.

**Implementation**

```python
import heapq

class Solution:
    def maxMinPath(self, edges, online, k):
        n = len(online)
        g = [[] for _ in range(n)]
        for u, v, w in edges:
            g[u].append((v, w))

        # max‑heap: store (-bottleneck, node, cost)
        heap = [(-float('inf'), 0, 0)]   # negative for max‑heap
        # best[node] = dict{bottleneck: min_cost}
        best = [dict() for _ in range(n)]
        best[0][float('inf')] = 0

        while heap:
            neg_bott, u, cost = heapq.heappop(heap)
            bottleneck = -neg_bott
            if u == n - 1:
                return bottleneck
            # If we have a better record for this (node,bottleneck) skip
            if best[u].get(bottleneck, float('inf')) < cost:
                continue
            for v, w in g[u]:
                if not online[v] and v != n - 1:
                    continue
                nb = min(bottleneck, w)
                nc = cost + w
                if nc > k:
                    continue
                # keep only the smallest cost for each bottleneck at v
                if nb not in best[v] or nc < best[v][nb]:
                    best[v][nb] = nc
                    heapq.heappush(heap, (-nb, v, nc))
        return -1
```

**Complexity Analysis**

- Let `C` be the number of distinct edge costs (≤ m). Each node can store at most `C` bottleneck entries.  
- Time: O((n + m) · log (n·C)) due to heap operations.  
- Space: O(n·C) in the worst case (still large but far better than exponential).

### Approach 3: Binary Search + DP on DAG (most optimal)

**Intuition**  
The predicate “there exists a path with minimum edge ≥ T and total cost ≤ k” is monotonic in `T`.  
We can binary‑search the answer over the sorted unique edge costs.  
For a fixed threshold `T` we delete all edges with cost < T and ignore offline intermediate nodes.  
On the remaining graph we only need to know the **shortest** total‑cost path from 0 to n‑1; if that distance ≤ k the threshold works.  
Because the original graph is a DAG, the filtered graph is also a DAG, so we can compute shortest paths in linear time using a topological order DP (no need for Dijkstra).

**Algorithm**

1. Build adjacency list and compute indegrees.  
2. Obtain a topological order of the original DAG (once).  
3. Collect all distinct edge costs, sort them.  
4. Binary search over this list:  
   - For mid‑value `T`, run a DP over the topological order:  
     `dist[v] = min(dist[u] + w)` for every edge `u → v` with `w ≥ T` and `online[v]` true (or `v` is the target).  
   - If `dist[n‑1] ≤ k` → feasible, move left bound up; else move right bound down.  
5. Return the largest feasible cost, or -1 if none.

**Implementation**

```python
from typing import List

class Solution:
    def maxMinPath(self, edges: List[List[int]], online: List[bool], k: int) -> int:
        n = len(online)
        adj = [[] for _ in range(n)]
        indeg = [0] * n
        cost_set = set()
        for u, v, w in edges:
            adj[u].append((v, w))
            indeg[v] += 1
            cost_set.add(w)

        if not cost_set:
            return -1

        # ---------- topological order (once) ----------
        from collections import deque
        q = deque([i for i in range(n) if indeg[i] == 0])
        topo = []
        while q:
            u = q.popleft()
            topo.append(u)
            for v, _ in adj[u]:
                indeg[v] -= 1
                if indeg[v] == 0:
                    q.append(v)
        # graph is guaranteed to be a DAG, so topo will contain all nodes

        costs = sorted(cost_set)          # increasing

        INF = 10**18

        def feasible(threshold: int) -> bool:
            dist = [INF] * n
            dist[0] = 0
            for u in topo:
                if dist[u] == INF:
                    continue
                du = dist[u]
                for v, w in adj[u]:
                    if w >= threshold and (online[v] or v == n - 1):
                        nd = du + w
                        if nd < dist[v]:
                            dist[v] = nd
            return dist[n - 1] <= k

        lo, hi = 0, len(costs) - 1
        ans = -1
        while lo <= hi:
            mid = (lo + hi) // 2
            if feasible(costs[mid]):
                ans = costs[mid]
                lo = mid + 1
            else:
                hi = mid - 1
        return ans
```

**Complexity Analysis**

- Topological sort: O(n + m).  
- Each feasibility check scans all edges once: O(n + m).  
- Binary search over `U` distinct costs (`U ≤ m`): O(log U).  
- Total time: O((n + m) · log m).  
- Space: O(n + m) for adjacency list, indegree, distance array, and topological order.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the feasibility check for the sample input with threshold = 3 (the answer).

**Example**  
`edges = [[0,1,5],[1,3,10],[0,2,3],[2,3,4]]`  
`online = [True,True,True,True]`  
`k = 10`  

Topological order of the original DAG: `[0, 1, 2, 3]`.

#### Dry Run – feasibility(T = 3)

| Step | u   | dist[u] before | Edge (u→v, w) | w ≥ T? | online[v]? | cand = dist[u] + w | dist[v] after |
|------|-----|----------------|---------------|--------|------------|--------------------|---------------|
| 1    | 0   | 0              | 0→1 (5)       | ✅     | ✅         | 5                  | dist[1]=5    |
| 2    | 0   | 0              | 0→2 (3)       | ✅     | ✅         | 3                  | dist[2]=3    |
| 3    | 1   | 5              | 1→3 (10)      | ✅     | ✅         | 15                 | dist[3]=15   |
| 4    | 2   | 3              | 2→3 (4)       | ✅     | ✅         | 7                  | **dist[3]=7** (min) |
| 5    | 3   | 7              | –             | –      | –          | –                  | –            |

Final `dist[3] = 7 ≤ k (=10)`, so threshold = 3 is feasible.

Repeating the same process for thresholds 4,5,10 shows they are **not** feasible (the shortest path exceeds k or becomes disconnected).  
Binary search therefore returns 3 as the maximum feasible minimum edge cost.