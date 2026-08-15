class Solution {
public:
    int elevatorRequests(int n, vector<int>& requests) {
        int time = 0, cur = 0;
        for (int r : requests) {
            time += abs(r - cur);
            cur = r;
        }
        return time;
    }
};