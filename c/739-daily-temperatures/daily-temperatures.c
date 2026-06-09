// Daily Temperatures
// Source: https://leetcode.com/problems/daily-temperatures/
// Author: Ngoc-Tien To

// Description:
// Given an array of integers temperatures represents the daily temperatures, 
// return an array answer such that answer[i] is the number of days 
// you have to wait after the ith day to get a warmer temperature. 
// If there is no future day for which this is possible, keep answer[i] == 0 instead. (*)

// Idea:
// 1. Based on (*) statement, use the calloc() function to allocate the result array 
//    and initialize all elements with 0 value
// 2. Build a simple stack and use it to solve this problem efficiently
// 3. Traverse the array:
//      3.1 If stack is empty, push the current element into stack
//      3.2 While the current value is greater than value at the top of the stack:
//          a. Pop the top element from stack
//          b. Number of days to wait is (i - element's index)
//      3.3 Push the current element into stack
// 4. After traversing all elements, the values of elements that do not have a warmer day remain 0 by default
// 5. Return the result array

#define N 100000

struct Pair{
    int value;
    int index;
};

struct Stack{
    struct Pair pair[N];
    int capacity;
};

int isEmpty(struct Stack stack){
    return stack.capacity < 0;
}

struct Pair peek(struct Stack *stack){
    return stack->pair[stack->capacity];
}

struct Pair pop(struct Stack *stack){
    return stack->pair[stack->capacity--];
}

void push(struct Stack *s, struct Pair p){
    s->pair[++s->capacity] = p;
}

int* dailyTemperatures(int* temperatures, int temperaturesSize, int* returnSize) {
    int n = temperaturesSize;
    int *result = (int *)calloc(n, sizeof(int));
    (*returnSize) = n;

    struct Stack stack;
    stack.capacity = -1;

    struct Pair p[N];
    for(int i = 0; i < n; i++){
        p[i].value = temperatures[i];
        p[i].index = i;
    }

    for(int i = 0; i < n; i++){
        while(!isEmpty(stack) && temperatures[i] > peek(&stack).value){
            struct Pair temp = pop(&stack);
            result[temp.index] = i - temp.index;
        }
        push(&stack, p[i]);
    }

    return result;
}