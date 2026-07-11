class Solution {
public:
    int removeCoveredIntervals(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end(),
        [](auto &a, auto &b) {
            if (a[0] == b[0]) return a[1] > b[1];
            return a[0] < b[0];
        });

        int count = 0;
        int maxEnd = 0;

        for (auto &v : intervals) {
            int l = v[0], r = v[1];
            if (r > maxEnd) {
                count++;
                maxEnd = r;
            }
        }
        return count;
    }
};