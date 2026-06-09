// Source: https://leetcode.com/problems/valid-parentheses/
// Author: Ngoc-Tien To

// Idea:
// 1. Use a stack to track open parentheses.
// 2. Traverse the string character by character:
//    - If the current char is an open bracket ('(', '{', '['), push it into the stack.
//    - If it is a closing bracket (')', '}', ']'):
//         + If the stack is empty, return false (no matching open bracket).
//         + Otherwise, check if the top of the stack matches the current closing bracket.
//           If not matching, return false.
//           If matching, pop the top element.
// 3. After traversal, if the stack is empty → all brackets matched → return true.
//    Otherwise → return false.

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