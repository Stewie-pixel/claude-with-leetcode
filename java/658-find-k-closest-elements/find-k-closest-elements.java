class Solution {

    public List<Integer> findClosestElements(int[] arr, int k, int x) {
        int n = arr.length;
        int left = 0;
        int right = n - 1;

        List<Integer> res = new ArrayList<>();

        while (right - left + 1 > k) {
            if (
                Math.abs(arr[left] - x) < Math.abs(arr[right] - x) ||
                (Math.abs(arr[left] - x) == Math.abs(arr[right] - x) &&
                    arr[left] < arr[right])
            ) {
                right--;
            } else {
                left++;
            }
        }

        for (int i = left; i <= right; i++) {
            res.add(arr[i]);
        }

        return res;
    }
}
