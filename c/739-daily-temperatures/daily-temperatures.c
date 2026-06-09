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
