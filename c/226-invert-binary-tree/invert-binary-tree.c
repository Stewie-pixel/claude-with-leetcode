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
