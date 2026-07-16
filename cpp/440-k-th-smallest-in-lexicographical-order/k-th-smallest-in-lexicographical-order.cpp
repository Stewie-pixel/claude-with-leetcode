class Solution {
public:
    int findKthNumber(int n, int k) {
        long long curr = 1;
        k -= 1;
        while (k > 0) {
            long long steps = countSteps(n, curr);
            if (steps <= k) {
                curr += 1;
                k -= steps;
            } else {
                curr *= 10;
                k -= 1;
            }
        }
        return (int)curr;
    }
    
    long long countSteps(int n, long long prefix) {
        long long cur = prefix;
        long long nxt = prefix + 1;
        long long steps = 0;
        while (cur <= n) {
            steps += std::min((long long)n + 1, nxt) - cur;
            cur *= 10;
            nxt *= 10;
        }
        return steps;
    }
};