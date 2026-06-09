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
