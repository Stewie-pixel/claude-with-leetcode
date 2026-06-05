# Stack

## Video Solution

For more details about **Removing Stars From a String**, watch the walkthrough at [Resources](https://www.youtube.com/watch?v=Z6v0V8xYc6M).

## Concept

A *stack* lets us add elements to the top and remove the most‑recent element in O(1) time.  
In this problem each `*` means “delete the closest non‑star to its left”. That is exactly the behavior of a stack pop: when we see a star we discard the last character we have kept.

## When to Use It

Use a stack (or a mutable array acting as a stack) when the problem describes:

* “remove/undo the previous element”  
* “backspace‑like behavior”  
* “matching or canceling pairs where the canceling symbol appears after the symbol to cancel”

Typical clues: `*`, `#` (backspace), brackets that cancel, etc.

## Template

```python
# Generic stack‑like processing (push on normal chars, pop on special)
def process(s: str, special: str = '*') -> str:
    stack = []                     # acts as our stack
    for ch in s:
        if ch == special:
            if stack:              # guaranteed by problem statement
                stack.pop()        # remove closest left non‑special
        else:
            stack.append(ch)       # keep the character
    return ''.join(stack)
```

## LeetCode Problem Walkthrough

### Problem: 2390. Removing Stars From a String
https://leetcode.com/problems/removing-stars-from-a-string/

---

### Approach 1: Brute Force – Repeated Scanning

**Algorithm**
1. Convert the string to a list (or keep as string) so we can modify it.
2. Repeatedly scan from left to right looking for the first `*`.
3. When a `*` is found at index `i`, delete the character at `i‑1` and the `*` itself.
4. Continue until no `*` remains.
5. Join the remaining characters and return.

Because each deletion may shift the whole string, the worst‑case work is O(n²).

**Implementation**

```python
class Solution:
    def removeStars(self, s: str) -> str:
        chars = list(s)               # mutable copy
        i = 0
        while i < len(chars):
            if chars[i] == '*':
                # delete the character to the left and the star itself
                del chars[i-1:i+1]    # removes two elements
                # after deletion, step back to re‑check possible new star
                i = max(i-2, 0)
            else:
                i += 1
        return ''.join(chars)
```

**Complexity Analysis**
- Time complexity: O(n²) — each deletion may shift up to O(n) characters, and we may delete up to n/2 stars.  
- Space complexity: O(n) — the mutable list of characters.

---

### Approach 2: Stack (Optimal)

**Intuition**
A star always cancels the nearest preceding non‑star. Keeping a stack of characters we have seen lets us pop that character instantly when we meet a star, achieving linear time.

**Algorithm**
1. Initialise an empty list `stack`.
2. Iterate over each character `c` in the input string.
   - If `c` is `*`, pop the top of the stack (removes the closest left non‑star).
   - Otherwise, push `c` onto the stack.
3. After processing all characters, join the stack into a string and return it.

**Implementation**

```python
class Solution:
    def removeStars(self, s: str) -> str:
        stack = []                     # acts as a stack
        for ch in s:
            if ch == '*':
                stack.pop()           # remove closest left non‑star
            else:
                stack.append(ch)      # keep the character
        return ''.join(stack)
```

**Complexity Analysis**
- Time complexity: O(n) — one pass, each push/pop is O(1).  
- Space complexity: O(n) — the stack may hold up to n characters (when there are no stars).

---

### Approach 3: In‑Place Two‑Pointer (Array Simulation)

**Intuition**
If we are allowed to modify the input as a mutable array, we can reuse the same array as our stack: a write pointer indicates the current “top” of the stack. When we see a star we move the write pointer back; otherwise we write the character at the write pointer and advance it.

**Algorithm**
1. Convert the string to a list `arr` (so we can assign by index).
2. Initialise `write = 0` — points to the next free slot (stack size).
3. For each character `ch` in the original string:
   - If `ch == '*'`: decrement `write` (pop).
   - Else: assign `arr[write] = ch` and increment `write` (push).
4. The result is the first `write` elements of `arr` joined into a string.

**Implementation**

```python
class Solution:
    def removeStars(self, s: str) -> str:
        arr = list(s)                 # mutable copy
        write = 0                     # stack size / next write position
        for ch in s:
            if ch == '*':
                write -= 1            # pop
            else:
                    arr[write] = ch   # push
                    write += 1
        return ''.join(arr[:write])
```

**Complexity Analysis**
- Time complexity: O(n) — single pass, constant work per character.  
- Space complexity: O(n) — the character array (the output itself); auxiliary extra space is O(1).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Input: `s = "leet**cod*e"`

| Step | Index | Char | Stack (as list) | Action                           |
|------|-------|------|-----------------|----------------------------------|
| 1    | 0     | l    | ['l']           | push 'l'                         |
| 2    | 1     | e    | ['l','e']       | push 'e'                         |
| 3    | 2     | e    | ['l','e','e']   | push 'e'                         |
| 4    | 3     | t    | ['l','e','e','t']| push 't'                        |
| 5    | 4     | *    | ['l','e','e']   | pop → remove 't'                 |
| 6    | 5     | *    | ['l','e']       | pop → remove 'e'                 |
| 7    | 6     | c    | ['l','e','c']   | push 'c'                         |
| 8    | 7     | o    | ['l','e','c','o']| push 'o'                        |
| 9    | 8     | d    | ['l','e','c','o','d']| push 'd'                    |
|10    | 9     | *    | ['l','e','c','o']| pop → remove 'd'                |
|11    |10     | e    | ['l','e','c','o','e']| push 'e'                     |

Final stack → `"lecoe"`.

---

**Summary**

* The stack‑like approach (Approach 2) gives the optimal O(n) time and O(n) auxiliary space solution.  
* It directly mirrors the problem’s “delete the nearest left character on star” rule, making the reasoning clear and the implementation concise.  

Feel free to try the in‑place two‑pointer version if you want to avoid extra allocation beyond the input array. Happy coding!