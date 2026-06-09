int max(int a, int b){
    return a > b ? a : b;
}

int characterReplacement(char* s, int k) {
    int maxFrequency = 0;
    int n = strlen(s);
    int result = 0;
    int *count = (int *)calloc('Z' - 'A' + 1, sizeof(int));
    int left = 0;

    for(int right = 0; right < n; right++){
        count[(unsigned char)(s[right] - 'A')]++;
        maxFrequency = max(maxFrequency, count[(unsigned char)(s[right] - 'A')]);

        while((right - left + 1) - maxFrequency > k){
            count[(unsigned char)(s[left] - 'A')]--;
            left++;
        }

        result = max(result, right - left + 1);
    }

    free(count);
    return result;
}
