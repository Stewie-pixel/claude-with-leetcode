class Solution {
public:
    vector<string> res;
    vector<string> path;

    void backtrack(const string &s, int index) {
        if (path.size() == 4) {
            if (index == s.size()) {
                res.push_back(path[0] + "." + path[1] + "." + path[2] + "." + path[3]);
            }
            return;
        }

        for (int len = 1; len <= 3; len++) {
            if (index + len > s.size()) break;

            string part = s.substr(index, len);
            if (part[0] == '0' && part.size() > 1) continue;
            if (stoi(part) > 255) continue;

            path.push_back(part);
            backtrack(s, index + len);
            path.pop_back();
        }
    } 
    vector<string> restoreIpAddresses(string s) {
        backtrack(s, 0);
        return res;
    }
};