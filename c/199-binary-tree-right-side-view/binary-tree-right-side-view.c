// -----------------------------------------------------------------------------
// Problem: Binary Tree Right Side View
// Source : https://leetcode.com/problems/binary-tree-right-side-view/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Traverse the tree recursively in a right-priority order (right first, then left).
// 2. For each level (height):
//      - If this is the first time we reach that level, record the node value.
// 3. Use `height` to track depth and `size` to record the number of visible levels.
// 4. Return the collected rightmost node values.
//
// Time Complexity  : O(n)  (visit each node once)
// Space Complexity : O(h)  (recursive call stack, where h is tree height)
// -----------------------------------------------------------------------------



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
