// -----------------------------------------------------------------------------
// Problem: Clone Graph
// Source : https://leetcode.com/problems/clone-graph/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Task: Create a deep clone of an undirected graph (given a start node).
// 2. Approach: use DFS to traverse the original graph and create a cloned
//    node for each visited original node. Use a `visited` map/array to store
//    mapping original->clone so we (a) avoid recreating nodes and (b) handle
//    cycles correctly.
// 3. Implementation notes in this file:
//    - `visited[val]` caches the cloned node pointer for an original node with
//      value `val` (problem inputs in this repo assume values in [0..100]).
//    - `traversal()` does the recursive DFS clone: create node, store it in
//      visited, then clone neighbors recursively and link them.
//    - `cloneGraph()` initializes visited[] and starts traversal from `s`.
//
// Time Complexity  : O(N + E) where N = number of nodes visited, E = edges
// Space Complexity : O(N) for the visited mapping + recursion stack
// -----------------------------------------------------------------------------

#include <stdlib.h>

// Node structure for graph adjacency; uses an adjacency array of Node *
// - val: node value/identifier (used as index in visited[])
// - numNeighbors: number of adjacent nodes
// - neighbors: dynamic array of pointers to neighbor nodes
struct Node {
    int val;
    int numNeighbors;
    struct Node** neighbors;
};

/* global visited array (indexed by node->val).
 * Size 101 matches the problem/test data in this repo.
 * visited[val] stores the pointer to the cloned node for fast lookup.
 */
struct Node *visited[101];

/* Depth-first traversal helper that clones the given node and
 * recursively clones (and links) its neighbors. Returns pointer
 * to the cloned node corresponding to `node`.
 */
struct Node *traversal(struct Node *node){
    // base case: null input
    if(node == NULL){
        return NULL;
    }
    
    /* if we've already created a clone for this node, return it
     * (this handles cycles and avoids infinite recursion) */
    if(visited[node->val] != NULL){
        return visited[node->val];
    }

    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->val = node->val;
    newNode->numNeighbors = node->numNeighbors;
    newNode->neighbors = (struct Node **)malloc(sizeof(struct Node *) * newNode->numNeighbors);

    // mark this node as visited by storing cloned pointer
    visited[node->val] = newNode;

    // recursively clone and connect every neighbor
    for(int i = 0; i < newNode->numNeighbors; i++){
        newNode->neighbors[i] = traversal(node->neighbors[i]);
    }

    return newNode;
}

/* Entry point: prepares visited[] then begins traversal to clone
 * the entire graph starting from node `s`.
 */
struct Node *cloneGraph(struct Node *s) {
    // clear visited[] (reset clone cache) before starting
    for(int i = 0; i <= 100; i++){
        visited[i] = NULL;
    }
    return traversal(s);
}