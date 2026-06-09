struct ListNode {
    int val;
    struct ListNode *next;
};

void merge(struct ListNode *list1, struct ListNode *list2){
    if(!list2)
        return;

    struct ListNode *temp1 = list1->next;
    struct ListNode *temp2 = list2->next;

    list1->next = list2;
    list2->next = temp1;

    merge(temp1, temp2);
}

struct ListNode *reverse(struct ListNode *head){
    struct ListNode *previous = NULL;
    struct ListNode *current = head;
    while(current != NULL){
        struct ListNode *next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    return previous;
}

void reorderList(struct ListNode* head) {

    if(!head || !head->next)
        return;

    struct ListNode *slow = head, *fast = head;
    while(fast && fast->next){
        slow = slow->next;
        fast = fast->next->next;
    }

    struct ListNode *current = slow->next;
    slow->next = NULL;

    struct ListNode *previous = reverse(current);

    merge(head, previous);
}
