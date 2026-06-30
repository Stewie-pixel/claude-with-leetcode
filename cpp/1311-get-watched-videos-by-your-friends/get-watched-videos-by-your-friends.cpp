class Solution {
public:
    vector<string> watchedVideosByFriends(vector<vector<string>>& watchedVideos, vector<vector<int>>& friends, int id, int level) {
        int n = friends.size();
        vector<int> dist(n, -1);
        queue<int> q;
        dist[id] = 0;
        q.push(id);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : friends[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }

        unordered_map<string,int> freq;
        for (int i = 0; i < n; i++) {
            if (dist[i] == level) {
                for (auto &s : watchedVideos[i]) freq[s]++;
            }
        }

        vector<pair<string,int>> arr;
        for (auto &p : freq) arr.push_back(p);

        sort(arr.begin(), arr.end(), [](auto &a, auto &b){
            if (a.second != b.second) return a.second < b.second;
            return a.first < b.first;
        });

        vector<string> res;
        for (auto &p : arr) res.push_back(p.first);
        return res;
    }
};