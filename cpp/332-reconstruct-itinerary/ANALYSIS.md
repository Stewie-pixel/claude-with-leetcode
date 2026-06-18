# Graphs

## Video Solution

For more details about **Reconstruct Itinerary**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=ZyB_gQ8vqGA).

## Concept

Reconstructing an itinerary that uses every ticket exactly once is the classic **Eulerian path** problem on a directed graph where each ticket is a directed edge.  
Because we must begin at `"JFK"` and, when multiple choices exist, pick the smallest lexical destination, we can greedily always take the smallest available next airport.  
Hierholzer’s algorithm builds the Eulerian path by repeatedly following unused edges until we get stuck, then back‑tracking and splicing in sub‑cycles.  
Using a **min‑heap (priority queue)** for each vertex’s adjacency list guarantees that we always pick the lexicographically smallest next flight, which yields the required smallest‑lexical‑order itinerary when the path is finally reversed.

## When to Use It

- The problem asks to use **all edges exactly once** (or all tickets once).  
- You are given a **start vertex** (`"JFK"`).  
- When multiple valid routes exist, you need the **lexicographically smallest** one.  
- The underlying structure is a **directed graph** (or multigraph, because parallel tickets are allowed).  

In short: *Think Eulerian path / Hierholzer’s algorithm with a priority queue for lexical ordering.*

## Template

```python
from collections import defaultdict
import heapq

def eulerian_path(start, edges):
    """
    edges: list of (u, v) pairs (directed, may contain duplicates)
    returns list of vertices in the Eulerian path starting at `start`
    """
    # build adjacency list with min‑heap
    graph = defaultdict(list)
    for u, v in edges:
        heapq.heappush(graph[u], v)

    path = []          # will contain the itinerary in reverse order
    stack = [start]    # explicit stack for iterative Hierholzer

    while stack:
        while graph[stack[-1]]:               # while there are unused outgoing edges
            nxt = heapq.heappop(graph[stack[-1]])
            stack.append(nxt)                 # follow the smallest lexical edge
        # no more edges from current vertex -> add to path and backtrack
        path.append(stack.pop())

    return path[::-1]   # reverse to get correct order
```

## LeetCode Problem Walkthrough

### Problem: 332. Reconstruct Itinerary

https://leetcode.com/problems/reconstruct-itinerary/

---

### Approach 1: Brute Force (Backtracking)

**Algorithm**  
1. Build a adjacency list (multiset) of destinations for each origin.  
2. Perform DFS from `"JFK"` trying every possible next airport in lexical order.  
3. Keep a path list; when the path length equals `len(tickets) + 1` we have used all tickets → return the path.  
4. Backtrack by removing the last choice and trying the next alternative.

**Implementation**

```python
from collections import defaultdict

class Solution:
    def findItinerary(self, tickets):
        graph = defaultdict(list)
        for u, v in tickets:
            graph[u].append(v)
        for u in graph:
            graph[u].sort()               # lexical order

        path = ["JFK"]
        used = defaultdict(lambda: defaultdict(int))  # count of used edges

        def dfs(curr):
            if len(path) == len(tickets) + 1:
                return True
            for i, nxt in enumerate(graph[curr]):
                if used[curr][nxt] < graph[curr].count(nxt):
                    used[curr][nxt] += 1
                    path.append(nxt)
                    if dfs(nxt):
                        return True
                    path.pop()
                    used[curr][nxt] -= 1
            return False

        dfs("JFK")
        return path
```

**Complexity Analysis**  
- Time complexity: O((V+E)! ) in the worst case — we explore every permutation of edges.  
- Space complexity: O(V+E) — recursion stack + adjacency list + bookkeeping.

---

### Approach 2: Hierholzer’s Algorithm (Recursive + Min‑Heap)

**Intuition**  
Instead of trying every permutation, we can **always** take the smallest lexical next flight because any Eulerian trail that starts at `"JFK"` and respects the lexical rule must do so at each step.  
Hierholzer’s algorithm guarantees we will consume all edges exactly once by following edges until we get stuck, then back‑tracking and inserting the discovered cycles.

**Algorithm**  
1. Build adjacency list where each origin maps to a **min‑heap** of its destinations.  
2. Define a recursive `visit(airport)`: while the heap for `airport` is non‑empty, pop the smallest destination and recursively visit it.  
3. After processing all outgoing edges, append the current airport to the route list.  
4. Start the recursion from `"JFK"`.  
5. Reverse the collected route to obtain the forward itinerary.

**Implementation**

```python
from collections import defaultdict
import heapq

class Solution:
    def findItinerary(self, tickets):
        graph = defaultdict(list)
        for u, v in tickets:
            heapq.heappush(graph[u], v)

        route = []

        def visit(airport):
            while graph[airport]:
                nxt = heapq.heappop(graph[airport])
                visit(nxt)
            route.append(airport)

        visit("JFK")
        return route[::-1]
```

**Complexity Analysis**  
- Time complexity: O(E log V) — each edge is popped once from a heap (`log V` per pop).  
- Space complexity: O(V+E) — adjacency list + recursion stack (depth ≤ E).

---

### Approach 3: Hierholzer’s Algorithm (Iterative)

**Intuition**  
Recursion depth can be as large as the number of tickets (≤ 300), which is safe, but an iterative version removes recursion overhead and makes the algorithm easier to reason about in languages with low recursion limits.  
We simulate the call stack with an explicit stack: we keep moving forward along the smallest available edge until we hit a dead end, then we pop vertices from the stack and add them to the final path.

**Algorithm**  
1. Build adjacency list with min‑heaps as before.  
2. Initialise `stack = ["JFK"]` and `path = []`.  
3. While `stack` is not empty:  
   - While the top of the stack still has outgoing edges, pop the smallest destination from its heap and push it onto the stack.  
   - When the top has no more edges, pop it from the stack and append it to `path`.  
4. Reverse `path` to get the itinerary.

**Implementation**

```python
from collections import defaultdict
import heapq

class Solution:
    def findItinerary(self, tickets):
        graph = defaultdict(list)
        for u, v in tickets:
            heapq.heappush(graph[u], v)

        stack = ["JFK"]
        path = []

        while stack:
            while graph[stack[-1]]:
                nxt = heapq.heappop(graph[stack[-1]])
                stack.append(nxt)
            path.append(stack.pop())

        return path[::-1]
```

**Complexity Analysis**  
- Time complexity: O(E log V) — each edge removed from a heap once.  
- Space complexity: O(V+E) — adjacency list + explicit stack + output list.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the iterative Hierholzer algorithm on Example 2:

```
tickets = [["JFK","SFO"],["JFK","ATL"],["SFO","ATL"],["ATL","JFK"],["ATL","SFO"]]
```

Adjacency lists (min‑heap) after construction:
- JFK: [ATL, SFO]
- ATL: [JFK, SFO]
- SFO: [ATL]

#### Dry Run

| Step | Stack (top at right)                | Action taken                               | Path (built in reverse) |
|------|-------------------------------------|--------------------------------------------|--------------------------|
| 0    | [`JFK`]                             | start                                      | []                       |
| 1    | [`JFK`, `ATL`]                      | pop smallest from JFK → ATL                | []                       |
| 2    | [`JFK`, `ATL`, `JFK`]               | pop smallest from ATL → JFK                | []                       |
| 3    | [`JFK`, `ATL`, `JFK`, `SFO`]        | pop smallest from JFK → SFO                | []                       |
| 4    | [`JFK`, `ATL`, `JFK`, `SFO`, `ATL`] | pop smallest from SFO → ATL                | []                       |
| 5    | [`JFK`, `ATL`, `JFK`, `SFO`, `ATL`, `SFO`] | pop smallest from ATL → SFO          | []                       |
| 6    | [`JFK`, `ATL`, `JFK`, `SFO`, `ATL`, `SFO`] | ATL heap empty → pop `SFO` to path | [`SFO`]                  |
| 7    | [`JFK`, `ATL`, `JFK`, `SFO`, `ATL`] | ATL heap now has [`JFK`]; pop `JFK` to path| [`SFO`, `JFK`]           |
| 8    | [`JFK`, `ATL`, `JFK`, `SFO`]        | SFO heap empty → pop `SFO` to path         | [`SFO`, `JFK`, `SFO`]    |
| 9    | [`JFK`, `ATL`, `JFK`]               | JFK heap empty → pop `JFK` to path         | [`SFO`, `JFK`, `SFO`, `JFK`] |
|10    | [`JFK`, `ATL`]                      | ATL heap empty → pop `ATL` to path         | [`SFO`, `JFK`, `SFO`, `JFK`, `ATL`] |
|11    | [`JFK`]                             | JFK heap empty → pop `JFK` to path         | [`SFO`, `JFK`, `SFO`, `JFK`, `ATL`, `JFK`] |
|12    | []                                  | stack empty → stop                         |                          |

Reverse `path` → `["JFK","ATL","JFK","SFO","ATL","SFO"]`, which matches the expected output.

---