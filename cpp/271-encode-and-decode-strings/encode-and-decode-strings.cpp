// Source: https://leetcode.com/problems/encode-and-decode-strings/
// Author: Ngoc-Tien To

// Idea:
// 1. For each input string, first encode its length (convert length to string).
// 2. Append a delimiter (e.g., '#') to separate the length and the actual string.
// 3. Concatenate all encoded strings into one result string.
// 4. During decoding, read the length before '#', then extract the substring accordingly.

class Solution {
public:
    string encode(vector<string>& strs) {
        if(strs.empty())
            return "";
        vector<int> sizes;
        string result = "";
        for(string& str : strs){
            sizes.push_back(str.size());
        }
        for(int sz : sizes){
            result += to_string(sz) + ',';
        }
        result += '#';
        for(string& str : strs){
            result += str;
        }
        return result;
    }

    vector<string> decode(string s) {
        if(s.empty())
            return {};
        vector<string> result;
        vector<int> sizes;
        int i = 0;
        while(s[i] != '#'){
            string cur = "";
            while(s[i] != ','){
                cur += s[i];
                i++;
            }
            sizes.push_back(stoi(cur));
            i++;
        }
        i++;
        for(int sz : sizes){
            result.push_back(s.substr(i, sz));
            i += sz;
        }
        return result;
    }
};
