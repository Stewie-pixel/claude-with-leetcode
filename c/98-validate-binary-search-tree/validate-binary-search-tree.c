// -----------------------------------------------------------------------------
// Problem: Validate Binary Search Tree
// Source : https://leetcode.com/problems/validate-binary-search-tree/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- IDEA ----------------------------------
//
// 1. Use recursion to validate the BST property for every node.
// 2. For each node, ensure its value lies strictly between a given range (left, right).
// 3. Recursively validate the left subtree with updated upper bound (right = root->val),
//    and the right subtree with updated lower bound (left = root->val).
// 4. If all nodes satisfy the BST property, return true.
//
// Time Complexity  : O(n) — each node is visited once.
// Space Complexity : O(h) — recursion stack, where h is the height of the tree.
// -----------------------------------------------------------------------------


struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// Recursive helper to check if the subtree is valid within given bounds
bool checkValidBST(struct TreeNode *root, long long left, long long right) {
    if(root == NULL) {
        return true;
    }

    // Check BST property: current node's value must be between left and right bounds
    if(!(left < root->val && root->val < right)) {
        return false;
    }

    // Recursively validate left and right subtrees
    return checkValidBST(root->left, left, root->val) &&
           checkValidBST(root->right, root->val, right);
}

// Main function to validate BST
bool isValidBST(struct TreeNode* root) {
    return checkValidBST(root, LONG_MIN, LONG_MAX);
}
