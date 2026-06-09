struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* reverseList(struct ListNode* head) {

    if(head == 0 || head->next == 0) {
        return head;
    }

    struct ListNode *previous, *current, *nextNode;
    previous = NULL;
    current = nextNode = head;

    while(current != NULL){
        nextNode = current->next;
        current->next = previous;
        previous = current;
        current = nextNode;
    }

    head = previous;

    return head;
}
