class Solution {
public:
    vector<int> findDegrees(vector<vector<int>>& matrix) {
        int m = matrix.size();
        int n = matrix[0].size();
        int count = 0;
        vector<int> ans(m, 0);

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == 1)
                    count++;
            }
            ans[i] += count;
            count = 0;
        }

        return ans;
    }
};
