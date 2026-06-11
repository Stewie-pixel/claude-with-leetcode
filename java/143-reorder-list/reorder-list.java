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

    public void reorderList(ListNode head) {
        if (head == null || head.next == null) {
            return;
        }

        // 1. Find the middle node
        ListNode slowPointer = head;
        ListNode fastPointer = head;

        while (fastPointer.next != null && fastPointer.next.next != null) {
            slowPointer = slowPointer.next;
            fastPointer = fastPointer.next.next;
        }

        // 2. Reverse the right half of the linked list;
        ListNode prev = null;
        ListNode curr = slowPointer.next;

        slowPointer.next = null;

        while (curr != null) {
            ListNode next_node = curr.next;
            curr.next = prev;
            prev = curr;
            curr = next_node;
        }

        // 3. Merge two half of the linked list using two pointers from each
        ListNode first = head;
        ListNode second = prev;

        while (second != null) {
            ListNode nextNode1 = first.next;
            ListNode nextNode2 = second.next;

            first.next = second;
            second.next = nextNode1;

            first = nextNode1;
            second = nextNode2;
        }
    }
}
