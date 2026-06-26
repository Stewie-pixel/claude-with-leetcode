struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode *mergeTwoLists(struct ListNode *list1, struct ListNode *list2){
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

    if(list1){
        current->next = list1;
    }
    if(list2){
        current->next = list2;
    }

    return dummy.next;
}

struct ListNode *mergeRange(struct ListNode **lists, int left, int right){
    if(left > right)
        return NULL;
    if(left == right)
        return lists[left];

    int mid = (left + right) / 2;
    struct ListNode *list1 = mergeRange(lists, left, mid);
    struct ListNode *list2 = mergeRange(lists, mid + 1, right);

    return mergeTwoLists(list1, list2);
}

struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {
    if(listsSize <= 0)
        return NULL;
    return mergeRange(lists, 0, listsSize - 1);
}
