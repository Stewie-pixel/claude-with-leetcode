# Counter

## Video Solution

For more details about **Counter**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=yEGQQAG5V68).

## Concept

A **closure** is a function that “remembers” the environment in which it was created.  
In this problem we need a function that returns another function. The inner function keeps access to a variable (`count`) from its outer scope, so each call can read and update that variable without using globals or passing it explicitly.

Real‑world analogy: think of a **ticket counter** at a booth. The clerk (the outer function) sets the starting ticket number once. Every time a customer asks for a ticket (calling the inner function), the clerk hands out the current number and then increments the stored counter for the next customer.

## When to Use It

Use a closure when you need to:

* Keep private state that persists across multiple invocations of a function.
* Avoid exposing state as a global variable or as an object attribute when a simple callable is sufficient.
* Encapsulate related behavior (e.g., counters, memoizers, configuration factories).

Common LeetCode clues: “return a function”, “each call should … based on previous call”, “maintain state between calls”.

## Template

```python
def make_counter(start: int):
    """
    Returns a function that, when called, yields the current count
    and then increments it by 1.
    """
    count = start                     # captured variable (nonlocal)

    def counter():
        nonlocal count                # allow modification of the outer variable
        result = count
        count += 1
        return result

    return counter
```

---

## LeetCode Problem Walkthrough

### Problem: 2620. Counter

https://leetcode.com/problems/counter/

### Approach 1: Class‑Based Counter (Brute Force)

**Algorithm**
1. Define a class `Counter` with an initializer that stores the starting value.
2. Provide a method (or make the instance callable via `__call__`) that returns the current value and then increments it.
3. The factory function returns an instance of this class.

**Implementation**

```python
class Counter:
    def __init__(self, start: int):
        self.value = start

    def __call__(self) -> int:
        current = self.value
        self.value += 1
        return current


def createCounter(n: int) -> callable:
    return Counter(n)
```

**Complexity Analysis**

- Time complexity: O(1) — each call does constant work.
- Space complexity: O(1) — only one integer is stored.

---

### Approach 2: Closure with `nonlocal` (Intermediate Optimization)

**Intuition**
Instead of defining a whole class, we can capture the count in a nested function and use the `nonlocal` keyword to modify it. This removes the overhead of a class while keeping the same O(1) behavior.

**Algorithm**
1. In `createCounter`, initialize a local variable `count = n`.
2. Define an inner function that:
   * reads `count`,
   * increments it,
   * returns the previous value.
3. Return the inner function.

**Implementation**

```python
def createCounter(n: int):
    count = n                     # captured by the inner function

    def counter():
        nonlocal count            # allow us to rebind `count`
        result = count
        count += 1
        return result

    return counter
```

**Complexity Analysis**

- Time complexity: O(1) — constant work per call.
- Space complexity: O(1) — one integer stored in the closure.

---

### Approach 3: Iterator‑Based Counter (Most Optimal)

**Intuition**
Python’s built‑in `itertools.count` already implements an infinite counter that yields successive integers starting from a given value. Wrapping it in a lambda gives us the desired callable with zero extra code.

**Algorithm**
1. Import `itertools.count`.
2. Return a lambda that calls `next()` on the iterator created with `start=n`.

**Implementation**

```python
from itertools import count

def createCounter(n: int):
    c = count(n)                  # iterator producing n, n+1, n+2, …
    return lambda: next(c)
```

**Complexity Analysis**

- Time complexity: O(1) — `next` on an iterator is constant time.
- Space complexity: O(1) — the iterator holds only the current value.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the closure version with `n = 10` and three calls.

## Dry Run

Input: `n = 10`, calls = ["call","call","call"]

| Step | Action                               | `count` before | Returned | `count` after |
|------|--------------------------------------|----------------|----------|---------------|
| 0    | `counter = createCounter(10)`        | 10             | —        | 10            |
| 1    | `counter()`                          | 10             | 10       | 11            |
| 2    | `counter()`                          | 11             | 11       | 12            |
| 3    | `counter()`                          | 12             | 12       | 13            |

The table shows how the captured variable persists and is updated after each invocation, producing the sequence `[10, 11, 12]`.

--- 

**Summary**

* All three approaches run in O(1) time and O(1) space.
* The class version is explicit and easy to extend.
* The closure version is lightweight and idiomatic for simple stateful functions.
* The iterator version leverages the standard library for the most concise solution. 

Pick the style that best fits the constraints of your codebase; for interview problems, the closure version is often the expected answer.
