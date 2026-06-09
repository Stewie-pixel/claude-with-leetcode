// -----------------------------------------------------------------------------
// Problem: Count Good Nodes in Binary Tree
// Source : https://leetcode.com/problems/count-good-nodes-in-binary-tree/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Traverse the tree using DFS.
// 2. Keep track of the maximum value encountered on the path from the root to the current node.
// 3. A node is "good" if its value >= max value seen so far.
// 4. Recursively count good nodes in left and right subtrees.
//
// Time Complexity  : O(n)  (visit each node once)
// Space Complexity : O(h)  (recursive stack, where h is tree height)
// -----------------------------------------------------------------------------


struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

int max(int a, int b) {
    return a > b ? a : b;
}

int findGoodNodes(struct TreeNode *root, int maxValue) {
    if(root == NULL) {
        return 0;
    }

    int numberOfGoodNodes = 0;
    if(root->val >= maxValue) {
        numberOfGoodNodes = 1;
    }

    maxValue = max(root->val, maxValue);
    numberOfGoodNodes += findGoodNodes(root->left, maxValue);
    numberOfGoodNodes += findGoodNodes(root->right, maxValue);

    return numberOfGoodNodes;
}

int goodNodes(struct TreeNode* root) {
    return findGoodNodes(root, root->val);
}
