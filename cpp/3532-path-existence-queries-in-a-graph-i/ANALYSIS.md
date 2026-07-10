# Union Find (Disjoint Set Union)

## Video Solution

For more details about **Path Existence Queries in a Graph I**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=SIYHOBb-w38).

## Concept

Union Find (also called Disjoint Set Union, DSU) is a data structure that keeps track of a partition of a set into disjoint (non‑overlapping) subsets. It supports two main operations:

1. **Find(x)** – returns the representative (root) of the set containing `x`. With path compression, future queries become faster.
2. **Union(x, y)** – merges the sets containing `x` and `y`. Using union by rank/size keeps the tree shallow.

These operations run in nearly constant time (inverse Ackermann), making Union Find ideal for problems where we need to answer many connectivity queries after building a graph incrementally or statically.

## When to Use It

Use Union Find when you see:

- Questions asking “are two nodes in the same connected component?” after adding edges.
- A static graph where you can preprocess all edges once and then answer many queries.
- Problems involving merging groups, detecting cycles in an undirected graph, or Kruskal’s MST.
- The input size is large (up to 10⁵) and a per‑query BFS/DFS would be too slow.

## Template

```python
class DSU:
    def __init__(self, n: int):
        self.parent = list(range(n))   # each node is its own parent
        self.rank   = [0] * n          # approximate depth of tree

    def find(self, x: int) -> int:
        """Find root of x with path compression."""
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def union(self, x: int, y: int) -> None:
        """Union the sets containing x and y (by rank)."""
        xr, yr = self.find(x), self.find(y)
        if xr == yr:
            return
        if self.rank[xr] < self.rank[yr]:
            self.parent[xr] = yr
        elif self.rank[xr] > self.rank[yr]:
            self.parent[yr] = xr
        else:
            self.parent[yr] = xr
            self.rank[xr] += 1
```

## LeetCode Problem Walkthrough

### Problem: 3532. Path Existence Queries in a Graph I
https://leetcode.com/problems/path-existence-queries-in-a-graph-i/

We are given a sorted array `nums`. An undirected edge exists between indices `i` and `j` iff `|nums[i] - nums[j]| ≤ maxDiff`. Because the array is sorted, an edge can only exist between **nearby** elements; in fact, if `nums[i+1] - nums[i] > maxDiff` then no edge can cross that gap. Hence the graph splits into contiguous blocks where consecutive differences are ≤ `maxDiff`. Each block forms a connected component.

We need to answer many queries `(u, v)` asking whether `u` and `v` lie in the same component.

### Approach 1: Brute Force (BFS/DFS per query)

**Algorithm**

1. Build the full adjacency list: for each pair `(i, j)` with `|nums[i]-nums[j]| ≤ maxDiff` add an edge.  
   (In the worst case this is O(n²), too large.)
2. For each query, run BFS/DFS from `u` and see if we can reach `v`.

**Implementation**

```python
from collections import deque
class Solution:
    def pathExistenceQueries(self, n, nums, maxDiff, queries):
        # Build adjacency (inefficient for large n)
        adj = [[] for _ in range(n)]
        for i in range(n):
            for j in range(i+1, n):
                if nums[j] - nums[i] > maxDiff:
                    break          # because nums is sorted
                adj[i].append(j)
                adj[j].append(i)

        def bfs(src, dst):
            if src == dst:
                return True
            seen = [False] * n
            q = deque([src])
            seen[src] = True
            while q:
                cur = q.popleft()
                for nb in adj[cur]:
                    if not seen[nb]:
                        if nb == dst:
                            return True
                        seen[nb] = True
                        q.append(nb)
            return False

        return [bfs(u, v) for u, v in queries]
```

**Complexity Analysis**

- Time complexity: O(n² + Q·(n+E)) — building all edges is O(n²) in worst case; each BFS may traverse the whole graph.
- Space complexity: O(n+E) for adjacency list plus O(n) for BFS queue/visited.

This approach fails the constraints (`n` up to 10⁵).

---

### Approach 2: Union Find (Preprocess edges efficiently)

**Intuition**

Because `nums` is sorted, we only need to consider edges between **consecutive** indices. If `nums[i+1] - nums[i] ≤ maxDiff`, then `i` and `i+1` are directly connected; transitivity through such edges gives the full component. Any non‑consecutive pair that satisfies the distance condition will already be linked via a chain of consecutive edges.

Thus we can:

1. Iterate once over the array, union `i` with `i+1` whenever the difference ≤ `maxDiff`.
2. After processing, answer each query by checking whether `find(u) == find(v)`.

**Implementation**

```python
class DSU:
    def __init__(self, n):
        self.parent = list(range(n))
        self.rank   = [0] * n
    def find(self, x):
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]
    def union(self, x, y):
        xr, yr = self.find(x), self.find(y)
        if xr == yr: return
        if self.rank[xr] < self.rank[yr]:
            self.parent[xr] = yr
        elif self.rank[xr] > self.rank[yr]:
            self.parent[yr] = xr
        else:
            self.parent[yr] = xr
            self.rank[xr] += 1

class Solution:
    def pathExistenceQueries(self, n, nums, maxDiff, queries):
        dsu = DSU(n)
        for i in range(1, n):
            if nums[i] - nums[i-1] <= maxDiff:
                dsu.union(i, i-1)
        return [dsu.find(u) == dsu.find(v) for u, v in queries]
```

**Complexity Analysis**

- Time complexity: O(n α(n) + Q α(n)) ≈ O(n + Q) — one pass to build DSU, then near‑constant per query.
- Space complexity: O(n) for parent and rank arrays.

---

### Approach 3: Linear Scan Component ID (Simplified Union Find)

**Intuition**

Since edges only exist between consecutive elements only when the gap ≤ `maxDiff`, the connected components are exactly the maximal sub‑arrays where every consecutive difference ≤ `maxDiff`. We can label each index with a component ID by scanning left to right and incrementing the ID whenever we encounter a gap > `maxDiff`. Two nodes are connected iff they share the same ID.

This is essentially what Union Find does, but we avoid the DSU overhead because the union pattern is strictly linear.

**Implementation**

```python
class Solution:
    def pathExistenceQueries(self, n, nums, maxDiff, queries):
        comp = [0] * n          # component id for each index
        cur = 0
        for i in range(1, n):
            if nums[i] - nums[i-1] > maxDiff:
                cur += 1        # start a new component
            comp[i] = cur
        # comp[0] stays 0
        return [comp[u] == comp[v] for u, v in queries]
```

**Complexity Analysis**

- Time complexity: O(n + Q) — one linear scan plus O(1) per query.
- Space complexity: O(n) for the component array.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the linear‑scan component labeling on Example 2:

```
n = 4
nums = [2, 5, 6, 8]
maxDiff = 2
queries = [[0,1],[0,2],[1,3],[2,3]]
```

| Step | i | nums[i] | nums[i]-nums[i-1] | > maxDiff? | cur (comp id) | comp[i] |
|------|---|---------|-------------------|-----------|---------------|---------|
| init | 0 | 2       | –                 | –         | 0             | 0       |
| 1    | 1 | 5       | 5‑2 = 3           | **yes**   | 1             | 1       |
| 2    | 2 | 6       | 6‑5 = 1           | no        | 1             | 1       |
| 3    | 3 | 8       | 8‑6 = 2           | no        | 1             | 1       |

Resulting `comp = [0, 1, 1, 1]`.

Now answer queries:

- `[0,0]` → comp[0] = 0, comp[0] = 0 → true (trivial self‑path)
- `[0,1]` → 0 vs 1 → false
- `[0,2]` → 0 vs 1 → false
- `[1,3]` → 1 vs 1 → true
- `[2,3]` → 1 vs 1 → true

Matches the expected output `[false, false, true, true]` (note the first query in the example was `[0,0]` giving `true`).

---

**Summary**

- For connectivity queries on a graph defined by a threshold on a sorted array, the graph decomposes into contiguous blocks.
- Union Find (DSU) gives a general, reusable solution.
- When the edge pattern is restricted to consecutive elements, a simple linear scan labeling components is sufficient and faster in practice.
- Both approaches achieve O(n + Q) time and O(n) space, easily fitting the limits.