class Solution {
public:
    int subsequencePairCount(vector<int>& nums) {
        const long long MOD = 1000000007;
        int maxVal = *max_element(nums.begin(), nums.end());
        int n = nums.size();

        vector<int> freq(maxVal + 1, 0);
        for (int x : nums) freq[x]++;

        vector<long long> divCount(maxVal + 1, 0);
        for (int v = 1; v <= maxVal; v++) {
            long long c = 0;
            for (int k = v; k <= maxVal; k += v) c += freq[k];
            divCount[v] = c;
        }

        vector<int> mu(maxVal + 1, 0);
        vector<bool> comp(maxVal + 1, false);
        vector<int> primes;
        mu[1] = 1;
        for (int i = 2; i <= maxVal; i++) {
            if (!comp[i]) { primes.push_back(i); mu[i] = -1; }
            for (int p : primes) {
                long long ip = (long long)i * p;
                if (ip > maxVal) break;
                comp[ip] = true;
                if (i % p == 0) { mu[ip] = 0; break; }
                else mu[ip] = -mu[i];
            }
        }

        vector<long long> pow2(n + 1), pow3(n + 1);
        pow2[0] = 1; pow3[0] = 1;
        for (int i = 1; i <= n; i++) {
            pow2[i] = pow2[i - 1] * 2 % MOD;
            pow3[i] = pow3[i - 1] * 3 % MOD;
        }

        long long answer = 0;
        for (int g = 1; g <= maxVal; g++) {
            if (divCount[g] == 0) continue;
            long long sumP = 0;
            for (int m1 = g; m1 <= maxVal; m1 += g) {
                int mu1 = mu[m1 / g];
                if (mu1 == 0) continue;
                for (int m2 = g; m2 <= maxVal; m2 += g) {
                    int mu2 = mu[m2 / g];
                    if (mu2 == 0) continue;

                    long long L = (long long)m1 / __gcd(m1, m2) * m2;
                    long long nL = (L <= maxVal) ? divCount[(int)L] : 0;
                    long long n1 = divCount[m1] - nL;
                    long long n2 = divCount[m2] - nL;

                    long long R = (pow2[n1] * pow2[n2] % MOD * pow3[nL] % MOD
                                   - pow2[n2] * pow2[nL] % MOD
                                   - pow2[n1] * pow2[nL] % MOD
                                   + 1 + 2 * MOD) % MOD;

                    long long term = R * mu1 % MOD * mu2 % MOD;
                    sumP = ((sumP + term) % MOD + MOD) % MOD;
                }
            }
            answer = (answer + sumP) % MOD;
        }

        return (int)((answer % MOD + MOD) % MOD);
    }
};