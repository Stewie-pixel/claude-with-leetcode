# Tree DFS

## Video Solution

For more details about **Maximum Depth of Binary Tree**, watch the walkthrough at [https://www.youtube.com/watch?v=hTM3phVI6YQ](https://www.youtube.com/watch?v=hTM3phVI6YQ)

## Concept

Depth‑first search (DFS) explores a tree by going as deep as possible along each branch before backtracking.  
For computing the maximum depth, DFS lets us compute the depth of the left and right sub‑trees recursively and then take the larger one, adding one for the current node.

**Real‑world analogy:** Measuring the height of a tree by climbing each branch to its tip, noting the highest point you reach, then returning to the trunk and trying another branch.

## When to Use It

Use DFS (especially the recursive form) when you see:
- A binary tree problem that asks for a property of a path from root to leaf (depth, sum, diameter, etc.).
- The solution naturally breaks down into “solve the same problem for left child and right child”.
- You need to visit every node exactly once and the call‑stack depth is acceptable (tree height ≤ 10⁴ in LeetCode constraints).

## Template

```python
# Recursive DFS template for binary tree problems
def dfs(node):
    if not node:                 # base case: empty subtree
        return BASE_VALUE        # depends on the problem (0 for depth, True/False for existence, etc.)
    left  = dfs(node.left)       # solve left sub‑problem
    right = dfs(node.right)      # solve right sub‑problem
    return COMBINE(left, right, node)   # merge results with current node
```

---

## LeetCode Problem Walkthrough

### Problem: 104. Maximum Depth of Binary Tree
https://leetcode.com/problems/maximum-depth-of-binary-tree/

### Approach 1: Recursive DFS (Straight‑forward)

**Algorithm**
1. If the current node is `None`, its depth is `0`.
2. Otherwise, recursively compute the depth of the left subtree (`left_depth`) and the right subtree (`right_depth`).
3. The depth of the current node is `max(left_depth, right_depth) + 1`.
4. Return the depth from the root.

**Implementation**

```python
# Definition for a binary tree node (provided by LeetCode)
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def maxDepth(self, root: Optional[TreeNode]) -> int:
        # Base case: empty tree has depth 0
        if not root:
            return 0
        # Recursively get depths of left and right sub‑trees
        left_depth  = self.maxDepth(root.left)
        right_depth = self.maxDepth(root.right)
        # Current node adds one level to the deeper sub‑tree
        return max(left_depth, right_depth) + 1
```

**Complexity Analysis**
- Time complexity: **O(N)** — each node is visited exactly once.
- Space complexity: **O(H)** — recursion stack stores at most the height of the tree (worst‑case O(N) for a skewed tree, O(log N) for balanced).

---

### Approach 2: Iterative DFS (Explicit Stack)

**Intuition**
The recursive solution uses the call stack to keep track of nodes whose sub‑trees are still being processed. We can simulate that behavior with our own stack, storing pairs `(node, current_depth)`. This avoids recursion limits and makes the traversal order explicit.

**Algorithm**
1. Initialize a stack with the root node and depth `1` (if root exists).
2. Pop a node and its depth from the stack.
3. Update the answer with the maximum depth seen so far.
4. Push the left and right children (if they exist) onto the stack with depth `+1`.
5. Repeat until the stack is empty.
6. Return the recorded maximum depth.

**Implementation**

```python
class Solution:
    def maxDepth(self, root: Optional[TreeNode]) -> int:
        if not root:
            return 0

        max_depth = 0
        stack = [(root, 1)]               # (node, depth so far)

        while stack:
            node, depth = stack.pop()
            max_depth = max(max_depth, depth)

            if node.left:
                stack.append((node.left, depth + 1))
            if node.right:
                stack.append((node.right, depth + 1))

        return max_depth
```

**Complexity Analysis**
- Time complexity: **O(N)** — each node is pushed and popped once.
- Space complexity: **O(N)** in the worst case (stack may hold all nodes of a completely unbalanced tree); average O(H).

---

### Approach 3: Breadth‑First Search (Level Order Traversal)

**Intuition**
The depth of a tree equals the number of levels (layers) from root to the deepest leaf. By traversing the tree level‑by‑level (BFS) and counting how many times we finish a level, we directly obtain the depth without recursion or explicit depth tracking per node.

**Algorithm**
1. If the root is `None`, return `0`.
2. Initialize a queue with the root node.
3. While the queue is not empty:
   - Increment `depth` (we are about to process a new level).
   - Record the current queue size (`level_size`) — this many nodes belong to the present level.
   - Pop `level_size` nodes, pushing their non‑null children into the queue.
4. After the loop, `depth` holds the number of levels processed.
5. Return `depth`.

**Implementation**

```python
from collections import deque

class Solution:
    def maxDepth(self, root: Optional[TreeNode]) -> int:
        if not root:
            return 0

        q = deque([root])
        depth = 0

        while q:
            depth += 1                     # we are starting a new level
            level_size = len(q)            # nodes belonging to this level
            for _ in range(level_size):
                node = q.popleft()
                if node.left:
                    q.append(node.left)
                if node.right:
                    q.append(node.right)

        return depth
```

**Complexity Analysis**
- Time complexity: **O(N)** — each node is enqueued and dequeued exactly once.
- Space complexity: **O(W)** — where W is the maximum width of the tree (the largest number of nodes at any level). In the worst case of a completely balanced tree, W ≈ N/2 → **O(N)**.

---

## Dry Run

We trace the recursive solution on the example tree:

```
Input: root = [3,9,20,null,null,15,7]
          3
        /   \
       9    20
           /  \
          15   7
```

| Step | Call (node)          | left_depth | right_depth | Returned depth (max+1) |
|------|----------------------|------------|-------------|------------------------|
| 1    | maxDepth(3)          | ?          | ?           | ?                      |
| 2    | → maxDepth(9)        | 0 (null)   | 0 (null)    | max(0,0)+1 = **1**      |
| 3    | → maxDepth(20)       | ?          | ?           | ?                      |
| 4    | →→ maxDepth(15)      | 0          | 0           | **1**                  |
| 5    | →→ maxDepth(7)       | 0          | 0           | **1**                  |
| 6    | ← maxDepth(20) = max(1,1)+1 = **2** |
| 7    | ← maxDepth(3) = max(1,2)+1 = **3** |

Result: `3`, matching the expected output.

---