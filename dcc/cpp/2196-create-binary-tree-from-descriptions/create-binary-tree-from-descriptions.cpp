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
    TreeNode* createBinaryTree(vector<vector<int>>& descriptions) {
        unordered_map<int, TreeNode*> map;
        unordered_set<int> children;

        for (auto& d : descriptions) {
            int parent = d[0], child = d[1], isLeft = d[2];

            if (!map.count(parent)) map[parent] = new TreeNode(parent);
            if (!map.count(child)) map[child] = new TreeNode(child);

            if (isLeft) 
                map[parent]->left = map[child];
            else
                map[parent]->right = map[child];

            children.insert(child);
        }

        for (auto& d : descriptions) {
            int parent = d[0];
            if (!children.count(parent))
                return map[parent];
        }
        return nullptr;
    }
};