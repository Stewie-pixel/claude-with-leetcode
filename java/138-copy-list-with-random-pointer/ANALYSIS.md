# Linked List

## Video Solution

For more details about **Copy List with Random Pointer**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=5Y2EiZST97Y).

## Concept

Creating a deep copy of a linked list where each node has an extra **random** pointer cannot be done by simply copying the `next` pointers, because the `random` pointers may point to any node (or `null`). The challenge is to recreate the same structure without reusing any original nodes.  
A common real‑world analogy is photocopying a set of index cards where each card may have a note pointing to another card in the deck; you need to produce a completely new deck that preserves both the order and the internal notes.

## When to Use It

Use this technique when you see:
- A linked list where each node contains an extra pointer (e.g., `random`, `arbitrary`, `sibling`).
- The problem asks for a **deep copy** (no shared nodes with the original).
- Constraints allow O(n) time and either O(n) or O(1) extra space.

## Template

Below is a reusable Python template for copying a linked list with an arbitrary pointer using a hash map (O(n) time, O(n) space). The interleaving/O(1)‑space variant follows the same pattern but modifies the list in place.

```python
# Definition for a Node.
class Node:
    def __init__(self, x: int, next: 'Node' = None, random: 'Node' = None):
        self.val = int(x)
        self.next = next
        self.random = random

def copyRandomList(head: 'Node') -> 'Node':
    if not head:
        return None

    # Step 1: create a map from original node -> its copy
    old_to_new = {}
    cur = head
    while cur:
        old_to_new[cur] = Node(cur.val)
        cur = cur.next

    # Step 2: assign next and random pointers using the map
    cur = head
    while cur:
        copy = old_to_new[cur]
        copy.next = old_to_new.get(cur.next)          # None if cur.next is None
        copy.random = old_to_new.get(cur.random)      # None if cur.random is None
        cur = cur.next

    return old_to_new[head]
```

---

## LeetCode Problem Walkthrough

### Problem: 138. Copy List with Random Pointer  

https://leetcode.com/problems/copy-list-with-random-pointer/

---

### Approach 1: Brute Force (O(n²) time, O(1) space)

**Algorithm**  
For each node in the original list, create a copy node. To set its `random` pointer, scan the original list from the head to find the node that the original `random` points to, then use the same index to pick the corresponding copy node. This requires a linear scan for every node, leading to quadratic time.

**Implementation**

```python
def copyRandomList(head: 'Node') -> 'Node':
    if not head:
        return None

    # First pass: create copy nodes (only val set)
    cur = head
    copies = []
    while cur:
        copies.append(Node(cur.val))
        cur = cur.next

    # Second pass: fill next and random by index search
    cur = head
    i = 0
    while cur:
        copy = copies[i]
        # next pointer
        if cur.next:
            # find index of cur.next
            nxt = head
            j = 0
            while nxt is not cur.next:
                nxt = nxt.next
                j += 1
            copy.next = copies[j]
        # random pointer
        if cur.random:
            rnd = head
            j = 0
            while rnd is not cur.random:
                rnd = rnd.next
                j += 1
            copy.random = copies[j]
        cur = cur.next
        i += 1

    return copies[0]
```

**Complexity Analysis**  
- Time complexity: O(n²) — for each of the n nodes we may traverse up to n nodes to locate the random target.  
- Space complexity: O(n) — we store the list of copy nodes (the output itself does not count as extra space; if we ignore the output, auxiliary space is O(1)).

---

### Approach 2: Hash Map (O(n) time, O(n) space)

**Intuition**  
If we could remember which copy corresponds to each original node, we could set the `next` and `random` pointers in a single pass. A hash map (`original -> copy`) gives us O(1) lookup for any node.

**Algorithm**  
1. First pass: iterate through the original list, create a copy node for each original node, and store the mapping in a hash map.  
2. Second pass: iterate again, and for each original node retrieve its copy from the map, then set `copy.next = map[original.next]` and `copy.random = map[original.random]` (using `None` when the original pointer is `None`).  
3. Return the copy of the head node.

**Implementation**

```python
def copyRandomList(head: 'Node') -> 'Node':
    if not head:
        return None

    old_to_new = {}
    cur = head
    while cur:
        old_to_new[cur] = Node(cur.val)
        cur = cur.next

    cur = head
    while cur:
        copy = old_to_new[cur]
        copy.next = old_to_new.get(cur.next)
        copy.random = old_to_new.get(cur.random)
        cur = cur.next

    return old_to_new[head]
```

**Complexity Analysis**  
- Time complexity: O(n) — two linear passes over the list.  
- Space complexity: O(n) — the hash map stores an entry for each of the n nodes.

---

### Approach 3: Interleaving / O(1) Space (O(n) time, O(1) extra space)

**Intuition**  
We can avoid the hash map by weaving the copied nodes directly into the original list: after each original node we insert its copy. This layout lets us set the `random` pointers by using the fact that the copy of a node’s `random` is just the node next to the original `random`. Finally we unweave the list to restore the original and extract the copy list.

**Algorithm**  
1. **Insert copies**: Traverse the original list; for each node `orig`, create `copy = Node(orig.val)` and insert it right after `orig` (`orig.next = copy; copy.next = orig_next`).  
2. **Assign random pointers**: Traverse again; for each original node `orig`, its copy is `orig.next`. Set `orig.next.random = orig.random.next` if `orig.random` exists (because the node after `orig.random` is its copy).  
3. **Separate lists**: Traverse once more, restoring the `next` pointers of the original list and extracting the copy list by linking the `next` pointers of the copy nodes.

**Implementation**

```python
def copyRandomList(head: 'Node') -> 'Node':
    if not head:
        return None

    # 1️⃣  Weave copies into the original list
    cur = head
    while cur:
        nxt = cur.next
        copy = Node(cur.val)
        cur.next = copy
        copy.next = nxt
        cur = nxt

    # 2️⃣  Assign random pointers for the copies
    cur = head
    while cur:
        copy = cur.next
        if cur.random:
            copy.random = cur.random.next   # the node after original random is its copy
        cur = copy.next                     # move to next original node

    # 3️⃣  Unweave to retrieve the copy list and restore original list
    cur = head
    copy_head = head.next
    while cur:
        copy = cur.next
        cur.next = copy.next                # restore original next
        cur = cur.next                      # move to next original node
        if cur:
            copy.next = cur.next            # link copy to next copy
    return copy_head
```

**Complexity Analysis**  
- Time complexity: O(n) — three linear passes.  
- Space complexity: O(1) — only a few pointer variables are used; the output list does not count as extra space.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example  

#### Dry Run

We'll trace the **hash‑map** approach on the example:

```
Input: [[7,null],[13,0],[11,4],[10,2],[1,0]]
```

| Step | Original node (val) | Copy created? | map[original] → copy | Action for next/random |
|------|---------------------|--------------|----------------------|------------------------|
| 1    | 7 (idx0)            | yes          | 7̂ → copy0(7)        | –                      |
| 2    | 13 (idx1)           | yes          | 13̂ → copy1(13)      | –                      |
| 3    | 11 (idx2)           | yes          | 11̂ → copy2(11)      | –                      |
| 4    | 10 (idx3)           | yes          | 10̂ → copy3(10)      | –                      |
| 5    | 1  (idx4)           | yes          | 1̂  → copy4(1)       | –                      |

Second pass – set pointers:

| Step | Original node | copy node | copy.next = map[orig.next] | copy.random = map[orig.random] |
|------|---------------|----------|----------------------------|--------------------------------|
| 1    | 7̂            | copy0    | copy1 (13̂)                | None (orig.random is null)     |
| 2    | 13̂           | copy1    | copy2 (11̂)                | copy0 (orig.random points to 7̂) |
| 3    | 11̂           | copy2    | copy3 (10̂)                | copy4 (orig.random points to 1̂) |
| 4    | 10̂           | copy3    | copy4 (1̂)                 | copy2 (orig.random points to 11̂) |
| 5    | 1̂            | copy4    | None                       | copy0 (orig.random points to 7̂) |

Resulting copy list matches the original structure:  
`[7,null] → [13,0] → [11,4] → [10,2] → [1,0]`.

---

Follow the steps above to implement any of the three approaches; the hash‑map version is the most straightforward to explain and debug, while the interleaving version achieves optimal O(1) extra space. Choose the version that best fits the constraints of your interview or coding session.