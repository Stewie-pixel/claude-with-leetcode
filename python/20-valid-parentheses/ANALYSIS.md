# Stack

## Video Solution

For more details about **Valid Parentheses**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=7-_V-ufnF4c)

## Concept

A **stack** is a Last‑In‑First‑Out (LIFO) data structure.  
When validating parentheses we push every opening bracket onto the stack.  
When we encounter a closing bracket it must match the most recent unmatched opening bracket – the top of the stack.  
If it matches we pop that opening bracket; otherwise the string is invalid.  
After processing the whole string the stack must be empty (every opening bracket was closed).

## When to Use It

Use a stack when you see:
- Matching pairs that must be closed in the reverse order of opening (parentheses, brackets, braces).
- Nested structures where the inner-most item must be processed first (e.g., expression evaluation, syntax checking).
- Problems that ask “is this sequence well‑formed?” or “remove adjacent duplicates”.

## Template

```python
# Generic stack‑based validation template
def is_valid_sequence(s: str, pairs: dict) -> bool:
    """
    s          : input string
    pairs      : mapping opening -> expected closing bracket
    """
    stack = []
    for ch in s:
        if ch in pairs:               # opening bracket
            stack.append(ch)
        else:                         # closing bracket
            if not stack:             # nothing to match
                return False
            opening = stack.pop()
            if pairs[opening] != ch:  # mismatched type
                return False
    return not stack                  # valid only if nothing left unmatched
```

---

## LeetCode Problem Walkthrough

### Problem: 20. Valid Parentheses

https://leetcode.com/problems/valid-parentheses/

### Approach 1: Brute Force (Repeated Removal)

**Algorithm**  
Repeatedly scan the string and delete any occurrence of the valid pairs `"()"`, `"[]"`, `"{}"`.  
After each scan, if the string did not change, stop.  
If the final string is empty, all brackets were matched; otherwise at least one bracket remains unmatched.

**Implementation**

```python
class Solution:
    def isValid(self, s: str) -> bool:
        # Keep removing pairs until nothing changes
        while True:
            new_s = s.replace("()", "").replace("[]", "").replace("{}", "")
            if new_s == s:          # no more pairs could be removed
                break
            s = new_s
        return not s                # empty string => valid
```

**Complexity Analysis**

- Time complexity: O(n²) — each full scan is O(n) and in the worst case we perform O(n) scans (e.g., "((((....))))").
- Space complexity: O(n) — the intermediate string copies.

### Approach 2: Stack (Optimized)

**Intuition**  
The most recent opening bracket must be closed first (LIFO).  
A stack lets us remember the order of openings and instantly check the top element against the current closing bracket.

**Algorithm**  
1. Initialise an empty stack.  
2. For each character `c` in the string:  
   - If `c` is an opening bracket, push it onto the stack.  
   - If `c` is a closing bracket:  
     * If the stack is empty → invalid (no opening to match).  
     * Pop the top opening bracket; if it does not map to `c` → invalid.  
3. After processing all characters, the string is valid only if the stack is empty.

**Implementation**

```python
class Solution:
    def isValid(self, s: str) -> bool:
        """
        :type s: str
        :rtype: bool
        """
        # mapping from opening to its expected closing bracket
        mapping = {
            "(": ")",
            "{": "}",
            "[": "]"
        }

        stack = []  # will hold opening brackets

        for ch in s:
            # Push opening brackets onto the stack
            if ch in mapping:
                stack.append(ch)
            # Process closing brackets
            elif ch in mapping.values():
                # No matching opening bracket
                if not stack:
                    return False
                # Get the most recent opening bracket
                top = stack.pop()
                # Current closing bracket does not match the expected one
                if ch != mapping[top]:
                    return False
            # (The problem guarantees only bracket characters, so no else needed)

        # Valid only if all opening brackets have been matched
        return not stack
```

**Complexity Analysis**

- Time complexity: O(n) — each character is examined once.
- Space complexity: O(n) — in the worst case (e.g., `"(((..."`) all characters are stored on the stack.

### Approach 3: Stack with Early Exit (Optional Optimisation)

**Intuition**  
While scanning left‑to‑right we can stop early if the remaining characters are insufficient to close the currently open brackets.  
This does not change asymptotic complexity but can cut work on clearly invalid inputs.

**Algorithm**  
Same as Approach 2, but before processing each character we check:  
`if len(s) - i - 1 < len(stack): return False`  
because there aren’t enough characters left to close every opening bracket currently on the stack.

**Implementation**

```python
class Solution:
    def isValid(self, s: str) -> bool:
        mapping = {"(": ")", "{": "}", "[": "]"}
        stack = []
        n = len(s)

        for i, ch in enumerate(s):
            # Early exit: not enough chars left to close what's on stack
            if n - i - 1 < len(stack):
                return False

            if ch in mapping:
                stack.append(ch)
            elif ch in mapping.values():
                if not stack:
                    return False
                top = stack.pop()
                if ch != mapping[top]:
                    return False

        return not stack
```

**Complexity Analysis**

- Time complexity: O(n) — each character still processed at most once.
- Space complexity: O(n) — stack storage unchanged.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the sample input `s = "()[]{}"` (expected output `True`).

## Dry Run

Input: s = "()[]{}"  

| Step | i | ch | Action (stack before)                     | Stack after | Comment |
|------|---|----|-------------------------------------------|-------------|---------|
| 0    | 0 | '('| push '('                                 | ['(']       | opening |
| 1    | 1 | ')'| pop '(' matches ')' → valid pair         | []          | matched |
| 2    | 2 | '['| push '['                                 | ['[']       | opening |
| 3    | 3 | ']'| pop '[' matches ']' → valid pair         | []          | matched |
| 4    | 4 | '{'| push '{'                                 | ['{']       | opening |
| 5    | 5 | '}'| pop '{' matches '}' → valid pair         | []          | matched |
| End  |   |    | stack empty → **valid**                    |             |         |

If we run the same trace on `s = "([)]"` we see a mismatch at step 3, causing an early `False`.  

--- 

*End of lecture.*
