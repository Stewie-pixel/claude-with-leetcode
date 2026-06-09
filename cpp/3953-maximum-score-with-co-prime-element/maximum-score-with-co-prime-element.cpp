class Solution {
public:
    int maxScore(vector<int>& nums, int maxVal) {
        int M = *max_element(nums.begin(), nums.end());
        int bound = max(M, maxVal);

        vector<int> freq(M + 1, 0);
        for (int x : nums) freq[x]++;

        vector<int> cnt(M + 1, 0);
        for (int d = 1; d <= M; d++)
            for (int k = d; k <= M; k += d)
                cnt[d] += freq[k];

        vector<int> spf(bound + 1);
        iota(spf.begin(), spf.end(), 0);
        for (int i = 2; (long long)i * i <= bound; i++)
            if (spf[i] == i)
                for (int j = i * i; j <= bound; j += i)
                    if (spf[j] == j) spf[j] = i;

        auto getPrimes = [&](int v) {
            vector<int> primes;
            while (v > 1) {
                int p = spf[v];
                primes.push_back(p);
                while (v % p == 0) v /= p;
            }
            return primes;
        };

        auto badCount = [&](int v) -> long long {
            auto primes = getPrimes(v);
            int m = primes.size();
            long long bad = 0;
            for (int mask = 1; mask < (1 << m); mask++) {
                long long prod = 1;
                int parity = 0;
                for (int i = 0; i < m; i++) {
                    if (mask & (1 << i)) {
                        prod *= primes[i];
                        parity ^= 1;
                    }
                }
                if (prod <= M) {
                    if (parity) bad += cnt[prod];
                    else bad -= cnt[prod];
                }
            }
            return bad;
        };

        long long ans = 0;

        for (int x = 1; x <= M; x++) {
            if (!freq[x]) continue;
            long long bc = badCount(x);
            long long score = (x == 1) ? 1LL : (x - bc + 1);
            ans = max(ans, score);
        }

        for (int v = 1; v <= maxVal; v++) {
            long long bc = badCount(v);
            long long score = (long long)v - max(1LL, bc);
            ans = max(ans, score);
        }

        return ans;
    }
};
