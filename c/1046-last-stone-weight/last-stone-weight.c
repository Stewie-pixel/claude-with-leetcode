// -----------------------------------------------------------------------------
// Problem: Last Stone Weight
// Source : https://leetcode.com/problems/last-stone-weight/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// 1. Task: Repeatedly smash the two heaviest stones; if equal both destroyed,
//    else the difference remains. Return last stone weight or 0.
// 2. Approach: simple simulation using sorting each round (or heap for better
//    complexity). This implementation sorts repeatedly for simplicity.
//
// Time Complexity  : O(n^2 log n) with repeated sorts (can be improved with heap)
// Space Complexity : O(1)
// -----------------------------------------------------------------------------

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