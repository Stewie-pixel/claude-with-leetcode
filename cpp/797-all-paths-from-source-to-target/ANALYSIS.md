# Backtracking

## Video Solution

For more details about **All Paths From Source to Target**, watch the walkthrough at [https://www.youtube.com/watch?v=PmeUj2Tq4Ww](https://www.youtube.com/watch?v=PmeUj2Tq4Ww).

## Concept

Backtracking is a systematic way to iterate through all possible configurations of a search space.  
We build a solution incrementally (e.g., a path) and, as soon as we realize the current partial solution cannot lead to a valid complete solution, we **backtrack**—undo the last choice and try a different one.  
In the context of a directed acyclic graph (DAG), we start at node 0, extend the current path by following each outgoing edge, and when we reach the target node n‑1 we record the path. Because the graph is acyclic we never need to worry about cycles; the recursion will naturally terminate.

A real‑world analogy: exploring every possible route in a maze where you never revisit a intersection (the DAG guarantee) and you draw each successful route on a map as you find it.

## When to Use It

Use backtracking / DFS when you see:

- “Find **all** …” (all paths, all subsets, all permutations, etc.)  
- The input size is small enough that enumerating all possibilities is feasible (here n ≤ 15).  
- The structure is a graph or tree and you need to explore every route from a source to a destination.  
- No obvious greedy or DP shortcut exists; you must examine each choice.

## Template

```python
def backtrack(state, choices):
    # state: current partial solution (e.g., the path built so far)
    # choices: list of next options we can take from the current state

    if is_solution(state):
        record_solution(state)          # we found a complete answer
        return

    for choice in choices:
        make_choice(state, choice)      # extend state
        backtrack(state, next_choices(state, choice))
        undo_choice(state, choice)      # backtrack: restore state
```

For graph path‑finding we specialize this template:
- `state` = current path list.
- `choices` = outgoing edges from the last node in the path.
- `is_solution` = last node equals target (`n‑1`).
- `record_solution` = copy the path into the answer list.

---

## LeetCode Problem Walkthrough

### Problem: 797. All Paths From Source to Target  
https://leetcode.com/problems/all-paths-from-source-to-target/

---

### Approach 1: Brute Force Depth‑First Search (Backtracking)

**Algorithm**  
1. Start a DFS from node 0, keeping a temporary list `path` that stores the vertices visited so far.  
2. Append the current node to `path`.  
3. If the current node is the target (`n‑1`), add a copy of `path` to the answer.  
4. Otherwise, recursively explore each neighbor of the current node.  
5. Before returning, remove the current node from `path` (backtrack) so it can be reused in other branches.

**Implementation**

```python
class Solution:
    def allPathsSourceTarget(self, graph: List[List[int]]) -> List[List[int]]:
        target = len(graph) - 1
        ans: List[List[int]] = []
        path: List[int] = []

        def dfs(node: int) -> None:
            path.append(node)                 # extend current path
            if node == target:                # reached destination
                ans.append(path.copy())       # store a copy of the path
            else:
                for nxt in graph[node]:       # try every outgoing edge
                    dfs(nxt)
            path.pop()                        # backtrack

        dfs(0)
        return ans
```

**Complexity Analysis**  
- **Time complexity**: O(P · N) where P is the number of distinct paths and N ≤ 15 is the average path length (we copy the path when storing it). In the worst case the number of paths can be exponential (≈ 2^{N}) for a dense DAG, so the bound is exponential.  
- **Space complexity**: O(N) for the recursion stack and the temporary `path`, plus O(P · N) for the output itself (required to hold all paths).

---

### Approach 2: Memoized Depth‑First Search (Top‑Down DP)

**Intuition**  
The brute‑force DFS recomputes the same sub‑problem many times: the set of all paths from an intermediate node `u` to the target is independent of how we arrived at `u`. By caching (memoizing) the result for each node we avoid repeated work. The memo stores **all** paths from that node to the target; we then prepend the current node to each cached suffix.

**Algorithm**  
1. Define a recursive function `paths_from(u)` that returns a list of paths (each path is a list) from node `u` to the target.  
2. Base case: if `u == target`, return `[[target]]`.  
3. If the result for `u` is already memoized, return it.  
4. Otherwise, for each neighbor `v` of `u`, recursively obtain `paths_from(v)`.  
5. For each returned path `p`, create a new path `[u] + p` and collect all of them.  
6. Memoize and return the collected list for `u`.  
7. Kick off the recursion from node 0.

**Implementation**

```python
from functools import lru_cache
from typing import List

class Solution:
    def allPathsSourceTarget(self, graph: List[List[int]]) -> List[List[int]]:
        target = len(graph) - 1

        @lru_cache(maxsize=None)
        def dfs(u: int) -> List[tuple]:
            if u == target:
                return [(target,)]               # store as tuple for hashability
            res: List[tuple] = []
            for v in graph[u]:
                for suffix in dfs(v):
                    res.append((u,) + suffix)
            return res

        # Convert tuples back to lists for the required output format
        return [list(p) for p in dfs(0)]
```

**Complexity Analysis**  
- **Time complexity**: O(P · N + N + E). Each edge is traversed once per distinct path that goes through it, but thanks to memoization each node’s outgoing edges are processed only once per distinct suffix. The dominant term remains the size of the output P · N (we still have to build every path).  
- **Space complexity**: O(N) recursion stack + O(N · P) for the memo (each node stores all paths starting from it) + O(P · N) for the final answer. In big‑O terms this is O(P · N).

---

### Approach 3: Iterative Breadth‑First Search Using a Queue

**Intuition**  
Instead of recursion we can simulate the same exploration with an explicit queue (or stack). Each queue entry holds the current node and the path taken to reach it. When we pop an entry, if the node is the target we store the path; otherwise we push all neighbors with the path extended by that neighbor. This approach avoids recursion limits and is often easier to reason about for iterative thinkers.

**Algorithm**  
1. Initialize a queue with a tuple `(0, [0])` – start node and path containing only the source.  
2. While the queue is not empty:  
   a. Pop the front element `(node, path)`.  
   b. If `node == target`, append `path` to the answer.  
   c. Otherwise, for each neighbor `nbr` of `node`, push `(nbr, path + [nbr])` onto the queue.  
3. Return the accumulated answer.

**Implementation**

```python
from collections import deque
from typing import List

class Solution:
    def allPathsSourceTarget(self, graph: List[List[int]]) -> List[List[int]]:
        target = len(graph) - 1
        ans: List[List[int]] = []
        q: deque[tuple[int, List[int]]] = deque()
        q.append((0, [0]))                     # (current node, path so far)

        while q:
            node, path = q.popleft()
            if node == target:
                ans.append(path)
            else:
                for nxt in graph[node]:
                    q.append((nxt, path + [nxt]))
        return ans
```

**Complexity Analysis**  
- **Time complexity**: O(P · N) – each complete path is constructed exactly once when it reaches the target, and each extension step copies the current path (cost proportional to its length).  
- **Space complexity**: O(W·N) where W is the maximum width of the queue (the number of partial paths stored at any level). In the worst case this can also be O(P · N), matching the output size.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the brute‑force DFS on the example `graph = [[1,2],[3],[3],[]]` (target = 3).

#### Dry Run

Input: `graph = [[1,2],[3],[3],[]]`

```
| Step | Call dfs(node) | path (before push) | Action                           | path (after push) | Note                                   |
|------|----------------|--------------------|----------------------------------|-------------------|----------------------------------------|
| 1    | dfs(0)         | []                 | push 0                           | [0]               | start                                   |
| 2    | dfs(1)         | [0]                | push 1                           | [0,1]             | follow edge 0→1                         |
| 3    | dfs(3)         | [0,1]              | push 3 (target)                  | [0,1,3]           | reached target → record [0,1,3]         |
| 4    | dfs(3) ret     | [0,1,3]            | pop 3                            | [0,1]             | backtrack from 3                        |
| 5    | dfs(1) ret     | [0,1]              | pop 1                            | [0]               | backtrack from 1                        |
| 6    | dfs(2)         | [0]                | push 2                           | [0,2]             | follow edge 0→2                         |
| 7    | dfs(3)         | [0,2]              | push 3 (target)                  | [0,2,3]           | reached target → record [0,2,3]         |
| 8    | dfs(3) ret     | [0,2,3]            | pop 3                            | [0,2]             | backtrack from 3                        |
| 9    | dfs(2) ret     | [0,2]              | pop 2                            | []                | backtrack from 2                        |
|10    | dfs(0) ret     | []                 | pop 0 (call ends)                | []                | all done                                |
```

The algorithm records the two paths `[0,1,3]` and `[0,2,3]`, matching the expected output.

---