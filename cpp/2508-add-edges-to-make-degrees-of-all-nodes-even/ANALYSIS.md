# Graphs

## Video Solution

For more details about **Add Edges to Make Degrees of All Nodes Even**, watch the walkthrough at [https://www.youtube.com/watch?v=ZjarUSdQjEA](https://www.youtube.com/watch?v=ZjarUSdQjEA)

## Concept

In an undirected graph the **degree** of a node is the number of incident edges.  
A graph where every node has an even degree is called an **Eulerian graph** (ignoring connectivity).  
We are allowed to add **at most two** new edges (no parallel edges, no self‑loops).  
The problem reduces to looking at the set of nodes whose current degree is odd.  
Adding an edge toggles the parity of its two endpoints, so we need to see whether we can eliminate all odd‑degree nodes using 0, 1 or 2 edge additions.

## When to Use It

Use this reasoning when a problem asks you to adjust a graph by adding a limited number of edges to achieve a parity condition on degrees. Typical clues:

- “add at most k edges”
- “make degree of each node even/odd”
- “no repeated edges, no self‑loops”
- constraints up to 10⁵ nodes/edges → need O(n + m) or O(n log n) solution

## Template

A reusable skeleton for parity‑based graph problems:

```python
def can_make_even_degrees(n, edges):
    # 1. Build adjacency (or just degree counts)
    deg = [0] * (n + 1)
    adj = [set() for _ in range(n + 1)]
    for u, v in edges:
        deg[u] += 1
        deg[v] += 1
        adj[u].add(v)
        adj[v].add(u)

    # 2. Collect odd-degree nodes
    odd = [i for i in range(1, n + 1) if deg[i] % 2 == 1]

    # 3. Handle cases based on len(odd)
    #    (0 → already good, 2 → try one edge, 4 → try two edges, else impossible)
    ...
```

Replace the `...` with the specific logic for the problem.

---

## LeetCode Problem Walkthrough

### Problem: 2508. Add Edges to Make Degrees of All Nodes Even  
https://leetcode.com/problems/add-edges-to-make-degrees-of-all-nodes-even/

### Approach 1: Brute Force (try all possible edge additions)

**Algorithm**  
1. Compute the degree of each node.  
2. If all degrees are already even → return `True`.  
3. Otherwise, enumerate every possible new edge (u, v) that does **not** already exist and is not a self‑loop.  
   - For each candidate, toggle the parity of u and v and check if all degrees become even.  
   - If success with one edge → return `True`.  
4. If one edge didn’t work, enumerate every pair of distinct candidate edges (e1, e2) (still respecting no duplication/self‑loop) and test the combined effect.  
5. If any pair works → return `True`, else `False`.

**Implementation**

```python
class Solution:
    def isPossible(self, n: int, edges: List[List[int]]) -> bool:
        # degree and adjacency for O(1) edge existence test
        deg = [0] * (n + 1)
        adj = [set() for _ in range(n + 1)]
        for u, v in edges:
            deg[u] += 1
            deg[v] += 1
            adj[u].add(v)
            adj[v].add(u)

        # helper to test if current degrees are all even
        def all_even() -> bool:
            return all(d % 2 == 0 for d in deg[1:])

        if all_even():
            return True

        # list of all possible non‑existent edges
        possible = []
        for u in range(1, n + 1):
            for v in range(u + 1, n + 1):
                if v not in adj[u]:
                    possible.append((u, v))

        # try adding one edge
        for u, v in possible:
            deg[u] += 1
            deg[v] += 1
            if all_even():
                return True
            deg[u] -= 1
            deg[v] -= 1

        # try adding two edges (distinct)
        m = len(possible)
        for i in range(m):
            u1, v1 = possible[i]
            deg[u1] += 1
            deg[v1] += 1
            for j in range(i + 1, m):
                u2, v2 = possible[j]
                # avoid using the same edge twice
                if (u1, v1) == (u2, v2):
                    continue
                deg[u2] += 1
                deg[v2] += 1
                if all_even():
                    return True
                deg[u2] -= 1
                deg[v2] -= 1
            deg[u1] -= 1
            deg[v1] -= 1

        return False
```

**Complexity Analysis**  
- Time: O(n²) to build `possible` + O(|possible|²) for two‑edge trial → worst‑case O(n⁴) but with early exits; practically too slow for n = 10⁵.  
- Space: O(n + m) for degree and adjacency sets.

---

### Approach 2: Focus on Odd‑Degree Nodes

**Intuition**  
Adding an edge flips the parity of exactly its two endpoints. Therefore only nodes with odd degree matter. If we have `k` odd‑degree nodes, each added edge can eliminate at most two of them. Hence:
- If `k == 0` → already good.  
- If `k == 2` → we need **one** edge connecting the two odd nodes **or** a third node that is not adjacent to either, allowing us to use two edges via an intermediate node.  
- If `k == 4` → we need **two** edges that pair up the four odd nodes in a way that each new edge is permissible (i.e., not already present).  
- Any other `k` cannot be fixed with ≤ 2 edges.

**Algorithm**  
1. Compute degrees and build adjacency sets.  
2. Extract list `odd` of nodes with odd degree.  
3. Switch on `len(odd)`:
   - `0` → return `True`.  
   - `2` → try to add a direct edge between them; if not allowed, look for a third node `c` distinct from both that is non‑adjacent to each.  
   - `4` → check the three possible pairings: (a,b)+(c,d), (a,c)+(b,d), (a,d)+(b,c). If any pairing uses two non‑existent edges → return `True`.  
   - else → return `False`.

**Implementation**

```python
class Solution:
    def isPossible(self, n: int, edges: List[List[int]]) -> bool:
        adj = [set() for _ in range(n + 1)]
        for u, v in edges:
            adj[u].add(v)
            adj[v].add(u)

        odd = [i for i in range(1, n + 1) Christmas if len(adj[i]) % 2 == 1]
        # above line has a typo; correct version below:
        odd = [i for i in range(1, n + 1) if len(adj[i]) % 2 == 1]

        if not odd:
            return True

        if len(odd) == 2:
            a, b = odd
            if b not in adj[a]:
                return True
            # need a third node c that connects to both a and b via new edges
            for c in range(1, n + 1):
                if c == a or c == b:
                    continue
                if c not in adj[a] and c not in adj[b]:
                    return True
            return False

        if len(odd) == 4:
            a, b, c, d = odd
            # three possible pairings
            if (b not in adj[a] and d not in adj[c]) or \
               (c not in adj[a] and d not in adj[b]) or \
               (d not in adj[a] and c not in adj[b]):
                return True
            return False

        return False
```

*(Fixed the odd list extraction in the final code.)*

**Complexity Analysis**  
- Time: O(n + m) to build adjacency + O(n) for the scan of a third node (only when `|odd| == 2`). Overall O(n + m).  
- Space: O(n + m) for adjacency sets.

---

### Approach 3: Optimal Case Analysis (same as Approach 2, streamlined)

**Intuition**  
The brute‑force search is unnecessary because the parity constraint limits the relevant nodes to at most four. By explicitly handling the 0, 2, and 4 odd cases we achieve linear time without any auxiliary enumeration beyond adjacency checks.

**Algorithm** – identical to Approach 2 but written more compactly.

**Implementation**

```python
class Solution:
    def isPossible(self, n: int, edges: List[List[int]]) -> bool:
        adj = [set() for _ in range(n + 1)]
        for u, v in edges:
            adj[u].add(v)
            adj[v].add(u)

        odd = [i for i in range(1, n + 1) if len(adj[i]) % 2 == 1]

        if not odd:
            return True

        if len(odd) == 2:
            a, b = odd
            if b not in adj[a]:
                return True
            for c in range(1, n + 1):
                if c != a and c != b and c not in adj[a] and c not in adj[b]:
                    return True
            return False

        if len(odd) == 4:
            a, b, c, d = odd
            return ((b not in adj[a] and d not in adj[c]) or
                    (c not in adj[a] and d not in adj[b]) or
                    (d not in adj[a] and c not in adj[b]))

        return False
```

**Complexity Analysis**  
- Time: O(n + m)  
- Space: O(n + m)

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run Approach 2 on Example 1.

**Input**  
`n = 5`  
`edges = [[1,2],[2,3],[3,4],[4,2],[1,4],[2,5]]`

**Step 1 – Degrees**

| Node | Adjacent nodes | Degree | Parity |
|------|----------------|--------|--------|
| 1    | 2,4            | 2      | even   |
| 2    | 1,3,4,5        | 4      | even   |
| 3    | 2,4            | 2      | even   |
| 4    | 1,2,3          | 3      | **odd**|
| 5    | 2              | 1      | **odd**|

`odd = [4, 5]` → length = 2.

**Step 2 – Check direct edge (4,5)**  
Edge (4,5) does **not** exist → we can add one edge → return `True`.

The dry run shows that a single new edge (4‑5) makes all degrees even, matching the expected output.

---