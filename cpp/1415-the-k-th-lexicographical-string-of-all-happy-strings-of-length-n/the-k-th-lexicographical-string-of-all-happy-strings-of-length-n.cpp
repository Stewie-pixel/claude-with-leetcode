class Solution {
public:
    string getHappyString(int n, int k) {
        long long total = 3LL * (1LL << (n - 1));
        if (k > total) return "";

        string res = "";
        char prev = 0;

        for (int i = 0; i < n; i++) {
            for (char c : {'a', 'b', 'c'}) {
                if (c == prev) continue;

                long long block = 1LL << (n - i - 1);

                if (k > block) {
                    k -= block;
                } else {
                    res.push_back(c);
                    prev = c;
                    break;
                }
            }
        }
        return res;
    }
};