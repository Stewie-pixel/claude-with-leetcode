# Tree DFS

## Video Solution

For more details about **Longest Univalue Path**, watch the walkthrough at [Leetcode - 687 . Longest Univalue Path | DFS](https://www.youtube.com/watch?v=-67v9vJGs6Y)

## Concept

The longest univalue path in a binary tree is the greatest number of edges connecting nodes that all share the same value.  
Because the path must be continuous, it can only go **down** from a node to its descendants, but the overall longest path may bend at a node (using both left and right arms).  

A depth‑first search (DFS) naturally computes, for each node, the longest univalue arm that starts at that node and goes downward. While returning from recursion we can combine the left and right arms to see if a longer “through‑node” path exists.

## When to Use It

Use a tree‑DFS approach when you see:

* A binary tree (or general tree) where you need information from subtrees.
* The answer depends on combining results from left and right children (e.g., path length, diameter, sum).
* The problem asks for a property that must hold for **all nodes on a path** (same value, increasing sequence, etc.).
* You need to compute a value that can be expressed as a function of the node’s value and the same‑value arms from its children.

## Template

```python
# Generic DFS that returns information needed from a subtree
def dfs(node):
    if not node:
        return base_case          # e.g., 0 for length, None for pointer, etc.

    left  = dfs(node.left)        # recurse left
    right = dfs(node.right)       # recurse right

    # Combine children's results to compute answer for this node
    # Update a nonlocal/global variable if the optimum can pass through `node`
    # Return the value that the parent needs (e.g., longest arm from this node)

    return value_for_parent
```

## LeetCode Problem Walkthrough

### Problem: 687. Longest Univalue Path

https://leetcode.com/problems/longest-univalue-path/

---

### Approach 1: Brute Force – Check Every Node as Path Center

**Algorithm**

1. For each node in the tree, treat it as the “highest” node of a candidate path.
2. From that node, compute the longest univalue arm going left (only through children with the same value) and the longest arm going right.
3. The path length using this node as the pivot is `left_arm + right_arm`.
4. Keep the maximum over all nodes.
5. To compute an arm, we simply walk down while the child values match the starting node’s value (a simple helper DFS).

**Implementation**

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def longestUnivaluePath(self, root: Optional[TreeNode]) -> int:
        def arm_length(node, parent_val):
            """Return length of univalue arm starting at `node` going downward,
               assuming `node.val == parent_val`."""
            if not node or node.val != parent_val:
                return 0
            # extend through left or right child
            return 1 + max(arm_length(node.left, parent_val),
                           arm_length(node.right, parent_val))

        def dfs(node):
            if not node:
                return 0
            # arms where current node is the highest point
            left_arm  = arm_length(node.left,  node.val) if node.left  else 0
            right_arm = arm_length(node.right, node.val) if node.right else 0
            # best path that uses this node as the turning point
            self.ans = max(self.ans, left_arm + right_arm)
            # recurse to consider other nodes as potential centers
            dfs(node.left)
            dfs(node.right)

        self.ans = 0
        dfs(root)
        return self.ans
```

**Complexity Analysis**

- Time complexity: O(N²) — for each of the N nodes we may traverse O(N) nodes to compute its arms in the worst case (e.g., a chain of identical values).
- Space complexity: O(H) — recursion stack where H is tree height (worst‑case O(N)).

---

### Approach 2: Single‑Pass DFS (Global Answer)

**Intuition**

Instead of recomputing arms for every node, we can compute, in one post‑order traversal, the longest univalue arm that *starts* at the current node and goes downward.  
While returning from a child we already know the child's arm length; if the child's value matches the current node, we can extend that arm by one edge.  
The longest path that passes through the current node is then the sum of the left and right arms (both possibly zero). Keeping a global maximum yields the answer in linear time.

**Algorithm**

1. Recursively DFS each node.
2. For a node, obtain `left_len` = longest univalue arm from its left child (only if child.val == node.val), similarly `right_len`.
3. Update the global answer with `left_len + right_len`.
4. Return `max(left_len, right_len)` to the parent — this is the longest arm that can be extended upward through this node.
5. Leaf nodes return 0.

**Implementation**

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def longestUnivaluePath(self, root: Optional[TreeNode]) -> int:
        self.best = 0                     # stores the longest path found (in edges)

        def dfs(node):
            if not node:
                return 0

            left  = dfs(node.left)        # longest arm from left subtree
            right = dfs(node.right)       # longest arm from right subtree

            # If left child exists and matches node.val, we can extend the arm
            left_arm  = left + 1 if node.left  and node.left.val  == node.val else 0
            # Same for right child
            right_arm = right + 1 if node.right and node.right.val == node.val else 0

            # Path that uses node as the highest point
            self.best = max(self.best, left_arm + right_arm)

            # Return the longest arm that can be passed to the parent
            return max(left_arm, right_arm)

        dfs(root)
        return self.best
```

**Complexity Analysis**

- Time complexity: O(N) — each node is visited once.
- Space complexity: O(H) — recursion stack, O(N) in worst case (skewed tree), O(log N) for balanced.

---

### Approach 3: Iterative Post‑Order Using Explicit Stack

**Intuition**

The recursive solution can be transformed into an iterative one to avoid call‑stack limits on very deep trees.  
We perform a post‑order traversal with two stacks (or one stack + visited flag). For each node we need the arm lengths of its left and right children, which we store in a hash map (`arm[node]`). After processing children we compute the current node's arm and update the global answer exactly as in the recursive version.

**Algorithm**

1. Push `(root, False)` onto a stack (`False` = children not processed yet).
2. While stack not empty:
   * Pop `(node, visited)`.
   * If `node` is `None`, continue.
   * If not visited:
        - Push `(node, True)` back (to process after children).
        - Push `(node.right, False)` and `(node.left, False)`.
   * Else (children already processed):
        - Retrieve left arm length from `arm.get(node.left, 0)` if child exists and matches value, else 0 (add 1 for the edge).
        - Same for right.
        - Update global answer with `left_arm + right_arm`.
        - Store `arm[node] = max(left_arm, right_arm)` for parent use.
3. Return the global answer.

**Implementation**

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def longestUnivaluePath(self, root: Optional[TreeNode]) -> int:
        if not root:
            return 0

        stack = [(root, False)]          # (node, children_processed?)
        arm   = {}                        # node -> longest univalue arm starting at node
        best  = 0

        while stack:
            node, visited = stack.pop()
            if not node:
                continue

            if not visited:
                # Post‑order: children first
                stack.append((node, True))
                stack.append((node.right, False))
                stack.append((node.left, False))
            else:
                # Children have been processed; compute arm lengths
                left_arm  = arm.get(node.left, 0) + 1 if node.left  and node.left.val  == node.val else 0
                right_arm = arm.get(node.right, 0) + 1 if node.right and node.right.val == node.val else 0

                best = max(best, left_arm + right_arm)
                arm[node] = max(left_arm, right_arm)

        return best
```

**Complexity Analysis**

- Time complexity: O(N) — each node pushed/popped a constant number of times.
- Space complexity: O(N) — explicit stack plus the `arm` dictionary (can be optimized to O(H) with a visited‑flag trick, but O(N) is fine for the constraints).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the tree from Example 1:

```
Input: root = [5,4,5,1,1,null,5]

          5
        /   \
       4     5
      / \     \
     1   1     5
```

We’ll show the recursive Approach 2 (single‑pass DFS).

#### Dry Run

| Step | node | node.val | left returned | right returned | left_arm (if match) | right_arm (if match) | best updated | value returned to parent |
|------|------|----------|---------------|----------------|----------------------|----------------------|--------------|--------------------------|
| 1    | 1 (left‑left) | 1 | 0 | 0 | 0 (no match with parent 4) | 0 | best = 0 | 0 |
| 2    | 1 (left‑right) | 1 | 0 | 0 | 0 (no match with parent 4) | 0 | best = 0 | 0 |
| 3    | 4 (left child) | 4 | 0 (from left‑left) | 0 (from left‑right) | 0 (4≠1) | 0 (4≠1) | best = 0 | 0 |
| 4    | 5 (right‑right) | 5 | 0 | 0 | 0 (no child) | 0 | best = 0 | 0 |
| 5    | 5 (right child) | 5 | 0 (left null) | 0 (from right‑right) | 0 (left null) | 1 (5==5 → 0+1) | best = max(0,0+1)=1 | max(0,1)=1 |
| 6    | 5 (root) | 5 | 0 (from left subtree) | 1 (from right subtree) | 1 (5==5 → 0+1) | 2 (5==5 → 1+1) | best = max(1, 1+2)=3 → **but note:** we count edges, so path length = left_arm + right_arm = 1 + 2 = 3 edges? Wait: left_arm=1 (edge root‑>right child), right_arm=2 (edges root‑>right child‑>right‑grandchild). The path that goes left‑arm + right_arm would be left arm (root‑>right child) + right arm (root‑>right‑child‑>grandchild) which actually double‑counts the root‑>right child edge. The correct formula for the path **through** the node is left_arm + right_arm where each arm counts edges **downward** from the node. Here left_arm = 1 (root→right child), right_arm = 2 (root→right child→grandchild). The combined path would be left_arm + right_arm = 3 edges, which corresponds to the path grandchild‑>right child‑>root‑>right child? That's not a simple path. The algorithm’s standard interpretation is that left_arm and right_arm are the lengths of the longest univalue arms **starting** at the node and going down left/right respectively. The longest path that passes through the node is left_arm + right_arm (edges). For this tree, the longest univalue path is actually the two edges on the right side: root‑>right child‑>right‑grandchild (value 5). That length is 2 edges, captured when we consider the node = right child: its left_arm=0, right_arm=1 → best = max(previous, 0+1)=1; then at root we compute left_arm=1 (root→right child) and right_arm=0 (since root’s right child’s value matches but we only take the arm from child, not grandchild). Actually we need to re‑evaluate: The algorithm returns to parent the max arm length (not necessarily the arm that includes the parent’s value if child matches). Let's redo with correct values: 

We’ll instead present a cleaner dry run using the example from LeetCode’s official solution (tree with root 5, left 4, right 5, left’s children 1,1, right’s right child 5). The longest univalue path is 2 (the two 5‑edges on the right). 

Below is a corrected dry run focusing on the nodes that matter.

| Step | node | node.val | left_ret | right_ret | left_arm (if match) | right_arm (if match) | best after this node | returned to parent |
|------|------|----------|----------|-----------|----------------------|----------------------|----------------------|--------------------|
| 1 | left‑left (1) | 1 | 0 | 0 | 0 | 0 | 0 | 0 |
| 2 | left‑right (1) | 1 | 0 | 0 | 0 | 0 | 0 | 0 |
| 3 | left (4) | 4 | 0 | 0 | 0 (4≠1) | 0 (4≠1) | 0 | 0 |
| 4 | right‑right (5) | 5 | 0 | 0 | 0 | 0 | 0 | 0 |
| 5 | right (5) | 5 | 0 (left null) | 0 (from right‑right) | 0 (no left) | 1 (5==5 → 0+1) | best = max(0,0+1)=1 | max(0,1)=1 |
| 6 | root (5) | 5 | 0 (from left subtree) | 1 (from right subtree) | 1 (5==5 → 0+1) | 0 (5==5? child val=5 matches, but arm from child is 1, we add 1 only if child.val == node.val → yes, so right_arm = 1+1 =2? Wait: child returned 1 meaning the longest arm starting at right child is 1 edge (right child→its right child). To extend through root we add edge root‑>right child, so right_arm = child_ret + 1 = 1+1 =2) | best = max(1, left_arm + right_arm) = max(1, 1+2)=3? That seems off. Let's step back: The standard formula is: 
   left_arrow = left_ret + 1 if node.left and node.left.val == node.val else 0
   right_arrow = right_ret + 1 if node.right and node.right.val == node.val else 0
   ans = max(ans, left_arrow + right_arrow)
   return max(left_arrow, right_arrow)

Using that:
- For node right (5): left_ret=0, right_ret=0 → left_arrow=0, right_arrow=1 (since right child exists and matches) → ans = max(0,0+1)=1 → return max(0,1)=1.
- For root (5): left_ret=0, right_ret=1 → left_arrow = 0 (left child val 4 !=5) → 0; right_arrow = 1+1 =2 (right child exists and matches) → ans = max(previous ans 1, 0+2)=2 → return max(0,2)=2.

Thus final ans = 2, which matches expected output.

We'll present the dry run using this correct formula.

#### Correct Dry Run (using the standard formula)

| Step | node | node.val | left_ret | right_ret | left_arrow | right_arrow | ans after node | return value |
|------|------|----------|----------|-----------|------------|-------------|----------------|--------------|
| 1 | 1 (LL) | 1 | 0 | 0 | 0 | 0 | 0 | 0 |
| 2 | 1 (LR) | 1 | 0 | 0 | 0 | 0 | 0 | 0 |
| 3 | 4 (L) | 4 | 0 | 0 | 0 (4≠1) | 0 (4≠1) | 0 | 0 |
| 4 | 5 (RR) | 5 | 0 | 0 | 0 | 0 | 0 | 0 |
| 5 | 5 (R) | 5 | 0 (null) | 0 (RR) | 0 | 1 (5==5 → 0+1) | ans = max(0,0+1)=1 | max(0,1)=1 |
| 6 | 5 (root) | 5 | 0 (L) | 1 (R) | 0 (5≠4) | 2 (5==5 → 1+1) | ans = max(1,0+2)=2 | max(0,2)=2 |

**Result:** `ans = 2` → longest univalue path length = 2 edges (the right‑hand 5‑5‑5 chain).

---

**Summary of this lecture**

* Tree‑DFS lets us compute, in one pass, the longest univalue arm from each node.
* By combining left and right arms we obtain the best path that bends at a node.
* The brute‑force method works but is quadratic; the single‑pass DFS is optimal (O(N) time, O(H) space).
* The same pattern applies to many tree‑diameter‑style problems (e.g., diameter of binary tree, longest consecutive sequence, etc.).

Commit this file as `/lectures/YYYY-MM-DD-tree-dfs.md` (using today’s date) and continue tomorrow with the next topic in the rotation.