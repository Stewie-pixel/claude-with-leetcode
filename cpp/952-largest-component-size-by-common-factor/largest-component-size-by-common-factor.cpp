class Solution {
public:
    vector<int> parent, rank_;

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
    }

    int largestComponentSize(vector<int>& nums) {
        int m = *max_element(nums.begin(), nums.end());
        parent.resize(m + 1);
        rank_.resize(m + 1, 0);
        iota(parent.begin(), parent.end(), 0);

        for (int num : nums) {
            int temp = num;
            for (int d = 2; d * d <= temp; d++) {
                if (temp % d == 0) {
                    unite(num, d);
                    while (temp % d == 0) temp /= d;
                }
            }
            if (temp > 1) unite(num, temp);
        }

        unordered_map<int, int> count;
        int ans = 0;
        for (int n : nums)
            ans = max(ans, ++count[find(n)]);
        return ans;
    }
};
