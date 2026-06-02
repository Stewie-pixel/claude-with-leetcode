class Solution {
public:
    vector<int> limitOccurrences(vector<int>& nums, int k) {
        int write = 0;
        int count = 0;

        for (int read = 0; read < nums.size(); read++) {
            if (read == 0 || nums[read] != nums[read - 1]) {
                count = 1;
            } else {
                count++;
            }

            if (count <= k) {
                nums[write++] = nums[read];
            }
        }

        nums.resize(write);
        return nums;
    }
};