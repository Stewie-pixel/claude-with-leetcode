// Source: https://leetcode.com/problems/group-anagrams/
// Author: Ngoc-Tien To

// Idea:
// 1. For each string, sort its characters to create a "key" for anagram detection.
// 2. Store the sorted string and its original index in a helper array.
// 3. Sort the helper array by these keys so that all anagrams are grouped together.
// 4. Traverse the helper array:
//    - If the current key differs from the previous one → start a new group in result.
//    - Add the original string (by index) to the current group.
// 5. Return the 2D result array along with returnColumnSizes and returnSize.


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

    // Store string and index of that string to helper
    for(int i = 0; i < strsSize; i++){
        s[i].str = (char *)malloc(sizeof(char) * (strlen(strs[i]) + 1));
        strcpy(s[i].str, strs[i]);
        s[i].index = i;
    }

    // Sort characters inside each string
    for(int i = 0; i < strsSize; i++){
        qsort(s[i].str, strlen(s[i].str), sizeof(char), compareChars);
    }

    // Sort helper array by sorted strings (ascending order)
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
    // Handle edge cases (empty input or allocation failure)
    if(strsSize <= 0){
        *returnSize = 0;
        return NULL;
    }

    char ***result = allocateResult(strsSize, returnColumnSizes);
    if (!result){
        return NULL;
    }

    for (int i = 0; i < strsSize; i++) {
        result[i] = (char **)malloc(sizeof(char *) * strsSize);
        if (!result[i]){
            for(int j = 0; j < i; j++){
                free(result[j]);
            }
            free(*returnColumnSizes);
            free(result);
            return NULL;
        }
    }

    int groupCount = 0;

    struct Helper *s = createHelperArray(strs, strsSize);

    int pos = s[0].index;

    // Add the first anagram to the result
    int index = (*returnColumnSizes)[groupCount];
    result[groupCount][index] = (char *)malloc(sizeof(char) * (strlen(strs[pos]) + 1));
    strcpy(result[groupCount][index], strs[pos]);
    (*returnColumnSizes)[groupCount]++;

    for(int i = 0; i < strsSize - 1; i++){
        pos = s[i + 1].index;
        if(strcmp(s[i + 1].str, s[i].str) != 0){ // If the current anagram wasn't stored in the result
            groupCount++;                        // New group found → start filling next result cell
        }

        // Add current anagram
        index = (*returnColumnSizes)[groupCount];
        result[groupCount][index] = (char *)malloc(sizeof(char) * (strlen(strs[pos]) + 1));
        strcpy(result[groupCount][index], strs[pos]);
        (*returnColumnSizes)[groupCount]++;
    }

    groupCount++;

    // Finalize result: shrink allocations to actual sizes
    *returnSize = groupCount;
    *returnColumnSizes = (int *)realloc(*returnColumnSizes, sizeof(int) * groupCount);
    result = (char ***)realloc(result, sizeof(char **) * groupCount);
    for(int i = 0; i < groupCount; i++){
        result[i] = (char **)realloc(result[i], sizeof(char *) * (*returnColumnSizes)[i]);
    }

    freeFunc(s, strsSize);

    return result;
}