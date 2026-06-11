# Tree BFS

## Video Solution

For more details about **Number of Ways to Assign Edge Weights I**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=yTKP2itIBIU)

## Concept

Breadth‑First Search (BFS) explores a tree level by level. Starting from the root, we visit all nodes at distance 1, then distance 2, and so on. The depth of a node is the number of edges on the path from the root to that node. By keeping track of the current depth while we push children into the queue, we can compute the **maximum depth** of the tree in a single pass.

When the problem only cares about the path from the root to *any* deepest node, we first find that maximum depth `L`. The actual edges not on that path can be ignored, so the task reduces to: *given a path of `L` edges, each edge can be weighted 1 (odd) or 2 (even), in how many ways does the total sum become odd?*

## When to Use It

Use BFS on a tree when you see:
- Need to find the **minimum number of edges** between two nodes (shortest path in an unweighted tree).
- Compute the **height / maximum depth** of a rooted tree.
- Process nodes **level by level** (e.g., printing each level, finding the deepest leaf).
- The problem statement mentions “rooted at node 1”, “maximum depth”, or “distance from root”.

## Template

```python
from collections import deque

def tree_bfs_max_depth(n, edges):
    """
    Returns the maximum depth (number of edges from root to deepest node)
    of a tree rooted at 1.
    """
    # build adjacency list
    adj = [[] for _ in range(n + 1)]
    for u, v in edges:
        adj[u].append(v)
        adj[v].append(u)

    q = deque([(1, 0)])          # (node, depth_from_root)
    visited = {1}
    max_depth = 0

    while q:
        node, depth = q.popleft()
        max_depth = max(max_depth, depth)

        for nb in adj[node]:
            if nb not in visited:
                visited.add(nb)
                q.append((nb, depth + 1))

    return max_depth
```

## LeetCode Problem Walkthrough

### Problem: 3558. Number of Ways to Assign Edge Weights I

https://leetcode.com/problems/number-of-ways-to-assign-edge-weights-i/

---

### Approach 1: Brute Force

**Algorithm**
1. Build the adjacency list and run BFS to obtain the length `L` of the root‑to‑deepest path.
2. Generate every possible assignment of weights `1` or `2` to the `L` edges (there are `2^L` possibilities).
3. For each assignment, compute the sum of the chosen weights; increment a counter if the sum is odd.
4. Return the counter modulo `10^9+7`.

**Implementation**

```python
from collections import deque

class Solution:
    def assignEdgeWeights(self, edges):
        n = len(edges) + 1
        MOD = 10**9 + 7
        adj = [[] for _ in range(n + 1)]
        for u, v in edges:
            adj[u].append(v)
            adj[v].append(u)

        # BFS to find max depth L
        q = deque([(1, 0)])
        visited = {1}
        max_depth = 0
        while q:
            node, depth = q.popleft()
            max_depth = max(max_depth, depth)
            for nb in adj[node]:
                if nb not in visited:
                    visited.add(nb)
                    q.append((nb, depth + 1))

        L = max_depth
        # brute force over all 2^L assignments
        ans = 0
        for mask in range(1 << L):          # each bit: 0 -> weight 2 (even), 1 -> weight 1 (odd)
            total = 0
            for i in range(L):
                if mask >> i & 1:
                    total += 1              # weight 1 (odd)
                else:
                    total += 2              # weight 2 (even)
            if total % 2 == 1:
                ans += 1
        return ans % MOD
```

**Complexity Analysis**
- Time complexity: `O(2^L * L)` – we enumerate all assignments and sum `L` bits each.
- Space complexity: `O(N)` for the adjacency list and BFS queue (ignoring the exponential output).

*Why it’s infeasible*: `L` can be up to `10^5`, making `2^L` astronomically large.

---

### Approach 2: Parity DP (Dynamic Programming on Parity)

**Intuition**
Only the **parity** (odd/even) of each edge weight matters for the final sum’s parity.
- Weight 1 contributes `1` (odd) → parity 1.
- Weight 2 contributes `0` (even) → parity 0.
Thus each edge is a binary choice: add `1` or add `0` modulo 2.
We need the number of binary strings of length `L` whose sum modulo 2 equals 1.

Let `dp[i][p]` = number of ways to assign weights to the first `i` edges so that the parity of the partial sum is `p` (`p` ∈ {0, 1}).  
Transition:  
`dp[i+1][p]   += dp[i][p]`   (choose weight 2 → adds 0)  
`dp[i+1][p^1] += dp[i][p]`   (choose weight 1 → flips parity)

Starting with `dp[0][0] = 1` (empty prefix has even sum). After processing all `L` edges, answer = `dp[L][1]`.

Because the transition is symmetric, we can solve it in closed form: exactly half of the `2^L` strings have odd parity → `2^{L-1}`.

**Implementation**

```python
from collections import deque

class Solution:
    def assignEdgeWeights(self, edges):
        n = len(edges) + 1
        MOD = 10**9 + 7
        adj = [[] for _ in range(n + 1)]
        for u, v in edges:
            adj[u].append(v)
            adj[v].append(u)

        # BFS to obtain max depth L
        q = deque([(1, 0)])
        visited = {1}
        max_depth = 0
        while q:
            node, depth = q.popleft()
            max_depth = max(max_depth, depth)
            for nb in adj[node]:
                if nb not in visited:
                    visited.add(nb)
                    q.append((nb, depth + 1))

        L = max_depth
        # Number of ways to get odd sum = 2^{L-1}
        return pow(2, L - 1, MOD)
```

**Complexity Analysis**
- Time complexity: `O(N)` – building the adjacency list + BFS traversal.
- Space complexity: `O(N)` – adjacency list and visited set/queue.

*Why it works*: The DP counts exactly the number of binary strings with odd parity, which simplifies to `2^{L-1}`.

---

### Approach 3: Direct Combinatorial Formula

**Intuition**
Each edge independently chooses weight 1 (odd) or weight 2 (even).  
The total sum is odd **iff** an odd number of edges receive weight 1.  
Out of `2^L` total assignments, exactly half have an odd number of 1’s (by symmetry of the binomial distribution).  
Therefore the answer is `2^{L-1}`.

We can also see it via the binomial theorem:  
`#ways = Σ_{k odd} C(L, k) = 2^{L-1}`.

**Implementation** (same as Approach 2, but we skip the DP explanation)

```python
from collections import deque

class Solution:
    def assignEdgeWeights(self, edges):
        n = len(edges) + 1
        MOD = 10**9 + 7
        adj = [[] for _ in range(n + 1)]
        for u, v in edges:
            adj[u].append(v)
            adj[v].append(u)

        q = deque([(1, 0)])
        visited = {1}
        max_depth = 0
        while q:
            node, depth = q.popleft()
            max_depth = max(max_depth, depth)
            for nb in adj[node]:
                if nb not in visited:
                    visited.add(nb)
                    q.append((nb, depth + 1))

        L = max_depth
        return pow(2, L - 1, MOD)
```

**Complexity Analysis**
- Time complexity: `O(N)` – single BFS pass.
- Space complexity: `O(N)` – adjacency list + queue.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the algorithm on Example 2.

**Input:** `edges = [[1,2],[1,3],[3,4],[3,5]]`  
The tree (root =���1):

```
    1
   / \
  2   3
     / \
    4   5
```

#### BFS to find max depth

| Step | Node popped | Depth | Neighbors enqueued (depth+1) | Visited set                | Max depth so far |
|------|-------------|-------|------------------------------|----------------------------|------------------|
| 1    | 1           | 0     | 2 (1), 3 (1)                 | {1,2,3}                    | 0 |
| 2    | 2           | 1     | –                            | {1,2,3}                    | 1 |
| 3    | 3           | 1     | 4 (2), 5 (2)                 | {1,2,3,4,5}                | 1 |
| 4    | 4           | 2     | –                            | {1,2,3,4,5}                | 2 |
| 5    | 5           | 2     | –                            | {1,2,3,4,5}                | 2 |

Maximum depth `L = 2`.

#### Counting assignments

We need the number of ways to assign weights to 2 edges so the sum is odd.

List all 2² = 4 possibilities (edge 1, edge 2):

| Assignment (w1,w2) | Sum | Odd? |
|--------------------|-----|------|
| (1,1)              | 2   | No   |
| (1,2)              | 3   | Yes  |
| (2,1)              | 3   | Yes  |
| (2,2)              | 4   | No   |

Exactly 2 assignments give an odd sum → answer `2`.

Our formula: `2^{L-1} = 2^{2-1} = 2` ✅

--- 

**End of lecture**.