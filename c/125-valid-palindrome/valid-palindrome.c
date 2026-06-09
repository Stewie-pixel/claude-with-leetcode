bool isPalindrome(char* s) {
    if(strlen(s) == 0){
        return true;
    }

    int n = strlen(s);
    char *newStr = (char *)malloc(sizeof(char) * (n + 1));
    int len = 0;
    for(int i = 0; i < n; i++){
        if(isalnum(s[i])){
            newStr[len] = tolower(s[i]);
            len++;
        }
    }

    int i = 0, j = len - 1;
    while(i < j){
        if(newStr[i] != newStr[j]){
            free(newStr);
            return false;
        }
        i++;
        j--;
    }

    free(newStr);
    return true;
}
