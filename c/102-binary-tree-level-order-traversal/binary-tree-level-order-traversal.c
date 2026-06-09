// -----------------------------------------------------------------------------
// Problem: Binary Tree Level Order Traversal
// Source : https://leetcode.com/problems/binary-tree-level-order-traversal/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Use recursion to traverse the tree and store values level by level.
// 2. Maintain:
//      - `result`: a 2D array storing node values for each level.
//      - `returnColumnSizes`: the number of nodes in each level.
//      - `returnSize`: total number of levels.
// 3. For each node:
//      - If visiting a new level, allocate space for it.
//      - Store the node's value in the correct level.
// 4. After traversal, shrink allocated memory to the exact sizes.
// 5. Return the 2D result array.
//
// Time Complexity  : O(n)
// Space Complexity : O(n)
// -----------------------------------------------------------------------------


#define MAX 2000

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

void traversal(struct TreeNode *root, int **result, int *columns, int **rows, int height) {
    if(root == NULL)
        return;

    // If reaching a new level, initialize memory for it
    if((*columns) == height) {
        (*columns)++;
        (*rows)[height] = 0;
        result[height] = (int *)malloc(sizeof(int) * MAX);
    }

    // Store current node value in the corresponding level
    result[height][((*rows)[height])++] = root->val;

    // Recursive calls for left and right subtrees
    traversal(root->left, result, columns, rows, height + 1);
    traversal(root->right, result, columns, rows, height + 1);
}

int** levelOrder(struct TreeNode* root, int* returnSize, int** returnColumnSizes) {
    if(root == NULL) {
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }

    int **result = (int **)malloc(sizeof(int *) * MAX);
    (*returnColumnSizes) = (int *)calloc(MAX, sizeof(int));
    (*returnSize) = 0;

    traversal(root, result, returnSize, returnColumnSizes, 0);

    // Resize allocated memory to fit actual levels
    (*returnColumnSizes) = (int *)realloc((*returnColumnSizes), sizeof(int) * (*returnSize));
    for(int i = 0; i < (*returnSize); i++) {
        result[i] = (int *)realloc(result[i], sizeof(int) * ((*returnColumnSizes)[i]));
    }

    return result;
}
