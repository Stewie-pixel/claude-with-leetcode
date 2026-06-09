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
