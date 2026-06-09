// -----------------------------------------------------------------------------
// Problem: Merge k Sorted Lists
// Source : https://leetcode.com/problems/merge-k-sorted-lists/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- DESCRIPTION ----------------------------------
//
// You are given an array of k linked-lists, each linked-list is sorted in ascending order.
// Merge all the linked-lists into one sorted linked-list and return it.
//
// Example:
//   Input: lists = [[1,4,5],[1,3,4],[2,6]]
//   Output: [1,1,2,3,4,4,5,6]
//
// Constraints:
//   - k == lists.length
//   - 0 <= k <= 10⁴
//   - 0 <= Node.val <= 100
//   - The total number of nodes in all lists is in the range [0, 10⁴].
//
// ----------------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Use a divide-and-conquer approach (similar to merge sort).
// 2. Recursively split the list of linked lists into halves.
// 3. Merge two sorted linked lists at a time.
// 4. The merge operation between two lists is O(n), and divide & conquer gives O(N log k).
//
//  Time Complexity  : O(N log k)
//  Space Complexity : O(log k) due to recursion
// -----------------------------------------------------------------------------


struct ListNode {
    int val;
    struct ListNode *next;
};

// 1. Merge two sorted linked lists into one
struct ListNode *mergeTwoLists(struct ListNode *list1, struct ListNode *list2){
    struct ListNode dummy;                   // temporary dummy head
    struct ListNode *current = &dummy;       // pointer to build merged list

    // 1.1 Compare and link smaller nodes first
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

    // 1.2 Attach remaining nodes (if any)
    if(list1){
        current->next = list1;
    }
    if(list2){
        current->next = list2;
    }

    return dummy.next;   // 1.3 Return merged list head
}

// 2. Recursively merge lists in range [left, right]
struct ListNode *mergeRange(struct ListNode **lists, int left, int right){
    if(left > right)
        return NULL;                     // 2.1 Base case: invalid range
    if(left == right)
        return lists[left];              // 2.2 Base case: single list

    int mid = (left + right) / 2;        // 2.3 Divide range
    struct ListNode *list1 = mergeRange(lists, left, mid);
    struct ListNode *list2 = mergeRange(lists, mid + 1, right);

    return mergeTwoLists(list1, list2);  // 2.4 Conquer: merge two halves
}

// 3. Main function to merge all k lists
struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {
    if(listsSize <= 0)
        return NULL;                     // 3.1 Handle empty input
    return mergeRange(lists, 0, listsSize - 1); // 3.2 Merge all recursively
}
