/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    int dfs(TreeNode* root, long long target) {
        if (!root) return 0;

        int count = (root->val == target) ? 1 : 0;

        count += dfs(root->left, target - root->val);
        count += dfs(root->right, target - root->val);

        return count;
    }

    int pathSum(TreeNode* root, int targetSum) {
        if (!root) return 0;

        return dfs(root, targetSum) + pathSum(root->left, targetSum) + pathSum(root->right, targetSum);
    }
};