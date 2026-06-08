class Solution {

    public int[] pivotArray(int[] nums, int pivot) {
        int n = nums.length;
        int[] res = new int[n];
        int pointerRes = 0;

        for (int i = 0; i < n; i++) {
            if (nums[i] < pivot) {
                res[pointerRes] = nums[i];
                pointerRes += 1;
            }
        }

        for (int i = 0; i < n; i++) {
            if (nums[i] == pivot) {
                res[pointerRes] = nums[i];
                pointerRes += 1;
            }
        }

        for (int i = 0; i < n; i++) {
            if (nums[i] > pivot) {
                res[pointerRes] = nums[i];
                pointerRes += 1;
            }
        }

        return res;
    }
}
