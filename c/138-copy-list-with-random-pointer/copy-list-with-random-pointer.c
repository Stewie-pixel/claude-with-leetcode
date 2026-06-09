struct Node {
    int val;
    struct Node *next;
    struct Node *random;
};

struct Pair{
    int index;
    struct Node *current;
};

struct Node *create(int val){

    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->val = val;
    newNode->next = NULL;
    newNode->random = NULL;
    return newNode;
}

struct Node* copyRandomList(struct Node* head) {

    if(!head)
        return NULL;

    struct Pair *pair = (struct Pair *)malloc(sizeof(struct Pair) * (1e3));
    int size = 0;
	struct Node *temp = head;

    while(temp){
        pair[size].index = -1;
        pair[size].current = temp;
        size++;
        temp = temp->next;
    }

    for(int i = 0; i < size; i++){
        if(pair[i].current->random){
            for(int j = 0; j < size; j++){
                if(pair[i].current->random == pair[j].current){
                    pair[i].index = j;
                    break;
                }
            }
        }
    }

    for(int i = 0; i < size; i++){
        int val = pair[i].current->val;
        pair[i].current = create(val);
    }

    for(int i = 0; i < size - 1; i++){
        pair[i].current->next = pair[i + 1].current;
    }

    for(int i = 0; i < size; i++){
        if(pair[i].index != -1){
            pair[i].current->random = pair[pair[i].index].current;
        }
    }

    struct Node *newHead = pair[0].current;
    free(pair);

    return newHead;
}
