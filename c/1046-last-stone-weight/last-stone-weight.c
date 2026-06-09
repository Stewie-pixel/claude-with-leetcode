#include <stdlib.h>

int compare(const void *a, const void *b){
    return *(int *)b - *(int *)a;
}

int lastStoneWeight(int* stones, int stonesSize) {
    if(stonesSize <= 1){
        return stones[0];
    }

    int n = stonesSize;
    while(n != 1){
        qsort(stones, stonesSize, sizeof(int), compare);

        stones[0] -= stones[1];
        stones[1] = 0;

        n--;
    }

    return stones[0];
}
