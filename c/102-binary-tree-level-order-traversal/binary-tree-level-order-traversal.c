#define MAX 2000

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

void traversal(struct TreeNode *root, int **result, int *columns, int **rows, int height) {
    if(root == NULL)
        return;

    if((*columns) == height) {
        (*columns)++;
        (*rows)[height] = 0;
        result[height] = (int *)malloc(sizeof(int) * MAX);
    }

    result[height][((*rows)[height])++] = root->val;

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

    (*returnColumnSizes) = (int *)realloc((*returnColumnSizes), sizeof(int) * (*returnSize));
    for(int i = 0; i < (*returnSize); i++) {
        result[i] = (int *)realloc(result[i], sizeof(int) * ((*returnColumnSizes)[i]));
    }

    return result;
}
