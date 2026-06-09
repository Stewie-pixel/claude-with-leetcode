bool checkInclusion(char* s1, char* s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    if (len1 > len2) {
        return false;
    }

    int *count = (int *)calloc(26, sizeof(int));

    for (int i = 0; i < len1; i++) {
        count[s1[i] - 'a']++;
    }

    for (int i = 0; i < len2; i++) {

        count[s2[i] - 'a']--;

        if (i >= len1) {
            count[s2[i - len1] - 'a']++;
        }

        int j;
        for (j = 0; j < 26; j++) {
            if (count[j] != 0) break;
        }

        if (j == 26) {
            free(count);
            return true;
        }
    }

    free(count);
    return false;
}
