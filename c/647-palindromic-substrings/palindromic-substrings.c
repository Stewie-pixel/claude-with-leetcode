#include <stdlib.h>
#include <string.h>

int countSubstrings(char* s) {
    int n = strlen(s);
    int result = 0;
    int l, r;
    for(int i = 0; i < n; i++){
        l = r = i;
        while(l >= 0 && r < n && s[l] == s[r]){
            result++;
            l--;
            r++;
        }

        l = i; r = i + 1;
        while(l >= 0 && r < n && s[l] == s[r]){
            result++;
            l--;
            r++;
        }
    }

    return result;
}
