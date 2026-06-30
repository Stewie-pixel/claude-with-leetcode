class Solution {
public:
    int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
        if (source == target) return 0;

        int n = routes.size();

        unordered_map<int, vector<int>> stops;
        for (int bus = 0; bus < n; bus++) {
            for (int stop : routes[bus]) {
                stops[stop].push_back(bus);
            }
        }

        queue<int> q;
        vector<bool> visited_bus(n, false);
        unordered_set<int> visited_stop;

        for (int bus : stops[source]) {
            q.push(bus);
            visited_bus[bus] = true;
        }

        int busesTaken = 1;

        while (!q.empty()) {
            int size = q.size();
            while (size--) {
                int bus = q.front();
                q.pop();

                for (int stop : routes[bus]) {
                    if (stop == target) return busesTaken;

                    for (int next : stops[stop]) {
                        if (!visited_bus[next]) {
                            visited_bus[next] = true;
                            q.push(next);
                        }
                    }
                }
            }
            busesTaken++;
        }
        return -1;
    }
};