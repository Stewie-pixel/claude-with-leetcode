double myPow(double x, int n) {
    long long N = n;
    double ans = 1.0;
    if (N < 0) {
        x = 1.0 / x;
        N = -N;
    }
    double cur = x;
    while (N > 0) {
        if (N & 1) {
            ans *= cur;
        }
        cur *= cur;
        N >>= 1;
    }
    return ans;
}
