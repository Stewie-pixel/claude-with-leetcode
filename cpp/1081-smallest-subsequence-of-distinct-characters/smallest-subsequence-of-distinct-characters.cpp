class Solution {
public:
    string smallestSubsequence(string s) {
        vector<int> freq(26, 0);
        vector<bool> stack(26, false);

        for (char c : s) {
            freq[c - 'a']++;
        }

        string st = "";
        for (char c : s) {
            int idx = c - 'a';
            freq[idx]--;

            if (stack[idx]) continue;

            while (!st.empty() && c < st.back() && freq[st.back() - 'a'] > 0) {
                stack[st.back() - 'a'] = false;
                st.pop_back();
            }
            st.push_back(c);
            stack[idx] = true;
        }
        return st;
    }
};