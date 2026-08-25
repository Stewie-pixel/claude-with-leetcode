class Solution {
public:
    int rob(vector<int>& nums) {
        int rob1 = 0, rob2 = 0;
        for (int n : nums) {
            int newRob = max(rob1 + n, rob2);
            rob1 = rob2;
            rob2 = newRob;
        }
        return rob2;
    }
};