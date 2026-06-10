**GitHub Issue: Lecture – Fast & Slow Pointers (Linked List Cycle)**  

---  

# Fast & Slow Pointers  

## Video Solution  

For more details about **Linked List Cycle**, watch the walkthrough at [https://www.youtube.com/watch?v=gBTe7lFR3vc](https://www.youtube.com/watch?v=gBTe7lFR3vc).  

## Concept  

The fast‑and‑slow pointer technique (also called Floyd’s Tortoise and Hare) uses two pointers that traverse a sequence at different speeds. If there is a cycle, the faster pointer will eventually “lap” the slower one and they will meet. If there is no cycle, the fast pointer will reach the end (`null`).  

*Real‑world analogy:* Imagine a running track where one runner jogs at 1 m/s and another sprints at 2 m/s. If the track is a loop, the sprinter will eventually catch up to the jogger. If the track is a straight line that ends, the sprinter will reach the finish line first.  

## When to Use It  

Use fast‑and‑slow pointers when you see:  

- A linked list (or any sequence accessed via `next` pointers) and you need to detect a cycle.  
- Problems asking for the start of a cycle, the length of a cycle, or determining if two linked lists intersect.  
- Situations where you need O(1) extra space and can afford O(n) time.  

## Template  

```python
# Fast & Slow Pointers template for cycle detection in a singly linked list
def has_cycle(head):
    # Edge case: empty list or single node without a self‑loop
    if not head or not head.next:
        return False

    slow = head          # moves 1 step each iteration
    fast = head          # moves 2 steps each iteration

    while fast and fast.next:   # ensure we can advance fast two steps
        slow = slow.next
        fast = fast.next.next
        if slow is fast:        # meeting point → cycle exists
            return True

    return False                # fast reached the end → no cycle
```

---

## LeetCode Problem Walkthrough  

### Problem: 141. Linked List Cycle  

https://leetcode.com/problems/linked-list-cycle/  

### Approach 1: Brute Force (Hash Set)  

**Algorithm**  
Traverse the list while storing each visited node in a hash set. If we encounter a node that is already in the set, a cycle exists. If we reach `null`, there is no cycle.  

**Implementation**  

```python
def hasCycle(head):
    seen = set()
    cur = head
    while cur:
        if cur in seen:          # node visited before → cycle
            return True
        seen.add(cur)
        cur = cur.next
    return False
```

**Complexity Analysis**  

- Time complexity: O(n) — each node is visited once.  
- Space complexity: O(n) — we may store every node in the set.  

### Approach 2: Marking Nodes (In‑Place Modification)  

**Intuition**  
If we are allowed to modify the list temporarily, we can mark visited nodes (e.g., by setting a special flag or overwriting `next` with a sentinel). When we encounter a marked node again, a cycle exists. This avoids extra space but changes the input structure.  

**Algorithm**  
Iterate through the list. For each node, if its `next` pointer is already a special marker (e.g., a dummy node), return `True`. Otherwise, temporarily replace `next` with the marker and move to the original next node (saved before overwriting).  

**Implementation**  

```python
def hasCycle(head):
    marker = ListNode(0)          # any node not part of the original list
    cur = head
    while cur:
        if cur.next is marker:    # we have seen this node before
            return True
        nxt = cur.next            # save original next
        cur.next = marker         # mark as visited
        cur = nxt                 # proceed with original next
    return False
```

**Complexity Analysis**  

- Time complexity: O(n) — each node processed once.  
- Space complexity: O(1) — only a constant number of extra pointers.  
*Note:* This approach mutates the list; acceptable only if the caller allows it.  

### Approach 3: Fast & Slow Pointers (Floyd’s Tortoise and Hare)  

**Intuition**  
By moving two pointers at different speeds, a cycle guarantees that the faster pointer will eventually lap the slower one. If there is no cycle, the fast pointer will hit the list’s end. This meets the O(1) space follow‑up requirement.  

**Algorithm**  

1. Handle trivial cases (empty list or single node).  
2. Initialize `slow` and `fast` at the head.  
3. While `fast` and `fast.next` are not null:  
   - Advance `slow` by one step.  
   - Advance `fast` by two steps.  
   - If `slow` equals `fast`, a cycle exists → return `True`.  
4. If the loop exits, no cycle → return `False`.  

**Implementation**  

```python
def hasCycle(head):
    if not head or not head.next:
        return False

    slow = head
    fast = head

    while fast and fast.next:
        slow = slow.next          # 1 step
        fast = fast.next.next     # 2 steps
        if slow is fast:          # meeting point
            return True

    return False
```

**Complexity Analysis**  

- Time complexity: O(n) — in the worst case, the fast pointer traverses the list twice.  
- Space complexity: O(1) — only two pointers are used regardless of input size.  

## Provide a Visual Demonstration  

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example  

### Dry Run  

Input: `head = [3,2,0,-4]`, cycle connects node at index 1 (value 2) to tail.  

| Step | slow (node) | fast (node) | Action                              |
|------|-------------|-------------|-------------------------------------|
| 0    | 3           | 3           | Initialize both at head             |
| 1    | 2           | 0           | slow → 2, fast → 0 (two steps)     |
| 2    | 0           | 2           | slow → 0, fast → 2 (two steps)     |
| 3    | -4          | -4          | slow → -4, fast → -4 (two steps) → **meeting** → cycle detected |

Since `slow` and `fast` meet at node `-4`, we return `True`.  

If the list were `[1]` with `pos = -1` (no cycle):  

| Step | slow | fast | Action                              |
|------|------|------|-------------------------------------|
| 0    | 1    | 1    | Start                               |
| 1    | (null) | (null) | fast.next is null → loop ends → return `False` |

---  

*End of lecture.*