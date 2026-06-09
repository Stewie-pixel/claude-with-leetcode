class Solution {
public:
    string decodeString(string s) {
        stack<int> nums;
        stack<string> strs;

        string res = "";
        int num = 0;

        for (char c : s) {
            if (isdigit(c)) {
                num = num * 10 + (c - '0');
            }
            else if (c == '[') {
                nums.push(num);
                strs.push(res);
                num = 0;
                res = "";
            }
            else if (c == ']') {
                int repeat = nums.top(); nums.pop();
                string prev = strs.top(); strs.pop();

                string expanded = "";
                while (repeat--) expanded += res;

                res = prev + expanded;
            }
            else {
                res += c;
            }
        }

        return res;
    }
};
