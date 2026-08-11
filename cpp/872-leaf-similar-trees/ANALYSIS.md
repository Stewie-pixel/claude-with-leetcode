# Tree DFS

## Video Solution

For more details about **Leaf-Similar Trees**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=Nr8dbnL0_cM)

## Concept

Depth‑First Search (DFS) explores a tree by going as deep as possible along each branch before backtracking.  
When we need the **leaf value sequence** (the values of all leaves from left to right), a simple DFS that visits left child before right child naturally yields the leaves in the required order.  
Think of it like reading a book: you read the leftmost page completely before moving to the next page on the right.

## When to Use It

Use a tree DFS when you see:
- “leaf”, “leaf sequence”, “leaf values”
- Need to collect or compare information from the leaves of a binary tree
- The problem involves traversing the entire tree but only cares about nodes with no children
- Constraints are small enough for an O(N) traversal (here ≤ 200 nodes)

## Template

```python
# Reusable DFS to collect leaf values left‑to‑right
def dfs_collect_leaves(node, leaves):
    if not node:
        return
    # Leaf node: no children
    if not node.left and not node.right:
        leaves.append(node.val)
        return
    # Recurse left first, then right to preserve left‑to‑right order
    dfs_collect_leaves(node.left, leaves)
    dfs_collect_leaves(node.right, leaves)
```

## LeetCode Problem Walkthrough

### Problem: 872. Leaf-Similar Trees

https://leetcode.com/problems/leaf-similar-trees/

### Approach 1: Recursive DFS (Brute Force / Straightforward)

**Algorithm**
1. Define a helper that performs a DFS and appends a node’s value to a list when the node is a leaf.
2. Run the helper on both trees to obtain two leaf sequences.
3. Return `True` if the two sequences are identical, otherwise `False`.

**Implementation**

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def leafSimilar(self, root1: Optional[TreeNode], root2: Optional[TreeNode]) -> bool:
        def get_leaves(node, leaves):
            if not node:
                return
            if not node.left and not node.right:   # leaf
                leaves.append(node.val)
                return
            get_leaves(node.left, leaves)
            get_leaves(node.right, leaves)

        leaves1, leaves2 = [], []
        get_leaves(root1, leaves1)
        get_leaves(root2, leaves2)
        return leaves1 == leaves2
```

**Complexity Analysis**
- Time complexity: O(N₁ + N₂) — each node is visited once in each tree.
- Space complexity: O(H₁ + H₂) for the recursion stack, where H is the height of the tree (worst‑case O(N) for a skewed tree) plus O(L₁ + L₂) for storing the leaf values (L = number of leaves).

### Approach 2: Iterative DFS using an Explicit Stack

**Intuition**
The recursive solution uses the call stack implicitly. We can replace it with our own stack to avoid recursion limits and make the control flow explicit. The order of processing (push right child first, then left) ensures that when we pop from the stack we visit the left subtree before the right one, preserving left‑to‑right leaf order.

**Algorithm**
1. For each tree, initialize an empty stack with its root and an empty leaf list.
2. While the stack is not empty:
   - Pop a node.
   - If it is a leaf, append its value to the leaf list.
   - Otherwise push its right child then its left child (so left is processed first).
3. Compare the two leaf lists.

**Implementation**

```python
class Solution:
    def leafSimilar(self, root1: Optional[TreeNode], root2: Optional[TreeNode]) -> bool:
        def get_leaves_iter(root):
            leaves, stack = [], [root] if root else []
            while stack:
                node = stack.pop()
                if not node.left and not node.right:   # leaf
                    leaves.append(node.val)
                else:
                    # push right first so that left is processed next
                    if node.right:
                        stack.append(node.right)
                    if node.left:
                        stack.append(node.left)
            return leaves

        return get_leaves_iter(root1) == get_leaves_iter(root2)
```

**Complexity Analysis**
- Time complexity: O(N₁ + N₂) — each node is pushed and popped at most once.
- Space complexity: O(N₁ + N₂) in the worst case for the explicit stack (a completely unbalanced tree) plus O(L₁ + L₂) for the leaf lists.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the recursive DFS on the first example tree.

**Input (root1)**  
```
        3
       / \
      5   1
     / \ / \
    6  2 9 8
       / \
      7   4
```
Leaf sequence expected: `[6,7,4,9,8]`

#### Dry Run (root1)

| Step | Node | Action (leaf?) | Leaves so far |
|------|------|----------------|---------------|
| 1    | 3    | not leaf → go left | [] |
| 2    | 5    | not leaf → go left | [] |
| 3    | 6    | **leaf** → add 6 | [6] |
| 4    | 2    | not leaf → go left | [6] |
| 5    | 7    | **leaf** → add 7 | [6,7] |
| 6    | 4    | **leaf** → add 4 | [6,7,4] |
| 7    | 1    | not leaf → go right (left subtree done) | [6,7,4] |
| 8    | 9    | **leaf** → add 9 | [6,7,4,9] |
| 9    | 8    | **leaf** → add 8 | [6,7,4,9,8] |

The same process on `root2` yields an identical list, so the function returns `True`.

--- 

This lecture covers the Tree DFS pattern, shows both recursive and iterative implementations, proves correctness with a dry run, and provides the required complexity analysis. Commit the file as `/lectures/2025-09-17-tree-dfs.md`.