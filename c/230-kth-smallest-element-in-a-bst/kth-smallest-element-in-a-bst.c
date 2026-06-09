// -----------------------------------------------------------------------------
// Problem: Kth Smallest Element in a BST
// Source : https://leetcode.com/problems/kth-smallest-element-in-a-bst/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Perform an in-order traversal (Left → Root → Right).
// 2. Keep track of the number of nodes visited.
// 3. When visiting the k-th node, store its value as the result.
// 4. Stop traversing early when the k-th node is found.
//
// Time Complexity  : O(k)  (we stop after visiting k nodes)
// Space Complexity : O(h)  (recursion stack, h = height of the tree)
// -----------------------------------------------------------------------------


struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

void inorder(struct TreeNode *root, int *result, int *index, int k) {
    if(root == NULL)
        return;

    inorder(root->left, result, index, k);

    if((*index) < k) {
        *result = root->val;
        (*index)++;
    }

    inorder(root->right, result, index, k);
}

int kthSmallest(struct TreeNode* root, int k) {
    int result = root->val;
    int index = 0;

    inorder(root, &result, &index, k);
    return result;
}
