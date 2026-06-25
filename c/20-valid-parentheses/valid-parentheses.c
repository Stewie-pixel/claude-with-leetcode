bool isValid(char* s) {
    int n = strlen(s);
    char *stack = (char *)malloc((n + 1) * sizeof(char));
    int top = -1;
    for(int i = 0; i < n; i++){
        if(s[i] == '(' || s[i] == '{' || s[i] == '['){
            stack[++top] = s[i];
        }
        else{
            if(top == -1){
                free(stack);
                return false;
            }
            else if(stack[top] == '(' && s[i] != ')'){
                free(stack);
                return false;
            }
            else if(stack[top] == '[' && s[i] != ']'){
                free(stack);
                return false;
            }
            else if(stack[top] == '{' && s[i] != '}'){
                free(stack);
                return false;
            }
            else top--;
        }
    }

    free(stack);
    return (top == -1);
}
