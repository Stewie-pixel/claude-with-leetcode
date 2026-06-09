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

    return max(maxDepth(root->left), maxDepth(root->right)) + 1;
}
