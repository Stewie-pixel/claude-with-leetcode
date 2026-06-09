#include <stdlib.h>

struct Node {
    int val;
    int numNeighbors;
    struct Node** neighbors;
};

struct Node *visited[101];

struct Node *traversal(struct Node *node){

    if(node == NULL){
        return NULL;
    }

    if(visited[node->val] != NULL){
        return visited[node->val];
    }

    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->val = node->val;
    newNode->numNeighbors = node->numNeighbors;
    newNode->neighbors = (struct Node **)malloc(sizeof(struct Node *) * newNode->numNeighbors);

    visited[node->val] = newNode;

    for(int i = 0; i < newNode->numNeighbors; i++){
        newNode->neighbors[i] = traversal(node->neighbors[i]);
    }

    return newNode;
}

struct Node *cloneGraph(struct Node *s) {

    for(int i = 0; i <= 100; i++){
        visited[i] = NULL;
    }
    return traversal(s);
}
