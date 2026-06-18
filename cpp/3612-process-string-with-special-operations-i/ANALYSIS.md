# Stack

## Video Solution

For more details about **Process String with Special Operations I**, watch the walkthrough at [https://www.youtube.com/watch?v=3nHr5oF-jfc](https://www.youtube.com/watch?v=3nHr5oF-jfc)

## Concept

The problem asks us to build a string by applying four simple operations while scanning the input from left to right:

* **Letter** – append it.
* **`*`** – delete the last character (like a backspace).
* **`#`** – duplicate the whole current string.
* **`%`** – reverse the whole current string.

All operations affect the *entire* current result, but they can be performed incrementally if we keep the result in a mutable container (e.g., a Python list) and update it step‑by‑step. This is analogous to a text editor where you type, press backspace, copy‑paste the whole document, or hit a “reverse” button.

## When to Use It

Think of this pattern when a problem describes:

* A stream of characters where special symbols modify the *already built* output.
* Operations like “delete last”, “duplicate everything”, or “reverse everything”.
* The input size is modest, so a direct simulation is acceptable.

In short: **use a simple simulation (often a stack‑like builder) when you see per‑character instructions that mutate the accumulated result.**

## Template

```python
# result acts like a stack / mutable builder
result = []          # list of characters for O(1) append/pop
for ch in s:
    if ch.isalpha():                 # lowercase letter
        result.append(ch)
    elif ch == '*':                  # backspace
        if result:
            result.pop()
    elif ch == '#':                  # duplicate
        result.extend(result)        # copy current content
    elif ch == '%':                  # reverse
        result.reverse()             # in‑place reversal
# convert list back to string
return ''.join(result)
```

## LeetCode Problem Walkthrough

### Problem: 3612. Process String with Special Operations I

https://leetcode.com/problems/process-string-with-special-operations-i/

---

### Approach 1: Brute‑Force Simulation (Direct String)

**Algorithm**  
Iterate over the input string. Keep a Python string `res` and apply each rule literally:

* Letter → `res += ch`
* `*` → `res = res[:-1]` (if not empty)
* `#` → `res = res + res`
* `%` → `res = res[::-1]`

Because strings are immutable in Python, each operation that creates a new string copies the whole current content, leading to a worst‑case **O(n²)** time where *n* is the length of the final string (duplication can double the size each time).

**Implementation**

```python
class Solution:
    def processStr(self, s: str) -> str:
        res = ""
        for ch in s:
            if 'a' <= ch <= 'z':          # letter
                res += ch
            elif ch == '*':               # backspace
                if res:
                    res = res[:-1]
            elif ch == '#':               # duplicate
                res = res + res
            elif ch == '%':               # reverse
                res = res[::-1]
        return res
```

**Complexity Analysis**

- **Time complexity:** O(n²) – each `#` or `%` may copy the whole string, and there can be O(n) such operations.
- **Space complexity:** O(n) – the final result string.

---

### Approach 2: Simulation with a Mutable List (Stack‑like)

**Intuition**  
Using a mutable list avoids the repeated allocation of new strings for append/pop. The only expensive operations are duplication (`#`) and reversal (`%`), which still need to touch every element, but we avoid the extra factor of copying the string on every single character append. This brings the asymptotic bound down to **O(n²)** in the worst case (due to `#` and `%`) but with a much smaller constant factor and cleaner code.

**Algorithm**  
Maintain a list `stack` representing the current result.

* Letter → `stack.append(ch)`
* `*` → `stack.pop()` if not empty
* `#` → `stack.extend(stack)` (copy the current list)
* `%` → `stack.reverse()` (in‑place)

Finally, join the list into a string.

**Implementation**

```python
class Solution:
    def processStr(self, s: str) -> str:
        stack = []                # acts as a stack / mutable builder
        for ch in s:
            if 'a' <= ch <= 'z':          # letter
                stack.append(ch)
            elif ch == '*':               # backspace
                if stack:
                    stack.pop()
            elif ch == '#':               # duplicate
                stack.extend(stack)       # copy current content
            elif ch == '%':               # reverse
                stack.reverse()           # in‑place reversal
        return ''.join(stack)
```

**Complexity Analysis**

- **Time complexity:** O(n²) – each `#` or `%` may process O(k) elements where k is the current length; in the worst case (e.g., alternating letters and `#`) the total work is quadratic.
- **Space complexity:** O(n) – the list holds at most the final result.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Input: `s = "a#b%*"`

| Step | i | ch | Operation               | stack (as list) | Current result |
|------|---|----|-------------------------|-----------------|----------------|
| 0    | 0 | a  | append `'a'`            | ['a']           | "a"            |
| 1    | 1 | #  | duplicate               | ['a','a']       | "aa"           |
| 2    | 2 | b  | append `'b'`            | ['a','a','b']   | "aab"          |
| 3    | 3 | %  | reverse                 | ['b','a','a']   | "baa"          |
| 4    | 4 | *  | pop last                | ['b','a']       | "ba"           |

Final result: `"ba"` (matches the expected output).

---

**Summary**  
We simulated the editor‑like operations using a mutable list (stack‑like builder). The approach follows the straightforward left‑to‑right processing dictated by the problem statement, yields correct results for all constraints, and avoids unnecessary string allocations. The same idea can be adapted to languages where strings are mutable (e.g., C++ `std::string`) or where a deque is preferred for efficient front/back operations.