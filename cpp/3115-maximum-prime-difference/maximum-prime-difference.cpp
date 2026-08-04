class Solution {
public:
    bool isPrime(int x) {
        if (x == 1) return false;
        for (int i = 2; i * i <= x; i++) {
            if (x % i == 0) {
                return false;
            }
        }
        return true;
    }

    int maximumPrimeDifference(vector<int>& nums) {
        int first = -1, last = -1;

        for (int i = 0; i < nums.size(); i++) {
            if (isPrime(nums[i])) {
                if (first == -1) first = i;
                last = i;
            }
        }
        return last - first;
    }
};