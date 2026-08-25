class Solution {
public:
    vector<int> remainingMethods(int n, int k, vector<vector<int>>& invocations) {
        vector<vector<int>> g(n);
        for (auto &e : invocations) {
            g[e[0]].push_back(e[1]);
        }

        vector<int> suspicious(n, 0);
        stack<int> st;
        st.push(k);
        suspicious[k] = 1;

        while (!st.empty()) {
            int u = st.top(); st.pop();
            for (int v : g[u]) {
                if (!suspicious[v]) {
                    suspicious[v] = 1;
                    st.push(v);
                }
            }
        }

        for (auto &e : invocations) {
            int a = e[0], b = e[1];
            if (!suspicious[a] && suspicious[b]) {
                vector<int> all(n);
                iota(all.begin(), all.end(), 0);
                return all;
            }
        }

        vector<int> ans;
        for (int i = 0; i < n; i++) {
            if (!suspicious[i]) {
                ans.push_back(i);
            }
        }
        return ans;
    }
};