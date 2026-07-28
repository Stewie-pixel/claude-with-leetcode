class Solution {
public:
    vector<int> maxActiveSectionsAfterTrade(string s, vector<vector<int>>& queries) {
        int n = s.size();
        int ones = ranges::count(s, '1');

        vector<int> zs, ze;
        for (int i = 0; i < n; ) {
            if (s[i] == '0') {
                int j = i;
                while (j < n && s[j] == '0') ++j;
                zs.push_back(i); ze.push_back(j - 1);
                i = j;
            } else ++i;
        }
        int nblocks = zs.size();

        vector<int> V;
        for (int j = 0; j + 1 < nblocks; ++j)
            V.push_back((ze[j] - zs[j] + 1) + (ze[j + 1] - zs[j + 1] + 1));

        int nv = V.size();
        vector<vector<int>> sparse{V};
        for (int half = 1; half * 2 <= nv; half *= 2) {
            auto& prev = sparse.back();
            vector<int> next;
            next.reserve(prev.size() - half);
            for (int i = 0; i + half < (int)prev.size(); ++i)
                next.push_back(max(prev[i], prev[i + half]));
            sparse.push_back(move(next));
        }

        auto rmq = [&](int lo, int hi) {
            int t = bit_width(unsigned(hi - lo + 1)) - 1;
            return max(sparse[t][lo], sparse[t][hi - (1 << t) + 1]);
        };

        auto clip = [&](int j, int l, int r) {
            return V[j] - max(0, l - zs[j]) - max(0, ze[j + 1] - r);
        };

        auto gain = [&](int l, int r) {
            if (nblocks < 2) return 0;
            int ja = ranges::lower_bound(ze, l) - ze.begin();
            int jb = ranges::upper_bound(zs, r) - zs.begin() - 2;
            if (ja > jb) return 0;
            return max({clip(ja, l, r), clip(jb, l, r), jb - ja >= 2 ? rmq(ja + 1, jb - 1) : 0});
        };

        vector<int> ans;
        ans.reserve(queries.size());
        for (auto& q : queries) ans.push_back(ones + gain(q[0], q[1]));
        return ans;
    }
};