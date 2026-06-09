struct ListNode {
    int val;
    struct ListNode *next;
};

bool hasCycle(struct ListNode *head) {

    struct ListNode *slow, *fast;
    slow = fast = head;

    while(fast != NULL && fast->next != NULL){

        slow = slow->next;
        fast = fast->next->next;

        if(slow == fast)
            return true;
    }

    return false;
}
