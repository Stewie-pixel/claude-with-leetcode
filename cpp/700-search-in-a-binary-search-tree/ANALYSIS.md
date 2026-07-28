# Search in a Binary Search Tree

## Video Solution

For more details about **Search in a Binary Search Tree**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=VKkbggYPyEI).

## Concept

A Binary Search Tree (BST) enforces an ordering property: for every node, all values in its left subtree are smaller, and all values in its right subtree are larger. This property lets us discard half of the tree at each step, similar to binary search on a sorted array.

**Analogy:** Imagine looking for a name in a phone book that is sorted alphabetically. You open the book to the middle, compare the target name with the page’s name, and then decide to search only the left half or the right half. You repeat until you find the name or exhaust the book. In a BST, each node plays the role of that “middle page”.

## When to Use It

Use BST search when you see:
- The input is explicitly a binary search tree.
- You need to find a node with a given value.
- The problem hints at ordering (e.g., “left child < parent < right child”).
- You are asked to return the subtree rooted at the found node (or null if not found).

## Template

Here is a reusable Python template for searching a BST (iterative version). Replace the comment with your logic.

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def searchBST(self, root: Optional[TreeNode], val: int) -> Optional[TreeNode]:
        # Iterative BST search using the ordering property
        while root:
            if val < root.val:
                root = root.left          # discard right subtree
            elif val > root.val:
                root = root.right         # discard left subtree
            else:
                return root               # found the node
        return None                       # not found
```

---

## LeetCode Problem Walkthrough

### Problem: 700. Search in a Binary Search Tree

https://leetcode.com/problems/search-in-a-binary-search-tree/

### Approach 1: Brute Force (Full Tree Traversal)

**Algorithm**
- Traverse the entire tree (e.g., using DFS or BFS) without using the BST property.
- At each node, check if its value equals `val`.
- If found, return that node; otherwise continue searching both children.
- If the traversal ends without a match, return `None`.

**Implementation**

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def searchBST(self, root: Optional[TreeNode], val: int) -> Optional[TreeNode]:
        if not root:
            return None
        if root.val == val:
            return root
        # Search left and right subtrees (ignoring BST ordering)
        left_result = self.searchBST(root.left, val)
        if left_result:
            return left_result
        return self.searchBST(root.right, val)
```

**Complexity Analysis**
- Time complexity: O(N) — we may visit every node in the worst case.
- Space complexity: O(H) — recursion stack depth equals tree height (O(N) for skewed tree, O(log N) for balanced).

### Approach 2: Recursive BST Property (Optimized)

**Intuition**
Because the tree is a BST, we can decide which subtree to explore based on the comparison between `val` and the current node’s value, eliminating the need to search both sides.

**Algorithm**
- If the current node is `None`, return `None`.
- If `root.val == val`, return `root`.
- If `val < root.val`, recursively search the left subtree.
- Else (`val > root.val`), recursively search the right subtree.

**Implementation**

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def searchBST(self, root: Optional[TreeNode], val: int) -> Optional[TreeNode]:
        if not root:
            return None
        if root.val == val:
            return root
        if val < root.val:
            return self.searchBST(root.left, val)
        else:
            return self.searchBST(root.right, val)
```

**Complexity Analysis**
- Time complexity: O(H) — we follow a single path from root to leaf, where H is tree height.
- Space complexity: O(H) — recursion stack (same as time). In a balanced BST, H = O(log N); worst‑case skewed tree gives O(N).

### Approach 3: Iterative BST Property (Most Optimal)

**Intuition**
The recursive solution can be turned into an iterative loop to avoid auxiliary stack space, achieving O(1) extra memory while preserving the O(H) runtime.

**Algorithm**
- Start at the root.
- While the current node is not `None`:
  - If `val` equals the node’s value, return the node.
  - If `val` is less, move to the left child.
  - Otherwise, move to the right child.
- If the loop exits, the value is not present; return `None`.

**Implementation**

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def searchBST(self, root: Optional[TreeNode], val: int) -> Optional[TreeNode]:
        while root:
            if val < root.val:
                root = root.left
            elif val > root.val:
                root = root.right
            else:
                return root
        return None
```

**Complexity Analysis**
- Time complexity: O(H) — we visit at most one node per level.
- Space complexity: O(1) — only a few pointers are used; no recursion or auxiliary data structures.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the iterative algorithm on Example 1.

**Input:**  
```
      4
     / \
    2   7
   / \
  1   3
```
`val = 2`

| Step | Current node value | Comparison (val vs. node) | Action                     |
|------|--------------------|---------------------------|----------------------------|
| 1    | 4                  | 2 < 4                     | Move to left child (2)     |
| 2    | 2                  | 2 == 2                    | Found → return node 2      |

The returned node is the root of the subtree `[2,1,3]`, matching the expected output.

For Example 2 (`val = 5`) the trace would be:

| Step | Current node value | Comparison | Action                     |
|------|--------------------|------------|----------------------------|
| 1    | 4                  | 5 > 4      | Move to right child (7)    |
| 2    | 7                  | 5 < 7      | Move to left child (None)  |
| 3    | None               | —          | Exit loop → return None    |

Thus the algorithm correctly returns `null` (shown as `[]` in LeetCode output).