class Solution {
public:
    int sumOfGoodIntegers(int n, int k) {
        long long total = 0;
        for (int x = max(1, n - k); x <= n + k; x++) {
            if ((n & x) == 0) {
                total += x;
            }
        }
        return total;
    }
};
