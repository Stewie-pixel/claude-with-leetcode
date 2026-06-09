// -----------------------------------------------------------------------------
// Problem: Reverse Linked List
// Source : https://leetcode.com/problems/reverse-linked-list/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- DESCRIPTION ----------------------------------
//
// Given the head of a singly linked list, reverse the list and return the new head.
//
// Example:
//   Input: head = [1,2,3,4,5]
//   Output: [5,4,3,2,1]
//
// Constraints:
//   - The number of nodes in the list is in the range [0, 5000].
//   - -5000 <= Node.val <= 5000
//
// Follow up: A linked list can be reversed either iteratively or recursively.
// Could you implement both?
//
// ----------------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Use three pointers: previous, current, and nextNode.
// 2. Iterate through the list:
//       - Temporarily store nextNode = current->next
//       - Reverse the current node’s link: current->next = previous
//       - Move previous and current one step forward
// 3. When the loop ends, previous will point to the new head.
// 4. Return previous as the new head.
//
//  Time Complexity  : O(N)
//  Space Complexity : O(1)
// -----------------------------------------------------------------------------


struct ListNode {
    int val;
    struct ListNode *next;
};


struct ListNode* reverseList(struct ListNode* head) {
    // 1. Base case: empty list or single node
    if(head == 0 || head->next == 0) {
        return head;
    }

    // 2. Initialize pointers
    struct ListNode *previous, *current, *nextNode;
    previous = NULL;
    current = nextNode = head;

    // 3. Traverse and reverse the links
    while(current != NULL){
        nextNode = current->next;   // Save next node
        current->next = previous;   // Reverse link
        previous = current;         // Move previous forward
        current = nextNode;         // Move current forward
    }

    // 4. Update head to the new first node
    head = previous;

    return head;
}
