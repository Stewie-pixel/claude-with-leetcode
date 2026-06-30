# Fast & Slow Pointers

## Video Solution

For more details about **Maximum Twin Sum of a Linked List**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=doj95MelfSA)

## Concept

The fast & slow pointer technique (also called tortoise and hare) uses two pointers that traverse a sequence at different speeds.  
The **slow** pointer moves one step at a time, while the **fast** pointer moves two steps.  
When the fast pointer reaches the end, the slow pointer will be at the middle (for even length) or just past the middle (for odd length).  
This lets us find the middle, detect cycles, or split a list without extra storage.

**Analogy:** Two runners on a circular track – one runs twice as fast. When the faster runner laps the slower one, the slower runner has covered exactly half the track.

## When to Use It

Use fast & slow pointers when you see:
- Need to find the middle of a linked list
- Detect a cycle in a linked list
- Check if a linked list is a palindrome
- Find the k‑th node from the end (by offsetting the fast pointer)
- Split a list into two halves for further processing

## Template

```python
# Generic fast & slow pointer template for linked lists
def fast_slow_template(head):
    slow = fast = head
    # Move fast two steps, slow one step until fast hits the end
    while fast and fast.next:
        slow = slow.next          # 1 step
        fast = fast.next.next     # 2 steps
    # At this point, `slow` is at the middle (or just after middle for odd length)
    return slow
```

---

## LeetCode Problem Walkthrough

### Problem: 2130. Maximum Twin Sum of a Linked List
https://leetcode.com/problems/maximum-twin-sum-of-a-linked-list/

### Approach 1: Brute Force (Array)

**Algorithm**
1. Traverse the linked list and store each node’s value in an array.
2. Use two indices, `i` from the start and `j` from the end, moving toward the center.
3. For each pair, compute `arr[i] + arr[j]` and keep the maximum.
4. Return the maximum sum found.

**Implementation**

```python
# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next

class Solution:
    def pairSum(self, head: Optional[ListNode]) -> int:
        values = []
        cur = head
        while cur:
            values.append(cur.val)
            cur = cur.next

        i, j = 0, len(values) - 1
        max_sum = 0
        while i < j:
            max_sum = max(max_sum, values[i] + values[j])
            i += 1
            j -= 1
        return max_sum
```

**Complexity Analysis**
- Time complexity: O(n) — one pass to copy values, another pass over half the array.
- Space complexity: O(n) — we store all node values in an array.

---

### Approach 2: Stack (Half‑Space)

**Intuition**
We only need to compare the first half with the reversed second half.  
Instead of storing the whole list, we can push the first half onto a stack while advancing a slow pointer.  
After reaching the middle, we pop from the stack to get the values of the first half in reverse order and pair them with the second half.

**Algorithm**
1. Use slow & fast pointers to find the middle; while moving, push each visited node’s value onto a stack.
2. If the list length is even, after the loop `slow` points to the start of the second half.
3. Traverse the second half with a pointer; for each node, pop the top of the stack (which holds the corresponding twin from the first half) and compute the sum.
4. Track the maximum sum.

**Implementation**

```python
class Solution:
    def pairSum(self, head: Optional[ListNode]) -> int:
        stack = []
        slow = fast = head
        # Push first half onto stack while finding middle
        while fast and fast.next:
            stack.append(slow.val)
            slow = slow.next
            fast = fast.next.next

        # For even length, slow is at start of second half
        max_sum = 0
        while slow:
            twin_val = stack.pop()          # value from the first half
            max_sum = max(max_sum, twin_val + slow.val)
            slow = slow.next
        return max_sum
```

**Complexity Analysis**
- Time complexity: O(n) — each node visited a constant number of times.
- Space complexity: O(n/2) = O(n) — stack holds at most half the nodes.

---

### Approach 3: Fast & Slow + In‑Place Reverse (Optimal)

**Intuition**
We can achieve O(1) extra space by reversing the second half of the list in place.  
After reversing, the two halves are aligned head‑to‑head, allowing a single linear scan to compute twin sums without additional containers.

**Algorithm**
1. Find the middle of the list using slow & fast pointers (`slow` ends at the node **before** the start of the second half).
2. Break the list after `slow` and reverse the second half.
3. Scan both halves simultaneously, computing the sum of each pair and tracking the maximum.
4. (Optional) Restore the original list order by reversing the second half again and reconnecting.

**Implementation**

```python
class Solution:
    def pairSum(self, head: Optional[ListNode]) -> int:
        # 1️⃣ Find middle (prev of second half)
        slow = fast = head
        while fast.next and fast.next.next:
            slow = slow.next
            fast = fast.next.next

        # 2️⃣ Reverse second half
        second = slow.next
        slow.next = None                     # terminate first half
        prev = None
        while second:
            nxt = second.next
            second.next = prev
            prev = second
            second = nxt
        # `prev` is now the head of reversed second half

        # 3️⃣ Compute max twin sum
        first, second = head, prev
        max_sum = 0
        while second:                        # both halves have equal length
            max_sum = max(max_sum, first.val + second.val)
            first = first.next
            second = second.next
        return max_sum
```

**Complexity Analysis**
- Time complexity: O(n) — each step (finding middle, reversing, scanning) touches each node a constant number of times.
- Space complexity: O(1) — only a few pointer variables are used; reversal is done in place.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the optimal approach (Fast & Slow + Reverse) on the example `head = [5,4,2,1]`.

#### Dry Run

| Step | slow (value) | fast (value) | Action                              |
|------|--------------|--------------|-------------------------------------|
| Init | 5            | 5            | both start at head                  |
| 1    | 4            | 2            | move slow→4, fast→2 (two steps)     |
| 2    | 2            | None         | fast.next is None → stop loop       |

After loop: `slow` points to node with value **2** (the end of first half).  
`slow.next` (value **2**) is the start of the second half.

**Reverse second half** (`[2,1]` → `[1,2]`):

```
prev = None
curr = 2
Iteration 1: nxt = 1, 2.next = None, prev = 2, curr = 1
Iteration 2: nxt = None, 1.next = 2, prev = 1, curr = None
```
Reversed half head points to node **1**, list now: `[5,4]` + `[1,2]`.

**Scan pairs**:

| first.val | second.val | sum | max_sum |
|-----------|------------|-----|---------|
| 5         | 1          | 6   | 6       |
| 4         | 2          | 6   | 6       |

Result: **6**.

--- 

*End of lecture.*