struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {

    struct ListNode **result = (struct ListNode **)malloc(sizeof(struct ListNode *) * 1000);
    int count = 0;

    int add = 0;

    struct ListNode *node1 = l1, *node2 = l2;

    while(node1 != NULL || node2 != NULL || add != 0){

        int sum1 = (node1 != NULL) ? node1->val : 0;
        int sum2 = (node2 != NULL) ? node2->val : 0;

        int sum = sum1 + sum2 + add;

        result[count] = (struct ListNode *)malloc(sizeof(struct ListNode));
        result[count]->val = sum % 10;
        count++;

        add = sum < 10 ? 0 : 1;

        node1 = (node1 != NULL) ? node1->next : NULL;
        node2 = (node2 != NULL) ? node2->next : NULL;
    }

    result[count - 1]->next = NULL;

    result = (struct ListNode **)realloc(result, sizeof(struct ListNode *) * count);

    for(int i = 0; i < count - 1; i++){
        result[i]->next = result[i + 1];
    }

    struct ListNode *newHead = result[0];

    free(result);

    return newHead;
}
