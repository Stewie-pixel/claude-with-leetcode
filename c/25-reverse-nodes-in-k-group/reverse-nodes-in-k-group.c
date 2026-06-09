// -----------------------------------------------------------------------------
// Problem: Reverse Nodes in k-Group
// Source : https://leetcode.com/problems/reverse-nodes-in-k-group/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- DESCRIPTION -------------------------------------
//
// Given the head of a linked list, reverse the nodes of the list k at a time, and return the modified list.
// k is a positive integer and is less than or equal to the length of the linked list. 
// If the number of nodes is not a multiple of k then left-out nodes, in the end, should remain as it is.
// You may not alter the values in the list's nodes, only nodes themselves may be changed.
//
// ----------------------------------------------------------------------------------


// ---------------------------------- IDEA -------------------------------------
//
// 1. The list should be reversed in groups of size k.
// 2. If fewer than k nodes remain, leave them as is.
// 3. Two approaches are provided:
//
//      - Solution 1: Iterative approach using helper functions
//      - Solution 2: Recursive approach (simpler and elegant)
//
// -----------------------------------------------------------------------------


/**
 * Definition for singly-linked list.
 */
struct ListNode {
    int val;
    struct ListNode *next;
};


// *************************************** SOLUTION 1 ***************************************
//
// 1. Reverse exactly k nodes starting from a given head.
// 2. Track next node after reversed section to reconnect the list.
// 3. Use a helper function `getLengthOfList()` to know when to stop.
//
struct ListNode *reverseKNodes(struct ListNode *head, int k) {
    // 1. Base case: empty list
    if(!head) {
        return NULL;
    }

    // 2. Find the node after k nodes (to reconnect later)
    struct ListNode *nextNode = head;
    int index = k + 1;
    while(--index) {
        nextNode = nextNode->next;
    }

    // 3. Reverse k nodes
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


// 4. Count total nodes in list
int getLengthOfList(struct ListNode *head) {
    int length = 0;
    while(head) {
        length++;
        head = head->next;
    }
    return length;
}


// 5. Main iterative solution
struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    int length = getLengthOfList(head);
    if(length < k) {
        return head;
    }

    struct ListNode dummy, *current = head, *previous = head;
    dummy.next = head;
    int mark = 0;

    // 6. Process groups of size k
    while(length >= k) {
        struct ListNode *currentHead = reverseKNodes(current, k);

        // 6.1 Connect reversed group to previous
        if(!mark) {
            dummy.next = currentHead;
            mark = 1;
        }
        else {
            previous->next = currentHead;
        }

        // 6.2 Move pointers forward
        previous = current;
        current = current->next;
        length -= k;
    }

    return dummy.next;
}



// *************************************** SOLUTION 2 ***************************************
//
// Recursive approach:
// 1. Check if at least k nodes remain.
// 2. Reverse the first k nodes.
// 3. Recursively call the function for the remaining nodes.
// 4. Connect the reversed group to the next group.
//
struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    // 1. Base case: empty list
    if(!head) {
        return NULL;
    }

    // 2. Check if there are enough nodes to reverse
    struct ListNode *checkSize = head;
    for(int i = 0; i < k; i++) {
        if(!checkSize) {
            return head; // fewer than k nodes → do not reverse
        }
        checkSize = checkSize->next;
    }

    // 3. Reverse k nodes
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

    // 4. Recursively reverse the remaining nodes
    if(next) {
        head->next = reverseKGroup(next, k);
    }

    // 5. Return new head of reversed group
    return previous;
}
