// -----------------------------------------------------------------------------
// Problem: Same Tree
// Source : https://leetcode.com/problems/same-tree/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- IDEA ----------------------------------
//
// 1. If both nodes are NULL → trees are identical up to this point.
// 2. If only one of them is NULL → trees differ.
// 3. If both exist but values differ → trees differ.
// 4. Otherwise, recursively check left and right subtrees.
//
// Time Complexity  : O(n)  (each node visited once)
// Space Complexity : O(h)  (h = height of the tree, recursion stack)
// -----------------------------------------------------------------------------


struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

bool isSameTree(struct TreeNode* p, struct TreeNode* q) {
    // Case 1: both nodes are NULL → same
    if(p == NULL && q == NULL) {
        return true;
    }

    // Case 2: one is NULL and the other is not → different
    if((p == NULL && q != NULL) || (q == NULL && p != NULL)) {
        return false;
    }

    // Case 3: values differ → different
    if(p->val != q->val) {
        return false;
    }

    // Case 4: recursively check left and right subtrees
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}
