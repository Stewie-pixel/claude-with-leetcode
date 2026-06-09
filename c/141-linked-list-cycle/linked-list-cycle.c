// -----------------------------------------------------------------------------
// Problem: Linked List Cycle
// Source : https://leetcode.com/problems/linked-list-cycle/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- DESCRIPTION ----------------------------------
//
// Given head, the head of a linked list, determine if the linked list has a cycle.
// A cycle exists if there is a node in the list that can be reached again 
// by continuously following the next pointer.
//
// Example:
//   Input: head = [3,2,0,-4], pos = 1
//   Output: true
//
//   Input: head = [1,2], pos = 0
//   Output: true
//
//   Input: head = [1], pos = -1
//   Output: false
//
// Constraints:
//   - The number of nodes in the list is in the range [0, 10⁴].
//   - -10⁵ <= Node.val <= 10⁵
//
// ----------------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Use two pointers: slow and fast.
// 2. Move slow by 1 step each time and fast by 2 steps.
// 3. If the linked list has a cycle, slow and fast will eventually meet.
// 4. If fast reaches NULL or fast->next is NULL, the list has no cycle.
// 5. Return true if they meet, otherwise return false.
//
//  Time Complexity  : O(n)
//  Space Complexity : O(1)
// -----------------------------------------------------------------------------


struct ListNode {
    int val;
    struct ListNode *next;
};

bool hasCycle(struct ListNode *head) {

    // 1. Initialize slow and fast pointers to the head
    struct ListNode *slow, *fast;
    slow = fast = head;

    // 2. Traverse the list while fast and fast->next are not NULL
    while(fast != NULL && fast->next != NULL){

        // 3. Move slow by one step and fast by two steps
        slow = slow->next;
        fast = fast->next->next;

        // 4. If they meet, cycle exists
        if(slow == fast) 
            return true;
    }

    // 5. If the loop ends, no cycle found
    return false;
}
