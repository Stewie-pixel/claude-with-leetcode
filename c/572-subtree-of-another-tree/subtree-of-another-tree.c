struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

bool isSameTree(struct TreeNode *p, struct TreeNode *q) {
    if(p == NULL && q == NULL) {
        return true;
    }
    if((p == NULL && q != NULL) || (q == NULL && p != NULL)) {
        return false;
    }
    if(p->val != q->val) {
        return false;
    }
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}

bool isSubtree(struct TreeNode* root, struct TreeNode* subRoot) {
    if(root == NULL && subRoot == NULL) {
        return true;
    }

    return root != NULL &&
           (isSameTree(root, subRoot) ||
            isSubtree(root->left, subRoot) ||
            isSubtree(root->right, subRoot));
}
