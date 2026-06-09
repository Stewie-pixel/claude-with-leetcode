bool isAnagram(char* s, char* t) {
    int lenS = strlen(s);
    int lenT = strlen(t);
    if(lenS != lenT)
        return false;

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
