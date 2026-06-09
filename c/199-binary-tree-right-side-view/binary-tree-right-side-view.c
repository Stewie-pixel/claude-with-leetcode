struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

void traversal(struct TreeNode *root, int *res, int *size, int height){
    if(root == NULL){
        return;
    }
    if(height >= (*size)){
        (*size)++;
    }
    traversal(root->left, res, size, height + 1);
    res[height] = root->val;
    traversal(root->right, res, size, height + 1);
}

int* rightSideView(struct TreeNode* root, int* returnSize) {
    if(root == NULL){
        (*returnSize) = 0;
        return NULL;
    }
    int *result = (int *)malloc(sizeof(int) * 100);
    (*returnSize) = 0;
    traversal(root, result, returnSize, 0);
    result = (int *)realloc(result, sizeof(int) * (*returnSize));
    return result;
}
