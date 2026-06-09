// -----------------------------------------------------------------------------
// Problem: Diameter of Binary Tree
// Source : https://leetcode.com/problems/diameter-of-binary-tree/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- IDEA ----------------------------------
//
// 1. Use DFS to compute the depth of each node.
// 2. For each node, the longest path that passes through it equals:
//        left subtree height + right subtree height.
// 3. Keep track of the maximum of all such paths using a global (or pointer) variable.
// 4. The recursion returns the height of the current node to its parent.
//
// Time Complexity  : O(n)  (each node visited once)
// Space Complexity : O(h)  (recursive stack, where h is the height of the tree)
// -----------------------------------------------------------------------------


struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

int max(int a, int b) {
    return a > b ? a : b;
}

int dfs(struct TreeNode *root, int *answer) {
    if(root == NULL) {
        return 0;
    }

    int left = dfs(root->left, answer);
    int right = dfs(root->right, answer);

    *answer = max(*answer, left + right);
    return max(left, right) + 1;
}

int diameterOfBinaryTree(struct TreeNode* root) {
    int answer = 0;
    dfs(root, &answer);
    return answer;
}
