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
