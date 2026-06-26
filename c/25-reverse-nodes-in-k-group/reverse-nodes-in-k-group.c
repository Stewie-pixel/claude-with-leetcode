/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
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