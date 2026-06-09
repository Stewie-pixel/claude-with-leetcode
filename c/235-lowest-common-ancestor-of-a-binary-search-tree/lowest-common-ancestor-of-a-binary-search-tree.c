// -----------------------------------------------------------------------------
// Problem: Lowest Common Ancestor of a Binary Search Tree
// Source : https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// Because it is a Binary Search Tree (BST):
// - All nodes in the left subtree < root->val
// - All nodes in the right subtree > root->val
//
// Algorithm:
// 1. If both p and q are greater than root → move to the right subtree.
// 2. If both p and q are smaller than root → move to the left subtree.
// 3. Otherwise, root is the Lowest Common Ancestor.
//
// Time Complexity  : O(h)   (h = height of the BST)
// Space Complexity : O(h)   (recursive stack)
// -----------------------------------------------------------------------------


struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a > b ? b : a;
}

struct TreeNode* lowestCommonAncestor(struct TreeNode* root, struct TreeNode* p, struct TreeNode* q) {
    if(root == NULL)
        return NULL;

    // If both p and q are greater than root, LCA lies in the right subtree
    if(max(p->val, q->val) > root->val && min(p->val, q->val) > root->val)
        return lowestCommonAncestor(root->right, p, q);

    // If both p and q are smaller than root, LCA lies in the left subtree
    else if(max(p->val, q->val) < root->val && min(p->val, q->val) < root->val)
        return lowestCommonAncestor(root->left, p, q);

    // Otherwise, root is the split point → this is the LCA
    else
        return root;
}
