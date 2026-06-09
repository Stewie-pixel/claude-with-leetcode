class Solution {
public:
    int longestSubarray(vector<int>& nums) {
        int prev = 0;
        int curr = 0;
        int zeros = 0;
        int best = 0;

        for (int n : nums) {
            if (n == 1) {
                curr++;
            } else {
                zeros++;
                prev = curr;
                curr = 0;
            }
            best = max(best, prev + curr);
        }

        return zeros == 0 ? best - 1 : best;
    }
};
