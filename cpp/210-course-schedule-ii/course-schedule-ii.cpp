#include <vector>
#include <queue>
using namespace std;

class Solution {
public:

    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {

        int n = numCourses;

        vector<vector<int>> adj(n);

        vector<int> indegree(n, 0);

        vector<int> res;

        for(auto &node : prerequisites){

            adj[node[1]].push_back(node[0]);

            indegree[node[0]]++;
        }

        queue<int> q;

        for(int i = 0; i < n; i++){
            if(indegree[i] == 0){

                q.push(i);
            }
        }

        while(!q.empty()){

            int node = q.front();

            q.pop();

            res.push_back(node);

            for(auto adj : adj[node]){

                indegree[adj]--;

                if(indegree[adj] == 0){
                    q.push(adj);
                }
            }
        }

        if(res.size() == n){
            return res;
        }

        return {};
    }
};
