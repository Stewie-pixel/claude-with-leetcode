# Heap / Priority Queue

## Video Solution

For more details about **Merge k Sorted Lists**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=q5a5OiGbT6Q).

## Concept

A **min‑heap (priority queue)** lets us repeatedly extract the smallest element among a collection of sorted lists in `O(log k)` time, where `k` is the number of lists.  
By always pulling the current smallest head node and then pushing the next node from the same list, we can build the final sorted list in one pass.

**Real‑world analogy:** Imagine you have several checkout lines at a grocery store, each line already sorted by the time customers arrived. To serve everyone in overall order, you repeatedly look at the person at the front of each line, pick the one who arrived earliest, send them to the cashier, and then let the next person in that line step forward. A min‑heap does exactly that “look at the fronts” step efficiently.

## When to Use It

Use a heap / priority queue when you see:

- Multiple sorted inputs (arrays, linked lists, streams) that need to be merged.
- The problem asks for the **k smallest** or **k largest** elements across several sorted sources.
- You need to repeatedly retrieve the minimum (or maximum) element from a dynamic set.

## Template

```python
import heapq
from typing import List, Optional

# Definition for singly‑linked list.
class ListNode:
    def __init__(self, val: int = 0, next: Optional['ListNode'] = None):
        self.val = val
        self.next = next

def merge_k_lists(lists: List[Optional[ListNode]]) -> Optional[ListNode]:
    # 1. Dummy head for the result list
    dummy = ListNode()
    tail = dummy

    # 2. Initialise the heap with the first node of each non‑empty list
    min_heap = []                         # entries are (value, unique_id, node)
    for i, node in enumerate(lists):
        if node:
            heapq.heappush(min_heap, (node.val, i, node))

    # 3. Extract the smallest, attach it, and push the next node from the same list
    while min_heap:
        val, _, node = heapq.heappop(min_heap)
        tail.next = node                  # attach smallest node
        tail = tail.next                  # move tail forward
        if node.next:                     # if there is a next node in the same list
            heapq.heappush(min_heap, (node.next.val, id(node.next), node.next))

    return dummy.next
```

---

## LeetCode Problem Walkthrough

### Problem: 23. Merge k Sorted Lists

https://leetcode.com/problems/merge-k-sorted-lists/

### Approach 1: Brute Force – Flatten and Sort

**Algorithm**
1. Traverse every list and copy all node values into a plain Python list.
2. Sort the list of values.
3. Create a new linked list from the sorted values and return its head.

**Implementation**

```python
def merge_k_lists_bruteforce(lists: List[Optional[ListNode]]) -> Optional[ListNode]:
    values = []
    for head in lists:
        while head:
            values.append(head.val)
            head = head.next
    values.sort()

    dummy = ListNode()
    cur = dummy
    for v in values:
        cur.next = ListNode(v)
        cur = cur.next
    return dummy.next
```

**Complexity Analysis**
- Time complexity: O(N log N) — we sort N total node values.
- Space complexity: O(N) — we store all values in an auxiliary list.

---

### Approach 2: Divide and Conquer – Pairwise Merge

**Intuition**
Merging two sorted lists is linear. If we repeatedly merge pairs of lists, the total work resembles a merge‑sort tree: each level processes all N nodes, and there are `log₂ k` levels.

**Algorithm**
1. Recursively split the array of lists into halves until each sub‑array contains 0 or 1 list.
2. Merge the two resulting sorted lists using the classic two‑pointer merge.
3. Return the merged list upward.

**Implementation**

```python
def merge_two(l1: Optional[ListNode], l2: Optional[ListNode]) -> Optional[ListNode]:
    dummy = ListNode()
    tail = dummy
    while l1 and l2:
        if l1.val < l2.val:
            tail.next = l1
            l1 = l1.next
        else:
            tail.next = l2
            l2 = l2.next
        tail = tail.next
    tail.next = l1 if l1 else l2
    return dummy.next

def merge_k_lists_divide_conquer(lists: List[Optional[ListNode]]) -> Optional[ListNode]:
    if not lists:
        return None

    def helper(left: int, right: int) -> Optional[ListNode]:
        if left > right:
            return None
        if left == right:
            return lists[left]
        mid = (left + right) // 2
        l1 = helper(left, mid)
        l2 = helper(mid + 1, right)
        return merge_two(l1, l2)

    return helper(0, len(lists) - 1)
```

**Complexity Analysis**
- Time complexity: O(N log k) — each of the `log k` levels processes every node once.
- Space complexity: O(log k) — recursion stack depth (can be made O(1) with iterative bottom‑up merging).

---

### Approach 3: Min‑Heap (Priority Queue)

**Intuition**
Instead of merging lists in pairs, we keep the current smallest head of each list in a heap. Extracting the minimum gives us the next element of the final list in `O(log k)` time, and we then replace it with the next node from the same list. This avoids the extra `log` factor that appears when we repeatedly re‑merge already merged lists.

**Algorithm**
1. Push the first node of every non‑empty list into a min‑heap ordered by node value.
2. Repeatedly pop the smallest node, attach it to the result list, and push its `next` node (if any) back into the heap.
3. When the heap is empty, all nodes have been processed.

**Implementation** (see the template above – reproduced here for completeness)

```python
import heapq
from typing import List, Optional

class ListNode:
    def __init__(self, val: int = 0, next: Optional['ListNode'] = None):
        self.val = val
        self.next = next

def merge_k_lists(lists: List[Optional[ListNode]]) -> Optional[ListNode]:
    dummy = ListNode()
    tail = dummy
    min_heap = []                     # (value, unique_id, node)

    for i, node in enumerate(lists):
        if node:
            heapq.heappush(min_heap, (node.val, i, node))

    while min_heap:
        val, _, node = heapq.heappop(min_heap)
        tail.next = node
        tail = tail.next
        if node.next:
            heapq.heappush(min_heap, (node.next.val, id(node.next), node.next))

    return dummy.next
```

**Complexity Analysis**
- Time complexity: O(N log k) — each of the N nodes is popped and pushed once; each heap operation costs `O(log k)`.
- Space complexity: O(k) — the heap stores at most one node from each of the k lists.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the heap‑based algorithm on the example:

**Input:** `lists = [[1→4→5], [1→3→4], [2→6]]`

| Step | Heap (val, list_id, node)                                 | Popped | Result list so far | Action after pop (push next) |
|------|------------------------------------------------------------|--------|--------------------|------------------------------|
| Init | (1,0,1→4→5), (1,1,1→3→4), (2,2,2→6)                       | –      | –                  | –                            |
| 1    | pop (1,0) → result: 1                                      | 1      | 1                  | push (4,0,4→5)               |
|      | heap: (1,1,1→3→4), (2,2,2→6), (4,0,4→5)                    |        |                    |                              |
| 2    | pop (1,1) → result: 1→1                                    | 1      | 1→1                | push (3,1,3→4)               |
|      | heap: (2,2,2→6), (3,1,3→4), (4,0,4→5)                      |        |                    |                              |
| 3    | pop (2,2) → result: 1→1→2                                  | 2      | 1→1→2              | push (6,2,6)                 |
|      | heap: (3,1,3→4), (4,0,4→5), (6,2,6)                        |        |                    |                              |
| 4    | pop (3,1) → result: 1→1→2→3                                | 3      | 1→1→2→3            | push (4,1,4)                 |
|      | heap: (4,0,4→5), (4,1,4), (6,2,6)                          |        |                    |                              |
| 5    | pop (4,0) → result: 1→1→2→3→4                              | 4      | 1→1→2→3→4          | push (5,0,5)                 |
|      | heap: (4,1,4), (5,0,5), (6,2,6)                            |        |                    |                              |
| 6    | pop (4,1) → result: 1→1→2→3→4→4                            | 4      | 1→1→2→3→4→4        | push (None) – list 1 exhausted|
|      | heap: (5,0,5), (6,2,6)                                     |        |                    |                              |
| 7    | pop (5,0) → result: 1→1→2→3→4→4→5                          | 5      | 1→1→2→3→4→4→5      | push (None) – list 0 exhausted|
|      | heap: (6,2,6)                                              |        |                    |                              |
| 8    | pop (6,2) → result: 1→1→2→3→4→4→5→6                        | 6      | 1→1→2→3→4→4→5→6    | push (None) – list 2 exhausted|
|      | heap: empty                                                |        |                    |                              |

Final merged list: `[1,1,2,3,4,4,5,6]` – matches expected output.

---