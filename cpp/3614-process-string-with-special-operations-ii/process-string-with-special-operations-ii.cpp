class Solution {
public:
    char processStr(string s, long long k) {
        int n = s.size();
        vector<long long> lens(n + 1, 0);
        
        for (int i = 0; i < n; i++) {
            if (s[i] == '*') lens[i+1] = max(0LL, lens[i] - 1);
            else if (s[i] == '#') lens[i+1] = lens[i] * 2;
            else if (s[i] == '%') lens[i+1] = lens[i];
            else lens[i+1] = lens[i] + 1;
        }
        
        if (k >= lens[n]) return '.';
        
        for (int i = n - 1; i >= 0; i--) {
            long long prevLen = lens[i];
            
            if (s[i] == '#') {
                if (k >= prevLen) k -= prevLen;
            } else if (s[i] == '%') {
                k = prevLen - 1 - k;
            } else if (s[i] != '*') {
                if (k == prevLen) return s[i];
            }
        }
        return '.';
    }
};