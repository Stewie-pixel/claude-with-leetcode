# Fast & Slow Pointers

## Video Solution

For more details about **Maximum Twin Sum of a Linked List**, watch the walkthrough at [https://www.youtube.com/watch?v=doj95MelfSA](https://www.youtube.com/watch?v=doj95MelfSA)

## Concept

The fast‑and‑slow pointer technique uses two pointers that move through a list at different speeds.  
The “slow” pointer advances one node per step while the “fast” pointer advances two nodes per step.  
When the fast pointer reaches the end, the slow pointer will be at the middle of the list.  
This lets us split a list into halves without first computing its length.

A common pattern is: find the middle → reverse the second half → now we can traverse both halves simultaneously to compare or combine values.

## When to Use It

Use fast & slow pointers when you see:
- Need to find the middle of a linked list (especially for even‑length lists)
- Problems that involve pairing nodes from opposite ends (palindromes, twin sums, reordering)
- Situations where you want to avoid extra O(n) space by processing two halves in‑place

## Template

```python
# Find middle of linked list
slow = fast = head
while fast and fast.next:
    slow = slow.next          # moves 1 step
    fast = fast.next.next     # moves 2 steps

# At this point, `slow` is the start of the second half
# (for even length lists it points to the first node of the second half)

# Optional: reverse the second half
prev = None
curr = slow
while curr:
    nxt = curr.next
    curr.next = prev
    prev = curr
    curr = nxt
# `prev` now heads the reversed second half

# Use both halves as needed
p1, p2 = head, prev
while p2:                     # or while p1 and p2 depending on problem
    # work with p1.val and p2.val
    p1 = p1.next
    p2 = p2.next
```

## LeetCode Problem Walkthrough

### Problem: 2130. Maximum Twin Sum of a Linked List

https://leetcode.com/problems/maximum-twin-sum-of-a-linked-list/

### Approach 1: Brute Force (Array)

**Algorithm**  
1. Traverse the linked list and store each node’s value in an array `vals`.  
2. Because the list length `n` is even, the twin of index `i` is index `n‑1‑i`.  
3. For each `i` from `0` to `n/2‑1`, compute `vals[i] + vals[n‑1‑i]` and keep the maximum.

**Implementation**

```python
# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next

class Solution:
    def pairSum(self, head: Optional[ListNode]) -> int:
        vals = []
        cur = head
        while cur:
            vals.append(cur.val)
            cur = cur.next

        n = len(vals)
        max_sum = 0
        for i in range(n // 2):
            twin_sum = vals[i] + vals[n - 1 - i]
            if twin_sum > max_sum:
                max_sum = twin_sum

        return max_sum
```

**Complexity Analysis**

- Time complexity: O(n) — one pass to copy values, another pass over half the array.  
- Space complexity: O(n) — the array stores all node values.

### Approach 2: Fast & Slow Pointers + In‑Place Reversal

**Intuition**  
Instead of storing all values, we can find the middle of the list with fast/slow pointers, reverse the second half in‑place, then walk both halves simultaneously. Each step gives us a twin pair, and we track the maximum sum. This reduces auxiliary space to O(1).

**Algorithm**  
1. Use `slow` and `fast` to locate the start of the second half (`slow`).  
2. Reverse the list starting from `slow`.  
3. Initialize two pointers: `p1` at the head (first half) and `p2` at the head of the reversed second half.  
4. While `p2` is not null:  
   - Compute `p1.val + p2.val`.  
   - Update the answer with the larger sum.  
   - Advance both pointers.  
5. Return the maximum sum found.

**Implementation**

```python
# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next

class Solution:
    def pairSum(self, head: Optional[ListNode]) -> int:
        # 1️⃣ Find middle (start of second half)
        slow = fast = head
        while fast and fast.next:
            slow = slow.next          # moves 1 step
            fast = fast.next.next     # moves 2 steps

        # 2️⃣ Reverse second half
        prev = None
        curr = slow
        while curr:
            nxt = curr.next
            curr.next = prev
            prev = curr
            curr = nxt
        # `prev` now heads the reversed second half

        # 3️⃣ Scan both halves and compute max twin sum
        max_sum = 0
        p1, p2 = head, prev
        while p2:                     # second half length equals first half length
            twin_sum = p1.val + p2.val
            if twin_sum > max_sum:
                max_sum = twin_sum
            p1 = p1.next
            p2 = p2.next

        return max_sum
```

**Complexity Analysis**

- Time complexity: O(n) — each step (finding middle, reversing, scanning) visits each node a constant number of times.  
- Space complexity: O(1) — only a few pointer variables are used; the list is reversed in‑place.

### Approach 3: Stack (Alternative O(n) Space)

**Intuition**  
If we prefer not to modify the original list, we can push the first half of node values onto a stack while using fast/slow to find the middle. Then, as we traverse the second half, we pop from the stack to get the twin value. This also yields O(n) time and O(n) space but keeps the input list intact.

**Algorithm**  
1. Move `slow` and `fast` to locate the middle; while doing so, push `slow.val` onto a stack each time `slow` advances (this stores the first half).  
2. After the loop, if the list length is even, `slow` is at the start of the second half.  
3. Traverse the second half with a pointer `p = slow`. For each node, pop the top value from the stack (its twin) and compute the sum. Track the maximum.  
4. Return the maximum sum.

**Implementation**

```python
class Solution:
    def pairSum(self, head: Optional[ListNode]) -> int:
        stack = []
        slow = fast = head

        # Step 1: find middle while storing first half
        while fast and fast.next:
            stack.append(slow.val)
            slow = slow.next
            fast = fast.next.next

        # Step 2: traverse second half and compute twin sums
        max_sum = 0
        while slow:
            twin_val = stack.pop()          # twin from first half
            twin_sum = twin_val + slow.val
            if twin_sum > max_sum:
                max_sum = twin_sum
            slow = slow.next

        return max_sum
```

**Complexity Analysis**

- Time complexity: O(n) — one pass to find middle/push, another pass over second half/pop.  
- Space complexity: O(n/2) = O(n) — the stack holds the first half of the values.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Input: `[5, 4, 2, 1]` (linked list: 5 → 4 → 2 → 1)

We'll trace **Approach 2** (fast/slow + reverse).

| Step | slow (node) | fast (node) | Action                                      |
|------|-------------|-------------|---------------------------------------------|
| Init | 5           | 5           | slow=fast=head                              |
| 1    | 4           | 2           | slow moves to 4, fast jumps to 2            |
| 2    | 2           | None        | slow moves to 2, fast jumps to None (end)   |

After the loop, `slow` points to node with value `2` (start of second half).

**Reverse second half (starting at node 2):**  
Original second half: `2 → 1`  
After reversal: `1 → 2` (`prev` points to node 1)

Now scan both halves:

| p1 (first half) | p2 (reversed second half) | Sum | Max so far |
|-----------------|---------------------------|-----|------------|
| 5               | 1                         | 6   | 6          |
| 4               | 2                         | 6   | 6          |

Maximum twin sum = 6, matching the expected output.