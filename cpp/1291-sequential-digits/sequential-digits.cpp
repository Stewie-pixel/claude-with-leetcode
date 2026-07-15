class Solution {
public:
    vector<int> sequentialDigits(int low, int high) {
        string digits = "123456789";
        vector<int> ans;

        for (int len = 2; len <= 9; len++) {
            for (int start = 0; start + len <= 9; start++) {
                int num = stoi(digits.substr(start, len));
                if (low <= num && num <= high) {
                    ans.push_back(num);
                }
            }
        }
        return ans;
    }
};