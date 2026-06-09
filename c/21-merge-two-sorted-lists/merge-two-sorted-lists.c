// -----------------------------------------------------------------------------
// Problem: Merge Two Sorted Lists
// Source : https://leetcode.com/problems/merge-two-sorted-lists/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- DESCRIPTION ----------------------------------
//
// You are given the heads of two sorted linked lists list1 and list2.
// Merge the two lists into a single sorted linked list and return its head.
// The new list should be made by splicing together the nodes of the first two lists.
//
// Example:
//   Input: list1 = [1,2,4], list2 = [1,3,4]
//   Output: [1,1,2,3,4,4]
//
// Constraints:
//   - The number of nodes in both lists is in the range [0, 50].
//   - -100 <= Node.val <= 100
//   - Both list1 and list2 are sorted in non-decreasing order.
//
// ----------------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Use a dummy node to simplify edge cases.
// 2. Maintain a pointer `current` that always points to the last node of the merged list.
// 3. Compare the heads of both lists and attach the smaller one to `current->next`.
// 4. Continue until one list becomes empty.
// 5. Append the remaining nodes from the non-empty list.
// 6. Return `dummy.next` as the head of the merged linked list.
//
//  Time Complexity  : O(m + n)
//  Space Complexity : O(1)
// -----------------------------------------------------------------------------


struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) {
    struct ListNode dummy;                 // 1. Dummy node to simplify head handling
    struct ListNode *current = &dummy;     // 2. Pointer to build the merged list

    // 3. Compare nodes from both lists
    while(list1 && list2){
        if(list1->val < list2->val){
            current->next = list1;         // 3.1 Attach smaller node
            list1 = list1->next;           // 3.2 Move pointer forward
        }
        else{
            current->next = list2;
            list2 = list2->next;
        }
        current = current->next;           // 3.3 Move current forward
    }

    // 4. Attach remaining nodes (if any)
    current->next = list1 ? list1 : list2;

    // 5. Return the head of the merged list
    return dummy.next;
}
