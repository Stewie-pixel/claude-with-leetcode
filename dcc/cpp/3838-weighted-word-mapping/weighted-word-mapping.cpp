class Solution {
public:
    string mapWordWeights(vector<string>& words, vector<int>& weights) {
        string ans;
        ans.reserve(words.size());

        for (const string& w : words) {
            int sum = 0;
            for (char c : w) {
                sum += weights[c - 'a'];
            }

            int mod = sum % 26;
            char map = char('z' - mod);
            ans.push_back(map);
        }

        return ans;
    }
};