class Solution {
public:
    vector<int> pivotArray(vector<int>& nums, int pivot) {
        vector<int> left, middle, right;

        for (int x : nums) {
            if (x < pivot) left.push_back(x);
            else if (x > pivot) right.push_back(x);
            else middle.push_back(x);
        }

        vector<int> ans;
        ans.reserve(nums.size());
        ans.insert(ans.end(), left.begin(), left.end());
        ans.insert(ans.end(), middle.begin(), middle.end());
        ans.insert(ans.end(), right.begin(), right.end());

        return ans;
    }
};
