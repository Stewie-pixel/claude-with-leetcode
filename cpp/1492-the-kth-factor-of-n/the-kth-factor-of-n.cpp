class Solution {
public:
    int kthFactor(int n, int k) {
        vector<int> small, big;

        for (int i = 1; i * i <= n; i++) {
            if (n % i == 0) {
                small.push_back(i);
                if (i != n / i) {
                    big.push_back(n / i);
                }
            }
        }

        vector<int> factors = small;
        for (int i = big.size() - 1; i >= 0; i--) {
            factors.push_back(big[i]);
        }

        if (k <= factors.size()) {
            return factors[k - 1];
        }
        return -1;
    }
};