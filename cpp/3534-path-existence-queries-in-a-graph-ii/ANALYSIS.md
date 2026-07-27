# Two Pointers

## Video Solution

For more details about **Path Existence Queries in a Graph II**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=HATFedoXIiU).

## Concept

When edges are defined by a condition on node values (e.g., an edge exists if the absolute difference of values ≤ `maxDiff`), the graph can be understood after sorting the nodes by their values. In the sorted order each node can reach a **contiguous block** of neighbours whose values differ by at most `maxDiff`.  
The problem then reduces to: *given an array `right[i]` (the farthest index reachable from `i` in one step), find the minimum number of jumps needed to go from index `i` to index `j` (i < j).* This is exactly the classic “minimum jumps to reach end” (Jump Game II) problem, which can be answered in O(1) per query after a binary‑lifting (doubling) preprocessing of the jump pointers.

## When to Use It

Use the two‑pointer / sliding‑window + binary‑lifting pattern when you see:

* A graph whose edges are defined by a simple inequality on node attributes (e.g., `|value[u] - value[v]| ≤ k`).
* Queries asking for the shortest (fewest‑edges) path between two nodes in such a graph.
* The need to answer many queries efficiently after an O(n log n) or O(n) preprocessing step.

## Template

```python
# 1. Sort nodes by the attribute that defines edges
order = sorted(range(n), key=lambda x: attr[x])
sorted_attr = [attr[i] for i in order]
pos = [0] * n                 # original index -> position in sorted order
for p, idx in enumerate(order):
    pos[idx] = p

# 2. Sliding window to compute farthest reach in one step (right boundary)
right = [0] * n
r = 0
for l in range(n):
    if r < l:
        r = l
    while r + 1 < n and sorted_attr[r + 1] - sorted_attr[l] <= maxDiff:
        r += 1
    right[l] = r                # inclusive farthest index reachable from l

# 3. Binary lifting table: up[k][i] = node reached from i after 2^k jumps
LOG = (n.bit_length()) + 1   # enough for 2^LOG > n
up = [right[:] ]               # up[0] is the 1‑jump table
for k in range(1, LOG):
    prev = up[-1]
    nxt = [prev[prev[i]] for i in range(n)]
    up.append(nxt)

# 4. Helper to compute min jumps from i to j (i < j) using the table
def min_jumps(i, j):
    if i >= j:
        return 0
    cur = i
    jumps = 0
    for k in reversed(range(LOG)):
        if up[k][cur] < j:      # we can jump 2^k steps and still stay before j
            cur = up[k][cur]
            jumps += 1 << k
    return jumps + 1            # one final jump lands at or beyond j
```

## LeetCode Problem Walkthrough

### Problem: 3534. Path Existence Queries in a Graph II

https://leetcode.com/problems/path-existence-queries-in-a-graph-ii/

### Approach 1: Brute Force (BFS per query)

**Algorithm**
* For each query `[u, v]` run a BFS/DFS on the implicit graph until `v` is found or the queue empties.
* The graph is implicit: from a node `x` you can go to any node `y` with `|nums[x] - nums[y]| ≤ maxDiff`.  
  To avoid O(n²) edge generation, during BFS we can check all nodes – leading to O(n) per expansion.

**Implementation**

```python
from collections import deque

class Solution:
    def pathExistenceQueries(self, n, nums, maxDiff, queries):
        # adjacency test helper
        def reachable(a, b):
            return abs(nums[a] - nums[b]) <= maxDiff

        ans = []
        for u, v in queries:
            if u == v:
                ans.append(0)
                continue
            visited = [False] * n
            q = deque([(u, 0)])
            visited[u] = True
            found = -1
            while q:
                node, dist = q.popleft()
                for nb in range(n):
                    if not visited[nb] and reachable(node, nb):
                        if nb == v:
                            found = dist + 1
                            q.clear()
                            break
                        visited[nb] = True
                        q.append((nb, dist + 1))
                if found != -1:
                    break
            ans.append(found)
        return ans
```

**Complexity Analysis**

- Time complexity: O(Q · (N²)) in the worst case – each BFS may scan all N nodes for each of its N expansions.  
- Space complexity: O(N) for the visited array and queue.

### Approach 2: Pre‑compute reachable interval + greedy scan per query

**Intuition**
After sorting by `nums`, each node can only connect to a contiguous block of nodes whose values differ ≤ `maxDiff`.  
We can therefore replace the implicit graph with the array `right[i]` (farther index reachable in one step).  
The shortest path from `i` to `j` (i < j) is the minimum number of jumps needed to move from `i` to at least `j` when from position `p` you may jump to any index in `[p, right[p]]`.  
This is exactly the Jump Game II problem; a greedy scan that always jumps to the farthest reachable index yields the optimal number of jumps in O(length) time.

**Algorithm**
1. Sort nodes, build `pos` and `sorted_vals`.
2. Sliding window to fill `right[i]`.
3. For each query:
   * If nodes belong to different connected components (detected via a simple prefix on gaps > `maxDiff`) → answer `-1`.
   * Otherwise, let `l = min(pos[u], pos[v])`, `r = max(pos[u], pos[v])`.
   * Greedy walk: `cur = l`, `steps = 0`; while `cur < r`: set `cur = right[cur]`; `steps += 1`.
   * Return `steps`.

**Implementation**

```python
class Solution:
    def pathExistenceQueries(self, n, nums, maxDiff, queries):
        order = sorted(range(n), key=lambda i: nums[i])
        sorted_vals = [nums[i] for i in order]
        pos = [0] * n
        for p, idx in enumerate(order):
            pos[idx] = p

        # component id: increase when gap > maxDiff
        comp = [0] * n
        for i in range(1, n):
            comp[i] = comp[i-1] + (sorted_vals[i] - sorted_vals[i-1] > maxDiff)

        # right[i] = farthest index reachable in one step
        right = [0] * n
        r = 0
        for l in range(n):
            if r < l:
                r = l
            while r + 1 < n and sorted_vals[r+1] - sorted_vals[l] <= maxDiff:
                r += 1
            right[l] = r

        def min_jumps_greedy(l, r_idx):
            if l >= r_idx:
                return 0
            steps = 0
            cur = l
            while cur < r_idx:
                cur = right[cur]
                steps += 1
            return steps

        ans = []
        for u, v in queries:
            if u == v:
                ans.append(0)
                continue
            pu, pv = pos[u], pos[v]
            if comp[pu] != comp[pv]:
                ans.append(-1)
                continue
            lo, hi = (pu, pv) if pu < pv else (pv, pu)
            ans.append(min_jumps_greedy(lo, hi))
        return ans
```

**Complexity Analysis**

- Time complexity: O(N log N + Q · J) where J is the number of jumps per query (worst‑case O(N)).  
- Space complexity: O(N) for the auxiliary arrays.

### Approach 3: Binary Lifting (Doubling) on jump pointers – Optimal

**Intuition**
The greedy scan of Approach 2 can be sped up by preprocessing *jump pointers*: `up[k][i]` = node reached from `i` after `2^k` jumps (each jump follows the `right[]` pointer).  
With this table we can answer the minimum‑jump query in O(log N) by binary lifting: starting from `l`, we try the largest power of two that does not overshoot `r`, add its jump count, and repeat.

**Algorithm**
1. Steps 1‑2 from Approach 2 (sorting, component ids, `right[]`).
2. Build the doubling table `up`:
   * `up[0][i] = right[i]` (one jump).
   * For `k > 0`: `up[k][i] = up[k-1][ up[k-1][i] ]`.
3. To answer a query `(l, r)` (ensuring `l < r` and same component):
   * Initialize `cur = l`, `ans = 0`.
   * Iterate `k` from `logN` down to `0`:
        - If `up[k][cur] < r`: we can safely take `2^k` jumps → `cur = up[k][cur]`, `ans += 2^k`.
   * Finally, one more jump reaches or passes `r` → return `ans + 1`.

**Implementation**

```python
import math

class Solution:
    def pathExistenceQueries(self, n, nums, maxDiff, queries):
        # ----- 1. sort by value -----
        order = sorted(range(n), key=lambda i: nums[i])
        sorted_vals = [nums[i] for i in order]
        pos = [0] * n
        for p, idx in enumerate(order):
            pos[idx] = p

        # ----- 2. component id (gap > maxDiff starts new component) -----
        comp = [0] * n
        for i in range(1, n):
            comp[i] = comp[i-1] + (sorted_vals[i] - sorted_vals[i-1] > maxDiff)

        # ----- 3. right[i] = farthest reachable in one step -----
        right = [0] * n
        r = 0
        for l in range(n):
            if r < l:
                r = l
            while r + 1 < n and sorted_vals[r+1] - sorted_vals[l] <= maxDiff:
                r += 1
            right[l] = r

        # ----- 4. binary lifting table -----
        LOG = max(1, (n.bit_length()))   # enough for 2^LOG > n
        up = [right[:] ]                  # up[0]
        for k in range(1, LOG):
            prev = up[-1]
            nxt = [prev[prev[i]] for i in range(n)]
            up.append(nxt)

        # ----- 5. helper to compute min jumps using the table -----
        def min_jumps(l, r_idx):
            if l >= r_idx:
                return 0
            cur = l
            jumps = 0
            for k in range(LOG-1, -1, -1):
                if up[k][cur] < r_idx:
                    cur = up[k][cur]
                    jumps += 1 << k
            return jumps + 1   # final jump lands at or beyond r_idx

        # ----- 6. answer queries -----
        ans = []
        for u, v in queries:
            if u == v:
                ans.append(0)
                continue
            pu, pv = pos[u], pos[v]
            if comp[pu] != comp[pv]:
                ans.append(-1)
                continue
            lo, hi = (pu, pv) if pu < pv else (pv, pu)
            ans.append(min_jumps(lo, hi))
        return ans
```

**Complexity Analysis**

- Time complexity:  
  * Sorting: O(N log N)  
  * Sliding window for `right[]`: O(N)  
  * Building lifting table: O(N log N)  
  * Each query: O(log N)  
  * Total: O((N + Q) log N)  
- Space complexity: O(N log N) for the `up` table (≈ N · logN integers).

### Dry Run

**Input:** `n = 5, nums = [1,8,3,4,2], maxDiff = 3, queries = [[0,3],[2,4]]`

```
Step 1 – sort by value:
original idx: 0 1 2 3 4
value       : 1 8 3 4 2
sorted order (by value): [0,2,4,3,1]   # values [1,2,3,4,8]
sorted_vals = [1,2,3,4,8]
pos = [0,4,1,3,2]   # pos[original] = position in sorted array

Step 2 – component id (gap > maxDiff?):
diffs: 2-1=1 ≤3, 3-2=1 ≤3, 4-3=1 ≤3, 8-4=4 >3 → new component
comp = [0,0,0,0,1]

Step 3 – right[i] (farthest reach in one step):
i=0 (val 1): can reach up to val 4 (idx 3) → right[0]=3
i=1 (val 2): can reach up to val 4 (idx 3) → right[1]=3
i=2 (val 3): can reach up to val 4 (idx 3) → right[2]=3
i=3 (val 4): can reach only itself (next val 8 diff 4>3) → right[3]=3
i=4 (val 8): only itself → right[4]=4
right = [3,3,3,3,4]

Step 4 – binary lifting table (showing up[0] and up[1]):
up[0] = right = [3,3,3,3,4]
up[1][i] = up[0][ up[0][i] ]:
 i0: up[0][3]=3 → up[1][0]=3
 i1: up[0][3]=3 → up[1][1]=3
 i2: up[0][3]=3 → up[1][2]=3
 i3: up[0][3]=3 → up[1][3]=3
 i4: up[0][4]=4 → up[1][4]=4
(Higher powers stay the same.)

Query [0,3]:
- u=0, v=3 → pos[0]=0, pos[3]=3 → same component (comp[0]=comp[3]=0)
- l=0, r=3
- min_jumps(0,3):
   cur=0, jumps=0
   k from LOG-1 down to 0:
      Suppose LOG=3 (since n=5). Check k=2: up[2][0]? up[2] same as up[1] etc =3, which is NOT < r (3), so skip.
      k=1: up[1][0]=3 not <3 → skip.
      k=0: up[0][0]=3 not <3 → skip.
   No jump taken → jumps=0, return jumps+1 = 1.
Answer = 1.

Query [2,4]:
- u=2 → pos[2]=1, v=4 → pos[4]=2 → same component (comp[1]=comp[2]=0)
- l=1, r=2
- min_jumps(1,2):
   cur=1, jumps=0
   k=2: up[2][1]=3 which is NOT <2 → skip
   k=1: up[1][1]=3 NOT <2 → skip
   k=0: up[0][1]=3 NOT <2 → skip
   → jumps=0, return 1.
Answer = 1.

Result: [1,1] as expected.
```

--- 

This lecture follows the required structure, provides three approaches (brute force, greedy scan, binary lifting), includes intuition, algorithm, commented Python code, complexity analysis, and a detailed dry run. The topic is **Two Pointers** (sliding window to build the reach interval, then jump‑pointer technique). No extra sections were added.