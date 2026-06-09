class Solution {
public:
    long long totalWaviness(long long num1, long long num2) {
        return f(num2) - f(num1 - 1);
    }

private:
    long long memo_cnt[16][11][11][2][2];
    long long memo_wav[16][11][11][2][2];
    bool visited [16][11][11][2][2];
    string digits;
    int n;

    pair<long long,long long> dp(int pos, int p2, int p1, bool tight, bool started) {
        if (pos == n)
            return { started ? 1LL : 0LL, 0LL };

        int i2 = p2+1, i1 = p1+1, it = tight, is = started;
        if (visited[pos][i2][i1][it][is])
            return { memo_cnt[pos][i2][i1][it][is], memo_wav[pos][i2][i1][it][is] };

        visited[pos][i2][i1][it][is] = true;

        int lo = 0, hi = tight ? (digits[pos]-'0') : 9;
        long long tot_cnt = 0, tot_wav = 0;

        for (int d = lo; d <= hi; d++) {
            bool new_tight = tight && (d == hi);

            if (!started) {
                auto [c, w] = (d == 0)
                    ? dp(pos+1, -1, -1, new_tight, false)
                    : dp(pos+1, -1,  d, new_tight, true);
                tot_cnt += c;
                tot_wav += w;

            } else {
                long long bonus = 0;
                if (p2 != -1) {
                    bool is_peak   = (p2 < p1) && (p1 > d);
                    bool is_valley = (p2 > p1) && (p1 < d);
                    if (is_peak || is_valley) bonus = 1;
                }

                auto [c, w] = dp(pos+1, p1, d, new_tight, true);
                tot_cnt += c;
                tot_wav += w + bonus * c;
            }
        }

        memo_cnt[pos][i2][i1][it][is] = tot_cnt;
        memo_wav[pos][i2][i1][it][is] = tot_wav;
        return { tot_cnt, tot_wav };
    }

    long long f(long long num) {
        if (num <= 0) return 0;
        digits = to_string(num);
        n = (int)digits.size();
        memset(visited, false, sizeof(visited));
        return dp(0, -1, -1, true, false).second;
    }
};
