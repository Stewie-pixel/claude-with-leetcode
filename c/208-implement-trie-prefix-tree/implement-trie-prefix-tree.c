#define N 26

typedef struct Trie {
    struct Trie *children[26];
    bool isEnd;
} Trie;

Trie* trieCreate() {

    Trie *obj = (Trie *)calloc(1, sizeof(Trie));

    for(int i = 0; i < N; i++) {
        obj->children[i] = NULL;
    }

    obj->isEnd = false;
    return obj;
}

void trieInsert(Trie* obj, char* word) {
    if(obj == NULL) {
        return;
    }

    int len = strlen(word);
    Trie *current = obj;

    for(int i = 0; i < len; i++) {
        int c = word[i] - 'a';

        if(current->children[c] == NULL) {
            current->children[c] = trieCreate();
        }

        current = current->children[c];
    }

    current->isEnd = true;
}

bool trieSearch(Trie* obj, char* word) {
    if(obj == NULL) {
        return false;
    }

    int len = strlen(word);
    Trie *temp = obj;

    for(int i = 0; i < len; i++) {
        int c = word[i] - 'a';

        if(temp->children[c] == NULL) {
            return false;
        }

        temp = temp->children[c];
    }

    return temp->isEnd;
}

bool trieStartsWith(Trie* obj, char* prefix) {
    if(obj == NULL) {
        return false;
    }

    int len = strlen(prefix);
    Trie *temp = obj;

    for(int i = 0; i < len; i++) {
        int c = prefix[i] - 'a';

        if(temp->children[c] == NULL) {
            return false;
        }

        temp = temp->children[c];
    }

    return true;
}

void trieFree(Trie* obj) {
    if(obj == NULL) {
        return;
    }

    for(int i = 0; i < N; i++) {
        trieFree(obj->children[i]);
    }

    free(obj);
}
