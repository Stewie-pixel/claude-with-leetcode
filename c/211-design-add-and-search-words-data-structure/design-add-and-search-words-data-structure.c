// -----------------------------------------------------------------------------
// Problem : Design Add and Search Words Data Structure
// Source  : https://leetcode.com/problems/design-add-and-search-words-data-structure/
// Author  : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- IDEA ----------------------------------
//
// This problem is an extension of a Trie (prefix tree):
// 1️. Each node contains 26 pointers (for 'a'–'z') and a boolean `isEnd`.
// 2️. Insert: Similar to normal Trie insertion.
// 3️. Search: When encountering '.', we recursively explore all 26 possible paths.
// 4️. Free: Recursively delete all nodes.
//
// Time Complexity:
//   - addWord()  : O(L), where L = word length
//   - search()   : O(26^D * L), where D = number of '.' (worst case)
//
// Space Complexity: O(N), where N = total number of inserted characters
// -----------------------------------------------------------------------------


#define N 26   // Number of lowercase English letters


// ---------------------------------- STRUCT DEFINITION ----------------------------------

typedef struct WordDictionary {
    struct WordDictionary *child[N];  // Pointers to child nodes (for 'a'–'z')
    bool isEnd;                       // Marks end of a complete word
} WordDictionary;


// ---------------------------------- CREATE DICTIONARY ----------------------------------

WordDictionary* wordDictionaryCreate() {
    // Allocate memory for a new WordDictionary node
    WordDictionary *obj = (WordDictionary *)malloc(sizeof(WordDictionary));

    // Initialize all child pointers to NULL
    for (int i = 0; i < N; i++) {
        obj->child[i] = NULL;
    }

    // Initially, this node does not mark the end of any word
    obj->isEnd = false;
    return obj;
}


// ---------------------------------- ADD A WORD ----------------------------------

void wordDictionaryAddWord(WordDictionary* obj, char* word) {
    if (obj == NULL) {
        return;
    }

    int len = strlen(word);

    // Traverse each character in the word
    for (int i = 0; i < len; i++) {
        int c = word[i] - 'a'; // Convert char to index (0–25)

        // Create a new node if not already present
        if (obj->child[c] == NULL) {
            obj->child[c] = wordDictionaryCreate();
        }

        // Move to the next node
        obj = obj->child[c];
    }

    // Mark the end of the word
    obj->isEnd = true;
}


// ---------------------------------- SEARCH A WORD ----------------------------------

bool wordDictionarySearch(WordDictionary* obj, char* word) {
    if (obj == NULL) {
        return false;
    }

    WordDictionary *temp = obj;
    int len = strlen(word);

    // Traverse character by character
    for (int i = 0; i < len; i++) {
        // Case 1: If current character is '.', it can represent any letter
        if (word[i] == '.') {
            for (int j = 0; j < N; j++) {
                // For each possible child, if it exists, continue the search
                if (temp->child[j] != NULL) {
                    if (wordDictionarySearch(temp->child[j], word + i + 1)) {
                        return true; // Found a valid match
                    }
                }
            }
            // None of the children matched
            return false;
        }

        // Case 2: Normal character search
        int c = word[i] - 'a';
        if (temp->child[c] == NULL) {
            return false; // Path doesn't exist
        }
        temp = temp->child[c];
    }

    // Return true only if we reached a node marking the end of a word
    return temp->isEnd;
}


// ---------------------------------- FREE MEMORY ----------------------------------

void wordDictionaryFree(WordDictionary* obj) {
    if (obj == NULL) {
        return;
    }

    // Recursively free all child nodes
    for (int i = 0; i < N; i++) {
        wordDictionaryFree(obj->child[i]);
    }

    // Free current node
    free(obj);
}