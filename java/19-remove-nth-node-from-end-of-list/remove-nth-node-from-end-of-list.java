/**
 * Definition for singly-linked list.
 * public class ListNode {
 *     int val;
 *     ListNode next;
 *     ListNode() {}
 *     ListNode(int val) { this.val = val; }
 *     ListNode(int val, ListNode next) { this.val = val; this.next = next; }
 * }
 */
class Solution {

    public ListNode removeNthFromEnd(ListNode head, int n) {
        if (head == null || head.next == null) {
            return null;
        }

        // Reverse the linked list
        ListNode curr1 = head;
        ListNode prev1 = null;

        while (curr1 != null) {
            ListNode next_node1 = curr1.next;
            curr1.next = prev1;
            prev1 = curr1;
            curr1 = next_node1;
        }

        // Remove Nth Node From End of List
        int counter = 0;
        ListNode curr2 = prev1;
        ListNode prev2 = null;

        while (curr2 != null) {
            counter++;
            if (counter == n) {
                if (prev2 == null) {
                    prev1 = curr2.next;
                } else {
                    prev2.next = curr2.next;
                }
                break;
            }
            prev2 = curr2;
            curr2 = curr2.next;
        }

        // Reverse the linked list to return to the original state
        ListNode curr3 = prev1;
        ListNode prev3 = null;

        while (curr3 != null) {
            ListNode next_node2 = curr3.next;
            curr3.next = prev3;
            prev3 = curr3;
            curr3 = next_node2;
        }

        return prev3;
    }
}
