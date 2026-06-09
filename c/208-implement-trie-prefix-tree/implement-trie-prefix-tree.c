// -----------------------------------------------------------------------------
// Problem: Implement Trie (Prefix Tree)
// Source : https://leetcode.com/problems/implement-trie-prefix-tree/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- IDEA ----------------------------------
//
// 1. Each Trie node contains 26 child pointers (for 'a' to 'z') and a boolean flag `isEnd`.
// 2. Insert operation: traverse each character and create nodes if missing.
// 3. Search operation: traverse nodes, return true only if the final node isEnd == true.
// 4. Prefix operation: similar to search but doesn’t require isEnd == true.
// 5. Free operation: recursively free all children and current node.
//
// Time Complexity  : O(L) for each operation, where L = length of the word/prefix.
// Space Complexity : O(N), where N = total number of inserted characters.
// -----------------------------------------------------------------------------

#define N 26

typedef struct Trie {
    struct Trie *children[26]; // Array to store child nodes for each lowercase letter
    bool isEnd;                // Marks whether this node is the end of a word
} Trie;


// ------------------------------- CREATE TRIE ----------------------------------

Trie* trieCreate() {
    // Allocate memory for a new Trie node and initialize all fields
    Trie *obj = (Trie *)calloc(1, sizeof(Trie));

    // Initialize all child pointers to NULL
    for(int i = 0; i < N; i++) {
        obj->children[i] = NULL;
    }

    // The new node is not an end of any word yet
    obj->isEnd = false;
    return obj;
}


// ------------------------------- INSERT WORD ----------------------------------

void trieInsert(Trie* obj, char* word) {
    if(obj == NULL) {
        return;
    }

    int len = strlen(word);
    Trie *current = obj;
    // Traverse each character of the input word
    for(int i = 0; i < len; i++) {
        int c = word[i] - 'a'; // Convert char to index (0–25)

        // Create a new node if the path doesn’t exist
        if(current->children[c] == NULL) {
            current->children[c] = trieCreate();
        }

        // Move to the next node
        current = current->children[c];
    }

    // Mark the last node as the end of the word
    current->isEnd = true;
}


// ------------------------------- SEARCH WORD ----------------------------------

bool trieSearch(Trie* obj, char* word) {
    if(obj == NULL) {
        return false;
    }

    int len = strlen(word);
    Trie *temp = obj;
    // Traverse through each character of the word
    for(int i = 0; i < len; i++) {
        int c = word[i] - 'a';

        // If the path doesn’t exist → word not found
        if(temp->children[c] == NULL) {
            return false;
        }

        // Move deeper into the Trie
        temp = temp->children[c];
    }

    // The word exists only if isEnd == true
    return temp->isEnd;
}


// ------------------------------- PREFIX CHECK ----------------------------------

bool trieStartsWith(Trie* obj, char* prefix) {
    if(obj == NULL) {
        return false;
    }

    int len = strlen(prefix);
    Trie *temp = obj;
    // Traverse each character of the prefix
    for(int i = 0; i < len; i++) {
        int c = prefix[i] - 'a';

        // If the path doesn’t exist → prefix not found
        if(temp->children[c] == NULL) {
            return false;
        }

        temp = temp->children[c];
    }

    // Prefix found
    return true;
}


// ------------------------------- FREE MEMORY ----------------------------------

void trieFree(Trie* obj) {
    if(obj == NULL) {
        return;
    }

    // Recursively free all child nodes first
    for(int i = 0; i < N; i++) {
        trieFree(obj->children[i]);
    }

    // Then free the current node
    free(obj);
}
