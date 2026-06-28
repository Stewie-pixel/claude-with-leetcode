/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    if (head == NULL || k <= 1) {
        return head;
    }

    struct ListNode dummy = {0};
    dummy.next = head;
    struct ListNode *groupPrev = &dummy;

    while (1) {
        struct ListNode *kth = groupPrev;
        for (int i = 0; i < k && kth != NULL; i++) {
            kth = kth->next;
        }
        if (kth == NULL) {
            break;
        }

        struct ListNode *groupNext = kth->next;

        struct ListNode *prev = groupNext;
        struct ListNode *curr = groupPrev->next;
        while (curr != groupNext) {
            struct ListNode *tmp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = tmp;
        }

        struct ListNode *newGroupTail = groupPrev->next;
        groupPrev->next = prev;
        groupPrev = newGroupTail;
    }

    return dummy.next;
}