class Solution {
public:
    int longestSubsequence(vector<int>& nums) {
        auto total = 0, nonZero = 0;

        for (auto& n : nums) {
            nonZero |= n > 0;
            total ^= n;
        }

        return nonZero * (nums.size() - !total);
    }
};