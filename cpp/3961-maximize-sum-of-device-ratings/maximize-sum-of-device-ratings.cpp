class Solution {
public:
    long long maxRatings(vector<vector<int>>& units) {
        int m = units.size();
        int n = units[0].size();

        if (n == 1) {
            long long total = 0;
            for (auto& device : units)
                total += device[0];
            return total;
        }

        long long G = LLONG_MAX, sumMin2 = 0, minMin2 = LLONG_MAX;

        for (auto& device : units) {
            long long min1 = LLONG_MAX, min2 = LLONG_MAX;
            for (int u : device) {
                if (u <= min1) { min2 = min1; min1 = u; }
                else if (u < min2) { min2 = u; }
            }
            G = min(G, min1);
            sumMin2 += min2;
            minMin2 = min(minMin2, min2);
        }

        return G + sumMin2 - minMin2;
    }
};