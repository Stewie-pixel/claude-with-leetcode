#include <stdlib.h>
#include <stdbool.h>

struct Nodes{

    int *adj;

    int size;
};

struct Nodes *create(int n, int **list, int size){

    struct Nodes *nodes = (struct Nodes *)malloc(sizeof(struct Nodes) * n);

    for(int i = 0; i < n; i++){
        nodes[i].adj = (int *)malloc(sizeof(int) * n);
        nodes[i].size = 0;
    }

    for(int i = 0; i < size; i++){
        nodes[list[i][1]].adj[nodes[list[i][1]].size++] = list[i][0];
    }

    for(int i = 0; i < n; i++){
        nodes[i].adj = (int *)realloc(nodes[i].adj, sizeof(int) * nodes[i].size);
    }
    return nodes;
}

void freeNodes(struct Nodes *nodes, int n){

    for(int i = 0; i < n; i++){
        free(nodes[i].adj);
    }

    free(nodes);
}

bool dfs(struct Nodes *nodes, int u, int *color){

    color[u] = 1;

    for(int i = 0; i < nodes[u].size; i++){

        if(color[nodes[u].adj[i]] == 0){
            if(dfs(nodes, nodes[u].adj[i], color)){

                return true;
            }
        }

        else if(color[nodes[u].adj[i]] == 1){
            return true;
        }
    }

    color[u] = 2;
    return false;
}

bool canFinish(int numCourses, int** prerequisites, int prerequisitesSize, int* prerequisitesColSize) {

    int *color = (int *)calloc(numCourses, sizeof(int));

    struct Nodes *nodes = create(numCourses, prerequisites, prerequisitesSize);

    for(int i = 0; i < numCourses; i++){
        if(color[i] == 0){
            if(dfs(nodes, i, color)){

                free(color);
                freeNodes(nodes, numCourses);
                return false;
            }
        }
    }

    free(color);
    freeNodes(nodes, numCourses);
    return true;
}
