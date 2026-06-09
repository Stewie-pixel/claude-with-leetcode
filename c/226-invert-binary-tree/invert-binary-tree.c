// -----------------------------------------------------------------------------
// Problem: Invert Binary Tree
// Source : https://leetcode.com/problems/invert-binary-tree/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. If the node is NULL, return NULL.
// 2. Swap the left and right child of the current node.
// 3. Recursively apply this operation on both children.
// 4. Return the root after full inversion.
//
// Time Complexity  : O(n)  (each node is visited once)
// Space Complexity : O(h)  (recursive stack, where h is the height of the tree)
// -----------------------------------------------------------------------------


struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode* invertTree(struct TreeNode* root) {
    if(!root)
        return NULL;

    struct TreeNode *right = root->right;
    root->right = root->left;
    root->left = right;

    invertTree(root->left);
    invertTree(root->right);

    return root;
}
