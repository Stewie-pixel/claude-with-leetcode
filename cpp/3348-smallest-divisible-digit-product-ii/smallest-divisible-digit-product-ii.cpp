#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> primes = {2, 3, 5, 7};
    int maxPrime = 7;

    string smallestNumber(string num, long long t) {

        vector<int> primeCount(maxPrime + 1, 0);

        int numLength = num.length();
        int firstZeroIndexFromLeft = 0;

        for (int prime : primes) {
            while (t % prime == 0) {
                t /= prime;
                primeCount[prime]++;
            }
        }

        if (t != 1)
            return "-1";

        int minLength = getMinLength(primeCount);

        if (numLength < minLength) {
            string result(minLength, '0');
            return buildSuffix(primeCount, minLength, result);
        }

        string result(numLength + 1, '0');

        int i = 0;
        while (firstZeroIndexFromLeft < numLength) {

            result[++i] = num[firstZeroIndexFromLeft];

            if (result[i] == '0')
                break;

            logNum(primeCount, result[i], -1);
            firstZeroIndexFromLeft++;
        }

        if (getMinLength(primeCount) == 0) {

            if (firstZeroIndexFromLeft == numLength)
                return num;

            firstZeroIndexFromLeft++;

            for (int j = firstZeroIndexFromLeft; j < result.size(); j++)
                result[j] = '1';

            return result.substr(1, numLength);
        }

        int last = numLength - 1;

        for (int end = min(firstZeroIndexFromLeft, last); end >= 0; end--) {

            logNum(primeCount, result[end + 1], 1);

            while (++result[end + 1] <= '9') {

                logNum(primeCount, result[end + 1], -1);

                if (getMinLength(primeCount) <= last - end) {
                    return buildSuffix(primeCount, last - end, result);
                }

                logNum(primeCount, result[end + 1], 1);
            }
        }

        return buildSuffix(primeCount, result.length(), result);
    }

private:

    void logNum(vector<int>& primeCount, char num, int value) {

        if (num < '2')
            return;

        if (num == '9') {

            primeCount[3] += value * 2;

        } else if (num == '4') {

            primeCount[2] += value * 2;

        } else if (num == '8') {

            primeCount[2] += value * 3;

        } else if (num == '6') {

            primeCount[2] += value;
            primeCount[3] += value;

        } else {

            primeCount[num - '0'] += value;
        }
    }

    string buildSuffix(vector<int> primeCount,
                       int targetLength,
                       string result) {

        int index = result.length();

        while (primeCount[3] > 1) {
            primeCount[3] -= 2;
            result[--index] = '9';
        }

        while (primeCount[2] > 2) {
            primeCount[2] -= 3;
            result[--index] = '8';
        }

        while (primeCount[7]-- > 0)
            result[--index] = '7';

        if (primeCount[2] > 0 && primeCount[3] > 0) {

            result[--index] = '6';

            primeCount[2]--;
            primeCount[3]--;
        }

        while (primeCount[5]-- > 0)
            result[--index] = '5';

        while (primeCount[2] > 1) {

            primeCount[2] -= 2;
            result[--index] = '4';
        }

        while (primeCount[3] > 0) {

            primeCount[3]--;
            result[--index] = '3';
        }

        while (primeCount[2] > 0) {

            primeCount[2]--;
            result[--index] = '2';
        }

        while (index + targetLength != result.length())
            result[--index] = '1';

        if (targetLength == result.length())
            return result;

        return result.substr(1);
    }

    int getMinLength(vector<int>& primeCount) {

        int count2 = max(0, primeCount[2]);
        int count3 = max(0, primeCount[3]);

        int count23 = (count3 & 1) + (count2 % 3);

        return (count3 / 2)
                + (count2 / 3)
                + max(0, primeCount[7])
                + max(0, primeCount[5])
                + (count23 == 3 ? 2 : (count23 > 0 ? 1 : 0));
    }
};