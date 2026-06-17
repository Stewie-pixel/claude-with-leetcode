#include <stdlib.h>
#include <string.h>

    int n = strlen(s);

char* longestPalindrome(char* s) {
    int n = strlen(s);
    if (n == 0) {
        char *result = malloc(1);
        *result = '\0';
        return result;
    }
    int index = 0, maxLen = 0;
    int l, r;
    for(int i = 0; i < n; i++){
        l = r = i;
        while(r + 1 < n && s[r + 1] == s[r]){
            r++;
        }
        while(l >= 0 && r < n && s[l] == s[r]){
            if(r - l + 1 > maxLen){
                maxLen = r - l + 1;
                index = l;
            }
            l--;
            r++;
        }
    }
    char *result = (char *)malloc(sizeof(char) * (maxLen + 1));
    strncpy(result, s + index, maxLen);
    result[maxLen] = '\0';
    return result;
}
    for(int i = 0; i < n; i++){
        l = r = i;
        while(r + 1 < n && s[r + 1] == s[r]){
            r++;
        }

        while(l >= 0 && r < n && s[l] == s[r]){
            if(r - l + 1 > maxLen){
                maxLen = r - l + 1;
                index = l;
            }
            l--;
            r++;
        }
    }

    char *result = (char *)malloc(sizeof(char) * (maxLen + 1));
    strncpy(result, s + index, maxLen);
    result[maxLen] = '\0';
    return result;
}
