# Linked List

## Video Solution

For more details about **Reverse Nodes in k-Group**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=1UOPsfP85V4).

## Concept

Reversing a linked list in groups of size *k* means we take consecutive blocks of *k* nodes and reverse the order of nodes inside each block while keeping the blocks themselves in their original order. If the remaining nodes at the end are fewer than *k*, we leave them untouched. The core idea is to isolate each block, reverse it using the standard in‑place list reversal technique, and then reconnect the reversed block with the previously processed part of the list.

## When to Use It

Use this technique when you see:
- A singly‑linked list and a requirement to reverse nodes in fixed‑size groups.
- The problem statement mentions “reverse every k nodes” or similar.
- You need to preserve the relative order of groups but reverse the internals.

## Template

```python
# Definition for singly‑linked list.
class ListNode:
    def __init__(self, val=0, nxt=None):
        self.val = val
        self.next = nxt

def reverseKGroup(head: ListNode, k: int) -> ListNode:
    # Dummy node simplifies edge cases (head may change)
    dummy = ListNode(0, head)
    group_prev = dummy                     # node before the current k‑group

    while True:
        # 1️⃣ Check if there are at least k nodes left
        kth = group_prev
        for i in range(k):
            kth = kth.next
            if not kth:                    # fewer than k nodes → stop
                return dummy.next

        # 2️⃣ Nodes after the current group
        group_next = kth.next

        # 3️⃣ Reverse the k‑group (standard in‑place reversal)
        prev, cur = group_next, group_prev.next
        while cur is not group_next:
            nxt = cur.next
            cur.next = prev
            prev, cur = cur, nxt

        # 4️⃣ Reconnect the reversed group
        #    group_prev.next was the first node of the group; now it's the tail
        tail = group_prev.next
        group_prev.next = prev            # new head of the reversed group
        group_prev = tail                 # move group_prev to the tail for next iteration

    return dummy.next
```

## LeetCode Problem Walkthrough

### Problem: 25. Reverse Nodes in k-Group

https://leetcode.com/problems/reverse-nodes-in-k-group/

### Approach 1: Brute Force (Array‑Based)

**Algorithm**
1. Traverse the list and store all node values in an array `vals`.
2. Process `vals` in chunks of size `k`:
   - Reverse each full chunk.
   - Leave the last incomplete chunk as‑is.
3size < k) unchanged.
3. Rebuild a new linked list from the modified `vals` array and return its head.

**Implementation**

```python
def reverseKGroup_brute(head: ListNode, k: int) -> ListNode:
    # 1️⃣ Collect values
    vals = []
    cur = head
    while cur:
        vals.append(cur.val)
        cur = cur.next

    # 2️⃣ Reverse each k‑sized chunk
    for i in range(0, len(vals) - len(vals) % k, k):
        vals[i:i+k] = reversed(vals[i:i+k])

    # 3️⃣ Rebuild list
    dummy = ListNode(0)
    cur = dummy
    for v in vals:
        cur.next = ListNode(v)
        cur = cur.next
    return dummy.next
```

**Complexity Analysis**
- Time complexity: O(n) — one pass to collect values, one pass to rebuild, plus O(n) for chunk reversals.
- Space complexity: O(n) — auxiliary array storing all node values.

---

### Approach 2: Recursive (Divide & Conquer)

**Intuition**
If we can reverse the first *k* nodes and know the head of the rest of the list after processing, we can recursively solve the sub‑problem that starts after the current group. The recursion naturally handles the connection between groups.

**Algorithm**
1. Verify that at least *k* nodes exist from the current `head`. If not, return `head` unchanged.
2. Reverse the first *k* nodes using the standard iterative reversal (returns `new_head` and `new_tail`).
3. Recursively call `reverseKGroup` on the node after the `k`‑th node (`next_group_head`).
4. Link the tail of the reversed group to the result of the recursive call.
5. Return `new_head` as the head of the processed segment.

**Implementation**

```python
def reverseKGroup_recursive(head: ListNode, k: int) -> ListNode:
    # Helper: check if there are k nodes ahead
    def length_at_least_k(node, k):
        while node and k:
            node = node.next
            k -= 1
        return k == 0

    if not head or not length_at_least_k(head, k):
        return head                     # base case: fewer than k nodes left

    # Reverse first k nodes
    prev, cur = None, head
    for _ in range(k):
        nxt = cur.next
        cur.next = prev
        prev, cur = cur, nxt
    # after loop: prev = new head of k‑group, cur = (k+1)‑th node
    # head is now the tail of the reversed group

    # Recursively process the rest
    head.next = reverseKGroup_recursive(cur, k)
    return prev                         # new head of this segment
```

**Complexity Analysis**
- Time complexity: O(n) — each node is visited a constant number of times (once for length check, once for reversal).
- Space complexity: O(n/k) — recursion depth equals number of groups (worst‑case O(n) when k=1, but for typical k>1 it’s O(n/k)).

---

### Approach 3: Optimal In‑Place Pointer Manipulation

**Intuition**
We can avoid extra storage and recursion by iterating through the list, reversing each group in place using only a few pointers. A dummy node before the head simplifies edge cases (like when the head itself changes). For each group we:
1. Confirm that *k* nodes exist.
2. Disconnect the group, reverse it with the classic three‑pointer method.
3. Reattach the reversed group between the previous part and the next part.
4. Move the “group previous” pointer to the tail of the just‑processed group and repeat.

**Implementation** *(matches the provided C++ solution, translated to Python)*

```python
def reverseKGroup_optimal(head: ListNode, k: int) -> ListNode:
    if not head or k <= 1:
        return head

    dummy = ListNode(0, head)
    group_prev = dummy                     # node before the current k‑group

    while True:
        # 1️⃣ Find the kth node from group_prev
        kth = group_prev
        for _ in range(k):
            kth = kth.next
            if not kth:                    # fewer than k nodes remain
                return dummy.next

        group_next = kth.next              # node after the k‑group

        # 2️⃣ Reverse the k‑group
        prev, cur = group_next, group_prev.next
        while cur is not group_next:
            nxt = cur.next
            cur.next = prev
            prev, cur = cur, nxt

        # 3️⃣ Reconnect
        tail = group_prev.next             # original first node, now tail
        group_prev.next = prev             # prev is new head after reversal
        group_prev = tail                  # move to tail for next iteration

    return dummy.next
```

**Complexity Analysis**
- Time complexity: O(n) — each node is processed a constant number of times (once to locate kth, once during reversal).
- Space complexity: O(1) — only a handful of pointers are used regardless of input size.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

**Dry Run** (using the optimal in‑place approach)

Input: `head = [1,2,3,4,5]`, `k = 2`

```
Initial list (dummy → 1 → 2 → 3 → 4 → 5 → None)
group_prev points to dummy (0)

Iteration 1:
  - Find kth (2nd) node from group_prev: node 2
  - group_next = node 3
  - Reverse nodes [1,2]:
        prev = 3 (group_next), cur = 1
        Step A: cur=1 → nxt=2, 1.next=3, prev=1, cur=2
        Step B: cur=2 → nxt=3, 2.next=1, prev=2, cur=3 (stop)
  - Reconnect: dummy.next → 2, tail (old group_prev.next) = 1
                list now: dummy → 2 → 1 → 3 → 4 → 5 → None
  - group_prev moves to tail (node 1)

Iteration 2:
  - group_prev = node 1, find kth (2nd) → node 4
  - group_next = node 5
  - Reverse nodes [3,4]:
        prev = 5, cur = 3
        Step A: 3.next=5, prev=3, cur=4
        Step B: 4.next=3, prev=4, cur=5 (stop)
  - Reconnect: group_prev (node1).next → 4, tail = node3
                list: dummy → 2 → 1 → 4 → 3 → 5 → None
  - group_prev moves to tail (node3)

Iteration 3:
  - group_prev = node3, try to find kth (2nd) → need nodes ahead) → None (only node5 left)
  - Break loop, return dummy.next → [2,1,4,3,5]
```

The final list matches the expected output `[2,1,4,3,5]`.