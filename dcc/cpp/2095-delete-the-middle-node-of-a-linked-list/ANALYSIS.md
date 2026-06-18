# Fast and Slow Pointers

## Video Solution

For more details about **Delete the Middle Node of a Linked List**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=WT0O4TTjyNc).

## Concept

The fast and slow pointer technique (also called tortoise‑and‑hare) uses two pointers that move through a sequence at different speeds. The **slow** pointer advances one step at a time, while the **fast** pointer advances two steps. When the fast pointer reaches the end, the slow pointer will be at the middle (or just before the middle for even‑length lists). This lets us find a middle element in a single pass without extra storage.

A real‑world analogy: two runners on a track. If one runs twice as fast as the other, when the faster runner completes a lap, the slower runner is exactly halfway around.

## When to Use It

Use fast/slow pointers when you see:
- Finding the middle of a linked list or array.
- Detecting a cycle in a linked list.
- Problems that require the *k‑th* node from the end (by offsetting the fast pointer).
- Situations where you need to split a list into halves (e.g., merge sort on linked lists).

## Template

```python
# Fast and slow pointer template for linked lists
def fast_slow_head(head):
    # Edge case: empty list or single node
    if not head or not head.next:
        return head   # or None depending on problem

    slow = head          # moves 1 step
    fast = head          # moves 2 steps
    prev = None          # node before slow (useful for deletion)

    while fast and fast.next:
        prev = slow
        slow = slow.next
        fast = fast.next.next

    # At this point:
    #   slow  -> middle node (or second middle for even length)
    #   prev  -> node before slow
    #   fast  -> None (end) or last node (odd length)

    # Example use: delete the middle node
    if prev:
        prev.next = slow.next   # bypass slow
    else:
        # The list had only two nodes; slow is head, delete it
        head = head.next

    return head
```

## LeetCode Problem Walkthrough

### Problem: 2095. Delete the Middle Node of a Linked List

https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/

### Approach 1: Brute Force – Compute Length Then Delete

**Algorithm**
1. Traverse the list once to count the number of nodes `n`.
2. Compute the index of the middle node: `mid = n // 2` (0‑based).
3. Traverse again to the node just before the middle (`mid‑1` steps) and bypass the middle node.
4. Handle the edge case where the list has 0 or 1 node → return `None`.

**Implementation**

```python
# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next

class Solution:
    def deleteMiddle(self, head: Optional[ListNode]) -> Optional[ListNode]:
        # 1️⃣ Count nodes
        n = 0
        cur = head
        while cur:
            n += 1
            cur = cur.next

        # No middle to delete (0 or 1 node)
        if n <= 1:
            return None

        # 2️⃣ Find node before middle
        mid = n // 2          # index of middle node
        prev = None
        cur = head
        for _ in range(mid):
            prev = cur
            cur = cur.next    # cur is now the middle node

        # 3️⃣ Delete middle
        prev.next = cur.next
        return head
```

**Complexity Analysis**

- Time complexity: O(n) — two passes over the list (count + traverse to middle).
- Space complexity: O(1) — only a few pointers are used.

---

### Approach 2: Optimized – Fast and Slow Pointers (One Pass)

**Intuition**
Instead of counting the length first, we can let two pointers race through the list. The fast pointer moves twice as fast; when it hits the end, the slow pointer will be exactly at the middle (or the first middle for even lengths). Keeping a `prev` pointer lets us delete the middle node without a second pass.

**Algorithm**
1. Handle the trivial case: if the list has 0 or 1 node, return `None`.
2. Initialize `slow = head`, `fast = head`, and `prev = None`.
3. Move `slow` by one and `fast` by two steps each iteration, storing the previous `slow` in `prev`.
4. When the loop ends (`fast` is `None` or `fast.next` is `None`), `slow` points to the middle node.
5. Bypass `slow` by setting `prev.next = slow.next`.
6. Return the (possibly unchanged) head.

**Implementation**

```python
# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next

class Solution:
    def deleteMiddle(self, head: Optional[ListNode]) -> Optional[ListNode]:
        # Edge case: empty list or single node → nothing to keep
        if not head or not head.next:
            return None

        slow = head          # moves 1 step
        fast = head          # moves 2 steps
        prev = None          # node before slow

        while fast and fast.next:
            prev = slow
            slow = slow.next
            fast = fast.next.next

        # `slow` is the middle node, `prev` is its predecessor
        prev.next = slow.next   # delete the middle node
        return head
```

**Complexity Analysis**

- Time complexity: O(n) — single pass through the list.
- Space complexity: O(1) — only three pointers used.

---

### Approach 3: Most Optimal – Same as Approach 2 (fast/slow)  
*(For this problem the fast/slow method is already optimal; we present it again to emphasize the pattern.)*

**Intuition**
The fast/slow technique gives us the middle in one traversal with O(1) extra space, which is the best possible asymptotic performance for a singly‑linked list where we cannot index directly.

**Algorithm**
Identical to Approach 2.

**Implementation**

```python
# Same as Approach 2
class Solution:
    def deleteMiddle(self, head: Optional[ListNode]) -> Optional[ListNode]:
        if not head or not head.next:
            return None

        slow = head
        fast = head
        prev = None

        while fast and fast.next:
            prev = slow
            slow = slow.next
            fast = fast.next.next

        prev.next = slow.next
        return head
```

**Complexity Analysis**

- Time complexity: O(n) — one linear scan.
- Space complexity: O(1) — constant auxiliary space.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the fast/slow algorithm on the list `[1, 3, 4, 7, 1, 2, 6]` (example 1).

#### Dry Run

| Step | slow (value) | fast (value) | prev (value) | Action                              |
|------|--------------|--------------|--------------|-------------------------------------|
| Init | 1            | 1            | None         | start                               |
| 1    | 3            | 4            | 1            | move slow→3, fast→4                 |
| 2    | 4            | 1            | 3            | move slow→4, fast→1                 |
| 3    | 7            | 6            | 4            | move slow→7, fast→6                 |
| 4    | 2            | None         | 7            | move slow→2, fast→None (exit)       |

When the loop exits, `slow` points to node `7` (the middle) and `prev` points to node `4`.  
We set `prev.next = slow.next`, which links `4` → `1`, removing `7`.  
Resulting list: `[1, 3, 4, 1, 2, 6]`.

---

**Summary**  
- The fast/slow pointer pattern finds the middle of a linked list in one pass with O(1) space.  
- It is the optimal solution for “delete the middle node” and many related problems.  
- Always handle the 0‑ or 1‑node edge case separately.