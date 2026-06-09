// Source: https://leetcode.com/problems/valid-anagram/
// Author: Ngoc-Tien To

// Idea: Use an integer array of size 256 (ASCII charset) to count character frequencies.
// Step 1: In one loop, increment count for s[i] and decrement for t[i].
// Step 2: Traverse the count array, if any value != 0 → return false.
// Otherwise, return true.
// Complexity: O(n) time, O(1) space (fixed array size).


bool isAnagram(char* s, char* t) {
    int lenS = strlen(s);
    int lenT = strlen(t);
    if(lenS != lenT)
        return false;

    // Using calloc() to allocate and init 0 for all elements in array
    int *count = (int *)calloc(256, sizeof(int));
    for(int i = 0; i < lenS; i++){
        count[s[i]]++;
        count[t[i]]--;
    }
    for(int i = 0; i < 256; i++){
        if(count[i] != 0){
            free(count);
            return false;
        }
    }
    free(count);
    return true;
}