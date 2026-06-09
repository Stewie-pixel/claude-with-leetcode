// -----------------------------------------------------------------------------
// Problem: Reorder List
// Source : https://leetcode.com/problems/reorder-list/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- DESCRIPTION ----------------------------------
//
// You are given the head of a singly linked list.
// The list can be represented as:
//
//   L0 → L1 → … → Ln-1 → Ln
//
// Reorder the list to become:
//
//   L0 → Ln → L1 → Ln-1 → L2 → Ln-2 → …
//
// You must not modify the values in the list's nodes. 
// Only nodes themselves may be changed.
//
// Example:
//   Input: head = [1,2,3,4]
//   Output: [1,4,2,3]
//
// Constraints:
//   - The number of nodes in the list is in the range [1, 5 * 10^4].
//   - 1 <= Node.val <= 1000
//
// ----------------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Find the middle of the linked list using slow/fast pointers.
// 2. Split the list into two halves:
//       First half : head → ... → slow
//       Second half: slow->next → ... → end
// 3. Reverse the second half of the list.
// 4. Merge the two halves alternately to achieve the required order.
//
//  Time Complexity  : O(N)
//  Space Complexity : O(1)
// -----------------------------------------------------------------------------


struct ListNode {
    int val;
    struct ListNode *next;
};


// (Helper Function 1)
// Recursively merge two lists alternately.
// Example: list1 = 1->2->3, list2 = 6->5->4
// After merging → 1->6->2->5->3->4
void merge(struct ListNode *list1, struct ListNode *list2){
    if(!list2)
        return;

    struct ListNode *temp1 = list1->next;
    struct ListNode *temp2 = list2->next;

    list1->next = list2;
    list2->next = temp1;

    merge(temp1, temp2);
}


// (Helper Function 2)
// Reverse a singly linked list iteratively.
struct ListNode *reverse(struct ListNode *head){
    struct ListNode *previous = NULL;
    struct ListNode *current = head;
    while(current != NULL){
        struct ListNode *next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    return previous;
}


// (Main Function)
// Reorder the list as required.
void reorderList(struct ListNode* head) {
    // 1. Handle edge cases: empty list or single node
    if(!head || !head->next)
        return;
    
    // 2. Find the middle of the list (slow stops at middle)
    struct ListNode *slow = head, *fast = head;
    while(fast && fast->next){
        slow = slow->next;
        fast = fast->next->next;
    }

    // 3. Split into two halves
    struct ListNode *current = slow->next;
    slow->next = NULL;

    // 4. Reverse the second half
    struct ListNode *previous = reverse(current);

    // 5. Merge the two halves alternately
    merge(head, previous);
}
