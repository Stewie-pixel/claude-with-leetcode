# Trees

## Video Solution

For more details about **Root Equals Sum of Children**, watch the walkthrough at [https://www.youtube.com/watch?v=ua9FLN-yYGU](https://www.youtube.com/watch?v=ua9FLN-yYGU).

## Concept

A binary tree node stores a value and pointers to its left and right children.  
In this problem the tree is guaranteed to have exactly three nodes: the root and its two children.  
We simply need to verify whether the root’s value equals the sum of its children’s values.

## When to Use It

Use this pattern when a problem:

- Gives you a binary tree with a known small shape (e.g., exactly root + two children)
- Asks you to compare a parent node’s value with an aggregate (sum, product, etc.) of its immediate children
- Involves direct property checks on a node rather than traversing the whole tree

## Template

```python
# Definition for a binary tree node.
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

def check_property(root: TreeNode) -> bool:
    """
    Replace this comment with the specific property check.
    """
    # Example: return root.val == root.left.val + root.right.val
    pass
```

## LeetCode Problem Walkthrough

### Problem: 2236. Root Equals Sum of Children

https://leetcode.com/problems/root-equals-sum-of-children/

### Approach 1: Direct Access (Brute Force)

**Algorithm**
Because the tree is guaranteed to have exactly three nodes, we can directly read the root’s value and the values of its left and right children, then compare.

**Implementation**

```python
# Definition for a binary tree node.
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

class Solution:
    def checkTree(self, root: TreeNode) -> bool:
        # Directly access the three nodes and test the condition.
        return root.val == root.left.val + root.right.val
```

**Complexity Analysis**

- Time complexity: O(1) — we perform a constant number of attribute accesses and one addition.
- Space complexity: O(1) — no extra data structures are used.

### Approach 2: Helper Sum (Alternative Perspective)

**Intuition**
Even though the direct check is simplest, extracting the sum of children into a helper makes the intent clearer and would scale if we ever needed to check more children (e.g., an N‑ary tree). This reinforces the pattern of separating data retrieval from the logical test.

**Algorithm**
1. Compute the sum of the left and right child values via a small helper.
2. Compare the root’s value to that sum.
3. Return the boolean result.

**Implementation**

```python
# Definition for a binary tree node.
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

class Solution:
    def checkTree(self, root: TreeNode) -> bool:
        # Helper to compute children's sum
        def children_sum(node: TreeNode) -> int:
            return (node.left.val if node.left else 0) + (node.right.val if node.right else 0)

        return root.val == children_sum(root)
```

**Complexity Analysis**

- Time complexity: O(1) — the helper still touches only the two children.
- Space complexity: O(1) — constant auxiliary space.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: `root = [10,4,6]` (root.val = 10, left.val = 4, right.val = 6)

```
| Step | Action                              | root.val | left.val | right.val | left+right | Comparison (root.val == left+right) |
| ---- | ----------------------------------- | -------- | -------- | --------- | ---------- | ------------------------------------ |
| 1    | Read root value                     | 10       | 4        | 6         | –          | –                                    |
| 2    | Read left child value               | 10       | 4        | 6         | –          | –                                    |
| 3    | Read right child value              | 10       | 4        | 6         | –          | –                                    |
| 4    | Compute left + right                | 10       | 4        | 6         | 10         | –                                    |
| 5    | Compare root vs sum                 | 10       | 4        | 6         | 10         | True (10 == 10)                      |
```

Result: `true`.

--- 

*End of lecture.*