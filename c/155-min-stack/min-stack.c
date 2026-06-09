// Min Stack
// Source: https://leetcode.com/problems/min-stack/
// Author: Ngoc-Tien To

// Description:
// Design a stack that supports push, pop, top, and retrieving the minimum element in constant time.

// Implement the MinStack class:
// MinStack() initializes the stack object.
// void push(int val) pushes the element val onto the stack.
// void pop() removes the element on the top of the stack.
// int top() gets the top element of the stack.
// int getMin() retrieves the minimum element in the stack.
// You must implement a solution with O(1) time complexity for each function.

// Idea:
// 1. Use two arrays stack and minStack to implement min stack:
//      1.1 The stack array is used to store values as a normal stack
//      1.2 The minStack array store min value of each index of the stack
// 2. All operators of min stack are same to a normal stack, except for push operator:
// 3. When pushing a value into the stack, compare it with the current min (top of minStack) 
//    and push the smaller into minStack

#define N 3 * 1e4

typedef struct {
    int *stack;
    int *minStack;
    int cap;
} MinStack;


MinStack* minStackCreate() {
    MinStack *ms = (MinStack *)malloc(sizeof(MinStack));
    ms->stack = (int *)malloc(sizeof(int) * N);
    ms->minStack = (int *)malloc(sizeof(int) * N);
    ms->cap = -1;
    return ms;
}

void minStackPush(MinStack* obj, int val) {
    obj->cap++;
    obj->stack[obj->cap] = val;
    if(obj->cap > 0 && obj->minStack[obj->cap - 1] < val){
        val = obj->minStack[obj->cap - 1];
    }
    obj->minStack[obj->cap] = val;
}

void minStackPop(MinStack* obj) {
    obj->cap--;
}

int minStackTop(MinStack* obj) {
    return obj->stack[obj->cap];
}

int minStackGetMin(MinStack* obj) {
    return obj->minStack[obj->cap];
}

void minStackFree(MinStack* obj) {
    free(obj->stack);
    free(obj->minStack);
    free(obj);
}

/**
 * Your MinStack struct will be instantiated and called as such:
 * MinStack* obj = minStackCreate();
 * minStackPush(obj, val);
 
 * minStackPop(obj);
 
 * int param_3 = minStackTop(obj);
 
 * int param_4 = minStackGetMin(obj);
 
 * minStackFree(obj);
*/