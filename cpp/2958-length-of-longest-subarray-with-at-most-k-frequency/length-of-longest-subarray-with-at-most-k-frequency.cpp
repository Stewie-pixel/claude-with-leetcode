class Solution {
public:
    int maxSubarrayLength(vector<int>& nums, int k) {
        int n = nums.size();
        unordered_map<int, int> freq;
        int r = 1;
        int left = 0;

        for (int right = 0; right < n; right++) {
            int c = nums[right];
            freq[c]++;
            while (freq[c] > k) {
                int d = nums[left];
                freq[d]--;
                left++;
            }
            r = max(r, right - left + 1);
        }
        return r;
    }
};