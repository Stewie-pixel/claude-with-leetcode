class Solution {
public:
    string smallestPalindrome(string s) {
        int n = s.size();
        string left = s.substr(0, n / 2);
        sort(left.begin(), left.end());

        string middle = (n % 2 ? string(1, s[n / 2]) : "");
        string right(left.rbegin(), left.rend());

        return left + middle + right;
    }
};