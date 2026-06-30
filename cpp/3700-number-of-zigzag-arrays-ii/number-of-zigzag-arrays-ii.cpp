class Solution {
public:
    static const long long MOD = 1000000007;

    using Matrix = vector<vector<long long>>;
    using Vec = vector<long long>;

    Matrix mulMat(const Matrix &A, const Matrix &B) {
        int n = A.size();
        int m = B[0].size();
        int p = B.size();
        Matrix C(n, vector<long long>(m, 0));
        for (int i = 0; i < n; ++i) {
            for (int k = 0; k < p; ++k) {
                if (A[i][k] == 0) continue;
                long long aik = A[i][k];
                for (int j = 0; j < m; ++j) {
                    if (B[k][j] == 0) continue;
                    C[i][j] = (C[i][j] + aik * B[k][j]) % MOD;
                }
            }
        }
        return C;
    }

    Vec mulMatVec(const Matrix &A, const Vec &v) {
        int n = A.size();
        int m = v.size();
        Vec res(n, 0);
        for (int i = 0; i < n; ++i) {
            long long sum = 0;
            for (int j = 0; j < m; ++j) {
                if (A[i][j] == 0 || v[j] == 0) continue;
                sum += A[i][j] * v[j];
                if (sum >= (1LL << 61)) sum %= MOD;
            }
            res[i] = sum % MOD;
        }
        return res;
    }

    Matrix matPow(Matrix base, long long exp) {
        int n = base.size();
        Matrix res(n, vector<long long>(n, 0));
        for (int i = 0; i < n; ++i) res[i][i] = 1;
        while (exp > 0) {
            if (exp & 1) res = mulMat(res, base);
            base = mulMat(base, base);
            exp >>= 1;
        }
        return res;
    }

    int zigZagArrays(int n, int l, int r) {
        int m = r - l + 1;
        if (n == 1) return m;
        if (n == 2) return (int)((1LL * m * (m - 1)) % MOD);

        int sz = 2 * m;
        Matrix T(sz, vector<long long>(sz, 0));

        auto upIdx = [m](int j) { return j; };
        auto downIdx = [m](int j) { return m + j; };

        for (int j = 0; j < m; ++j) {
            int rowUp = upIdx(j);
            for (int i = 0; i < j; ++i) {
                int colDown = downIdx(i);
                T[rowUp][colDown] = 1;
            }

            int rowDown = downIdx(j);
            for (int i = j + 1; i < m; ++i) {
                int colUp = upIdx(i);
                T[rowDown][colUp] = 1;
            }
        }

        Vec v(sz, 0);
        for (int j = 0; j < m; ++j) {
            v[upIdx(j)] = j;
            v[downIdx(j)] = m - 1 - j;
        }

        long long power = n - 2;
        Matrix Tpow = matPow(T, power);
        Vec vn = mulMatVec(Tpow, v);

        long long ans = 0;
        for (int j = 0; j < m; ++j) {
            ans += vn[upIdx(j)];
            if (ans >= MOD) ans -= MOD;
            ans += vn[downIdx(j)];
            if (ans >= MOD) ans -= MOD;
        }
        return (int)ans;
    }

    int countZigZagArrays(int n, int l, int r) {
        return zigZagArrays(n, l, r);
    }
};
