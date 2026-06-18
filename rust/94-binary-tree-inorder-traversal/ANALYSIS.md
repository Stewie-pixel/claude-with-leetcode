# Tree DFS - Inorder Traversal

## Video Solution

For more details about **Binary Tree Inorder Traversal**, watch the walkthrough at [https://www.youtube.com/watch?v=g_S5WuasWUE](https://www.youtube.com/watch?v=g_S5WuasWUE)

## Concept

Inorder traversal visits a binary tree in the order **left → root → right**.  
If the tree is a Binary Search Tree (BST), this yields the node values in ascending order.  
Think of it as reading a book: you finish the left chapter, then read the current page, then move to the right chapter.

## When to Use It

Use inorder traversal when you see:
- A binary tree and need node values in **sorted** order (BST property).
- Problems that ask for the **k‑th smallest** element in a BST.
- Situations where you need to process nodes **between** two values (range queries).
- Any follow‑up that explicitly requests an **iterative** solution to avoid recursion depth limits.

## Template

A reusable recursive inorder template (Python):

```python
def inorder_traversal(root):
    """
    Returns a list of node values visited in inorder (left, root, right).
    """
    if not root:
        return []
    # left subtree
    left = inorder_traversal(root.left)
    # current node
    mid = [root.val]
    # right subtree
    right = inorder_traversal(root.right)
    return left + mid + right
```

For an iterative version, replace the recursion with an explicit stack (see Approach 2).

---

## LeetCode Problem Walkthrough

### Problem: 94. Binary Tree Inorder Traversal  
https://leetcode.com/problems/binary-tree-inorder-traversal/

---

### Approach 1: Brute Force (Recursive)

**Algorithm**  
1. If the current node is `None`, return an empty list.  
2. Recursively compute the inorder list of the left subtree.  
3. Append the current node’s value.  
4. Recursively compute the inorder list of the right subtree.  
5. Concatenate left + [val] + right and return.

**Implementation**

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def inorderTraversal(self, root: Optional[TreeNode]) -> List[int]:
        # Base case: empty subtree
        if not root:
            return []
        # Inorder: left, root, right
        left  = self.inorderTraversal(root.left)
        mid   = [root.val]
        right = self.inorderTraversal(root.right)
        return left + mid + right
```

**Complexity Analysis**

- Time complexity: **O(n)** — each node is visited exactly once.  
- Space complexity: **O(h)** — recursion stack depth equals tree height *h* (worst‑case O(n) for a skewed tree, O(log n) for balanced).

---

### Approach 2: Iterative with Explicit Stack (Intermediate Optimization)

**Intuition**  
The recursive solution uses the call stack to remember where to return after processing a subtree. We can simulate that behavior with our own `list` as a stack, eliminating recursion‑depth limits and making the control flow explicit.

**Algorithm**  
1. Initialize an empty stack and set `curr` to the root.  
2. While there are nodes to process (`curr` is not `None` **or** stack not empty):  
   a. Push all left children of `curr` onto the stack, moving `curr` leftwards until you hit `None`.  
   b. Pop the top node — this is the next node in inorder sequence.  
   c. Record its value.  
   d. Move `curr` to its right child to begin processing the right subtree.  
3. When both `curr` is `None` and the stack is empty, traversal is complete.

**Implementation**

```python
class Solution:
    def inorderTraversal(self, root: Optional[TreeNode]) -> List[int]:
        result = []
        stack = []
        curr = root

        while curr or stack:
            # Reach the leftmost node of the current subtree
            while curr:
                stack.append(curr)
                curr = curr.left

            # curr is None here, so pop the top
            curr = stack.pop()
            result.append(curr.val)   # Visit node
            curr = curr.right         # Now visit right subtree

        return result
```

**Complexity Analysis**

- Time complexity: **O(n)** — each node is pushed and popped exactly once.  
- Space complexity: **O(h)** — the stack holds at most the height of the tree (worst‑case O(n), average O(log n)).

---

### Approach 3: Morris Traversal (Most Optimal)

**Intuition**  
We can eliminate the auxiliary stack entirely by temporarily rewiring the tree: for each node, we find its inorder predecessor (the rightmost node in its left subtree) and make a temporary thread back to the current node. This lets us return to the current node after finishing the left subtree without extra memory. After processing, we remove the thread to restore the original tree.

**Algorithm**  
1. Initialize `curr` as root and an empty result list.  
2. While `curr` is not `None`:  
   a. If `curr` has **no left child**, visit `curr` (append its value) and move to `curr.right`.  
   b. Otherwise, find `predecessor`: the rightmost node in `curr`’s left subtree.  
      - If `predecessor.right` is `None`, set `predecessor.right = curr` (create thread) and move `curr = curr.left`.  
      - If `predecessor.right` is `curr` (thread already exists), remove it (`predecessor.right = None`), visit `curr`, then move `curr = curr.right`.  
3. Return the collected result.

**Implementation**

```python
class Solution:
    def inorderTraversal(self, root: Optional[TreeNode]) -> List[int]:
        result = []
        curr = root

        while curr:
            if curr.left is None:
                # No left subtree → visit node and go right
                result.append(curr.val)
                curr = curr.right
            else:
                # Find inorder predecessor of curr
                pred = curr.left
                while pred.right and pred.right is not curr:
                    pred = pred.right

                if pred.right is None:
                    # Make a temporary thread to curr
                    pred.right = curr
                    curr = curr.left
                else:
                    # Thread exists → left subtree already visited
                    pred.right = None   # Remove thread
                    result.append(curr.val)
                    curr = curr.right

        return result
```

**Complexity Analysis**

- Time complexity: **O(n)** — each edge is traversed a constant number of times (at most twice).  
- Space complexity: **O(1)** — only a few pointers are used; the tree is restored to its original shape.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the **iterative stack** approach (Approach 2) on the example from the statement.

**Input:**  
```
    1
     \
      2
     /
    3
```
Serialized as `[1,null,2,3]`. Expected inorder: `[1,3,2]`.

```
| Step | curr node | Stack (bottom → top)        | Action                              | Result so far |
|------|-----------|-----------------------------|-------------------------------------|---------------|
| 1    | 1         | []                          | push 1, move curr = 1.left = None   | []            |
| 2    | None      | [1]                         | pop 1 → visit 1, curr = 1.right = 2 | [1]           |
| 3    | 2         | []                          | push 2, move curr = 2.left = 3      | [1]           |
| 4    | 3         | [2]                         | push 3, move curr = 3.left = None   | [1]           |
| 5    | None      | [2, 3]                      | pop 3 → visit 3, curr = 3.right = None| [1,3]       |
| 6    | None      | [2]                         | pop 2 → visit 2, curr = 2.right = None| [1,3,2]    |
| 7    | None      | []                          | loop ends (curr empty & stack empty) | [1,3,2]      |
```

The traversal yields `[1,3,2]`, matching the expected output.  

--- 

*End of lecture.*