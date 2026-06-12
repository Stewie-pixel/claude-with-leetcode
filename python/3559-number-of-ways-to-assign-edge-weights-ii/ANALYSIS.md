# Lowest Common Ancestor (Binary Lifting)

## Video Solution

For more details about **Number of Ways to Assign Edge Weights II**, watch the walkthrough at [https://www.youtube.com/watch?v=GWFU72pPqZY](https://www.youtube.com/watch?v=GWFU72pPqZY)

## Concept

In a tree, the unique path between two nodes `u` and `v` goes up from `u` to their lowest common ancestor (LCA) and then down to `v`.  
If we know the depth of each node and can jump upwards in powers of two, we can find the LCA in logarithmic time.  
Once we have the LCA, the number of edges on the `u‑v` path is  

```
distance(u, v) = depth[u] + depth[v] - 2 * depth[lca]
```

The original problem reduces to: for a path of length `L`, how many assignments of weights {1,2} give an odd sum?  
An odd sum occurs exactly when an **odd number of edges receive weight 1**.  
Among `L` independent binary choices, half of the `2^L` assignments have odd parity → `2^(L-1)`.

Thus the core sub‑problem is answering many LCA queries fast on a static tree.

## When to Use It

Use binary‑lifting LCA when you see:

- A rooted (or can be rooted) tree with up to 10⁵ nodes.
- Many queries (≥10⁵) that ask for a property of the path between two nodes (distance, sum, min/max, etc.).
- The tree does not change between queries (static).
- You need `O(log N)` per query after `O(N log N)` preprocessing.

Typical clues: “path between two nodes”, “distance in a tree”, “lowest common ancestor”, “ancestor jumps”.

## Template

```python
MOD = 10**9 + 7

class LCABinaryLifting:
    def __init__(self, n, edges, root=1):
        self.n = n
        self.LOG = (n).bit_length()          # enough to cover 2^LOG > n
        self.adj = [[] for _ in range(n + 1)]
        for u, v in edges:
            self.adj[u].append(v)
            self.adj[v].append(u)

        self.depth = [0] * (n + 1)
        self.up = [[0] * self.LOG for _ in range(n + 1)]
        self._dfs(root, 0)

        # binary lifting table
        for j in range(1, self.LOG):
            for v in range(1, n + 1):
                self.up[v][j] = self.up[self.up[v][j-1]][j-1]

    def _dfs(self, v, p):
        self.up[v][0] = p
        for to in self.adj[v]:
            if to == p:
                continue
            self.depth[to] = self.depth[v] + 1
            self._dfs(to, v)

    def lca(self, u, v):
        if self.depth[u] < self.depth[v]:
            u, v = v, u
        # lift u to same depth as v
        diff = self.depth[u] - self.depth[v]
        for j in range(self.LOG):
            if diff >> j & 1:
                u = self.up[u][j]
        if u == v:
            return u
        # lift both while ancestors differ
        for j in range(self.LOG - 1, -1, -1):
            if self.up[u][j] != self.up[v][j]:
                u = self.up[u][j]
                v = self.up[v][j]
        return self.up[u][0]

    def distance(self, u, v):
        w = self.lca(u, v)
        return self.depth[u] + self.depth[v] - 2 * self.depth[w]

def solve(edges, queries):
    n = len(edges) + 1
    lca_helper = LCABinaryLifting(n, edges)
    # pre‑compute powers of 2 for 2^(L-1)
    pow2 = [1] * (n + 1)
    for i in range(1, n + 1):
        pow2[i] = (pow2[i-1] * 2) % MOD

    ans = []
    for u, v in queries:
        if u == v:
            ans.append(0)
        else:
            L = lca_helper.distance(u, v)
            ans.append(pow2[L-1])          # 2^(L-1) mod MOD
    return ans
```

## LeetCode Problem Walkthrough

### Problem: 3559. Number of Ways to Assign Edge Weights II
https://leetcode.com/problems/number-of-ways-to-assign-edge-weights-ii/

### Approach 1: Brute Force (Per‑query BFS/DFS)

**Algorithm**
1. For each query `(u, v)`:
   - Run a BFS/DFS from `u` to find `v` and record the number of edges traversed → path length `L`.
   - If `u == v`, answer is `0`.
   - Otherwise answer = `2^(L-1) mod MOD` (using fast exponentiation or a pre‑computed table).
2. Return the list of answers.

**Implementation**

```python
from collections import deque

class Solution:
    def assignEdgeWeights(self, edges, queries):
        MOD = 10**9 + 7
        n = len(edges) + 1
        adj = [[] for _ in range(n + 1)]
        for u, v in edges:
            adj[u].append(v)
            adj[v].append(u)

        def bfs_length(start, target):
            if start == target:
                return 0
            q = deque([(start, 0, -1)])   # node, distance, parent
            while q:
                node, dist, parent = q.popleft()
                if node == target:
                    return dist
                for nb in adj[node]:
                    if nb != parent:
                        q.append((nb, dist + 1, node))
            return 0   # tree guarantees reachability

        ans = []
        for u, v in queries:
            L = bfs_length(u, v)
            if L == 0:
                ans.append(0)
            else:
                ans.append(pow(2, L-1, MOD))
        return ans
```

**Complexity Analysis**
- Time: `O(Q * N)` – each BFS may visit all `N` nodes.
- Space: `O(N)` for the adjacency list and BFS queue.

### Approach 2: Pre‑compute Depths + Naive LCA (Climb by Parents)

**Intuition**
If we know each node’s depth and its immediate parent, we can obtain the LCA by repeatedly lifting the deeper node until both nodes are at the same depth, then moving both up together until they meet.  
This avoids a full BFS per query; we only walk up the tree, which in the worst case is `O(tree height)`.  
For a balanced tree the height is `O(log N)`, but for a line it degrades to `O(N)`.  
Still, it is much faster than the brute force when many queries are present.

**Algorithm**
1. One DFS from the root to fill `depth[]` and `parent[]` (the immediate ancestor).
2. For each query `(u, v)`:
   - If `u == v` → answer `0`.
   - Lift the deeper node using `parent[]` until depths equal.
   - Then lift both nodes together until they are equal → that node is the LCA.
   - Compute `L = depth[u] + depth[v] - 2*depth[lca]`.
   - Answer = `2^(L-1) mod MOD`.

**Implementation**

```python
class Solution:
    def assignEdgeWeights(self, edges, queries):
        MOD = 10**9 + 7
        n = len(edges) + 1
        adj = [[] for _ in range(n + 1)]
        for u, v in edges:
            adj[u].append(v)
            adj[v].append(u)

        depth = [0] * (n + 1)
        parent = [0] * (n + 1)

        def dfs(v, p):
            parent[v] = p
            for to in adj[v]:
                if to == p:
                    continue
                depth[to] = depth[v] + 1
                dfs(to, v)

        dfs(1, 0)

        def lca(u, v):
            if depth[u] < depth[v]:
                u, v = v, u
            # raise u
            while depth[u] > depth[v]:
                u = parent[u]
            # raise both
            while u != v:
                u = parent[u]
                v = parent[v]
            return u

        # pre‑compute powers of 2 up to n
        pow2 = [1] * (n + 1)
        for i in range(1, n + 1):
            pow2[i] = (pow2[i-1] * 2) % MOD

        ans = []
        for u, v in queries:
            if u == v:
                ans.append(0)
                continue
            w = lca(u, v)
            L = depth[u] + depth[v] - 2 * depth[w]
            ans.append(pow2[L-1])
        return ans
```

**Complexity Analysis**
- Time: `O(N + Q * H)` where `H` is the tree height (worst‑case `O(N)`, average `O(log N)` for random trees).
- Space: `O(N)` for adjacency list, depth, parent.

### Approach 3: Binary Lifting LCA (Most Optimal)

**Intuition**
The naive climbing can be sped up by jumping in powers of two, just like binary search on a sorted array.  
We pre‑compute `up[v][j]` = the 2ʲ‑th ancestor of `v`.  
To raise a node by `k` steps we decompose `k` into binary and use the table.  
Finding the LCA then becomes:
1. Equalize depths using the binary lifting table.
2. From the highest power downwards, jump both nodes when their 2ʲ‑th ancestors differ.
3. The immediate parent of the resulting nodes is the LCA.  
Each step uses at most `LOG = ⌈log₂ N⌉` jumps, giving `O(log N)` per query after `O(N log N)` preprocessing.

**Algorithm**
1. Build adjacency list.
2. DFS to compute `depth[]` and `up[v][0]` (direct parent).
3. Fill the binary lifting table `up[v][j]` for `j > 0`.
4. For each query:
   - If `u == v` → `0`.
   - Compute LCA via the table.
   - Path length `L = depth[u] + depth[v] - 2*depth[lca]`.
   - Answer = `2^(L-1) mod MOD`.
5. Return answers.

**Implementation** (the solution you submitted – reproduced here for completeness)

```python
from collections import deque

class Solution(object):
    def assignEdgeWeights(self, edges, queries):
        MOD = 10**9 + 7
        n = len(edges) + 1
        
        adj = [[] for _ in range(n + 1)]
        for u, v in edges:
            adj[u].append(v)
            adj[v].append(u)
            
        depth = [-1] * (n + 1)
        LOG = (n).bit_length()          # enough for 2^LOG > n
        up = [[0] * LOG for _ in range(n + 1)]
        
        # BFS to set depth and immediate parents
        queue = deque([1])
        depth[1] = 0
        while queue:
            curr = queue.popleft()
            for neighbor in adj[curr]:
                if depth[neighbor] == -1:
                    depth[neighbor] = depth[curr] + 1
                    up[neighbor][0] = curr
                    queue.append(neighbor)
                    
        # binary lifting table
        for j in range(1, LOG):
            for i in range(1, n + 1):
                up[i][j] = up[up[i][j-1]][j-1]
                
        def get_lca(u, v):
            if depth[u] < depth[v]:
                u, v = v, u
                
            # lift u to depth of v
            diff = depth[u] - depth[v]
            for j in range(LOG):
                if (diff >> j) & 1:
                    u = up[u][j]
                    
            if u == v:
                return u
                
            # lift both while ancestors differ
            for j in range(LOG - 1, -1, -1):
                if up[u][j] != up[v][j]:
                    u = up[u][j]
                    v = up[v][j]
                    
            return up[u][0]
            
        # pre‑compute powers of 2 for 2^(L-1)
        pow2 = [1] * (n + 1)
        for i in range(1, n + 1):
            pow2[i] = (pow2[i-1] * 2) % MOD
            
        answer = []
        for u, v in queries:
            if u == v:
                answer.append(0)
            else:
                lca = get_lca(u, v)
                L = depth[u] + depth[v] - 2 * depth[lca]
                answer.append(pow2[L - 1])
                
        return answer
```

**Complexity Analysis**
- Preprocessing:
  - DFS/BFS: `O(N)`
  - Building `up` table: `O(N log N)`
- Each query:
  - LCA: `O(log N)`
  - Path length & answer: `O(1)`
- Total time: `O((N + Q) log N)`
- Space: `O(N log N)` for the ancestor table plus `O(N)` for adjacency and depth.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the binary lifting LCA on the tree from Example 2.

```
Tree (root = 1):
    1
   / \
  2   3
     / \
    4   5
```

Depths: `depth[1]=0, depth[2]=1, depth[3]=1, depth[4]=2, depth[5]=2`.

Ancestor table (showing only 2⁰ and 2¹ for brevity):
```
up[v][0] = parent
up[v][1] = grandparent
```

#### Query: (2, 5)  → expected LCA = 1, path length = 3

| Step | Action                                   | u  | v  | depth[u] | depth[v] |
|------|------------------------------------------|----|----|----------|----------|
| 0    | start                                    | 2  | 5  | 1        | 2        |
| 1    | depth[v] > depth[u] → swap (u,v)         | 5  | 2  | 2        | 1        |
| 2    | lift u by diff=1 (binary 001) → j=0      | up[5][0]=3 | 2 | 1 | 1 |
| 3    | now depths equal (both 1)                | 3  | 2  | 1        | 1        |
| 4    | highest j where ancestors differ: j=0    | up[3][0]=1 | up[2][0]=1 → equal, so we skip lifting |
| 5    | after loop, u and v are different; LCA = up[u][0] = up[3][0] = 1 | 1 | 1 | 0 | 0 |

LCA = 1.  
Path length `L = depth[2] + depth[5] - 2*depth[1] = 1 + 2 - 0 = 3`.  
Answer = `2^(3-1) = 4` (matches example).

#### Query: (1, 4) → LCA = 1, L = 2 → answer = 2

| Step | Action               | u | v | depth[u] | depth[v] |
|------|----------------------|---|---|----------|----------|
| 0    | start                | 1 | 4 | 0        | 2        |
| 1    | lift v by diff=2 (10b) → j=1: up[4][1]=up[ up[4][0] ][0] = up[3][0]=1 | 1 | 1 | 0 | 0 |
| 2    | depths equal, u==v → LCA = 1 |   |   |          |          |

LCA = 1, `L = 0 + 2 - 0 = 2`, answer = `2^(2-1) = 2`.

These traces show how binary lifting lets us jump up the tree in logarithmic steps instead of walking edge‑by‑edge.

--- 

**Summary**  
- Identify the core sub‑problem: distance between two nodes in a tree.  
- Use binary lifting to preprocess ancestors in `O(N log N)`.  
- Answer each query in `O(log N)` → overall `O((N+Q) log N)`.  
- Convert distance `L` to number of odd‑weight assignments: `2^(L-1) mod MOD`.  

This pattern (LCA via binary lifting) is reusable for any static‑tree path query (sum, min/max, xor, etc.). Keep the template handy and adjust the per‑edge contribution logic to match the problem’s specific weight rule.