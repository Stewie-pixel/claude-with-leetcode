# Two Pointers

## Video Solution

For more details about **Rearrange String to Avoid Character Pair**, watch the walkthrough at [https://www.youtube.com/watch?v=8guD2xNLH4A](https://www.youtube.com/watch?v=8guD2xNLH4A).

## Concept

The goal is to reorder the characters of a string so that every occurrence of character **y** appears before every occurrence of character **x**, while keeping all other characters in any order.  
Think of arranging a line of people where everyone wearing a red shirt (**y**) must stand in front of everyone wearing a blue shirt (**x**); people wearing other colors can be anywhere in between.  
We can achieve this by partitioning the string into three contiguous blocks:  

1. All **y**'s  
2. All characters that are neither **x** nor **y**  
3. All **x**'s  

This is analogous to the Dutch National Flag problem (three‑way partition) where we rearrange an array around two pivot values.

## When to Use It

Use the two‑pointer / three‑way partition pattern when you see:

- Need to group elements according to two (or three) categories.
- Statements like “all **A** before all **B**”, or “elements less than pivot, equal to pivot, greater than pivot”.
- Problems that ask for a rearrangement satisfying a ordering constraint without caring about the internal order of the “middle” group.

## Template

```python
def three_way_partition(arr, low_val, high_val):
    """
    Rearrange arr so that:
        arr[0:left]   == low_val
        arr[left:mid] == other values
        arr[mid:]     == high_val
    Returns the rearranged array (in‑place).
    """
    left = 0          # boundary for low_val (y's)
    mid = 0           # current element
    right = len(arr) - 1   # boundary for high_val (x's)

    while mid <= right:
        if arr[mid] == low_val:          # put y at the front
            arr[left], arr[mid] = arr[mid], arr[left]
            left += 1
            mid += 1
        elif arr[mid] == high_val:       # put x at the back
            arr[mid], arr[right] = arr[right], arr[mid]
            right -= 1
        else:                            # middle element, just move on
            mid += 1
    return arr
```

---

## LeetCode Problem Walkthrough

### Problem: 2729. Rearrange String to Avoid Character Pair  

https://leetcode.com/problems/rearrange-string-to-avoid-character-pair/

### Approach 1: Brute Force (Generate All Permutations)

**Algorithm**  
1. Generate every distinct permutation of the input string `s`.  
2. For each permutation, check whether all `y` characters appear before any `x` character.  
3. Return the first permutation that satisfies the condition.

**Implementation**

```python
from itertools import permutations

class Solution:
    def rearrangeString(self, s: str, x: str, y: str) -> str:
        # Use a set to avoid duplicate permutations when s has repeated chars
        seen = set()
        for p in permutations(s):
            if p in seen:
                continue
            seen.add(p)
            t = ''.join(p)
            # Verify condition: no x appears before a y
            seen_x = False
            valid = True
            for ch in t:
                if ch == x:
                    seen_x = True
                elif ch == y and seen_x:
                    valid = False
                    break
            if valid:
                return t
        return ""   # fallback (should never happen for valid input)
```

**Complexity Analysis**  
- Time complexity: O(n! · n) — we generate n! permutations and each validation scans the string of length n.  
- Space complexity: O(n) for the recursion stack of `permutations` plus the set of seen permutations (worst‑case O(n!) but bounded by the small input limit).

---

### Approach 2: Counting & Concatenation (Optimal Linear Time)

**Intuition**  
If we know how many `y`'s, `x`'s, and other characters exist, we can directly build the answer: place all `y`'s first, then all other characters, then all `x`'s. No need to examine ordering inside the middle block because any order works.

**Algorithm**  
1. Count occurrences of `y`, `x`, and all other letters while scanning `s`.  
2. Construct the result string as: `'y' * count_y + middle_chars + 'x' * count_x`, where `middle_chars` is the concatenation of all non‑`x`/`non‑`y` characters in their original order (or any order).  

**Implementation**

```python
class Solution:
    def rearrangeString(self, s: str, x: str, y: str) -> str:
        before = []   # will hold all y's
        middle = []   # characters that are neither x nor y
        after = []    # will hold all x's

        for ch in s:
            if ch == y:
                before.append(ch)
            elif ch == x:
                after.append(ch)
            else:
                middle.append(ch)

        return ''.join(before + middle + after)
```

**Complexity Analysis**  
- Time complexity: O(n) — one pass to classify characters, plus O(n) to join the three lists.  
- Space complexity: O(n) for the three temporary lists (output itself also O(n)).  
  (If we count only auxiliary space besides the output, it is O(1) because we only store three counters and rebuild the string directly.)

---

### Approach 3: In‑Place Three‑Way Partition (Two Pointers)

**Intuition**  
Treat the string as a mutable array of characters. We can rearrange it in a single sweep using three regions:  
- `[0, low)` → all `y`'s  
- `[low, mid)` → characters that are neither `x` nor `y`  
- `[mid, high]` → unknown  
- `[high+1, end)` → all `x`'s  

We move `mid` forward, swapping `y`'s to the low boundary and `x`'s to the high boundary, exactly like the Dutch National Flag algorithm.

**Algorithm**  
1. Convert `s` to a list `arr` (strings are immutable in Python).  
2. Initialize `low = 0`, `mid = 0`, `high = len(arr) - 1`.  
3. While `mid <= high`:  
   - If `arr[mid] == y`: swap `arr[low]` and `arr[mid]`, increment `low` and `mid`.  
   - Else if `arr[mid] == x`: swap `arr[mid]` and `arr[high]`, decrement `high`.  
   - Else: increment `mid`.  
4. Join the list back into a string and return.

**Implementation**

```python
class Solution:
    def rearrangeString(self, s: str, x: str, y: str) -> str:
        arr = list(s)               # make it mutable
        low, mid = 0, 0
        high = len(arr) - 1

        while mid <= high:
            if arr[mid] == y:
                arr[low], arr[mid] = arr[mid], arr[low]
                low += 1
                mid += 1
            elif arr[mid] == x:
                arr[mid], arr[high] = arr[high], arr[mid]
                high -= 1
            else:
                mid += 1

        return ''.join(arr)
```

**Complexity Analysis**  
- Time complexity: O(n) — each element is examined at most once, and each swap is O(1).  
- Space complexity: O(1) auxiliary (the list conversion counts as O(n) but is required for in‑place mutation; if the language allowed mutable strings, extra space would be constant).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run Approach 3 (three‑way partition) on the example `s = "aabc"`, `x = 'a'`, `y = 'c'`.

**Input:** `arr = ['a','a','b','c']`, `x='a'`, `y='c'`  
We want all `c`'s before all `a`'s.

```
Initialize: low = 0, mid = 0, high = 3
arr = [ a , a , b , c ]
        ^   ^       ^
        l   m       h
```

| Step | mid | arr[mid] | Action                                   | arr after action               | low | mid | high |
|------|-----|----------|------------------------------------------|--------------------------------|-----|-----|------|
| 1    | 0   | 'a'      | not y, not x → just mid++                | [a, a, b, c]                   | 0   | 1   | 3    |
| 2    | 1   | 'a'      | not y, not x → mid++                     | [a, a, b, c]                   | 0   | 2   | 3    |
| 3    | 2   | 'b'      | middle → mid++                           | [a, a, b, c]                   | 0   | 3   | 3    |
| 4    | 3   | 'c'      | == y → swap arr[low] and arr[mid]        | [c, a, b, a] (swap 0↔3)        | 1   | 4   | 3    |
|      |     |          | low++, mid++ (now mid > high → stop)     |                                |     |     |      |

Final `arr = ['c','a','b','a']` → `"caba"` which satisfies the condition (all `c` before all `a`). Any ordering of the middle `'a','b'` is acceptable; the algorithm produced `'a','b'`.

---

**Summary**  
- Brute force shows the problem’s requirement but is infeasible for realistic sizes.  
- Counting & concatenation gives a simple, clear O(n) solution.  
- The in‑place three‑way partition demonstrates the classic two‑pointer technique, achieving O(n) time with O(1) extra space and directly illustrates the “all y before all x” grouping.  

Pick the approach that best fits your constraints; for interview clarity, the counting method is often sufficient, while the partition method showcases deeper algorithmic insight.