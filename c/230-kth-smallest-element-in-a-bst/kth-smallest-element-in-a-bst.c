struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

void inorder(struct TreeNode *root, int *result, int *index, int k) {
    if(root == NULL)
        return;

    inorder(root->left, result, index, k);

    if((*index) < k) {
        *result = root->val;
        (*index)++;
    }

    inorder(root->right, result, index, k);
}

int kthSmallest(struct TreeNode* root, int k) {
    int result = root->val;
    int index = 0;

    inorder(root, &result, &index, k);
    return result;
}
