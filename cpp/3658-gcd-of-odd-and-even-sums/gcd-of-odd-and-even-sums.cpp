class Solution {
public:
    int gcd(int a, int b) {
        if (a == 0 && b == 0) {
            return 0;
        }

        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    int sumOdd = 0, sumEven = 0;
    int gcdOfOddEvenSums(int n) {
        for (int i = 1; i <= n; i++) {
            sumOdd += 2LL * i - 1;
            sumEven += 2LL * i;
        }
        return gcd(sumOdd, sumEven);
    }
};