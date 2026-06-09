struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

bool checkValidBST(struct TreeNode *root, long long left, long long right) {
    if(root == NULL) {
        return true;
    }

    if(!(left < root->val && root->val < right)) {
        return false;
    }

    return checkValidBST(root->left, left, root->val) &&
           checkValidBST(root->right, root->val, right);
}

bool isValidBST(struct TreeNode* root) {
    return checkValidBST(root, LONG_MIN, LONG_MAX);
}
