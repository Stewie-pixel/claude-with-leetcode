// Evaluate Reverse Polish Notation
// Source: https://leetcode.com/problems/evaluate-reverse-polish-notation/
// Author: Ngoc-Tien To

// Description:
// You are given an array of strings tokens that represents an arithmetic expression in a Reverse Polish Notation.
// Evaluate the expression. Return an integer that represents the value of the expression.

// Idea:
// 1. Create a simple stack to use its operators
// 2. Traverse the array, if you meet an operator:
//      2.1 Pop two numbers from the stack (b = top(), pop; a = top(), pop)
//      2.2 Perform the operation (a <op> b)
//      2.3 Push the result back into the stack
// 3. If you meet a number, push it into the stack
// 4. After traversing all tokens, the only number left in stack is the final result

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