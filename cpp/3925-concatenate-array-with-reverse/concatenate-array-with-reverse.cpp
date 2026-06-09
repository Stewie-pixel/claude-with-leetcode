class Solution {
public:
    vector<int> concatWithReverse(vector<int>& nums) {
        int n = nums.size();
        vector<int> ans(2*n);

        for (int i = 0; i < n; i++) {
            ans[i] = nums[i];
            ans[2*n - 1 - i] = nums[i];
        }

        return ans;
    }
};
