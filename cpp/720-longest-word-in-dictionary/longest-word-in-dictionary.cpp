class Solution {
public:
    string longestWord(vector<string>& words) {
        sort(words.begin(), words.end());
        unordered_set<string> valid;
        valid.insert("");

        string best = "";

        for (auto& w : words) {
            string prefix = w.substr(0, w.size() - 1);
            if (valid.count(prefix)) {
                valid.insert(w);
                if (w.size() > best.size()) {
                    best = w;
                }
            }
        }

        return best;
    }
};