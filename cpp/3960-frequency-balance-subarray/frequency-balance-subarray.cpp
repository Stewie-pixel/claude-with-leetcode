class Solution {
public:
    int getLength(vector<int>& nums) {
        int n = nums.size(), best = 1;

        for (int i = 0; i < n; i++) {
            unordered_map<int,int> freq, freq_cnt;
            int ndistinct = 0;

            for (int j = i; j < n; j++) {
                int v = nums[j];
                int old_f = freq.count(v) ? freq[v] : 0;
                freq[v] = old_f + 1;

                if (old_f > 0 && --freq_cnt[old_f] == 0) {
                    freq_cnt.erase(old_f);
                    ndistinct--;
                }
                if (!freq_cnt.count(old_f + 1)) ndistinct++;
                freq_cnt[old_f + 1]++;

                if ((int)freq.size() == 1) {
                    best = max(best, j - i + 1);
                } else if (ndistinct == 2) {
                    int f1 = INT_MAX, f2 = INT_MIN;
                    for (auto& [f, _] : freq_cnt) {
                        f1 = min(f1, f); f2 = max(f2, f);
                    }
                    if (f2 == 2 * f1) best = max(best, j - i + 1);
                }
            }
        }
        return best;
    }
};