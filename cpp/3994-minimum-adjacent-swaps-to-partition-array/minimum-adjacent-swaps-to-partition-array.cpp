class Solution {
public:
    int minAdjacentSwaps(vector<int>& nums, int a, int b) {
        const long long MOD = 1e9 + 7;
        long long c0 = 0, c1 = 0, c2 = 0, total = 0;

        for (int x : nums) {
            if (x < a) {
                total += c1 + c2;
                c0++;
            } else if (x <= b) {
                total += c2;
                c1++;
            } else {
                c2++;
            }
        }
        return total % MOD;
    }
};