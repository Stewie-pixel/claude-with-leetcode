// -----------------------------------------------------------------------------
// Problem: Add Two Numbers
// Source : https://leetcode.com/problems/add-two-numbers/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- DESCRIPTION ----------------------------------
//
// You are given two non-empty linked lists representing two non-negative integers.
// The digits are stored in reverse order, and each node contains a single digit.
// Add the two numbers and return the sum as a linked list.
//
// Example:
//   Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
//   Output: 7 -> 0 -> 8
//
// Explanation:
//   342 + 465 = 807
//
// ----------------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Traverse both linked lists node by node.
// 2. At each step, add the digits and the carry from the previous step.
// 3. Create a new node for each digit of the result (sum % 10).
// 4. Update the carry if sum >= 10.
// 5. Continue until both lists and carry are processed.
// 6. Link all created nodes to form the final result linked list.
//
//  Time Complexity  : O(max(m, n))
//  Space Complexity : O(max(m, n))
// -----------------------------------------------------------------------------


struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {

    // 1. Allocate temporary array to store pointers of new nodes
    struct ListNode **result = (struct ListNode **)malloc(sizeof(struct ListNode *) * 1000);
    int count = 0;

    // 2. Variable to store carry (if sum >= 10)
    int add = 0;

    // 3. Create two pointers to traverse both lists
    struct ListNode *node1 = l1, *node2 = l2;

    // 4. Process all nodes and carry
    while(node1 != NULL || node2 != NULL || add != 0){

        // 4.1 Get current values, or 0 if list ended
        int sum1 = (node1 != NULL) ? node1->val : 0;
        int sum2 = (node2 != NULL) ? node2->val : 0;

        // 4.2 Compute the total sum
        int sum = sum1 + sum2 + add;
        
        // 4.3 Create new node for the current digit
        result[count] = (struct ListNode *)malloc(sizeof(struct ListNode));
        result[count]->val = sum % 10;
        count++;

        // 4.4 Update carry for next iteration
        add = sum < 10 ? 0 : 1;

        // 4.5 Move both pointers to the next node
        node1 = (node1 != NULL) ? node1->next : NULL;
        node2 = (node2 != NULL) ? node2->next : NULL;
    }

    // 5. Set last node's next pointer to NULL
    result[count - 1]->next = NULL;

    // 6. Resize the temporary array to fit the actual count
    result = (struct ListNode **)realloc(result, sizeof(struct ListNode *) * count);

    // 7. Link all nodes in correct order
    for(int i = 0; i < count - 1; i++){
        result[i]->next = result[i + 1];
    }

    // 8. The first node is the head of the new linked list
    struct ListNode *newHead = result[0];

    // 9. Free the temporary pointer array
    free(result);

    // 10. Return the head of the result linked list
    return newHead;
}
