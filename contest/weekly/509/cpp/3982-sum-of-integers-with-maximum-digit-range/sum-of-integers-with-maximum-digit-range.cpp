class Solution {
public:
    int maxDigitRange(vector<int>& nums) {
        auto range = [&](int x) {
            int min_digit = 9, max_digit = 0;
            while (x > 0) {
                int d = x % 10;
                max_digit = max(max_digit, d);
                min_digit = min(min_digit, d);
                x /= 10;
            }
            return max_digit - min_digit;
        };

        int best = 0;
        vector<int> ranges;
        for (int x : nums) {
            int r = range(x);
            ranges.push_back(r);
            best = max(best, r);
        }

        int sum = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (ranges[i] == best) {
                sum += nums[i];
            }
        }
        return sum;
    }
};