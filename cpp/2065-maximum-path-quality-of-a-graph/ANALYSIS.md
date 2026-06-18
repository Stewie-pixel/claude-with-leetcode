# Backtracking

## Video Solution

For more details about **Maximum Path Quality of a Graph**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=AZ1jxjQAW1Q).

## Concept

Backtracking is a systematic way to explore all possible configurations (paths, subsets, permutations, etc.) by building candidates incrementally and abandoning a candidate as soon as it determines that the candidate cannot possibly lead to a valid solution. In graph problems, we often use depth‑first search (DFS) with recursion, marking nodes as visited, exploring neighbors, and then undoing the mark (backtracking) to try other routes.

For the *Maximum Path Quality of a Graph* problem we need to find a walk that starts and ends at node 0, whose total travel time does not exceed `maxTime`, and that maximizes the sum of values of **distinct** nodes visited. Because we may revisit nodes, a simple “visited once” flag is insufficient; we keep a visit count so we know when a node is first entered (to add its value) and when we leave it (to restore the count for other branches).

## When to Use It

Use backtracking / DFS with state restoration when you see:
- A request to **explore all possible routes** or **paths** under a constraint (time, length, sum, etc.).
- The need to **collect a reward** (here, node values) only the **first time** a node is reached.
- The graph is **sparse** (each node has ≤ 4 edges), keeping the branching factor manageable.
- The constraint (`maxTime ≤ 100` and each edge costs ≥ 10) limits the depth of recursion, making exponential search feasible.

## Template

```python
def dfs(node, time_used, collected_value):
    # If we are back at the start, update answer
    if node == 0:
        nonlocal best
        best = max(best, collected_value)

    # Try every neighbor
    for nxt, w in graph[node]:
        if time_used + w > maxTime:
            continue                     # prune by time limit

        # Determine if we gain value from nxt
        added = values[nxt] if visit_count[nxt] == 0 else 0
        visit_count[nxt] += 1

        dfs(nxt, time_used + w, collected_value + added)

        # Backtrack: restore state
        visit_count[nxt] -= 1
```

## LeetCode Problem Walkthrough

### Problem: 2065. Maximum Path Quality of a Graph

https://leetcode.com/problems/maximum-path-quality-of-a-graph/

### Approach 1: Brute Force DFS (No Pruning)

**Algorithm**
1. Build an adjacency list from `edges`.
2. Keep a `visit_count` array (initially all zeros) to know how many times we have entered each node.
3. Start DFS from node 0 with `time_used = 0` and `collected_value = values[0]` (node 0 is considered visited once).
4. At each recursive call:
   - If the current node is 0, update the global maximum quality with `collected_value`.
   - For each neighbor, if adding the edge time does not exceed `maxTime`, recursively explore:
        - Increase `visit_count[neighbor]`.
        - If this is the first visit (`visit_count[neighbor] == 1`), add `values[neighbor]` to the collected value.
        - Recurse with updated time and value.
        - After returning, decrement `visit_count[neighbor]` to backtrack.
5. Return the global maximum after DFS finishes.

**Implementation**

```python
class Solution:
    def maximalPathQuality(self, values: List[int], edges: List[List[int]], maxTime: int) -> int:
        n = len(values)
        graph = [[] for _ in range(n)]
        for u, v, t in edges:
            graph[u].append((v, t))
            graph[v].append((u, t))

        visit_count = [0] * n
        visit_count[0] = 1                     # start at node 0
        best = 0

        def dfs(node: int, time_used: int, collected: int):
            nonlocal best
            if node == 0:
                best = max(best, collected)

            for nxt, w in graph[node]:
                nt = time_used + w
                if nt > maxTime:
                    continue
                added = values[nxt] if visit_count[nxt] == 0 else 0
                visit_count[nxt] += 1
                dfs(nxt, nt, collected + added)
                visit_count[nxt] -= 1

        dfs(0, 0, values[0])
        return best
```

**Complexity Analysis**
- Time complexity: O(b^L) — where `b` ≤ 4 is the maximum degree and `L` = `maxTime / min_edge_time` (≤ 10) is the maximum recursion depth; exponential in the worst case.
- Space complexity: O(n + L) — `visit_count` array of size n plus recursion stack depth L.

---

### Approach 2: DFS with Visit‑Count Pruning (Standard Backtracking)

**Intuition**
The brute force version already explores every feasible walk, but we can tighten the search by **pruning branches that cannot possibly beat the current best**. Before recursing, we compute an optimistic upper bound: even if we could magically collect the value of *every* still‑unvisited node, the total would be `collected + remaining_possible_value`. If this bound is ≤ `best`, we stop exploring that branch.

**Algorithm**
- Pre‑compute `sorted_values_desc` = node values sorted descending.
- Maintain a prefix sum array `prefix_sum[i]` = sum of the largest `i` node values.
- In DFS, compute how many distinct nodes are still unvisited (`remaining_nodes = n - distinct_visited`).
- The best extra value we could ever add is `prefix_sum[remaining_nodes]` (the sum of the largest remaining node values).
- If `collected + prefix_sum[remaining_nodes] <= best`, prune the recursion.

**Implementation**

```python
class Solution:
    def maximalPathQuality(self, values: List[int], edges: List[List[int]], maxTime: int) -> int:
        n = len(values)
        graph = [[] for _ in range(n)]
        for u, v, t in edges:
            graph[u].append((v, t))
            graph[v].append((u, t))

        # Pre‑compute optimistic gains: sum of k largest values
        sorted_vals = sorted(values, reverse=True)
        prefix = [0]
        for val in sorted_vals:
            prefix.append(prefix[-1] + val)

        visit_count = [0] * n
        visit_count[0] = 1
        distinct_visited = 1          # node 0 already counted
        best = 0

        def dfs(node: int, time_used: int, collected: int, distinct: int):
            nonlocal best
            if node == 0:
                best = max(best, collected)

            # Upper bound: even if we could take the values of the largest
            # 'remaining' unseen nodes, could we beat best?
            remaining = n - distinct
            if collected + prefix[remaining] <= best:
                return

            for nxt, w in graph[node]:
                nt = time_used + w
                if nt > maxTime:
                    continue
                added = 0
                newly = False
                if visit_count[nxt] == 0:
                    added = values[nxt]
                    newly = True
                visit_count[nxt] += 1
                dfs(nxt, nt, collected + added, distinct + (1 if newly else 0))
                visit_count[nxt] -= 1

        dfs(0, 0, values[0], distinct_visited)
        return best
```

**Complexity Analysis**
- Time complexity: Still exponential in worst case, but the bound pruning cuts many branches; practically much faster.
- Space complexity: O(n + L) — same as before (visit counts, recursion stack, prefix array).

---

### Approach 3: Branch‑and‑Bound with Neighbor Ordering

**Intuition**
Ordering the exploration of neighbors can lead to discovering high‑quality paths early, which raises the `best` threshold sooner and increases pruning effectiveness. By sorting adjacency lists in **descending order of node value** (or value‑to‑time ratio), we prioritize promising routes.

**Algorithm**
- After building the graph, sort each node’s neighbor list so that neighbors with larger `values[neighbor]` are tried first.
- Keep the same DFS with visit counts and the optimistic bound from Approach 2.
- The rest of the algorithm is unchanged.

**Implementation**

```python
class Solution:
    def maximalPathQuality(self, values: List[int], edges: List[List[int]], maxTime: int) -> int:
        n = len(values)
        graph = [[] for _ in range(n)]
        for u, v, t in edges:
            graph[u].append((v, t))
            graph[v].append((u, t))

        # Sort neighbors by node value descending (helps find good paths early)
        for u in range(n):
            graph[u].sort(key=lambda x: -values[x[0]])

        sorted_vals = sorted(values, reverse=True)
        prefix = [0]
        for val in sorted_vals:
            prefix.append(prefix[-1] + val)

        visit_count = [0] * n
        visit_count[0] = 1
        distinct_visited = 1
        best = 0

        def dfs(node: int, time_used: int, collected: int, distinct: int):
            nonlocal best
            if node == 0:
                best = max(best, collected)

            remaining = n - distinct
            if collected + prefix[remaining] <= best:
                return

            for nxt, w in graph[node]:
                nt = time_used + w
                if nt > maxTime:
                    continue
                added = 0
                newly = False
                if visit_count[nxt] == 0:
                    added = values[nxt]
                    newly = True
                visit_count[nxt] += 1
                dfs(nxt, nt, collected + added, distinct + (1 if newly else 0))
                visit_count[nxt] -= 1

        dfs(0, 0, values[0], distinct_visited)
        return best
```

**Complexity Analysis**
- Time complexity: Still exponential, but the improved ordering often reduces the constant factor dramatically.
- Space complexity: O(n + L) — adjacency lists plus visit counts and recursion stack.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on Example 1:

```
values = [0, 32, 10, 43]
edges  = [[0,1,10],[1,2,15],[0,3,10]]
maxTime = 49
```

Adjacency lists (sorted by neighbor value):
```
0: [(3,10), (1,10)]   # 43 > 32
1: [(0,10), (2,15)]   # 0 > 10
2: [(1,15)]
3: [(0,10)]
```

**Dry run (showing only steps that improve the answer)**

| Step | Node | Time | Visit counts (0,1,2,3) | Collected value | Action |
|------|------|------|------------------------|-----------------|--------|
| 1    | 0    | 0    | [1,0,0,0]              | 0               | start |
| 2    | 3    | 10   | [1,0,0,1]              | 0+43=43         | go 0→3 |
| 3    | 0    | 20   | [1,0,0,1]              | 43              | return 3→0 (update best=43) |
| 4    | 1    | 30   | [1,1,0,1]              | 43+32=75        | go 0→1 |
| 5    | 0    | 40   | [1,1,0,1]              | 75              | return 1→0 (update best=75) |
| 6    | 1    | 50   | exceed maxTime → backtrack |
| …    | …    | …    | …                      | …               | … continues exploring other branches but none exceed 75 |

The maximal quality found is **75**, matching the expected answer.

---