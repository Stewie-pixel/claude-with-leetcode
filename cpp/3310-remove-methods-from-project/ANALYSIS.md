# Graphs

## Video Solution

For more details about **Remove Methods From Project**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=P7Txr3Y4GSQ).

## Concept

A directed graph models method invocations: an edge `a → b` means method `a` calls method `b`.  
Starting from the buggy method `k`, we can follow outgoing edges to find every method that is reachable—these are the *suspicious* methods.  
The removal rule states that we may delete the whole suspicious set **only if** no method outside the set has an edge pointing into it. In graph terms, we need to check whether there exists any incoming edge from a non‑suspicious node to a suspicious node. If such an edge exists, removal is impossible and we return all methods; otherwise we return the non‑suspicious nodes.

## When to Use It

Use graph traversal (DFS/BFS) when the problem describes:

- Relationships defined as “A invokes/calls/triggers B”.
- Need to find all nodes reachable from a source.
- Need to verify whether any edge crosses a boundary (outside → inside).
- Constraints up to 10⁵ nodes and 2·10⁵ edges → O(N+E) solutions are required.

## Template

```python
# adjacency list for a directed graph
graph = [[] for _ in range(n)]
for u, v in edges:
    graph[u].append(v)

# iterative DFS (or BFS) to mark reachable nodes from start
def reachable_from(start):
    visited = [False] * n
    stack = [start]
    visited[start] = True
    while stack:
        u = stack.pop()
        for v in graph[u]:
            if not visited[v]:
                visited[v] = True
                stack.append(v)
    return visited
```

---

## LeetCode Problem Walkthrough

### Problem: 3310. Remove Methods From Project

https://leetcode.com/problems/remove-methods-from-project/

### Approach 1: Brute Force (Repeated DFS)

**Algorithm**
1. For each method `i` (0 … n‑1) run a DFS/BFS from `k` to see if `i` is reachable.  
   - This yields the suspicious set but repeats the traversal for every node.  
2. After we know which nodes are suspicious, scan all edges: if an edge `a → b` has `a` non‑suspicious and `b` suspicious, abort and return all methods.  
3. Otherwise, return the list of non‑suspicious methods.

**Implementation**

```python
class Solution:
    def remainingMethods(self, n: int, k: int, invocations: List[List[int]]) -> List[int]:
        # build graph
        g = [[] for _ in range(n)]
        for a, b in invocations:
            g[a].append(b)

        # helper: iterative DFS from k
        def dfs_marked():
            marked = [False] * n
            stack = [k]
            marked[k] = True
            while stack:
                u = stack.pop()
                for v in g[u]:
                    if not marked[v]:
                        marked[v] = True
                        stack.append(v)
            return marked

        # brute force: recompute the mark for every node (wasteful)
        suspicious = [False] * n
        for i in range(n):
            # we only need to know if i is reachable from k;
            # running a full DFS each time is O(N*(N+E))
            marked = dfs_marked()
            suspicious[i] = marked[i]

        # check for external incoming edge
        for a, b in invocations:
            if not suspicious[a] and suspicious[b]:
                return list(range(n))   # cannot remove anything

        return [i for i in range(n) if not suspicious[i]]
```

**Complexity Analysis**

- Time complexity: O(N·(N+E)) — we run a DFS from `k` for each of the `N` nodes.  
- Space complexity: O(N+E) — graph storage plus O(N) for the visited array per DFS (reused).

---

### Approach 2: Single Traversal + Edge Scan (Standard)

**Intuition**
The suspicious set is simply the set of nodes reachable from `k`. Computing it once with a single DFS/BFS gives us the answer in linear time. After we have the boolean `suspicious` array, we only need one pass over the edge list to detect any forbidden incoming edge.

**Algorithm**
1. Build adjacency list.  
2. Run one iterative DFS/BFS from `k` and mark all visited nodes as suspicious.  
3. Iterate over every invocation `[a, b]`: if `suspicious[a]` is `False` and `suspicious[b]` is `True`, an outside method calls a suspicious one → return all methods.  
4. If no such edge exists, collect and return all indices where `suspicious[i]` is `False`.

**Implementation**

```python
class Solution:
    def remainingMethods(self, n: int, k: int, invocations: List[List[int]]) -> List[int]:
        # 1️⃣ build graph
        g = [[] for _ in range(n)]
        for a, b in invocations:
            g[a].append(b)

        # 2️⃣ mark reachable (suspicious) nodes from k
        suspicious = [False] * n
        stack = [k]
        suspicious[k] = True
        while stack:
            u = stack.pop()
            for v in g[u]:
                if not suspicious[v]:
                    suspicious[v] = True
                    stack.append(v)

        # 3️⃣ check for any edge from outside → inside
        for a, b in invocations:
            if not suspicious[a] and suspicious[b]:
                return list(range(n))   # removal not allowed

        # 4️⃣ return remaining methods
        return [i for i in range(n) if not suspicious[i]]
```

**Complexity Analysis**

- Time complexity: O(N+E) — one graph construction, one DFS/BFS, and one edge scan.  
- Space complexity: O(N+E) — adjacency list plus O(N) for the `suspicious` boolean array.

---

### Approach 3: Early‑Exit During Edge Scan (Micro‑Optimization)

**Intuition**
While we still need the full reachable set to know which nodes are suspicious, we can stop scanning edges as soon as we discover the first violating edge. This does not change asymptotic complexity but can save work on large inputs where the violation appears early.

**Algorithm**
Same as Approach 2, but step 3 breaks out of the loop on the first forbidden edge and returns immediately.

**Implementation**

```python
class Solution:
    def remainingMethods(self, n: int, k: int, invocations: List[List[int]]) -> List[int]:
        # build graph
        g = [[] for _ in range(n)]
        for a, b in invocations:
            g[a].append(b)

        # mark suspicious nodes via DFS from k
        suspicious = [False] * n
        stack = [k]
        suspicious[k] = True
        while stack:
            u = stack.pop()
            for v in g[u]:
                if not suspicious[v]:
                    suspicious[v] = True
                    stack.append(v)

        # scan edges, exit early on first outside→inside edge
        for a, b in invocations:
            if not suspicious[a] and suspicious[b]:
                return list(range(n))

        return [i for i in range(n) if not suspicious[i]]
```

**Complexity Analysis**

- Time complexity: O(N+E) in the worst case; often better because we may stop early.  
- Space complexity: O(N+E) — same as before.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the algorithm on Example 2:

```
n = 5, k = 0
invocations = [[1,2],[0,2],[0,1],[3,4]]
```

**Step 1 – Build graph**

```
0 → [2, 1]
1 → [2]
2 → []
3 → [4]
4 → []
```

**Step 2 – DFS from k = 0 (mark suspicious)**

```
stack: [0]          visited[0]=True
pop 0 → push 2,1    visited[2]=True, visited[1]=True
stack: [2,1]
pop 1 → push 2 (already visited)
stack: [2]
pop 2 → no outgoing edges
stack: [] → done
suspicious = [True, True, True, False, False]
```

**Step 3 – Scan edges for outside→inside**

| Edge (a→b) | suspicious[a] | suspicious[b] | Verdict               |
|-----------|----------------|----------------|-----------------------|
| 1→2       | True           | True           | OK (inside→inside)    |
| 0→2       | True           | True           | OK                    |
| 0→1       | True           | True           | OK                    |
| 3→4       | False          | False          | OK (outside→outside) |

No edge from a non‑suspicious node to a suspicious node → removal allowed.

**Result:** return indices where suspicious is False → `[3, 4]`.

The table clearly shows why the algorithm returns `[3,4]`.  

--- 

*End of lecture.*