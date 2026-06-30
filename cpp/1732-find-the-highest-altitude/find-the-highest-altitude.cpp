class Solution {
public:
    int largestAltitude(vector<int>& gain) {
        int highest = 0;
        int altitude = 0;
        
        for (int n : gain) {
            altitude += n;
            highest = max(highest, altitude);
        }
        return highest;
    }
};