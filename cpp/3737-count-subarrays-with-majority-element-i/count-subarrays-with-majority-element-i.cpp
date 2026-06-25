class Solution {
public:
    int countMajoritySubarrays(vector<int>& nums, int target) {
       int ans = 0;
       int n = nums.size();
       
       for (int i = 0; i < n; i++) {
           int count = 0;
           for (int j = i; j < n; j++) {
               if (nums[j] == target) count++;
               if (count * 2 > (j - i + 1)) ans++;
           }
       }
       return ans;
    }
};