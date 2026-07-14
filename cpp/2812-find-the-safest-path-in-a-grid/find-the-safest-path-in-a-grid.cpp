class Solution {
public:
    int maximumSafenessFactor(vector<vector<int>>& grid) {
        int n = grid.size();
        vector<vector<int>> dist(n, vector<int>(n, INT_MAX));
        queue<pair<int,int>> q;

        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                if (grid[r][c] == 1) {
                    dist[r][c] = 0;
                    q.push({r, c});
                }
            }
        }

        int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();
            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr >= 0 && nr < n && nc >= 0 && nc < n) {
                    if (dist[nr][nc] > dist[r][c] + 1) {
                        dist[nr][nc] = dist[r][c] + 1;
                        q.push({nr, nc});
                    }
                }
            }
        }

        int low = 0, high = n * 2;
        auto can = [&](int v) {
            if (dist[0][0] < v) return false;
            queue<pair<int,int>> qq;
            vector<vector<bool>> vis(n, vector<bool>(n, false));
            qq.push({0,0});
            vis[0][0] = true;

            while (!qq.empty()) {
                auto [r, c] = qq.front(); qq.pop();
                if (r == n-1 && c == n-1) return true;

                for (auto& d : dirs) {
                    int nr = r + d[0], nc = c + d[1];
                    if (nr >= 0 && nr < n && nc >= 0 && nc < n &&
                        !vis[nr][nc] && dist[nr][nc] >= v) {
                        vis[nr][nc] = true;
                        qq.push({nr, nc});
                    }
                }
            }
            return false;
        };

        int ans = 0;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (can(mid)) {
                ans = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        return ans;
    }
};