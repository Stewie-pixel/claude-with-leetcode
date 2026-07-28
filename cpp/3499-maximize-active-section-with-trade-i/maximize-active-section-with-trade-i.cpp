class Solution {
public:
    int maxActiveSectionsAfterTrade(string s) {
        string t = "1" + s + "1";
        vector<pair<char,int>> segs;
        int n = t.size();
        for (int i = 0; i < n; ) {
            int j = i;
            while (j < n && t[j] == t[i]) j++;
            segs.push_back({t[i], j - i});
            i = j;
        }
        int total = 0;
        for (char c : s) if (c == '1') total++;

        int best = 0;
        for (int i = 1; i + 1 < (int)segs.size(); i++) {
            if (segs[i].first == '1' && segs[i-1].first == '0' && segs[i+1].first == '0') {
                best = max(best, segs[i-1].second + segs[i+1].second);
            }
        }
        return total + best;
    }
};