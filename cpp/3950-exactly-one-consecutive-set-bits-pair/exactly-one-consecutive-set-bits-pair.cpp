class Solution {
public:
    bool consecutiveSetBits(int n) {
        int x = n & (n >> 1);
        return x != 0 && (x & (x - 1)) == 0;
    }
};
