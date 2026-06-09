// -----------------------------------------------------------------------------
// Problem: Maximum Depth of Binary Tree
// Source : https://leetcode.com/problems/maximum-depth-of-binary-tree/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. If the current node is NULL → depth = 0.
// 2. Recursively find the depth of the left and right subtrees.
// 3. The depth of the current node = max(leftDepth, rightDepth) + 1.
//
// Time Complexity  : O(n)  (each node visited once)
// Space Complexity : O(h)  (h = height of the tree, due to recursion)
// -----------------------------------------------------------------------------


struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

int max(int a, int b) {
    return a > b ? a : b;
}

int maxDepth(struct TreeNode* root) {
    if(root == NULL) {
        return 0;
    }
    // Compute max depth of left and right subtrees, then add 1 for the current node
    return max(maxDepth(root->left), maxDepth(root->right)) + 1;
}
