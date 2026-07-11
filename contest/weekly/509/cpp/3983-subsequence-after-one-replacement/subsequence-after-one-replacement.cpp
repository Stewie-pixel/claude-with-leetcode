class Solution {
public:
    bool canMakeSubsequence(string s, string t) {
        int n = s.size(), m = t.size();
        vector<int> pre(n + 1), suf(n + 1);

        pre[0] = 0;
        int j = 0;
        for (int i = 0; i < n; i++) {
            while (j < m && t[j] != s[i]) j++;
            if (j < m) { j++; pre[i + 1] = j; }
            else { pre[i + 1] = m + 1; }
        }

        suf[n] = m;
        j = m - 1;
        for (int i = n - 1; i >= 0; i--) {
            while (j >= 0 && t[j] != s[i]) j--;
            if (j >= 0) { suf[i] = j; j--; }
            else { suf[i] = -1; }
        }

        if (pre[n] <= m) return true;

        for (int i = 0; i < n; i++) {
            if (pre[i] > m) break;
            if (suf[i + 1] < 0) continue;
            if (pre[i] < suf[i + 1]) return true;
        }

        return false;
    }
};