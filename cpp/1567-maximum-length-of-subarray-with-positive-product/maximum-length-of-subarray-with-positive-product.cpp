class Solution {
public:
    int getMaxLen(vector<int>& nums) {
        int n = nums.size();
        int ans = 0;

        int start = 0;

        while (start < n) {
            if (nums[start] == 0) {
                start++;
                continue;
            }

            int end = start;
            int negCount = 0;
            int firstNeg = -1, lastNeg = -1;

            while (end < n && nums[end] != 0) {
                if (nums[end] < 0) {
                    negCount++;
                    if (firstNeg == -1) firstNeg = end;
                    lastNeg = end;
                }
                end++;
            }

            if (negCount % 2 == 0) {
                ans = max(ans, end - start);
            } else {
                ans = max(ans, end - firstNeg - 1);
                ans = max(ans, lastNeg - start);
            }

            start = end;
        }

        return ans;
    }
};