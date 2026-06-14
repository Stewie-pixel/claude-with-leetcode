class Solution {
public:
    bool checkGoodInteger(int n) {
        int diff = 0;
        while (n > 0) {
            int d = n % 10;
            diff += d * d - d;
            n /= 10;
        }
        return diff >= 50;
    }
};