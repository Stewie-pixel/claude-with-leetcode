class Solution {
public:
    bool findSafeWalk(vector<vector<int>>& grid, int health) {
        int m = grid.size(); 
        int n = grid[0].size();
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));
        deque<pair<int, int>> dq;
        
        dist[0][0] = grid[0][0];
        dq.push_front({0, 0});
        
        int dx[4] = {1, -1, 0, 0};
        int dy[4] = {0, 0, 1, -1};
        
        while (!dq.empty()) {
            auto [x, y] = dq.front();
            dq.pop_front();
            
            for (int k = 0; k < 4; k++) {
                int nx = x + dx[k], ny = y + dy[k];
                if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;

                int w = grid[nx][ny];
                if (dist[x][y] + w < dist[nx][ny]) {
                    dist[nx][ny] = dist[x][y] + w;
                    if (w == 0)
                        dq.push_front({nx, ny});
                    else
                        dq.push_back({nx, ny});
                }
            }
        }

        return dist[m - 1][n - 1] < health;
    }
};