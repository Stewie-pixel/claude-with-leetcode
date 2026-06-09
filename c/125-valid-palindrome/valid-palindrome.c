// Source: https://leetcode.com/problems/valid-palindrome/
// Author: Ngoc-Tien To

// Idea:
// 1. Normalize the string: remove non-alphanumeric characters and convert all letters to lowercase.
// 2. Use two pointers, i starting from the beginning and j from the end:
//      2.1 If characters at i and j differ, return false.
//      2.2 Otherwise, move i forward and j backward.
// 3. If all characters match, return true.

bool isPalindrome(char* s) {
    if(strlen(s) == 0){
        return true;
    }

    // Normalize the string
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