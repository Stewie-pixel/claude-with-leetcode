struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

#define MIN -1000000000

int max(int a, int b) {
    return a > b ? a : b;
}

int onePathSum(struct TreeNode *root, int *maxSum) {
    if (!root) {
        return 0;
    }

    int leftSum = max(onePathSum(root->left, maxSum), 0);
    int rightSum = max(onePathSum(root->right, maxSum), 0);

    *maxSum = max(*maxSum, root->val + leftSum + rightSum);

    return root->val + max(leftSum, rightSum);
}

int maxPathSum(struct TreeNode* root) {
    int result = MIN;
    onePathSum(root, &result);
    return result;
}
