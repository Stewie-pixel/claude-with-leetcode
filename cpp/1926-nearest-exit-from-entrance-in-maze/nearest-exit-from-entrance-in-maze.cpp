class Solution {
public:
    int nearestExit(vector<vector<char>>& maze, vector<int>& entrance) {
        queue<pair<int, int>> pq;
        pq.push({entrance[0], entrance[1]});

        int moves = 1;
        int m = maze.size(), n = maze[0].size();

        vector<vector<int>> directions={{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

        maze[entrance[0]][entrance[1]] = '+';

        while (!pq.empty()) {
            int size = pq.size();

            for (int k = 0; k < size; k++) {
                auto [i, j] = pq.front();
                pq.pop();

                for (int size = 0; size < 4; size++) {
                    int x = i + directions[size][0];
                    int y = j + directions[size][1];

                    if (x < 0 || y < 0 || x >= m || y >= n || maze[x][y] == '+') {
                        continue;
                    } else if (x == 0 || y == 0 || x == m - 1 || y == n - 1) {
                        return moves;
                    }
                    maze[x][y] = '+';
                    pq.push({x, y});
                }
            }
            moves++;
        }
        return -1;
    }
};