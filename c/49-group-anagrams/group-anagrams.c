struct Helper{
    char *str;
    int index;
};

int compareChars(const void *a, const void *b){
    return *(char *)a - *(char *)b;
}

int compareHelpers(const void *a, const void *b){
    struct Helper *temp1 = (struct Helper *)a;
    struct Helper *temp2 = (struct Helper *)b;
    return strcmp(temp1->str, temp2->str);
}

struct Helper *createHelperArray(char **strs, int strsSize){
    struct Helper *s = (struct Helper *)malloc(sizeof(struct Helper) * strsSize);

    for(int i = 0; i < strsSize; i++){
        s[i].str = (char *)malloc(sizeof(char) * (strlen(strs[i]) + 1));
        strcpy(s[i].str, strs[i]);
        s[i].index = i;
    }

    for(int i = 0; i < strsSize; i++){
        qsort(s[i].str, strlen(s[i].str), sizeof(char), compareChars);
    }

    qsort(s, strsSize, sizeof(struct Helper), compareHelpers);

    return s;
}

void freeFunc(struct Helper *s, int size){
    for(int i = 0; i < size; i++){
        free(s[i].str);
    }
    free(s);
}

char*** allocateResult(int strsSize, int **returnColumnSizes){
    *returnColumnSizes = (int *)calloc(strsSize, sizeof(int));
    if (!*returnColumnSizes) return NULL;

    char ***result = (char ***)calloc(strsSize, sizeof(char **));
    if (!result){
        free(*returnColumnSizes);
        *returnColumnSizes = NULL;
        return NULL;
    }
    return result;
}

char*** groupAnagrams(char** strs, int strsSize, int* returnSize, int** returnColumnSizes) {
if (strsSize <= 0) {
    *returnSize = 0;
    return NULL;
}

struct Helper *s = createHelperArray(strs, strsSize);
qsort(s, strsSize, sizeof(struct Helper), compareHelpers);

// First pass: count the size of each group
int *groupSizes = (int *)malloc(strsSize * sizeof(int));
if (!groupSizes) {
    freeFunc(s, strsSize);
    return NULL;
}
int groupCount = 0;
int count = 1;
for (int i = 1; i < strsSize; i++) {
    if (strcmp(s[i].str, s[i-1].str) == 0) {
        count++;
    } else {
        groupSizes[groupCount] = count;
        groupCount++;
        count = 1;
    }
}
groupSizes[groupCount] = count;
groupCount++;

// Allocate return arrays
*returnColumnSizes = (int *)malloc(groupCount * sizeof(int));
if (!*returnColumnSizes) {
    free(groupSizes);
    freeFunc(s, strsSize);
    return NULL;
}
memcpy(*returnColumnSizes, groupSizes, groupCount * sizeof(int));
free(groupSizes);

*returnSize = groupCount;
char ***result = (char***)malloc(groupCount * sizeof(char**));
if (!result) {
    free(*returnColumnSizes);
    *returnColumnSizes = NULL;
    freeFunc(s, strsSize);
    return NULL;
}
for (int i = 0; i < groupCount; i++) {
    result[i] = (char**)malloc((*returnColumnSizes)[i] * sizeof(char*));
    if (!result[i]) {
        // Free previously allocated rows
        for (int j = 0; j < i; j++) {
            free(result[j]);
        }
        free(result);
        free(*returnColumnSizes);
        *returnColumnSizes = NULL;
        freeFunc(s, strsSize);
        return NULL;
    }
}

// Second pass: fill the result array
int groupIdx = 0;
int idxInGroup = 0;
for (int i = 0; i < strsSize; i++) {
    if (i > 0 && strcmp(s[i].str, s[i-1].str) != 0) {
        groupIdx++;
        idxInGroup = 0;
    }
    result[groupIdx][idxInGroup] = (char *)malloc(strlen(strs[s[i].index]) + 1);
    if (!result[groupIdx][idxInGroup]) {
        // Free everything allocated so far
        for (int j = 0; j < groupIdx; j++) {
            for (int k = 0; k < (*returnColumnSizes)[j]; k++) {
                free(result[j][k]);
            }
            free(result[j]);
        }
        free(result);
        free(*returnColumnSizes);
        *returnColumnSizes = NULL;
        freeFunc(s, strsSize);
        return NULL;
    }
    strcpy(result[groupIdx][idxInGroup], strs[s[i].index]);
    idxInGroup++;
}

freeFunc(s, strsSize);
return result;
}
