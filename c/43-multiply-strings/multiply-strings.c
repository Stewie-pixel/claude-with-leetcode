#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* multiply(char* num1, char* num2) {
    int n1 = strlen(num1);
    int n2 = strlen(num2);
    char *ans = (char *)malloc(sizeof(char) * 2);
    if((n1 == 1 && num1[0] == '0') || (n2 == 1 && num2[0] == '0')){
        ans[0] = '0';
        ans[1] = '\0';
        return ans;
    }

    int *res = (int *)calloc(n1 + n2, sizeof(int));

    for(int i = n1 - 1; i >= 0; i--){
        for(int j = n2 - 1; j >= 0; j--){
            int mul = (num1[i] - '0') * (num2[j] - '0');
            int sum = mul + res[i + j + 1];
            res[i + j + 1] = sum % 10;
            res[i + j] += sum / 10;
        }
    }

    int i = 0;

    while(i < n1 + n2 && res[i] == 0){
        i++;
    }

    ans = (char *)realloc(ans, sizeof(char) * (n2 + n1 - i + 2));
    int size = 0;
    while(i < n1 + n2){
        ans[size++] = res[i++] + '0';
    }
    ans[size] = '\0';
    free(res);

    return ans;
}
