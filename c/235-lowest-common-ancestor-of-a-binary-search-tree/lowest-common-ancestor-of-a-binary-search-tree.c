struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a > b ? b : a;
}

struct TreeNode* lowestCommonAncestor(struct TreeNode* root, struct TreeNode* p, struct TreeNode* q) {
    if(root == NULL)
        return NULL;

    if(max(p->val, q->val) > root->val && min(p->val, q->val) > root->val)
        return lowestCommonAncestor(root->right, p, q);

    else if(max(p->val, q->val) < root->val && min(p->val, q->val) < root->val)
        return lowestCommonAncestor(root->left, p, q);

    else
        return root;
}
