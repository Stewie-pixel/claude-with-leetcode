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

    public int pairSum(ListNode head) {
        // Find the middle node
        ListNode slowPointer = head;
        ListNode fastPointer = head;

        while (fastPointer.next != null && fastPointer.next.next != null) {
            slowPointer = slowPointer.next;
            fastPointer = fastPointer.next.next;
        }

        // Reverse the second half of the linked list
        ListNode prev = null;
        ListNode curr = slowPointer.next;

        slowPointer.next = null;

        while (curr != null) {
            ListNode nextNode = curr.next;
            curr.next = prev;
            prev = curr;
            curr = nextNode;
        }

        // Find Maximum Twin Sum of a Linked List
        ListNode firstHalf = head;
        ListNode secondHalf = prev;
        int res = 0;

        while (firstHalf != null) {
            int sum = firstHalf.val + secondHalf.val;
            res = Math.max(res, sum);
            firstHalf = firstHalf.next;
            secondHalf = secondHalf.next;
        }
        return res;
    }
}
