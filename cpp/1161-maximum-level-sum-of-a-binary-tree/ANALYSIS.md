# Tree BFS

## Video Solution

For more details about **Maximum Level Sum of a Binary Tree**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=9VFtLItLanM)

## Concept

Breadth‑First Search (BFS) visits a tree level by level. By using a queue we can process all nodes at the current depth before moving to the next depth. This makes it trivial to compute a value that depends on an entire level (e.g., sum, max, min) because we know exactly when a level starts and ends.

**Real‑world analogy:** Imagine you are checking the attendance of each floor in a building. You start with the ground floor (level 1), count everyone there, then move to the first floor (level 2), and so on. You never need to go back to a lower floor once you have finished counting it.

## When to Use It

Use BFS / level‑order traversal when you see any of the following clues in a problem statement:
- The tree is processed **level by level**.
- You need to compute an aggregate (sum, max, min, average) **per level**.
- The answer is a **level number** or depth.
- The problem mentions “children”, “nodes at the same depth”, or “breadth”.

## Template

```python
from collections import deque
from typing import Optional

# Definition for a binary tree node.
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

def bfs_level_order(root: Optional[TreeNode]) -> None:
    if not root:
        return

    q = deque([root])
    level = 0                     # current depth (0‑based, adjust as needed)

    while q:
        level_size = len(q)       # number of nodes on this level
        level += 1                # we are about to process this level

        # ---- place your level‑specific logic here ----
        # Example: compute sum of node values on this level
        level_sum = 0
        for _ in range(level_size):
            node = q.popleft()
            level_sum += node.val

            if node.left:
                q.append(node.left)
            if node.right:
                q.append(node.right)

        # ---- use level_sum (or any other metric) ----
        # e.g., compare with a global maximum, store in a list, etc.
        # For illustration we just print it:
        print(f"Level {level} sum = {level_sum}")
```

## LeetCode Problem Walkthrough

### Problem: 1161. Maximum Level Sum of a Binary Tree

https://leetcode.com/problems/maximum-level-sum-of-a-binary-tree/

---

### Approach 1: Brute Force – DFS with Level‑Sum Map

**Intuition**  
If we are allowed to use extra memory proportional to the height of the tree, we can perform a simple depth‑first search, keep track of the current depth, and accumulate the sum for each depth in a hash map (or list). After the traversal we scan the map to find the depth with the largest sum. This is conceptually straightforward and works for any tree shape.

**Algorithm**
1. Initialise a dictionary `level_sum` where `level_sum[depth]` stores the cumulative sum of node values at that depth.
2. Define a recursive helper `dfs(node, depth)`:
   - If `node` is `None`, return.
   - Add `node.val` to `level_sum[depth]`.
   - Recurse on left child with `depth+1`.
   - Recurse on right child with `depth+1`.
3. Call `dfs(root, 1)` (the problem defines root level as 1).
4. Iterate over `level_sum` to find the depth with the maximal sum; if several depths tie, return the smallest depth (as required).

**Implementation**

```python
from typing import Optional, Dict

class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

class Solution:
    def maxLevelSum(self, root: Optional[TreeNode]) -> int:
        level_sum: Dict[int, int] = {}

        def dfs(node: Optional[TreeNode], depth: int) -> None:
            if not node:
                return
            level_sum[depth] = level_sum.get(depth, 0) + node.val
            dfs(node.left, depth + 1)
            dfs(node.right, depth + 1)

        dfs(root, 1)

        max_sum = float('-inf')
        ans_level = 1
        for lvl, s in level_sum.items():
            if s > max_sum or (s == max_sum and lvl < ans_level):
                max_sum = s
                ans_level = lvl
        return ans_level
```

**Complexity Analysis**
- Time complexity: **O(N)** – each node is visited exactly once.
- Space complexity: **O(H)** for the recursion stack plus **O(L)** for the level‑sum map, where `H` is tree height and `L` is the number of distinct levels ( ≤ N). In the worst case (skewed tree) this is **O(N)**.

---

### Approach 2: Optimal – BFS Level‑Order Traversal

**Intuition**  
We can compute the level sum while we traverse the tree breadth‑first, without needing to store sums for all levels simultaneously. By processing nodes level‑by‑level (using the size of the queue before each level), we know exactly when a level ends and can immediately compare its sum to the best sum seen so far. This uses only the queue needed for BFS, giving a better constant factor and the same asymptotic bounds.

**Algorithm**
1. Initialise a queue with the root node. Set `level = 1`, `max_sum = -∞`, and `answer_level = 1`.
2. While the queue is not empty:
   - Record `level_size = len(queue)` – this is the number of nodes on the current level.
   - Initialise `curr_sum = 0`.
   - Repeat `level_size` times:
     - Pop a node from the front of the queue.
     - Add its value to `curr_sum`.
     - Push its left and right children (if they exist) onto the queue.
   - After processing the whole level, compare `curr_sum` with `max_sum`:
     - If `curr_sum` is larger, update `max_sum` and set `answer_level = level`.
     - If equal, keep the smaller level (the current `level` is already larger than any previous equal‑sum level because we traverse in increasing order, so we do nothing).
   - Increment `level` and continue.
3. Return `answer_level`.

**Implementation**

```python
from collections import deque
from typing import Optional

class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

class Solution:
    def maxLevelSum(self, root: Optional[TreeNode]) -> int:
        if not root:
            return 0   # per constraints this case never happens, but kept for completeness

        q = deque([root])
        level = 1
        max_sum = float('-inf')
        answer_level = 1

        while q:
            level_size = len(q)
            curr_sum = 0

            for _ in range(level_size):
                node = q.popleft()
                curr_sum += node.val

                if node.left:
                    q.append(node.left)
                if node.right:
                    q.append(node.right)

            if curr_sum > max_sum:
                max_sum = curr_sum
                answer_level = level
            # if equal we keep the earlier (smaller) level, which is already stored
            level += 1

        return answer_level
```

**Complexity Analysis**
- Time complexity: **O(N)** – each node is enqueued and dequeued exactly once.
- Space complexity: **O(W)** – where `W` is the maximum width of the tree (the largest number of nodes at any level). In the worst case of a completely balanced tree, `W ≈ N/2`, so the bound is **O(N)**; for a skewed tree it reduces to **O(1)**.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the BFS approach on the example from the statement:

```
Input: root = [1,7,0,7,-8,null,null]
```

Tree representation:

```
        1
       / \
      7   0
     / \
    7  -8
```

**Dry Run**

| Step | Queue (front→back)                     | level | level_size | curr_sum (after processing) | max_sum | answer_level |
|------|----------------------------------------|-------|------------|-----------------------------|---------|--------------|
| Init | [1]                                    | 1     | 1          | 0                           | -∞      | 1            |
| 1    | pop 1 → sum=1; push 7,0                | 1     | 1          | 1                           | 1       | 1            |
|      | queue now: [7,0]                       |       |            |                             |         |              |
| 2    | pop 7 → sum=7; push 7,-8               | 2     | 2          | 7                           | 7       | 2            |
|      | pop 0 → sum=7+0=7; push (none)         |       |            | 7                           | 7       | 2            |
|      | queue now: [7,-8]                      |       |            |                             |         |              |
| 3    | pop 7 → sum=7;                         | 3     | 2          | 7                           | 7       | 2            |
|      | pop -8 → sum=7+(-8)=-1                 |       |            | -1                          | 7       | 2            |
|      | queue now: []                          |       |            |                             |         |              |
| End  |                                        |       |            |                             |         |              |

The maximum level sum is **7**, occurring at level 2, so the answer is **2**.

--- 

This lecture follows the required format, provides both a brute‑force (DFS) and an optimal (BFS) solution, includes clear intuition, complexity analysis, and a step‑by‑step dry run. No extra sections or tangents were added. The file should be saved as:

```
/lectures/2025-09-26-tree-bfs.md
```

(Adjust the date if the actual commit date differs.)