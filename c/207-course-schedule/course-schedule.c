// -----------------------------------------------------------------------------
// Problem: Course Schedule
// Source : https://leetcode.com/problems/course-schedule/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// IDEA
// - We must decide whether all courses can be finished given prerequisites.
// - Approach: build adjacency list and use DFS with node-coloring to detect cycles.
// - If any cycle exists, courses cannot be finished.
// Time Complexity  : O(V + E)
// Space Complexity : O(V + E)
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdbool.h> 

// compact adjacency representation for each node
struct Nodes{
    // dynamic array storing adjacent nodes (neighbors)
    int *adj;
    // current number of neighbors stored in adj
    int size;
};

// create: allocate and fill graph nodes from prerequisite list
// n      : number of courses
// list   : prerequisites edges list (pairs [to, from])
// size   : number of edges in list
struct Nodes *create(int n, int **list, int size){
    // allocate array of Nodes for n courses
    struct Nodes *nodes = (struct Nodes *)malloc(sizeof(struct Nodes) * n);
    // initialize each node's adjacency array and size
    for(int i = 0; i < n; i++){
        nodes[i].adj = (int *)malloc(sizeof(int) * n);
        nodes[i].size = 0;
    }
    // fill adjacency arrays using the edge list: list[i][1] -> list[i][0]
    for(int i = 0; i < size; i++){
        nodes[list[i][1]].adj[nodes[list[i][1]].size++] = list[i][0];
    }
    // shrink adjacency arrays to their actual size
    for(int i = 0; i < n; i++){
        nodes[i].adj = (int *)realloc(nodes[i].adj, sizeof(int) * nodes[i].size);
    }
    return nodes;
}

// freeNodes: release memory used by nodes
void freeNodes(struct Nodes *nodes, int n){
    // free each node's adj array
    for(int i = 0; i < n; i++){
        free(nodes[i].adj);
    }
    // free array of nodes
    free(nodes);
}

// dfs: detects cycle starting from u using coloring
// color array: 0 = unvisited, 1 = visiting, 2 = visited
bool dfs(struct Nodes *nodes, int u, int *color){
    // mark as visiting
    color[u] = 1;
    // explore neighbors
    for(int i = 0; i < nodes[u].size; i++){
        // if neighbor not visited, recurse
        if(color[nodes[u].adj[i]] == 0){
            if(dfs(nodes, nodes[u].adj[i], color)){
                // cycle detected in recursion
                return true;
            }
        }
        // if neighbor is visiting, we found a back-edge -> cycle
        else if(color[nodes[u].adj[i]] == 1){
            return true;
        }
    }
    // mark fully visited
    color[u] = 2;
    return false; // no cycle from u
}

// canFinish: wrapper used by LeetCode to determine whether all courses
// can be completed (i.e., graph has no cycles)
bool canFinish(int numCourses, int** prerequisites, int prerequisitesSize, int* prerequisitesColSize) {
    // color array to track DFS state for each node
    int *color = (int *)calloc(numCourses, sizeof(int));
    // build graph from prerequisites
    struct Nodes *nodes = create(numCourses, prerequisites, prerequisitesSize);

    // check every course; if unvisited, run DFS cycle check
    for(int i = 0; i < numCourses; i++){
        if(color[i] == 0){
            if(dfs(nodes, i, color)){
                // cleanup and report cycle found
                free(color);
                freeNodes(nodes, numCourses);
                return false;
            }
        }
    }

    // no cycle found: cleanup and return true
    free(color);
    freeNodes(nodes, numCourses);
    return true;
}