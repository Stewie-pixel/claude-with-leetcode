class Solution {
public:
    int smallestNumber(int n, int t) {
        auto [q, r] = div(n, 10);

        int req = t / gcd(max(q, 1), t);
        int nxt;

        if (r % req == 0)
            nxt = r;
        else
            nxt = (r / req + 1) * req;

        if (nxt < 10)
            return q * 10 + nxt;

        return (q + 1) * 10;
    }
};