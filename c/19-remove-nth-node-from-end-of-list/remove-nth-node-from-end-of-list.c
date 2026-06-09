#define MAX 30

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* removeNthFromEnd(struct ListNode* head, int n) {
    if(head == NULL) {
        return head;
    }

    int count = 0;
    struct ListNode **container = (struct ListNode **)malloc(sizeof(struct ListNode *) * MAX);
    struct ListNode *current = head;
    while(current){
        container[count++] = current;
        current = current->next;
    }

    int index = count - n;

    if(index == 0){
        struct ListNode *temp = head;
        head = head->next;
        free(temp);
        free(container);
        return head;
    }

    struct ListNode *temp = container[index];
    container[index - 1]->next = container[index]->next;
    free(container[index]);
    free(container);

    return head;
}
