class Solution {
public:
    int minCostConnectPoints(vector<vector<int>>& points) {
        int n = points.size();
        vector<int> minDist(n, INT_MAX);
        vector<bool> used(n, false);

        minDist[0] = 0;
        int result = 0;

        for (int i = 0; i < n; i++) {
            int idx = -1;

            for (int j = 0; j < n; j++) {
                if (!used[j] && (idx == -1 || minDist[j] < minDist[idx])) {
                    idx = j;
                }
            }

            used[idx] = true;
            result += minDist[idx];

            for (int j = 0; j < n; j++) {
                if (!used[j]) {
                    int dist = abs(points[idx][0] - points[j][0]) +
                               abs(points[idx][1] - points[j][1]);
                    minDist[j] = min(minDist[j], dist);
                }
            }
        }

        return result;
    }
};