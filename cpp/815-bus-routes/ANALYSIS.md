```cpp
class Solution {
public:
    int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
        if (source == target) return 0;

        int n = routes.size();
        unordered_map<int, vector<int>> stops;
        for (int bus = 0; bus < n; ++bus) {
            for (int stop : routes[bus]) {
                stops[stop].push_back(bus);
            }
        }

        queue<int> q;
        vector<bool> visitedBus(n, false);
        // optional: unordered_set<int> visitedStop; // not required for correctness

        for (int bus : stops[source]) {
            q.push(bus);
            visitedBus[bus] = true;
        }

        int busesTaken = 1; // we have boarded the first bus
        while (!q.empty()) {
            int levelSize = q.size();
            while (levelSize--) {
                int bus = q.front(); q.pop();
                for (int stop : routes[bus]) {
                    if (stop == target) return busesTaken;
                    for (int nextBus : stops[stop]) {
                        if (!visitedBus[nextBus]) {
                            visitedBus[nextBus] = true;
                            q.push(nextBus);
                        }
                    }
                }
            }
            ++busesTaken;
        }
        return -1;
    }
};
```

https://www.youtube.com/watch?v=odmGyOJM5EY