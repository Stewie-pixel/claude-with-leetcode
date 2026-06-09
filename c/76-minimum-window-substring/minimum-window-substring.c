#define MAX 256
#define MAX_INT 1e9

char* minWindow(char* s, char* t) {
    int lenS = strlen(s);
    int lenT = strlen(t);
    if(lenT > lenS) {
        char *empty = (char *)malloc(1);
        empty[0] = '\0';
        return empty;
    }

    int count[MAX] = {0};
    for(int i = 0; i < lenT; i++) {
        count[t[i]]++;
    }

    int start = 0;
    int minLength = MAX_INT, left = 0, right = 0, need = lenT;
    while(right < lenS) {
        if(count[s[right]] > 0) {
            need--;
        }
        count[s[right]]--;
        right++;

        while(need == 0) {
            if(right - left < minLength) {
                minLength = right - left;
                start = left;
            }
            count[s[left]]++;
            if(count[s[left]] > 0) {
                need++;
            }
            left++;
        }
    }

    if(minLength == MAX_INT) {
        char *empty = (char *)malloc(1);
        empty[0] = '\0';
        return empty;
    }

    char *result = (char *)malloc(sizeof(char) * (minLength + 1));
    strncpy(result, s + start, minLength);
    result[minLength] = '\0';
    return result;
}
