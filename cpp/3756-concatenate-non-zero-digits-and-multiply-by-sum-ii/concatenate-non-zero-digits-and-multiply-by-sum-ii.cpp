class Solution {
public:
    static const int MOD = 1000000007;
    
    vector<int> sumAndMultiply(string s, vector<vector<int>>& queries) {
        int m = s.size();

        vector<int> nz_pos;
        vector<int> nz_val;

        for (int i = 0; i < m; i++) {
            if (s[i] != '0') {
                nz_pos.push_back(i);
                nz_val.push_back(s[i] - '0');
            }
        }

        int k = nz_pos.size();
        vector<long long> prefSum(k + 1, 0), prefCat(k + 1, 0), pow10(k + 1, 1);

        for (int i = 1; i <= k; i++) {
            prefSum[i] = prefSum[i - 1] + nz_val[i - 1];
            pow10[i] = (pow10[i - 1] * 10) % MOD;
            prefCat[i] = (prefCat[i - 1] * 10 + nz_val[i - 1]) % MOD;
        }

        vector<int> ans;
        ans.reserve(queries.size());

        for (auto &q : queries) {
            int l = q[0], r = q[1];

            int L = lower_bound(nz_pos.begin(), nz_pos.end(), l) - nz_pos.begin();
            int R = upper_bound(nz_pos.begin(), nz_pos.end(), r) - nz_pos.begin() - 1;

            if (L > R) {
                ans.push_back(0);
                continue;
            }

            int len = R - L + 1;

            long long x = (prefCat[R + 1] - (prefCat[L] * pow10[len]) % MOD + MOD) % MOD;
            long long sum = prefSum[R + 1] - prefSum[L];

            ans.push_back((x * sum) % MOD);
        }

        return ans;
    }
};