class Solution {
public:
    long long minEnergy(int n, int brightness, vector<vector<int>>& intervals) {
        long long min_bulbs = (brightness + 2) / 3;

        sort(intervals.begin(), intervals.end());

        long long total_time = 0;
        int cur_start = -1, cur_end = -1;

        for (auto& interval : intervals) {
            int s = interval[0], e = interval[1];
            if (cur_start == -1) {
                cur_start = s;
                cur_end = e;
            } else if (s <= cur_end) {
                cur_end = max(cur_end, e);
            } else {
                total_time += cur_end - cur_start + 1;
                cur_start = s;
                cur_end = e;
            }
        }

        if (cur_start != -1)
            total_time += cur_end - cur_start + 1;

        return min_bulbs * total_time;
    }
};