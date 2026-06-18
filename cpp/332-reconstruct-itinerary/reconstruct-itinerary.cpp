class Solution {
private:
    unordered_map<string, priority_queue<string, vector<string>, greater<string>>> graph;
    vector<string> itinerary;

    void dfs(const string& airport) {
        auto& destinations = graph[airport];
        while (!destinations.empty()) {
            string next = destinations.top();
            destinations.pop();
            dfs(next);
        }
        itinerary.push_back(airport);
    }

public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        for (const auto& ticket : tickets) {
            graph[ticket[0]].push(ticket[1]);
        }
        
        dfs("JFK");
        reverse(itinerary.begin(), itinerary.end());
        return itinerary;
    }
};