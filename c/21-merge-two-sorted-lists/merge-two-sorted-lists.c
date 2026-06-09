struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) {
    struct ListNode dummy;
    struct ListNode *current = &dummy;

    while(list1 && list2){
        if(list1->val < list2->val){
            current->next = list1;
            list1 = list1->next;
        }
        else{
            current->next = list2;
            list2 = list2->next;
        }
        current = current->next;
    }

    current->next = list1 ? list1 : list2;

    return dummy.next;
}
