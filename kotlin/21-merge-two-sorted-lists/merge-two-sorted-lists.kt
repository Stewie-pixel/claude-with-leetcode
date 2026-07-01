class Solution {

    fun mergeTwoLists(
        list1: ListNode?,
        list2: ListNode?
    ): ListNode? {

        if (list1 == null && list2 == null) {
            return null
        }

        var l1 = list1
        var l2 = list2

        val dummy = ListNode(0)
        var current = dummy

        while (l1 != null && l2 != null) {

            if (l1.`val` < l2.`val`) {

                current.next = l2
                l2 = l2.next

            } else {

                current.next = l1
                l1 = l1.next
            }

            current = current.next!!

            current.next = current
        }

        current.next = l2 ?: l1

        return dummy.next
    }
}