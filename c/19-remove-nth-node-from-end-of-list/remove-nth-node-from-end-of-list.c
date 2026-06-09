// -----------------------------------------------------------------------------
// Problem: Remove Nth Node From End of List
// Source : https://leetcode.com/problems/remove-nth-node-from-end-of-list/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- DESCRIPTION ----------------------------------
//
// Given the head of a linked list, remove the nth node from the end of the list
// and return its head.
//
// Example:
//   Input: head = [1,2,3,4,5], n = 2
//   Output: [1,2,3,5]
//
// Constraints:
//   - The number of nodes in the list is sz.
//   - 1 <= sz <= 30
//   - 0 <= Node.val <= 100
//   - 1 <= n <= sz
//
// ----------------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Traverse the linked list once and store all node addresses in an array.
// 2. Calculate the index of the node to remove: index = count - n.
// 3. If the node to remove is the head (index == 0):
//      - Move head to the next node.
//      - Free the old head.
// 4. Otherwise:
//      - Link the (index-1)th node to (index+1)th node.
//      - Free the node at position `index`.
// 5. Free the container array and return the modified head.
//
//  Time Complexity  : O(N)
//  Space Complexity : O(N)
// -----------------------------------------------------------------------------


#define MAX 30

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* removeNthFromEnd(struct ListNode* head, int n) {
    if(head == NULL) {
        return head;
    }

    // 1. Count nodes and store pointers in container
    int count = 0;
    struct ListNode **container = (struct ListNode **)malloc(sizeof(struct ListNode *) * MAX);
    struct ListNode *current = head;
    while(current){
        container[count++] = current;
        current = current->next;
    }

    // 2. Find the index of node to remove
    int index = count - n;

    // 3. Case 1: Remove the first node (head)
    if(index == 0){
        struct ListNode *temp = head;
        head = head->next;
        free(temp);
        free(container);
        return head;
    }

    // 4. Case 2: Remove a node in the middle or end
    struct ListNode *temp = container[index];
    container[index - 1]->next = container[index]->next;
    free(container[index]);
    free(container);

    // 5. Return the new head
    return head;
}
