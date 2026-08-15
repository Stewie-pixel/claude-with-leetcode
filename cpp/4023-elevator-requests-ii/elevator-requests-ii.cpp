class Solution {
public:
    long long elevatorRequests(int n, int start, vector<int>& requests) {
        vector<int> L, R;
        for (int r : requests) {
            if (r < start) L.push_back(r);
            else if (r > start) R.push_back(r);
        }
        sort(L.rbegin(), L.rend());
        sort(R.begin(), R.end());

        int p = L.size(), q = R.size();
        vector<long long> d(p + 1), e(q + 1);
        d[0] = 0;
        for (int i = 1; i <= p; i++) d[i] = (long long)start - L[i - 1];
        e[0] = 0;
        for (int j = 1; j <= q; j++) e[j] = (long long)R[j - 1] - start;

        const long long INF = LLONG_MAX / 4;

        vector<vector<long long>> S0(p + 1, vector<long long>(q + 1, INF));
        vector<vector<long long>> T0(p + 1, vector<long long>(q + 1, 0));
        vector<vector<long long>> S1(p + 1, vector<long long>(q + 1, INF));
        vector<vector<long long>> T1(p + 1, vector<long long>(q + 1, 0));

        S0[0][0] = 0; T0[0][0] = 0;
        S1[0][0] = 0; T1[0][0] = 0;

        for (int i = 0; i <= p; i++) {
            for (int j = 0; j <= q; j++) {
                if (i == 0 && j == 0) continue;
                long long K = (long long)(p - i) + (q - j);

                if (i >= 1) {
                    long long c1comp = INF, c1S = INF, c1T = 0;
                    long long c2comp = INF, c2S = INF, c2T = 0;
                    if (S0[i - 1][j] < INF) {
                        long long step = d[i] - d[i - 1];
                        c1T = T0[i - 1][j] + step;
                        c1S = S0[i - 1][j] + c1T;
                        c1comp = c1S + K * c1T;
                    }
                    if (j >= 1 ? (S1[i - 1][j] < INF) : (i - 1 == 0)) {
                        long long base = (j >= 1) ? S1[i - 1][j] : 0;
                        long long baseT = (j >= 1) ? T1[i - 1][j] : 0;
                        long long step = (j >= 1 ? e[j] : 0) + d[i];
                        c2T = baseT + step;
                        c2S = base + c2T;
                        c2comp = c2S + K * c2T;
                    }
                    if (c1comp <= c2comp) { S0[i][j] = c1S; T0[i][j] = c1T; }
                    else { S0[i][j] = c2S; T0[i][j] = c2T; }
                }

                if (j >= 1) {
                    long long c1comp = INF, c1S = INF, c1T = 0;
                    long long c2comp = INF, c2S = INF, c2T = 0;
                    if (S1[i][j - 1] < INF) {
                        long long step = e[j] - e[j - 1];
                        c1T = T1[i][j - 1] + step;
                        c1S = S1[i][j - 1] + c1T;
                        c1comp = c1S + K * c1T;
                    }
                    if (i >= 1 ? (S0[i][j - 1] < INF) : (j - 1 == 0)) {
                        long long base = (i >= 1) ? S0[i][j - 1] : 0;
                        long long baseT = (i >= 1) ? T0[i][j - 1] : 0;
                        long long step = (i >= 1 ? d[i] : 0) + e[j];
                        c2T = baseT + step;
                        c2S = base + c2T;
                        c2comp = c2S + K * c2T;
                    }
                    if (c1comp <= c2comp) { S1[i][j] = c1S; T1[i][j] = c1T; }
                    else { S1[i][j] = c2S; T1[i][j] = c2T; }
                }
            }
        }

        if (p == 0 && q == 0) return 0;
        long long ans = INF;
        if (p >= 1) ans = min(ans, S0[p][q]);
        if (q >= 1) ans = min(ans, S1[p][q]);
        return ans;
    }
};