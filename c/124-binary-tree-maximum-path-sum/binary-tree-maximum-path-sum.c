// -----------------------------------------------------------------------------
// Problem: Binary Tree Maximum Path Sum
// Source : https://leetcode.com/problems/binary-tree-maximum-path-sum/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- IDEA ----------------------------------
//
// 1. Use a recursive DFS helper function (`onePathSum`) to calculate the maximum
//    path sum that starts from each node and goes downward.
// 2. At each node, compute the maximum contribution from left and right subtrees,
//    but ignore negative sums (replace with 0).
// 3. Update the global maximum (`maxSum`) with the sum that passes through the
//    current node (root->val + leftSum + rightSum).
// 4. Return the maximum single-path sum starting from this node to be used by
//    its parent (root->val + max(leftSum, rightSum)).
//
// Time Complexity  : O(n)   — visit each node once
// Space Complexity : O(h)   — recursion stack (h = tree height)
// -----------------------------------------------------------------------------


// Definition for a binary tree node.
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

#define MIN -1000000000  

int max(int a, int b) {
    return a > b ? a : b;
}

// Helper function: returns maximum one-path sum from current node downwards
int onePathSum(struct TreeNode *root, int *maxSum) {
    if (!root) {
        return 0;
    }

    // Compute max path sum from left and right child (ignore negatives)
    int leftSum = max(onePathSum(root->left, maxSum), 0);
    int rightSum = max(onePathSum(root->right, maxSum), 0);

    // Update global maxSum if path through current node is better
    *maxSum = max(*maxSum, root->val + leftSum + rightSum);

    // Return max one-side path sum to be used by parent
    return root->val + max(leftSum, rightSum);
}

// Main function: returns the maximum path sum in the tree
int maxPathSum(struct TreeNode* root) {
    int result = MIN;
    onePathSum(root, &result);
    return result;
}
