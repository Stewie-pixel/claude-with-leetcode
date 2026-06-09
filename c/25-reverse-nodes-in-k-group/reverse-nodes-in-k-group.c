struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode *reverseKNodes(struct ListNode *head, int k) {

    if(!head) {
        return NULL;
    }

    struct ListNode *nextNode = head;
    int index = k + 1;
    while(--index) {
        nextNode = nextNode->next;
    }

    struct ListNode *previous = nextNode;
    struct ListNode *current, *next;
    current = next = head;
    while(k--) {
        next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }

    return previous;
}

int getLengthOfList(struct ListNode *head) {
    int length = 0;
    while(head) {
        length++;
        head = head->next;
    }
    return length;
}

struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    int length = getLengthOfList(head);
    if(length < k) {
        return head;
    }

    struct ListNode dummy, *current = head, *previous = head;
    dummy.next = head;
    int mark = 0;

    while(length >= k) {
        struct ListNode *currentHead = reverseKNodes(current, k);

        if(!mark) {
            dummy.next = currentHead;
            mark = 1;
        }
        else {
            previous->next = currentHead;
        }

        previous = current;
        current = current->next;
        length -= k;
    }

    return dummy.next;
}

struct ListNode* reverseKGroup(struct ListNode* head, int k) {

    if(!head) {
        return NULL;
    }

    struct ListNode *checkSize = head;
    for(int i = 0; i < k; i++) {
        if(!checkSize) {
            return head;
        }
        checkSize = checkSize->next;
    }

    int count = k;
    struct ListNode *previous = NULL;
    struct ListNode *current, *next;
    current = next = head;

    while(count--) {
        next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }

    if(next) {
        head->next = reverseKGroup(next, k);
    }

    return previous;
}
