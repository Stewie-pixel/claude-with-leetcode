// -----------------------------------------------------------------------------
// Problem: Balanced Binary Tree
// Source : https://leetcode.com/problems/balanced-binary-tree/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- IDEA ----------------------------------
//
// 1. Recursively compute the height of each subtree.
// 2. At every node, check whether the height difference between the left and right 
//    subtrees is at most 1.
// 3. If any subtree is unbalanced, propagate false upwards.
// 4. Otherwise, if all nodes satisfy the balance condition, return true.
//
// Time Complexity  : O(n²) in worst case (due to repeated height calculations).
// Space Complexity : O(h) where h is the height of the tree (recursive stack).
// -----------------------------------------------------------------------------



struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

int max(int a, int b) {
    return a > b ? a : b;
}

int getHeight(struct TreeNode *root) {
    if(root == NULL) {
        return 0;
    }
    return max(getHeight(root->left), getHeight(root->right)) + 1;
}

bool isBalanced(struct TreeNode* root) {
    if(root == NULL) {
        return 1;
    }
    int heightLeft = getHeight(root->left);
    int heightRight = getHeight(root->right);
    int balance = heightLeft - heightRight;
    if(balance < -1 || balance > 1) {
        return 0;
    }
    return isBalanced(root->left) && isBalanced(root->right);
}
