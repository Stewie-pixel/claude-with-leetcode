class Solution {
public:
    string rearrangeString(string s, char x, char y) {
        string before, middle, after;

        for (char c : s) {
            if (c == y) {
                before += c;
            } else if (c == x) {
                after += c;
            } else {
                middle += c;
            }
        }
        return before + middle + after;
    }
};