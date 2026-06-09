class Solution {
public:
    long long maxTotalValue(vector<int>& nums, int k) {
        long long max_val = *max_element(nums.begin(), nums.end());
        long long min_val = *min_element(nums.begin(), nums.end());

        return (long long)k * (max_val - min_val);
    }
};
