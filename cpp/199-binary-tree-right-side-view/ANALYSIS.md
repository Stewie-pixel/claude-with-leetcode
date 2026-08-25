# Tree BFS

## Video Solution

For more details about **Binary Tree Right Side View**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=3qiinyuuQ7s).

## Concept

Breadth‑First Search (BFS) visits a tree level by level using a queue.  
For the *right side view* we only need the **last** node encountered at each depth because it is the one visible from the right side.  
By processing nodes in the order they appear in a level (left → right) and remembering the value of the final node, we collect the view from top to bottom.

## When to Use It

Use BFS / level‑order traversal when you see:
- “level by level”, “each depth”, “rows of a tree”
- Problems that ask for the first/last node of each level (right/left side view, widest level, average of levels)
- Situations where you need to process siblings before moving deeper.

## Template

```python
from collections import deque
from typing import List, Optional

# Definition for a binary tree node (provided by LeetCode)
class TreeNode:
    def __init__(self, val: int = 0,
                 left: Optional['TreeNode'] = None,
                 right: Optional['TreeNode'] = None):
        self.val = val
        self.left = left
        self.right = right

def bfs_template(root: Optional[TreeNode]) -> List[int]:
    if not root:
        return []

    q = deque([root])
    result = []

    while q:
        level_size = len(q)          # number of nodes on current level
        for i in range(level_size):
            node = q.popleft()
            # <-- process node here (e.g., capture last node) -->
            if node.left:
                q.append(node.left)
            if node.right:
                q.append(node.right)
        # after the inner loop we have finished the level
        # (place any per‑level work here)

    return result
```

## LeetCode Problem Walkthrough

### Problem: 199. Binary Tree Right Side View  
https://leetcode.com/problems/binary-tree-right-side-view/

---

### Approach 1: Brute Force – Store All Levels

**Intuition**  
If we first gather every node of each depth (a list of lists), we can simply take the last element of each inner list. This is straightforward but uses extra space to keep all levels.

**Algorithm**
1. Perform a standard BFS, but instead of discarding nodes after processing a level, append each node’s value to a list representing that depth.
2. After the traversal, iterate over the depth‑lists and collect the last value of each.
3. Return the collected values.

**Implementation**

```python
from collections import deque
from typing import List, Optional

class Solution:
    def rightSideView(self, root: Optional[TreeNode]) -> List[int]:
        if not root:
            return []

        q = deque([root])
        levels: List[List[int]] = []          # levels[i] holds all node values at depth i

        while q:
            level_size = len(q)
            cur_level: List[int] = []

            for _ in range(level_size):
                node = q.popleft()
                cur_level.append(node.val)

                if node.left:
                    q.append(node.left)
                if node.right:
                    q.append(node.right)

            levels.append(cur_level)          # store the whole level

        # Extract the last element of each level
        return [lvl[-1] for lvl in levels]
```

**Complexity Analysis**

- Time complexity: **O(N)** – each node is visited once.  
- Space complexity: **O(N)** – the `levels` list stores every node value (worst case when the tree is a line).

---

### Approach 2: Optimized BFS – Capture Last Node Per Level

**Intuition**  
We don’t need to keep entire levels; we only need the *rightmost* node. While iterating through a level, the last node we see is exactly the one visible from the right. Thus we can update the answer on‑the‑fly, saving space.

**Algorithm**
1. Initialize a queue with the root.
2. While the queue is not empty:
   - Record the current queue size (`level_size`).
   - Loop `level_size` times, popping nodes from the front.
   - For each node, push its children (left then right) to the queue.
   - After processing each node, if it is the **last** iteration of the inner loop (`i == level_size‑1`), append its value to the answer list.
3. Return the answer list.

**Implementation**

```python
from collections import deque
from typing import List, Optional

class Solution:
    def rightSideView(self, root: Optional[TreeNode]) -> List[int]:
        if not root:
            return []

        q = deque([root])
        right_view: List[int] = []

        while q:
            level_size = len(q)
            for i in range(level_size):
                node = q.popleft()

                # If this is the last node of the current level, it's visible from the right
                if i == level_size - 1:
                    right_view.append(node.val)

                if node.left:
                    q.append(node.left)
                if node.right:
                    q.append(node.right)

        return right_view
```

**Complexity Analysis**

- Time complexity: **O(N)** – each node is enqueued and dequeued once.  
- Space complexity: **O(W)** – where *W* is the maximum width of the tree (the largest queue size). In the worst case (a complete tree) this is O(N/2) ≈ O(N), but for skewed trees it reduces to O(1).

---

### Approach 3: DFS – Right‑First Preorder (Alternative)

**Intuition**  
If we visit the right child before the left child, the first node we encounter at a new depth is the right‑most one. By keeping track of the maximum depth visited so far, we can record a node’s value the first time we reach a depth, which yields the right side view without extra queues.

**Algorithm**
1. Define a recursive helper `dfs(node, depth)`.
2. If `node` is `None`, return.
3. If `depth` equals the size of the result list, we have reached a new depth for the first time → append `node.val`.
4. Recurse on `node.right` then `node.left` (right‑first) with `depth + 1`.
5. Kick off the recursion from the root at depth `0`.

**Implementation**

```python
from typing import List, Optional

class Solution:
    def rightSideView(self, root: Optional[TreeNode]) -> List[int]:
        right_view: List[int] = []

        def dfs(node: Optional[TreeNode], depth: int) -> None:
            if not node:
                return
            # First time we see this depth -> rightmost node at this level
            if depth == len(right_view):
                right_view.append(node.val)
            # Right child first ensures we see the rightmost node first
            dfs(node.right, depth + 1)
            dfs(node.left, depth + 1)

        dfs(root, 0)
        return right_view
```

**Complexity Analysis**

- Time complexity: **O(N)** – each node is visited once.  
- Space complexity: **O(H)** – recursion stack depth equals the tree height *H* (worst case O(N) for a skewed tree, O(log N) for balanced).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the optimized BFS (Approach 2) on the example  
`root = [1,2,3,null,5,null,4]` (represented as:

```
        1
       / \
      2   3
       \   \
        5   4
```

#### Dry Run

| Step | Queue (front → back)            | i (index in level) | node.val | Action                                   | right_view so far |
|------|--------------------------------|--------------------|----------|------------------------------------------|-------------------|
| Init | [1]                            | –                  | –        | –                                        | []                |
| 1    | pop 1 → push 2,3               | i=0, size=2        | 1        | i==size‑1? **yes** → record 1            | [1]               |
|      | Queue now: [2,3]               |                    |          |                                          |                   |
| 2    | pop 2 → push 5                 | i=0, size=2        | 2        | i==size‑1? **no**                        | [1]               |
|      | Queue now: [3,5]               |                    |          |                                          |                   |
| 3    | pop 3 → push 4                 | i=1, size=2        | 3        | i==size‑1? **yes** → record 3            | [1,3]             |
|      | Queue now: [5,4]               |                    |          |                                          |                   |
| 4    | pop 5 → (no children)          | i=0, size=2        | 5        | i==size‑1? **no**                        | [1,3]             |
|      | Queue now: [4]                 |                    |          |                                          |                   |
| 5    | pop 4 → (no children)          | i=1, size=2        | 4        | i==size‑1? **yes** → record 4            | [1,3,4]           |
|      | Queue now: []                  |                    |          |                                          |                   |
| End  | –                              | –                  | –        | –                                        | [1,3,4]           |

The final `right_view` matches the expected output `[1,3,4]`.

--- 

**Summary**

- BFS level order is the natural fit for “right side view” problems.  
- The optimized BFS captures only the last node per level, giving O(N) time and O(W) space.  
- A DFS right‑first preorder offers an alternative with O(N) time and O(H) space.  
- Always state complexity; never skip the brute‑force → optimized progression.