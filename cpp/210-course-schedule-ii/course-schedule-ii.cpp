// -----------------------------------------------------------------------------
// Problem: Course Schedule II
// Source : https://leetcode.com/problems/course-schedule-ii/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. We need a valid ordering of courses given prerequisites — classic topological
//    sort problem on a directed graph.
// 2. Approach: build adjacency list + indegree array, use Kahn's algorithm
//    (BFS with a queue of zero-indegree nodes) to produce a valid course order.
// 3. If result contains all courses return that order, otherwise return {}.
//
// Time Complexity  : O(V + E)
// Space Complexity : O(V + E)
// -----------------------------------------------------------------------------

#include <vector>
#include <queue>
using namespace std;

// Solution class wrapper for the LeetCode interface
class Solution {
public:
    // findOrder: return a valid ordering of courses given prerequisites
    // numCourses: total number of courses (0..numCourses-1)
    // prerequisites: list of pairs [a, b] meaning b -> a (b is prereq of a)
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        // n is the number of vertices in the graph
        int n = numCourses;
        // adjacency list: for each course u, adj[u] lists courses that depend on u
        vector<vector<int>> adj(n);
        // indegree[v] = number of prerequisites for course v
        vector<int> indegree(n, 0);
        // res will hold the resulting topological order of courses
        vector<int> res;

        // build graph: for each prerequisite pair, add edge and update indegree
        for(auto &node : prerequisites){
            // node[1] -> node[0] (node[1] is prereq of node[0])
            adj[node[1]].push_back(node[0]);
            // increment indegree for destination
            indegree[node[0]]++;
        }

        // queue for Kahn's algorithm: stores nodes with indegree == 0
        queue<int> q;
        // initialize queue with all courses that have no prerequisites
        for(int i = 0; i < n; i++){
            if(indegree[i] == 0){
                // push course i as it can be taken immediately
                q.push(i);
            }
        }

        // process nodes in queue, producing topological order
        while(!q.empty()){
            // take one node with zero indegree
            int node = q.front();
            // remove it from queue
            q.pop();
            // append to result order
            res.push_back(node);

            // for every neighbor (course dependent on "node")
            for(auto adj : adj[node]){
                // reduce indegree since prerequisite "node" is taken
                indegree[adj]--;
                // if this neighbor has no more prerequisites, enqueue it
                if(indegree[adj] == 0){
                    q.push(adj);
                }
            }
        }
        
        // if we included all courses in the result, return order
        if(res.size() == n){
            return res;
        }
        // otherwise there was a cycle -> no valid ordering
        return {};
    }
};