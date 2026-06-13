class Solution {

    public int findDuplicate(int[] nums) {
        int n = nums.length;

        for (int i = 0; i < n; i++) {
            int index_num = Math.abs(nums[i]) - 1;

            if (nums[index_num] < 0) {
                return Math.abs(nums[i]);
            } else {
                nums[index_num] = -nums[index_num];
            }
        }

        return -1;
    }
}
