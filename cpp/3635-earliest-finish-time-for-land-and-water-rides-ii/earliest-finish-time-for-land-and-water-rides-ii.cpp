class Solution {
public:
    int earliestFinishTime(vector<int>& landStartTime, vector<int>& landDuration, vector<int>& waterStartTime, vector<int>& waterDuration) {
        int n = landStartTime.size(), m = waterStartTime.size();

        int minLandEnd = INT_MAX, minWaterEnd = INT_MAX;

        for (int i = 0; i < n; i++) minLandEnd  = min(minLandEnd,  landStartTime[i]  + landDuration[i]);

        for (int j = 0; j < m; j++) minWaterEnd = min(minWaterEnd, waterStartTime[j] + waterDuration[j]);

        int ans = INT_MAX;

        for (int j = 0; j < m; j++)
            ans = min(ans, max(minLandEnd, waterStartTime[j]) + waterDuration[j]);

        for (int i = 0; i < n; i++)
            ans = min(ans, max(minWaterEnd, landStartTime[i]) + landDuration[i]);

        return ans;
    }
};
