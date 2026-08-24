class Solution {
public:
    int kthDigit(long long k) {
        if (k <= 9) return (int)k;
        k -= 9;

        long long d = 2;
        while (true) {
            long long blocks = 9 * pow10(d - 2);
            long long digitsPerBlock = 10 * d;
            long long digitsInGroup = blocks * digitsPerBlock;
            if (k <= digitsInGroup) break;
            k -= digitsInGroup;
            d++;
        }

        long long digitsPerBlock = 10 * d;
        long long blockIndex = (k - 1) / digitsPerBlock;
        long long remainder = (k - 1) % digitsPerBlock;

        long long b = pow10(d - 2) + blockIndex;
        long long numberIndex = remainder / d;
        int digitIndexInNumber = remainder % d;

        long long number;
        if (b % 2 == 0)
            number = 10 * b + numberIndex;
        else
            number = 10 * b + 9 - numberIndex;

        string s = to_string(number);
        return s[digitIndexInNumber] - '0';
    }

private:
    long long pow10(int e) {
        long long r = 1;
        for (int i = 0; i < e; i++) r *= 10;
        return r;
    }
};