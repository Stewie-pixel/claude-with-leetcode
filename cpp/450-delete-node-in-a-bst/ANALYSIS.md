# Binary Search Tree

## Video Solution

For more details about **Delete Node in a BST**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=LFzAoJJt92M).

## Concept

In a Binary Search Tree (BST) every node satisfies the property:

- All values in the left subtree `<` node.val
- All values in the right subtree `>` node.val  

To delete a node while preserving the BST property we consider three cases:

1. **Node has no children** (leaf) – simply remove it.
2. **Node has one child** – replace the node with its child.
3. **Node has two children** – we cannot just remove it because we would break ordering.  
   Instead we replace the node’s value with either its **in‑order successor** (smallest value in the right subtree) or its **in‑order predecessor** (largest value in the left subtree), then delete that successor/predecessor node (which will fall into case 1 or 2).

The algorithm therefore consists of:
- Searching for the node with the given key (standard BST search).
- Applying the appropriate deletion case.

## When to Use It

Use this technique when a problem:
- Explicitly mentions a **Binary Search Tree** or BST property.
- Asks to **delete**, **insert**, or **search** for a value while keeping the tree valid.
- Gives a node value (`key`) and expects the **root** of the possibly modified tree to be returned.

Typical clues: “BST”, “delete node”, “return root”, “unique values”, “O(height of tree)” follow‑up.

## Template

```python
# Definition for a binary tree node.
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

def deleteNode(root: TreeNode, key: int) -> TreeNode:
    """
    Recursively delete a node with value `key` from the BST rooted at `root`.
    Returns the (possibly new) root.
    """
    if not root:
        return None

    # Standard BST search
    if key < root.val:
        root.left = deleteNode(root.left, key)
    elif key > root.val:
        root.right = deleteNode(root.right, key)
    else:  # found the node to delete
        # Case 1: no left child
        if not root.left:
            return root.right
        # Case 2: no right child
        if not root.right:
            return root.left

        # Case 3: two children -> use inorder successor
        successor = root.right
        while successor.left:          # find smallest in right subtree
            successor = successor.left
        root.val = successor.val       # replace value
        # Delete the successor node (it has at most one right child)
        root.right = deleteNode(root.right, successor.val)

    return root
```

---

## LeetCode Problem Walkthrough

### Problem: 450. Delete Node in a BST  
https://leetcode.com/problems/delete-node-in-a-bst/

### Approach 1: Brute Force – Rebuild Tree

**Intuition**  
A simple (but inefficient) way to delete a node is to ignore the BST structure, collect all remaining values, and rebuild a balanced BST from scratch. This guarantees correctness but does not meet the desired `O(height)` time.

**Algorithm**
1. Perform an in‑order traversal of the tree to obtain a sorted list of all node values.
2. Remove the target `key` from the list (if present).
3. Build a new height‑balanced BST from the sorted list (e.g., by recursively picking the middle element as root).

**Implementation**

```python
def deleteNode_bruteforce(root: TreeNode, key: int) -> TreeNode:
    # 1. In‑order traversal to get sorted values
    def inorder(node):
        return inorder(node.left) + [node.val] + inorder(node.right) if node else []

    vals = inorder(root)
    if key not in vals:
        return root                      # nothing to delete
    vals.remove(key)                     # O(n) removal

    # 2. Build balanced BST from sorted list
    def build(l, r):
        if l > r:
            return None
        m = (l + r) // 2
        node = TreeNode(vals[m])
        node.left = build(l, m - 1)
        node.right = build(m + 1, r)
        return node

    return build(0, len(vals) - 1)
```

**Complexity Analysis**
- Time complexity: O(n) — we visit every node for traversal and again for rebuilding.
- Space complexity: O(n) — the list of values plus recursion stack (O(log n) for rebuild, dominated by O(n)).

---

### Approach 2: Optimal – Recursive Deletion (BST Property)

**Intuition**  
Instead of discarding the BST structure, we locate the node using BST search and handle the three deletion cases directly. When the node has two children we replace it with its in‑order successor (the smallest node in the right subtree), which guarantees the BST property after removal. The successor itself has at most one child, so its deletion reduces to case 1 or 2.

**Algorithm**  
(See the template above; steps are:)
1. If `root` is `None`, return `None`.
2. Compare `key` with `root.val` to decide left/right recursion.
3. When `key == root.val`:
   - If left child missing → return right child.
   - If right child missing → return left child.
   - Otherwise find successor (`min` of right subtree), copy its value to `root`, then recursively delete that successor from the right subtree.
4. Return the (possibly updated) `root`.

**Implementation**  
*(Same as the template; shown here for completeness.)*

```python
def deleteNode(root: TreeNode, key: int) -> TreeNode:
    if not root:
        return None
    if key < root.val:
        root.left = deleteNode(root.left, key)
    elif key > root.val:
        root.right = deleteNode(root.right, key)
    else:  # found node to delete
        if not root.left:
            return root.right
        if not root.right:
            return root.left
        # two children: get inorder successor
        succ = root.right
        while succ.left:
            succ = succ.left
        root.val = succ.val
        root.right = deleteNode(root.right, succ.val)
    return root
```

**Complexity Analysis**
- Time complexity: O(h) — where h is the height of the tree. We descend once to find the node and, at most, descend again to find the successor. In a balanced BST h = O(log n); worst case (skewed) h = O(n).
- Space complexity: O(h) — recursion stack proportional to tree height.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

We delete `key = 3` from the BST:

```
          5
        /   \
       3     6
      / \     \
     2   4     7
```

**Steps**

| Step | Action                                            | Tree after action                                   |
|------|---------------------------------------------------|-----------------------------------------------------|
| 1    | Search: 3 < 5 → go left                           |                                                     |
| 2    | Found node with value 3 (has two children)       |                                                     |
| 3    | Find inorder successor: go right → 6, then left as far as possible → none, so successor = 4 | |
| 4    | Copy successor value (4) into node 3              |                                                     |
| 5    | Delete successor node (original 4) from right subtree (node 4 has no children) | |
| 6    | Resulting tree                                    |                                                     |

Resulting tree:

```
          5
        /   \
       4     6
      /       \
     2         7
```

(Any valid BST after deletion is accepted; e.g., using predecessor yields `[5,2,6,null,4,null,7]`.)
