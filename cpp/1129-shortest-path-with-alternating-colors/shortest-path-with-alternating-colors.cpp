class Solution {
public:
    vector<int> shortestAlternatingPaths(int n, vector<vector<int>>& redEdges, vector<vector<int>>& blueEdges) {
        vector<vector<int>> red(n), blue(n);
        for (auto &e : redEdges) red[e[0]].push_back(e[1]);
        for (auto &e : blueEdges) blue[e[0]].push_back(e[1]);

        vector<int> dist(n, -1);
        queue<array<int, 3>> q;

        vector<vector<bool>> visited(2, vector<bool>(n, false));

        q.push({0, -1, 0});
        dist[0] = 0;

        while (!q.empty()) {
            auto [node, lastColor, d] = q.front();
            q.pop();

            if (lastColor != 1) {
                for (int next : blue[node]) {
                    if (!visited[1][next]) {
                        visited[1][next] = true;
                        if (dist[next] == -1) dist[next] = d + 1;
                        q.push({next, 1, d + 1});
                    }
                }
            }

            if (lastColor != 0) {
                for (int next : red[node]) {
                    if (!visited[0][next]) {
                        visited[0][next] = true;
                        if (dist[next] == -1) dist[next] = d + 1;
                        q.push({next, 0, d + 1});
                    }
                }
            }
        }
        return dist;
    }
};