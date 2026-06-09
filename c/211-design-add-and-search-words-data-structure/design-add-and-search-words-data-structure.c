#define N 26

typedef struct WordDictionary {
    struct WordDictionary *child[N];
    bool isEnd;
} WordDictionary;

WordDictionary* wordDictionaryCreate() {

    WordDictionary *obj = (WordDictionary *)malloc(sizeof(WordDictionary));

    for (int i = 0; i < N; i++) {
        obj->child[i] = NULL;
    }

    obj->isEnd = false;
    return obj;
}

void wordDictionaryAddWord(WordDictionary* obj, char* word) {
    if (obj == NULL) {
        return;
    }

    int len = strlen(word);

    for (int i = 0; i < len; i++) {
        int c = word[i] - 'a';

        if (obj->child[c] == NULL) {
            obj->child[c] = wordDictionaryCreate();
        }

        obj = obj->child[c];
    }

    obj->isEnd = true;
}

bool wordDictionarySearch(WordDictionary* obj, char* word) {
    if (obj == NULL) {
        return false;
    }

    WordDictionary *temp = obj;
    int len = strlen(word);

    for (int i = 0; i < len; i++) {

        if (word[i] == '.') {
            for (int j = 0; j < N; j++) {

                if (temp->child[j] != NULL) {
                    if (wordDictionarySearch(temp->child[j], word + i + 1)) {
                        return true;
                    }
                }
            }

            return false;
        }

        int c = word[i] - 'a';
        if (temp->child[c] == NULL) {
            return false;
        }
        temp = temp->child[c];
    }

    return temp->isEnd;
}

void wordDictionaryFree(WordDictionary* obj) {
    if (obj == NULL) {
        return;
    }

    for (int i = 0; i < N; i++) {
        wordDictionaryFree(obj->child[i]);
    }

    free(obj);
}
