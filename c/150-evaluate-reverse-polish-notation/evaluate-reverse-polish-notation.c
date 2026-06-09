struct Stack{
    int a[10000];
    int top;
};

int pop(struct Stack *stack){
    return stack->a[stack->top--];
}

void push(struct Stack *stack, int val){
    stack->a[++stack->top] = val;
}

int isOperator(char s[]){
    if(strcmp(s, "+") == 0 || strcmp(s, "-") == 0 || strcmp(s, "*") == 0 || strcmp(s, "/") == 0){
        return 1;
    }
    return 0;
}

int evalRPN(char** tokens, int tokensSize) {
    struct Stack stack;
    stack.top = -1;
    int i = 0;
    while(i < tokensSize){
        if(isOperator(tokens[i])){
            int number1 = pop(&stack);
            int number2 = pop(&stack);
            if(strcmp(tokens[i], "+") == 0){
                push(&stack, number2 + number1);
            }
            else if(strcmp(tokens[i], "-") == 0){
                push(&stack, number2 - number1);
            }
            else if(strcmp(tokens[i], "*") == 0){
                push(&stack, number2 * number1);
            }
            else if(strcmp(tokens[i], "/") == 0){
                push(&stack, number2 / number1);
            }
        }
        else{
            push(&stack, atoi(tokens[i]));
        }
        i++;
    }
    return pop(&stack);
}
