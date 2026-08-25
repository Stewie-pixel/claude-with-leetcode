# Math & Geometry

## Video Solution

For more details about **Counter**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=oe3P3RkgvqU).

## Concept

The counter problem generates an arithmetic sequence starting at `n` with a common difference of 1. Each call to the returned function produces the next term in the sequence (n, n+1, n+2, ...). This is analogous to a ticket dispenser where the first ticket is #n, the next is #n+1, and so on. The core concept is maintaining state between function calls to track the current position in the sequence.

## When to Use It

Use this pattern when you need a function that:
- Returns a value that changes predictably on each invocation
- Requires persistent state between calls without using global variables
- Generates sequential values (like IDs, timestamps, or counters)
- Examples in DSA: 
  - Generating unique node IDs during tree/graph traversal
  - Tracking iteration count in custom iterators
  - Creating memoization keys in dynamic programming

## Template

```python
def createCounter(n):
    # State variable to track current value
    state = n
    
    def counter():
        # Modify state and return value
        nonlocal state  # Required to modify outer scope variable
        result = state
        state += 1
        return result
    
    return counter
```

## LeetCode Problem Walkthrough

### Problem: 2620. Counter

https://leetcode.com/problems/counter/

### Approach 1: Brute Force (History List)

**Algorithm**
Store every returned value in a list. The k-th call returns `n + k` (0-indexed), which we compute as `n + len(history)`. We append this value to history before returning it to maintain the sequence.

**Implementation**

```python
def createCounter(n):
    history = []  # Stores all previously returned values
    
    def counter():
        # Calculate next value: n + number of previous calls
        val = n + len(history)
        history.append(val)  # Record this value for future calls
        return val
    
    return counter
```

**Complexity Analysis**
- Time complexity: O(1) — List append and length access are O(1) amortized operations
- Space complexity: O(k) — Stores k values for k calls (linear space growth)

### Approach 2: Optimized Closure (Single Variable)

**Intuition**
We don't need the full history—only the current value to compute the next one. By tracking just the latest value with a single variable, we reduce space usage from O(k) to O(1) while maintaining O(1) time per call.

**Algorithm**
Initialize `current` to `n`. Each call returns `current` then increments it by 1. The `nonlocal` keyword allows the inner function to modify the outer function's variable.

**Implementation**

```python
def createCounter(n):
    current = n  # Tracks the next value to return
    
    def counter():
        nonlocal current  # Enable modification of outer variable
        result = current  # Capture current value before increment
        current += 1      # Prepare for next call
        return result
    
    return counter
```

**Complexity Analysis**
- Time complexity: O(1) — Constant-time arithmetic operations and assignment
- Space complexity: O(1) — Only one integer variable used regardless of call count

### Approach 3: Optimized Class-Based

**Intuition**
Encapsulate state in a class instance. The instance holds `current` as an attribute, and the method updates it on each call. This achieves identical complexity to the closure approach while being more familiar to OOP programmers.

**Algorithm**
Define a `Counter` class with:
- `__init__(self, n)`: Sets `self.current = n`
- `call(self)`: Returns `self.current` then increments it
Create an instance and return its `call` method (which is a callable bound to that instance).

**Implementation**

```python
def createCounter(n):
    class Counter:
        def __init__(self, n):
            self.current = n
        
        def call(self):
            result = self.current
            self.current += 1
            return result
    
    counter_instance = Counter(n)
    return counter_instance.call
```

**Complexity Analysis**
- Time complexity: O(1) — Attribute access and increment are constant time
- Space complexity: O(1) — Instance uses fixed space (one integer)

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

**Dry Run**  
Input: n = 10, calls = ["call", "call", "call"]  
*(Tracking Approach 2: Optimized Closure)*

| Step | current (before) | Action                          | Returned | current (after) |
|------|------------------|---------------------------------|----------|-----------------|
| 1    | 10               | result = 10; current → 11       | 10       | 11              |
| 2    | 11               | result = 11; current → 12       | 11       | 12              |
| 3    | 12               | result = 12; current → 13       | 12       | 13              |

**Output:** [10, 11, 12]  
*Matches expected behavior: first call returns n, each subsequent call returns previous + 1*