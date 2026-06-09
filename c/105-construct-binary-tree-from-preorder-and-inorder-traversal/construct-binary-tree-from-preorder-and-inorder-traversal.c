// -----------------------------------------------------------------------------
// Problem: Construct Binary Tree from Preorder and Inorder Traversal
// Source : https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. The first element in preorder[] is always the root node.
// 2. Find the root value’s index in inorder[] — everything to the left is the left subtree,
//    everything to the right is the right subtree.
// 3. Recursively construct left and right subtrees using the corresponding
//    slices of preorder[] and inorder[].
//
// Time Complexity  : O(n²)  (due to linear search in inorder[] per recursion)
// Space Complexity : O(h)   (recursive stack where h is the tree height)
// -----------------------------------------------------------------------------


struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode* buildTree(int* preorder, int preorderSize, int* inorder, int inorderSize) {
    if(preorderSize <= 0 || inorderSize <= 0) {
        return NULL;
    }

    // 1. The first element in preorder[] is the root
    struct TreeNode *root = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    root->val = preorder[0];

    // 2. Find root position in inorder[]
    int i = 0;
    while(i < inorderSize && inorder[i] != root->val) {
        i++;
    }

    // 3. Recursively build left and right subtrees
    root->left = buildTree(preorder + 1, i, inorder, i);
    root->right = buildTree(preorder + i + 1, preorderSize - i - 1, inorder + i + 1, inorderSize - i - 1);

    return root;
}
