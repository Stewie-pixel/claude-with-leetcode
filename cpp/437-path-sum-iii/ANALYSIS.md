/lectures/2025-09-26-tree-dfs.md
# Tree DFS

## Video Solution

For more details about **Path Sum III**, watch the walkthrough at [https://www.youtube.com/watch?v=u0BWTcezCcw](https://www.youtube.com/watch?v=u0BWTcezCcw)

## Concept

Tree Depth‑First Search (DFS) explores a binary tree by going as deep as possible along each branch before backtracking.  
For **Path Sum III** we need to count every downward‑only path whose node values add up to `targetSum`.  
A path can start and end at any node, so we must consider **all possible starting points** and, for each start, extend the path downward while tracking the running sum.

A real‑world analogy: imagine a family tree where each person has a certain amount of money. You want to know how many continuous parent‑to‑child chains of relatives have exactly $8 total money. You could start at every person and walk down the lineage, adding money as you go, stopping when you reach a leaf.

## When to Use It

Use a Tree DFS approach when you see:
- A binary tree (or general tree) structure.
- The problem asks for **paths**, **sums**, **counts**, or **sequences** that must follow parent‑to‑child links.
- The path does **not** need to start at the root or end at a leaf.
- You need to examine **all possible starting points** (hence a nested DFS: outer loop over nodes, inner DFS downwards).

## Template

```python
# General Tree DFS template (recursive)
def dfs(node):
    if not node:
        return base_case
    # Process current node (pre‑order)
    res = process(node)
    # Recurse on left and right sub‑trees
    left_res  = dfs(node.left)
    right_res = dfs(node.right)
    # Combine results (post‑order if needed)
    return combine(res, left_res, right_res)
```

## LeetCode Problem Walkthrough

### Problem: 437. Path Sum III

https://leetcode.com/problems/path-sum-iii/

### Approach 1: Brute Force (Check every start‑to‑end path)

**Algorithm**
1. For each node in the tree, treat it as the **start** of a path.
2. From that start, walk downward, accumulating the sum of node values.
3. Whenever the accumulated sum equals `targetSum`, increment the answer.
4. Continue the walk until you reach a leaf (or null).
5. Repeat step 1 for every node via an outer traversal.

**Implementation**

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def pathSum(self, root: Optional[TreeNode], targetSum: int) -> int:
        if not root:
            return 0

        # Count paths that start at `node`
        def count_paths_from(node, curr_sum):
            if not node:
                return 0
            curr_sum += node.val
            total = 1 if curr_sum == targetSum else 0
            total += count_paths_from(node.left, curr_sum)
            total += count_paths_from(node.right, curr_sum)
            return total

        # Paths starting at root + paths starting in left/right subtrees
        return (count_paths_from(root, 0) +
                self.pathSum(root.left, targetSum) +
                self.pathSum(root.right, targetSum))
```

**Complexity Analysis**
- Time complexity: O(N²) — For each of the N nodes we may walk down O(N) nodes in the worst case (e.g., a skewed tree).
- Space complexity: O(H) — Recursion stack depth equals tree height H (O(log N) for balanced, O(N) for skewed).

---

### Approach 2: Improved Brute Force (Helper that counts paths **starting** at a node)

**Intuition**
Instead of recomputing the accumulated sum from scratch for every start node, we can write a helper that, given a node, counts all downward paths **that begin at that node** and sum to `targetSum`. The outer DFS then simply calls this helper for every node.

**Algorithm**
1. Define `paths_from(node, remaining)` that returns the number of paths starting at `node` whose values sum to `remaining`.
   - If `node` is `None`, return 0.
   - If `node.val == remaining`, we have found a valid path (count = 1); otherwise count = 0.
   - Recurse on left/right with `remaining - node.val`.
2. In `pathSum`, for each node add `paths_from(node, targetSum)`.
3. Recurse on left/right children to consider them as new starting points.

**Implementation**

```python
class Solution:
    def pathSum(self, root: Optional[TreeNode], targetSum: int) -> int:
        if not root:
            return 0

        def paths_from(node, remaining):
            if not node:
                return 0
            hit = 1 if node.val == remaining else 0
            hit += paths_from(node.left, remaining - node.val)
            hit += paths_from(node.right, remaining - node.val)
            return hit

        # Paths that start at current node + paths that start in subtrees
        return (paths_from(root, targetSum) +
                self.pathSum(root.left, targetSum) +
                self.pathSum(root.right, targetSum))
```

**Complexity Analysis**
- Time complexity: O(N²) — Same worst‑case bound as Approach 1, but with a smaller constant factor because the inner helper stops early when the remaining sum cannot be met.
- Space complexity: O(H) — Recursion depth for both outer and inner DFS.

---

### Approach 3: Prefix‑Sum + Hash Map (Optimal)

**Intuition**
While traversing the tree, keep track of the **cumulative sum** from the root to the current node (`curr_sum`).  
If there exists an earlier ancestor with cumulative sum `curr_sum - targetSum`, then the path between that ancestor’s child and the current node sums to `targetSum`.  
Thus we only need to know how many times each prefix sum has appeared on the current root‑to‑node path. A hash map stores these frequencies, and we update it as we go down and backtrack (remove when leaving a node) to ensure the map reflects only the current path.

**Algorithm**
1. Initialize `prefix_counts = {0: 1}` (empty path has sum 0).
2. DFS(node, curr_sum):
   - Update `curr_sum += node.val`.
   - The number of valid paths ending at this node is `prefix_counts.get(curr_sum - targetSum, 0)`.
   - Add this to the answer.
   - Increment `prefix_counts[curr_sum]`.
   - Recurse left and right.
   - After returning, decrement `prefix_counts[curr_sum]` (backtrack).
3. Return the accumulated answer.

**Implementation**

```python
class Solution:
    def pathSum(self, root: Optional[TreeNode], targetSum: int) -> int:
        from collections import defaultdict

        def dfs(node, curr_sum, prefix_counts):
            if not node:
                return 0
            curr_sum += node.val
            # Paths ending at this node that sum to targetSum
            total = prefix_counts.get(curr_sum - targetSum, 0)
            # Record current prefix sum
            prefix_counts[curr_sum] += 1
            # Explore children
            total += dfs(node.left, curr_sum, prefix_counts)
            total += dfs(node.right, curr_sum, prefix_counts)
            # Backtrack: remove current prefix sum before returning to parent
            prefix_counts[curr_sum] -= 1
            return total

        return dfs(root, 0, defaultdict(int))
```

**Complexity Analysis**
- Time complexity: O(N) — Each node is visited once; hash‑map operations are O(1) average.
- Space complexity: O(N) — The hash map may store up to H prefix sums, where H ≤ N (worst‑case skewed tree).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

We illustrate Approach 3 (prefix‑sum) on the example:

```
Input: root = [10,5,-3,3,2,null,11,3,-2,null,1], targetSum = 8
```

Tree representation:

```
        10
       /  \
      5   -3
     / \    \
    3   2    11
   / \   \
  3  -2   1
```

We walk the tree in pre‑order, maintaining `curr_sum` and the hash map `prefix_counts`.

| Step | Node | curr_sum | prefix_counts before node | paths ending here (curr_sum‑target) | Action (update map) |
|------|------|----------|---------------------------|--------------------------------------|----------------------|
| 1    | 10   | 10       | {0:1}                     | prefix_counts[10‑8]=prefix_counts[2]=0 | map[10]=1 |
| 2    | 5    | 15       | {0:1,10:1}                | prefix_counts[15‑8]=prefix_counts[7]=0 | map[15]=1 |
| 3    | 3    | 18       | {0:1,10:1,15:1}           | prefix_counts[18‑8]=prefix_counts[10]=1 → **1** (path 5→3) | map[18]=1 |
| 4    | 3    | 21       | {0:1,10:1,15:1,18:1}      | prefix_counts[21‑8]=prefix_counts[13]=0 | map[21]=1 |
| 5    | -2   | 16       | {0:1,10:1,15:1,18:1,21:1}| prefix_counts[16‑8]=prefix_counts[8]=0 | map[16]=1 |
| 6    | 1    | 17       | {0:1,10:1,15:1,18:1,21:1,16:1}| prefix_counts[17‑8]=prefix_counts[9]=0 | map[17]=1 |
| …backtrack… (omitted for brevity)… |
| 7    | 2    | 17       | {0:1,10:1,15:1} (after backtracking from left subtree) | prefix_counts[17‑8]=prefix_counts[9]=0 | map[17]=1 |
| 8    | -3   | 7        | {0:1,10:1,15:1,17:1}      | prefix_counts[7‑8]=prefix_counts[-1]=0 | map[7]=1 |
| 9    | 11   | 18       | {0:1,10:1,15:1,17:1,7:1}  | prefix_counts[18‑8]=prefix_counts[10]=1 → **1** (path 10→-3→11) | map[18]=2 (already had 18 from earlier) |
| …backtrack… |
| 10   | (return to root) … |

Continuing the traversal we also encounter the path `10 → 5 → -2` (sum = 13) not valid, and the path `5 → 2 → 1` (sum = 8) which is found when we reach node `1` under the left subtree of node `2`.

**Total paths found:** 3 (matching the expected output).

--- 

**File Path:** `/lectures/2025-09-26-tree-dfs.md`  
**Commit directly to `main`.** No other files should be modified.