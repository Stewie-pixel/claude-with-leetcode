# Tree DFS

## Video Solution

For more details about **Longest ZigZag Path in a Binary Tree**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=hbzdyIlvBKI)

## Concept

Depth‑First Search (DFS) lets us visit every node once while carrying extra information about the path we have taken so far.  
For the ZigZag problem we need to know, for each node, how long a ZigZag path can be if we **arrive at this node coming from the left** (meaning the next step must go right) and if we **arrive coming from the right** (next step must go left). By propagating these two lengths upward we can compute the answer in a single traversal.

A real‑world analogy: imagine a person walking through a maze where they must alternate turning left and right at each intersection. At each intersection they remember how many steps they have taken if they turned left last time versus right last time. The longest alternating walk they can achieve is the answer.

## When to Use It

Use a tree‑DFS with state when you see:

- A binary tree and a property that depends on the **direction of movement** (left/right, up/down, etc.).
- The need to compute a value that can be built from the values of the left and right sub‑trees (post‑order style).
- Problems asking for the longest/shortest path that follows a rule alternating between two choices (e.g., ZigZag, alternating parity, etc.).

## Template

```python
# Generic DFS that returns information needed from sub‑trees.
def dfs(node):
    if not node:
        # base case – return neutral values for this problem
        return (default_left, default_right)

    # Recurse on children
    left_info  = dfs(node.left)
    right_info = dfs(node.right)

    # Combine children's info with current node to produce
    # the information this node should return to its parent.
    my_info = combine(left_info, right_info, node)

    # Optionally update a global answer here.
    update_answer(my_info)

    return my_info
```

## LeetCode Problem Walkthrough

### Problem: 1372. Longest ZigZag Path in a Binary Tree
https://leetcode.com/problems/longest-zigzag-path-in-a-binary-tree/

---

### Approach 1: Brute Force (Try Every Starting Point)

**Algorithm**
1. For every node in the tree, treat it as the start of a ZigZag path.
2. From that start, walk alternately left → right → left … while counting steps.
   - Do the same walk starting with a right step.
3. Keep the maximum length seen.
4. The walk from a given start can be done with a simple loop or recursion that follows the forced direction.

**Implementation**
```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def longestZigZag(self, root: Optional[TreeNode]) -> int:
        def walk(node: Optional[TreeNode], go_left: bool, steps: int) -> int:
            """Continue ZigZag from `node` in the indicated direction."""
            if not node:
                return steps
            if go_left:
                return walk(node.left, False, steps + 1)
            else:
                return walk(node.right, True, steps + 1)

        def dfs(node: Optional[TreeNode]) -> None:
            if not node:
                return
            # Try starting with a left step and a right step from this node
            nonlocal best
            best = max(best,
                       walk(node.left,  False, 1),   # first step left
                       walk(node.right, True,  1))   # first step right
            dfs(node.left)
            dfs(node.right)

        best = 0
        dfs(root)
        return best
```

**Complexity Analysis**
- Time complexity: O(N²) – For each of the N nodes we may walk down O(N) nodes in the worst case (e.g., a skewed tree).
- Space complexity: O(H) – recursion stack for the outer DFS, where H is the tree height (O(N) worst case).

---

### Approach 2: DFS Returning Two Lengths (Optimal)

**Intuition**
At each node we only need to know two values:
- `left_len`: longest ZigZag path that starts at this node and **first goes to the left child**.
- `right_len`: longest ZigZag path that starts at this node and **first goes to the right child**.

If we know these values for the left and right children, we can compute them for the current node:
- To go left first, we must then go right from the left child → `1 + right_len_of_left_child`.
- To go right first, we must then go left from the right child → `1 + left_len_of_right_child`.

While traversing we keep a global maximum of all `left_len` and `right_len` encountered.

**Algorithm**
1. Run a post‑order DFS.
2. For a `None` node return `(0, 0)` (no path can start from a null child).
3. For a real node:
   - Recursively get `(ll, rl)` from left child and `(lr, rr)` from right child.
   - `cur_left  = 1 + rl`   (go left then must go right)
   - `cur_right = 1 + lr`   (go right then must go left)
   - Update global answer with `cur_left` and `cur_right`.
   - Return `(cur_left, cur_right)` to the parent.
4. The answer is the maximum length seen; note that length counts **edges**, which matches the problem definition (nodes‑1).

**Implementation**
```python
class Solution:
    def longestZigZag(self, root: Optional[TreeNode]) -> int:
        self.best = 0

        def dfs(node: Optional[TreeNode]) -> tuple[int, int]:
            if not node:
                return (0, 0)                     # (left_len, right_len) from a null child

            left_left,  left_right  = dfs(node.left)   # from left child
            right_left, right_right = dfs(node.right)  # from right child

            # If we start by going left from this node, next step must go right
            cur_left  = 1 + left_right
            # If we start by going right from this node, next step must go left
            cur_right = 1 + right_left

            self.best = max(self.best, cur_left, cur_right)
            return (cur_left, cur_right)

        dfs(root)
        return self.best
```

**Complexity Analysis**
- Time complexity: O(N) – each node visited once.
- Space complexity: O(H) – recursion stack, O(N) worst case for a skewed tree, O(log N) for balanced.

---

### Approach 3: Iterative DFS with Explicit Stack

**Intuition**
The same two‑state logic can be executed iteratively to avoid recursion limits on deep trees. We keep a stack of tuples `(node, left_len_from_parent, right_len_from_parent, visited_flag)`. When we first see a node we push its children with the appropriate inherited lengths; on the second pop (post‑order) we compute the node’s own lengths and update the answer.

**Algorithm**
1. Initialise stack with `(root, 0, 0, False)`.
2. While stack not empty:
   - Pop top.
   - If node is `None`: continue.
   - If `visited_flag` is `False`:
        - Push `(node, left_len, right_len, True)` to process after children.
        - For left child: the length if we *arrive* at left child having come from a **right** step is `1 + right_len` (because we would go left then must go right). Push `(node.left, 0, 1 + right_len, False)`.
        - For right child: similarly push `(node.right, 1 + left_len, 0, False)`.
   - Else (visited_flag True): we have both children's results already incorporated in the lengths passed down; compute `cur_left = left_len`, `cur_right = right_len` (these are the longest ZigZag starting at this node going left/right). Update global answer.
3. Return answer.

**Implementation**
```python
class Solution:
    def longestZigZag(self, root: Optional[TreeNode]) -> int:
        if not root:
            return 0

        best = 0
        stack = [(root, 0, 0, False)]   # (node, left_len, right_len, processed_children)

        while stack:
            node, left_len, right_len, processed = stack.pop()
            if not node:
                continue
            if not processed:
                # Post‑order: push self again after children
                stack.append((node, left_len, right_len, True))
                # Left child: if we go to left child, the next step must be right,
                # so we pass a right‑length of 1 + current right_len
                stack.append((node.left, 0, 1 + right_len, False))
                # Right child: next step must be left
                stack.append((node.right, 1 + left_len, 0, False))
            else:
                # At this point left_len/right_len are the longest ZigZag
                # starting at this node going left/right respectively.
                best = max(best, left_len, right_len)

        return best
```

**Complexity Analysis**
- Time complexity: O(N) – each node pushed and popped a constant number of times.
- Space complexity: O(N) – explicit stack may hold up to O(N) nodes in the worst case.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimal recursive solution on the tree from Example 1.

```
Input: [1,null,1,1,1,null,null,1,1,null,1,null,null,null,1]
```

Schematic (only relevant nodes shown):

```
        1
         \
          1
         / \
        1   1
           / \
          1   1
         /     \
        1       1
                 \
                  1
```

We'll follow the DFS that returns `(left_len, right_len)` for each node.

| Node (val) | left child result (ll, lr) | right child result (rl, rr) | cur_left = 1 + lr | cur_right = 1 + rl | best so far |
|------------|----------------------------|-----------------------------|-------------------|--------------------|-------------|
| Leaf (bottom‑most 1) | (0,0) | (0,0) | 1 | 1 | 1 |
| Node just above leaf (val=1, left=null, right=leaf) | (0,0) | (1,1) | 1 + 1 = 2 | 1 + 0 = 1 | 2 |
| Node (val=1, left=leaf, right=null) | (1,1) | (0,0) | 1 + 0 = 1 | 1 + 1 = 2 | 2 |
| Node (val=1, left=null, right=subtree of height 2) | (0,0) | (2,1) *(from previous row)* | 1 + 1 = 2 | 1 + 2 = 3 | **3** |
| … (propagating upward) … | … | … | … | … | 3 (remains max) |

The maximum `best` encountered is **3**, which matches the expected output (path right → left → right, i.e., 3 edges).

---