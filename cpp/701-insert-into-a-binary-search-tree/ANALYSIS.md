# Binary Search Tree

For a Binary Search Tree (BST) every node satisfies  

* all values in the left subtree `< node.val`  
* all values in the right subtree `> node.val`

To insert a new value `val` we start at the root and walk down the tree:

* If `val` < current node’s value → go to the left child.  
  *If the left child is `nullptr`, we have found the place to insert and attach a new node there.*  
* Otherwise (`val` > current node’s value) → go to the right child, inserting when the right child is `nullptr`.

Because the problem guarantees that `val` does not already exist in the tree, we will always find a `nullptr` child where the new node can be attached. The loop stops as soon as we attach the node and we return the original root (which is unchanged unless the tree was empty).

The algorithm is iterative; it uses only a pointer that walks down the tree, so the extra space is O(1).

---

### Algorithm
```
insertIntoBST(root, val):
    if root is nullptr:
        return new TreeNode(val)

    cur = root
    loop forever:
        if val < cur->val:
            if cur->left is nullptr:
                cur->left = new TreeNode(val)
                break
            else:
                cur = cur->left
        else:   // val > cur->val (guaranteed not equal)
            if cur->right is nullptr:
                cur->right = new TreeNode(val)
                break
            else:
                cur = cur->right
    return root
```

### Correctness Proof  

We prove that the algorithm returns a BST that contains all original keys plus `val`.

*Lemma 1.* At each iteration `cur` points to a node whose subtree satisfies the BST property and does **not** contain `val`.

*Proof.* Initially `cur = root`, which trivially satisfies the lemma.  
Assume the lemma holds at the start of an iteration.  
- If `val < cur->val`, any node that could contain `val` must be in the left subtree (BST property). If `cur->left` is `nullptr`, the correct place for `val` is as the left child; we insert there and stop. Otherwise we set `cur = cur->left`, which is the root of the left subtree, preserving the lemma.  
- The case `val > cur->val` is symmetric for the right subtree. ∎

*Lemma 2.* When the loop terminates, a new node containing `val` is attached as a leaf child of a `nullptr` pointer found by the search, and the resulting tree satisfies the BST property.

*Proof.* By Lemma 1, at termination `cur` points to a node where the appropriate child pointer is `nullptr` and the BST property tells us that all keys in the existing subtree are either all `< val` (if we went left) or all `> val` (if we went right). Attaching `val` as that child preserves the ordering for every node, and the new node is a leaf, so the BST property holds for the whole tree. ∎

*Theorem.* The function returns the root of a BST that contains exactly the original keys plus `val`.

*Proof.* If the input tree is empty, we return a single node with `val`, which is trivially a BST containing the new key. Otherwise, by Lemma 2 the insertion preserves the BST property and adds `val` exactly once. No other nodes are modified, so the original keys remain. Hence the returned tree is a BST containing the original set ∪ {val}. ∎

### Complexity Analysis  

*Time:* In the worst case we travel from the root to a leaf, i.e., the height of the tree. For a BST with *n* nodes the height is O(n) in the worst case (completely unbalanced) and O(log n) for a balanced tree. Thus **O(h)** where *h* is the tree height.  
*Space:* Only a few pointers are used → **O(1)** auxiliary space.

---

### Reference Implementation (C++)

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        if (!root) return new TreeNode(val);

        TreeNode* cur = root;
        while (true) {
            if (val < cur->val) {
                if (!cur->left) {
                    cur->left = new TreeNode(val);
                    break;
                }
                cur = cur->left;
            } else { // val > cur->val (guaranteed not equal)
                if (!cur->right) {
                    cur->right = new TreeNode(val);
                    break;
                }
                cur = cur->right;
            }
        }
        return root;
    }
};
```

---

### Reference Video (NeetCode)

[Insert into a Binary Search Tree - Leetcode 701 - Python](https://www.youtube.com/watch?v=Cpg8f79luEA)

*The video walks through the same iterative insertion strategy explained above.*
