# Trees – Construct Binary Tree from Parent‑Child Descriptions  

## Concept  
Think of the input as a list of **family notes**: each note tells you who is the parent of whom and whether the child is the left or right sibling.  
If you lay out all the people (nodes) on a table and keep a record of who has ever appeared as a child, the person **never listed as a child** must be the root of the family tree.  
Once you know the root, you can simply follow the notes to attach each child to its correct side (left/right) of its parent.

## When to Use It  
Use this pattern when the problem gives you explicit **parent → child** relationships (often with a left/right flag) and asks you to rebuild the original binary tree.  
Typical clues in the statement:  
- “parent[i], child[i], isLeft[i]”  
- “construct the binary tree”  
- “return its root”  
- The tree is guaranteed to be valid and node values are unique.

## Template (Python)  
```python
# Definition for a binary tree node (LeetCode provides this)
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

def build_tree(descriptions):
    """
    descriptions: List[List[int]]  # [parent, child, isLeft]
    returns: TreeNode | None  # root of the reconstructed tree
    """
    # 1️⃣  Map value → actual TreeNode object (create on demand)
    nodes = {}
    # 2️⃣  Keep track of every value that appears as a child
    children = set()

    for parent_val, child_val, is_left in descriptions:
        # create nodes if we haven't seen them yet
        if parent_val not in nodes:
            nodes[parent_val] = TreeNode(parent_val)
        if child_val not in nodes:
            nodes[child_val] = TreeNode(child_val)

        # attach child to the correct side of its parent
        if is_left == 1:          # left child
            nodes[parent_val].left = nodes[child_val]
        else:                     # right child
            nodes[parent_val].right = nodes[child_val]

        # remember that child_val has a parent
        children.add(child_val)

    # 3️⃣  The root is the node that never appeared as a child
    for val in nodes:
        if val not in children:
            return nodes[val]      # root found
    return None                    # should never happen for a valid input
```

---

## LeetCode Problem Walkthrough  

### Problem: 2196. Create Binary Tree From Descriptions  
https://leetcode.com/problems/create-binary-tree-from-descriptions/

---

### Approach 1: Brute‑Force (O(N²) time, O(N) space)  
**Algorithm**  
1. For each description, create a node for the parent and child if they don’t already exist (using a simple list or dict).  
2. To find the root, repeatedly scan all nodes and check whether any other node points to it as a child.  
   - The node that never appears as a child in any description is the root.  
3. After the root is known, a second pass attaches each child to its parent using the left/right flag.

**Implementation**  
```python
class Solution:
    def createBinaryTree(self, descriptions: List[List[int]]) -> Optional[TreeNode]:
        # step 1 – create all nodes
        nodes = {}
        for p, c, _ in descriptions:
            if p not in nodes:
                nodes[p] = TreeNode(p)
            if c not in nodes:
                nodes[c] = TreeNode(c)

        # step 2 – find root by checking who never appears as a child
        children = {c for _, c, _ in descriptions}
        root_val = next(val for val in nodes if val not in children)
        root = nodes[root_val]

        # step 3 – attach children (second pass)
        for p, c, is_left in descriptions:
            if is_left == 1:
                nodes[p].left = nodes[c]
            else:
                nodes[p].right = nodes[c]

        return root
```

**Complexity Analysis**  
- Time complexity: **O(N²)** – the root search scans all nodes for each description (or we could say O(N) to build the set + O(N) to find root, but the naive “scan for parent” would be O(N²); we keep the set version to stay O(N) but still label it as a simple brute‑force idea).  
- Space complexity: **O(N)** – we store a node for each distinct value.

> *Note*: Even this “brute‑force” version is already linear thanks to the hash‑set for children; the real brute‑force would be to avoid the set and repeatedly search the list, which is why we present a slightly optimized version as Approach 1 and then show the truly optimal method next.

---

### Approach 2: Optimal Single‑Pass (O(N) time, O(N) space)  
**Intuition**  
While we are creating nodes and linking them, we can simultaneously record which values have ever appeared as a child.  
After processing all descriptions, the root is simply the node whose value never appeared in that child set.  
No second scan for linking is needed because we attached children on the fly.

**Algorithm**  
1. Initialise an empty map `val → TreeNode` and an empty set `children`.  
2. Iterate over each description `[parent, child, isLeft]`:  
   - Ensure both parent and child nodes exist in the map (create if missing).  
   - Attach the child to the parent’s left or right pointer according to `isLeft`.  
   - Add `child` to the `children` set.  
3. After the loop, iterate over the map keys; the key not present in `children` is the root value. Return its node.

**Implementation**  
```python
class Solution:
    def createBinaryTree(self, descriptions: List[List[int]]) -> Optional[TreeNode]:
        nodes = {}          # value → TreeNode
        children = set()    # all values that have a parent

        for parent, child, is_left in descriptions:
            # create nodes lazily
            if parent not in nodes:
                nodes[parent] = TreeNode(parent)
            if child not in nodes:
                nodes[child] = TreeNode(child)

            # link child to parent
            if is_left == 1:
                nodes[parent].left = nodes[child]
            else:
                nodes[parent].right = nodes[child]

            # remember that `child` has a parent
            children.add(child)

        # find the root: the node that never appeared as a child
        for val in nodes:
            if val not in children:
                return nodes[val]

        return None   # unreachable for a valid input
```

**Complexity Analysis**  
- Time complexity: **O(N)** – one pass to build the tree and record children, plus a final O(N) scan to locate the root.  
- Space complexity: **O(N)** – we store a node for each distinct value and a set of children.

---

### Provide a Visual Demonstration  

**Dry Run** (using Example 1)  

`descriptions = [[20,15,1],[20,17,0],[50,20,1],[50,80,0],[80,19,1]]`

| Step | parent | child | isLeft | nodes created/updated | children set after step |
|------|--------|-------|--------|-----------------------|--------------------------|
| 1    | 20     | 15    | 1      | 20→node, 15→node; 20.left = 15 | {15} |
| 2    | 20     | 17    | 0      | 20 already, 17→node; 20.right = 17 | {15, 17} |
| 3    | 50     | 20    | 1      | 50→node, 20 already; 50.left = 20 | {15, 17, 20} |
| 4    | 50     | 80    | 0      | 50 already, 80→node; 50.right = 80 | {15, 17, 20, 80} |
| 5    | 80     | 19    | 1      | 80 already, 19→node; 80.left = 19 | {15, 17, 20, 80, 19} |

After processing all rows, `nodes` contains `{20,15,17,50,80,19}`.  
Scanning for a value **not** in `children` yields `50` → root.

The final tree matches the expected output `[50,20,80,15,17,19]` (level‑order).

---

**Summary**  
- Identify the pattern: explicit parent‑child links → reconstruct tree.  
- Use a hashmap to store nodes and a set to track which values ever appear as a child.  
- One linear pass builds the tree; a final linear scan (or set difference) finds the root.  
- Always state time/space complexity and walk through at least one concrete example.  

--- 

*Save the above as a GitHub issue (or lecture file) following the naming convention `YYYY-MM-DD-trees.md` under `/lectures/`. Ensure the file matches the existing lecture style and does not duplicate any prior topic.*