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
