# Tree DFS

## Video Solution

For more details about **Count Good Nodes in Binary Tree**, watch the walkthrough at [https://www.youtube.com/watch?v=7cp5imvDzl4](https://www.youtube.com/watch?v=7cp5imvDzl4).

## Concept

Depth‑First Search (DFS) lets us visit every node while keeping information from the path that led to it.  
For this problem we only need to know the **largest value seen so far** on the root‑to‑current node path. If the current node’s value is ≥ that maximum, the node is *good*; otherwise it isn’t. By passing the running maximum as an argument during the traversal we can decide goodness in O(1) per node.

## When to Use It

Use a DFS with extra state when you see:
- “along the path from root to node …”
- “no node with value greater than X …”
- “maximum/minimum on the path …”
- Any property that can be updated incrementally while walking down a tree.

## Template

```python
def dfs(node, state):
    if not node:
        return base_case

    # update state with current node
    new_state = update(state, node)

    # process current node (e.g., count if good)
    result = process(node, new_state)

    # recurse on children
    left  = dfs(node.left,  new_state)
    right = dfs(node.right, new_state)

    return combine(result, left, right)   # depends on problem
```

---

# LeetCode Problem Walkthrough

### Problem: 1448. Count Good Nodes in Binary Tree  
https://leetcode.com/problems/count-good-nodes-in-binary-tree/

---

## Approach 1: Brute Force (Check Path for Each Node)

**Algorithm**  
1. For every node in the tree, retrieve the values on the path from the root to that node.  
2. Compute the maximum value on that path.  
3. If the node’s value ≥ this maximum, it is good.  
4. Sum over all nodes.

To get the path we first build a parent map via one DFS, then for each node we walk upwards using the map to collect ancestors.

**Implementation**

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def goodNodes(self, root: Optional[TreeNode]) -> int:
        if not root:
            return 0

        # 1️⃣ Build parent pointers
        parent = {root: None}
        stack = [root]
        while stack:
            node = stack.pop()
            if node.left:
                parent[node.left] = node
                stack.append(node.left)
            if node.right:
                parent[node.right] = node
                stack.append(node.right)

        # 2️⃣ Helper: max value on path from root to `node`
        def max_on_path(node):
            max_val = float('-inf')
            while node:
                max_val = max(max_val, node.val)
                node = parent[node]
            return max_val

        # 3️⃣ Count good nodes
        good = 0
        stack = [root]
        while stack:
            node = stack.pop()
            if node.val >= max_on_path(node):
                good += 1
            if node.left:
                stack.append(node.left)
            if node.right:
                stack.append(node.right)

        return good
```

**Complexity Analysis**

- Time complexity: O(n²) — for each of the n nodes we may walk up O(h) ≤ O(n) ancestors.  
- Space complexity: O(n) — parent map + stack.

---

## Approach 2: DFS Recursion (Carry Current Maximum)

**Intuition**  
While traversing the tree we can keep the maximum value seen so far on the current root‑to‑node path.  
If the current node’s value is ≥ that maximum, it is good; otherwise it isn’t.  
Updating the maximum is O(1), so each node is processed once.

**Algorithm**  
1. Start DFS at the root with `current_max = root.val`.  
2. At each node:  
   - If `node.val >= current_max` → increment good count.  
   - Update `current_max = max(current_max, node.val)`.  
   - Recurse on left and right children with the updated maximum.  
3. Return the accumulated count.

**Implementation**

```python
class Solution:
    def goodNodes(self, root: Optional[TreeNode]) -> int:
        def dfs(node, current_max):
            if not node:
                return 0
            good = 1 if node.val >= current_max else 0
            new_max = max(current_max, node.val)
            return good + dfs(node.left, new_max) + dfs(node.right, new_max)

        return dfs(root, root.val)
```

**Complexity Analysis**

- Time complexity: O(n) — each node visited once.  
- Space complexity: O(h) — recursion stack, where h is tree height (worst‑case O(n) for a skewed tree).

---

## Approach 3: Iterative DFS with Explicit Stack

**Intuition**  
The recursive solution can be rewritten iteratively to avoid recursion‑depth limits.  
We store tuples `(node, current_max)` on a stack and process them in a loop, updating the maximum exactly as in the recursive version.

**Algorithm**  
1. Initialise stack with `(root, root.val)` and `good = 0`.  
2. While stack not empty:  
   - Pop `(node, cur_max)`.  
   - If `node.val >= cur_max` → `good += 1`.  
   - Compute `new_max = max(cur_max, node.val)`.  
   - Push left child with `new_max` if it exists.  
   - Push right child with `new_max` if it exists.  
3. Return `good`.

**Implementation**

```python
class Solution:
    def goodNodes(self, root: Optional[TreeNode]) -> int:
        if not root:
            return 0

        good = 0
        stack = [(root, root.val)]

        while stack:
            node, cur_max = stack.pop()
            if node.val >= cur_max:
                good += 1
            new_max = max(cur_max, node.val)
            if node.left:
                stack.append((node.left, new_max))
            if node.right:
                stack.append((node.right, new_max))

        return good
```

**Complexity Analysis**

- Time complexity: O(n) — each node processed once.  
- Space complexity: O(h) — stack size mirrors recursion depth.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimal recursive approach on the example tree:

```
Input: root = [3,1,4,3,null,1,5]
```

| Step | Node | current_max (before node) | node.val >= current_max? | Good? | new_max (after node) |
|------|------|---------------------------|--------------------------|-------|----------------------|
| 1    | 3    | 3                         | 3 ≥ 3 → True             | ✅    | 3                    |
| 2    | 1    | 3                         | 1 ≥ 3 → False            | ❌    | 3                    |
| 3    | 3 (left of 1) | 3          | 3 ≥ 3 → True             | ✅    | 3                    |
| 4    | 4    | 3                         | 4 ≥ 3 → True             | ✅    | 4                    |
| 5    | 1 (left of 4) | 4          | 1 ≥ 4 → False            | ❌    | 4                    |
| 6    | 5    | 4                         | 5 ≥ 4 → True             | ✅    | 5                    |

Total good nodes = 4 (nodes 3, 3, 4, 5), matching the expected output.

---