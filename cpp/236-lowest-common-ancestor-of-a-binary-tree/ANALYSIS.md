# Tree DFS

## Video Solution

For more details about **Lowest Common Ancestor of a Binary Tree**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=8ohqokWDloA).

## Concept

The **lowest common ancestor (LCA)** of two nodes `p` and `q` in a binary tree is the deepest node that has both `p` and `q` as descendants (a node can be a descendant of itself).  
Think of a family tree: if you start at the root and walk down, the first place where the paths to `p` and `q` split is their LCA.  
A depth‑first search (post‑order traversal) lets us explore each subtree and report whether it contains `p`, `q`, or both.

## When to Use It

Use a recursive DFS / post‑order approach when you see:
- A binary tree problem that asks for a relationship between two nodes (e.g., LCA, distance, checking if one node is ancestor of another).
- The solution can be expressed as “return information from left and right subtrees and combine at the current node”.
- You need to know whether a subtree contains a target node.

## Template

```python
# Generic post‑order DFS that returns some info from subtrees
def dfs(node):
    if not node:
        return base_case          # e.g., None, False, 0
    left_info  = dfs(node.left)   # process left subtree
    right_info = dfs(node.right)  # process right subtree
    # combine left_info, right_info, and node.val to produce answer for this node
    return combined_info
```

---

## LeetCode Problem Walkthrough

### Problem: 236. Lowest Common Ancestor of a Binary Tree

https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree/

### Approach 1: Brute Force – Store Root‑to‑Node Paths

**Algorithm**
1. Find the path from the root to node `p` by DFS; store the sequence of nodes.
2. Find the path from the root to node `q` similarly.
3. Compare the two paths from the start; the last matching node before they diverge is the LCA.

**Implementation**

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, x):
#         self.val = x
#         self.left = None
#         self.right = None

class Solution:
    def lowestCommonAncestor(self, root: 'TreeNode', p: 'TreeNode', q: 'TreeNode') -> 'TreeNode':
        def get_path(node, target, path):
            if not node:
                return False
            path.append(node)
            if node is target:
                return True
            if get_path(node.left, target, path) or get_path(node.right, target, path):
                return True
            path.pop()                     # backtrack
            return False

        p_path, q_path = [], []
        get_path(root, p, p_path)
        get_path(root, q, q_path)

        # Find last common node
        lca = None
        for u, v in zip(p_path, q_path):
            if u is v:
                lca = u
            else:
                break
        return lca
```

**Complexity Analysis**

- Time complexity: O(N) — we may visit each node twice (once for each path).
- Space complexity: O(H) — recursion stack plus two path lists, where H is tree height (worst‑case O(N)).

---

### Approach 2: Recursive Post‑Order (Optimal)

**Intuition**
Instead of building explicit paths, we can let each recursive call tell its parent whether the subtree contains `p` or `q`.  
- If the current node is `p` or `q`, it bubbles up as a potential ancestor.
- If both left and right subtrees report finding a target, the current node is the LCA (the split point).
- If only one side reports a target, that target (or its ancestor) is propagated upward.

**Algorithm**
1. Base case: if `node` is `None`, `p`, or `q`, return `node`.
2. Recurse on left and right children.
3. If both left and right returns are non‑`None`, the current node is the LCA → return it.
4. Otherwise, return whichever side is non‑`None` (propagate the found node upward).

**Implementation**

```python
class Solution:
    def lowestCommonAncestor(self, root: 'TreeNode', p: 'TreeNode', q: 'TreeNode') -> 'TreeNode':
        if not root or root is p or root is q:
            return root

        left  = self.lowestCommonAncestor(root.left,  p, q)
        right = self.lowestCommonAncestor(root.right, p, q)

        # p and q found in different subtrees → root is LCA
        if left and right:
            return root
        # otherwise return the non‑null side (could be p, q, or LCA found below)
        return left if left else right
```

**Complexity Analysis**

- Time complexity: O(N) — each node visited once.
- Space complexity: O(H) — recursion stack depth equals tree height (O(N) worst case, O(log N) balanced).

---

### Approach 3: Iterative with Parent Pointers

**Intuition**
If we store each node’s parent while traversing the tree (e.g., via BFS or DFS), we can ascend from `p` to the root, marking ancestors. Then ascend from `q` until we hit a marked node — that is the LCA. This avoids recursion and works well when the tree is given with parent links or we can afford extra O(N) space.

**Algorithm**
1. Use a stack (or queue) to perform DFS; record each node’s parent in a hash map.
2. Stop once both `p` and `q` have their parents recorded.
3. Build a set of ancestors of `p` by following parent pointers up to the root.
4. Move `q` upward through its parents; the first ancestor found in `p`’s set is the LCA.

**Implementation**

```python
class Solution:
    def lowestCommonAncestor(self, root: 'TreeNode', p: 'TreeNode', q: 'TreeNode') -> 'TreeNode':
        # Step 1: record parents
        stack = [root]
        parent = {root: None}
        while p not in parent or q not in parent:
            node = stack.pop()
            if node.left:
                parent[node.left] = node
                stack.append(node.left)
            if node.right:
                parent[node.right] = node
                stack.append(node.right)

        # Step 2: collect ancestors of p
        ancestors = set()
        while p:
            ancestors.add(p)
            p = parent[p]

        # Step 3: find first common ancestor of q
        while q not in ancestors:
            q = parent[q]
        return q
```

**Complexity Analysis**

- Time complexity: O(N) — we may traverse the whole tree to fill the parent map, then at most O(H) to climb ancestors.
- Space complexity: O(N) — parent dictionary plus ancestor set (worst‑case linear).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

We trace the recursive solution on the tree from Example 1:

```
          3
        /   \
       5     1
      / \   / \
     6   2 0   8
        / \
       7   4
```

Looking for `p = 5`, `q = 1`.

```
| Step | Node | left result | right result | Action/Return |
|------|------|-------------|--------------|---------------|
| 1    | 3    | (explore left) | (explore right) | — |
| 2    | 5    | left = 6 → None | right = subtree (2,7,4) | — |
| 3    | 6    | None | None | not p/q → return None |
| 4    | 2    | left = 7 → None | right = 4 → None | not p/q → return None |
| 5    | 7    | None | None | not p/q → return None |
| 6    | 4    | None | None | not p/q → return None |
| 7    | 2    | left=None, right=None → return None |
| 8    | 5    | left=None, right=None → **but node is p** → return 5 |
| 9    | 1    | left = 0 → None | right = 8 → None | **node is q** → return 1 |
|10    | 3    | left = 5, right = 1 → both non‑None → **return 3 (LCA)** |
```

The algorithm bubbles up `5` from the left subtree and `1` from the right; at node `3` both sides are non‑null, so `3` is returned as the lowest common ancestor.

--- 

**End of lecture**.  
Save this as `/lectures/YYYY-MM-DD-tree-dfs.md` (using today’s date) in the repository.