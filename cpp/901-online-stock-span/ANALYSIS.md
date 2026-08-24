# Monotonic Stack

## Video Solution

For more details about **Online Stock Span**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=slYh0ZNEqSw)

## Concept

The *stock span* problem asks for, each day, how many consecutive previous days (including today) had a price less than or equal to today’s price.  
A naïve scan backwards would be O(n) per query.  
Observation: if we already know the span of a previous day whose price ≤ today’s price, we can **skip** all those days at once because they are guaranteed to be part of today’s span.  
By keeping a **monotonic decreasing stack** of (price, span) pairs, each price is pushed once and popped at most once, giving an amortized O(1) solution.

## When to Use It

Use a monotonic stack when you need to know “how many consecutive elements satisfy a condition looking backward/forward”, e.g.:

- Stock span, next greater element, previous smaller element  
- Problems that mention “consecutive days”, “until a larger/smaller value appears”, “window that stops when condition breaks”

**Clues in the statement:**  
*“maximum number of consecutive days … for which the stock price was less than or equal to the price of that day.”*  
→ Look for a condition that breaks when a larger price appears → monotonic stack.

## Template

```python
class StockSpanner:
    def __init__(self):
        # each element is a tuple (price, span_of_that_price)
        self.stack = []

    def next(self, price: int) -> int:
        span = 1
        # pop all previous prices that are <= current price
        while self.stack and self.stack[-1][0] <= price:
            span += self.stack[-1][1]   # add their spans
            self.stack.pop()
        self.stack.append((price, span))
        return span
```

## LeetCode Problem Walkthrough

### Problem: 901. Online Stock Span

https://leetcode.com/problems/online-stock-span/

### Approach 1: Brute Force

**Algorithm**  
Store every price in a list. For each new price, walk backward through the list counting how many consecutive prices are ≤ current price until a larger price is met.

**Implementation**

```python
class StockSpanner:
    def __init__(self):
        self.prices = []          # history of prices

    def next(self, price: int) -> int:
        self.prices.append(price)
        span = 1
        i = len(self.prices) - 2   # start checking previous day
        while i >= 0 and self.prices[i] <= price:
            span += 1
            i -= 1
        return span
```

**Complexity Analysis**

- Time complexity: **O(n)** per `next` call in the worst case (e.g., strictly increasing prices).  
- Space complexity: **O(n)** to store all prices.

### Approach 2: Monotonic Stack – Price‑Span Pairs (Optimized)

**Intuition**  
If a previous day’s price is ≤ today’s price, its entire span is also ≤ today’s price, so we can add that span in one step and discard that day from future consideration. Keeping the stack **monotonically decreasing in price** ensures each price is processed at most twice (push + pop).

**Algorithm**  

1. Initialize an empty stack.  
2. For each incoming `price`:  
   * Set `span = 1`.  
   * While stack not empty **and** `stack.top.price ≤ price`:  
        - Add `stack.top.span` to `span`.  
        - Pop the stack.  
   * Push `(price, span)` onto the stack.  
   * Return `span`.

**Implementation**

```python
class StockSpanner:
    def __init__(self):
        self.stack = []          # each element: (price, span)

    def next(self, price: int) -> int:
        span = 1
        while self.stack and self.stack[-1][0] <= price:
            span += self.stack[-1][1]
            self.stack.pop()
        self.stack.append((price, span))
        return span
```

**Complexity Analysis**

- Time complexity: **O(1) amortized** per call – each price is pushed once and popped at most once.  
- Space complexity: **O(n)** – worst case stack holds all prices (strictly decreasing sequence).

### Approach 3: Index‑Based Monotonic Stack (Alternative View)

**Intuition**  
Instead of storing the span with each price, keep a stack of **indices** where prices are in decreasing order. The span for the current index equals the distance to the previous greater element (index of last price > current). If none exists, the span reaches the start.

**Algorithm**  

1. Maintain `prices` list and `stack` of indices (prices[stack[i]] decreasing).  
2. For each new `price` at index `i`:  
   * While stack not empty **and** `prices[stack.top] ≤ price`: pop.  
   * If stack empty → `span = i + 1` (all previous days).  
   * Else → `span = i - stack.top`.  
   * Push `i` onto stack.  
   * Return `span`.

**Implementation**

```python
class StockSpanner:
    def __init__(self):
        self.prices = []          # raw price history
        self.stack = []           # indices of decreasing prices

    def next(self, price: int) -> int:
        self.prices.append(price)
        i = len(self.prices) - 1
        while self.stack and self.prices[self.stack[-1]] <= price:
            self.stack.pop()
        if not self.stack:
            span = i + 1
        else:
            span = i - self.stack[-1]
        self.stack.append(i)
        return span
```

**Complexity Analysis**

- Time complexity: **O(1) amortized** per call.  
- Space complexity: **O(n)** for the price list and the stack.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimized (price‑span) stack on the example from the statement:

```
Operations: [100, 80, 60, 70, 60, 75, 85]
```

| Step | price | stack before (price,span)                     | popped (price,span) | accumulated span | stack after push                | returned |
|------|-------|-----------------------------------------------|---------------------|------------------|--------------------------------|----------|
| 1    | 100   | []                                            | –                   | 1                | [(100,1)]                      | 1        |
| 2    | 80    | [(100,1)]                                     | – (100>80)          | 1                | [(100,1),(80,1)]               | 1        |
| 3    | 60    | [(100,1),(80,1)]                              | – (80>60)           | 1                | [(100,1),(80,1),(60,1)]        | 1        |
| 4    | 70    | [(100,1),(80,1),(60,1)]                       | (60,1)              | 1+1=2            | [(100,1),(80,1),(70,2)]        | 2        |
| 5    | 60    | [(100,1),(80,1),(70,2)]                       | – (70>60)           | 1                | [(100,1),(80,1),(70,2),(60,1)] | 1        |
| 6    | 75    | [(100,1),(80,1),(70,2),(60,1)]                | (60,1),(70,2)       | 1+1+2=4          | [(100,1),(80,1),(75,4)]        | 4        |
| 7    | 85    | [(100,1),(80,1),(75,4)]                       | (75,4),(80,1),(100,1)| 1+4+1+1=7? Wait recalc: start span=1, pop 75 (add4)→5, pop80(add1)→6, pop100(add1)→7? Actually after popping 100 stack empty, span becomes 1+4+1+1 =7? Let's recompute correctly: |
|      |       |                                               |                     |                  |                                |          |

Let's correct step 7:

- Start span = 1  
- Pop (75,4) → span = 1+4 = 5  
- Pop (80,1) → span = 5+1 = 6  
- Pop (100,1) → span = 6+1 = 7  

But expected answer is 6. Why? Because after popping 75 and 80 we should stop at 100? Actually condition is `price <= current price`. 100 ≤ 85? No, 100 > 85, so we should **not** pop 100. Oops, my earlier popped 100 incorrectly. Let's redo:

Step 7 (price=85):  
stack before = [(100,1),(80,1),(75,4)]  
- top (75,4): 75 ≤ 85 → pop, span=1+4=5  
- new top (80,1): 80 ≤ 85 → pop, span=5+1=6  
- new top (100,1): 100 ≤ 85? **False**, stop.  
Push (85,6).  
Return 6.

| 7    | 85    | [(100,1),(80,1),(75,4)]                       | (75,4),(80,1)       | 1+4+1=6          | [(100,1),(85,6)]               | 6        |

The table above shows the correct trace.

This dry run demonstrates how each price’s span is built by accumulating the spans of popped smaller-or-equal prices, yielding the expected outputs `[1,1,1,2,1,4,6]`.
