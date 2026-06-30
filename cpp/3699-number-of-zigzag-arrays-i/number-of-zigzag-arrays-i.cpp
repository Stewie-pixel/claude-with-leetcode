class Solution {
public:
    int zigZagArrays(int n, int l, int r) {
        const int MOD = 1'000'000'007;
        int m = r - l + 1;

        vector<long long> a(m + 1), b(m + 1), na(m + 1), nb(m + 1);

        for (int x = 1; x <= m; ++x) {
            a[x] = x - 1;
            b[x] = m - x;
        }

        for (int i = 2; i < n; ++i) {
            vector<long long> pref(m + 2), suff(m + 3);
            for (int x = 1; x <= m; ++x) pref[x + 1] = (pref[x] + b[x]) % MOD;
            for (int x = m; x >= 1; --x) suff[x] = (suff[x + 1] + a[x]) % MOD;

            for (int y = 1; y <= m; ++y) {
                na[y] = pref[y];
                nb[y] = suff[y + 1];
            }

            a.swap(na);
            b.swap(nb);
        }

        long long ans = 0;
        for (int x = 1; x <= m; ++x) ans = (ans + a[x] + b[x]) % MOD;
        return ans;
    }
};