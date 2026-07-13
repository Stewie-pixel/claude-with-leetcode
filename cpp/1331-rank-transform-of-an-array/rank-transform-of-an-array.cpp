class Solution {
public:
    vector<int> arrayRankTransform(vector<int>& arr) {
        vector<int> sorted = arr;
        sort(sorted.begin(), sorted.end());
        
        unordered_map<int, int> rank;
        int r = 1;
        for (int x : sorted) {
            if (!rank.count(x)) {
                rank[x] = r++;
            }
        }

        vector<int> ans(arr.size());
        for (int i = 0; i < arr.size(); i++) {
            ans[i] = rank[arr[i]];
        }
        return ans;
    }
};