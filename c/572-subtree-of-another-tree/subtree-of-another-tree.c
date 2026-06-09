// -----------------------------------------------------------------------------
// Problem: Subtree of Another Tree
// Source : https://leetcode.com/problems/subtree-of-another-tree/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Use a helper function `isSameTree()` to check if two trees are identical.
// 2. For each node in the main tree `root`, check whether the subtree starting 
//    at that node is identical to `subRoot`.
// 3. Recursively check left and right subtrees if not identical at current node.
//
// Time Complexity  : O(m * n) in worst case, where m and n are node counts.
// Space Complexity : O(h), where h is the height of the tree (recursion stack).
// -----------------------------------------------------------------------------


struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// Helper function to check if two trees are identical
bool isSameTree(struct TreeNode *p, struct TreeNode *q) {
    if(p == NULL && q == NULL) {
        return true;
    }
    if((p == NULL && q != NULL) || (q == NULL && p != NULL)) {
        return false;
    }
    if(p->val != q->val) {
        return false;
    }
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}

// Main function to check if subRoot is a subtree of root
bool isSubtree(struct TreeNode* root, struct TreeNode* subRoot) {
    if(root == NULL && subRoot == NULL) {
        return true;
    }

    return root != NULL && 
           (isSameTree(root, subRoot) || 
            isSubtree(root->left, subRoot) || 
            isSubtree(root->right, subRoot));
}
