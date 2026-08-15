class Solution {
public:
    int minOperations(string s) {
        int n = s.size();
        int ans = INT_MAX;
        for (int k = 0; k < n; k++) {
            int cost = k;
            for (int i = 0, j = n - 1; i < j; i++, j--) {
                char a = s[(k + i) % n];
                char b = s[(k + j) % n];
                int diff = abs(a - b);
                cost += min(diff, 26 - diff);
            }
            ans = min(ans, cost);
        }
        return ans;
    }
};