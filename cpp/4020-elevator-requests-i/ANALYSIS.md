# Arrays & Hashing

## Video Solution

For more details about **Elevator Requests I**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=kmErWhbsEeM).

## Concept

The elevator problem reduces to measuring how far the elevator travels between consecutive stops. Starting at floor 0, each request tells us the next floor to visit. The time needed to go from the current floor `cur` to the next request `r` is simply the absolute distance `|r - cur|`. Summing these distances for all requests yields the total time.

A real‑world analogy: imagine you are walking along a hallway numbered 0…n‑1. You receive a list of room numbers to visit in order. The total steps you take is the sum of the distances between each successive room (including the first step from the entrance at 0).

## When to Use It

Use this pattern when you see:
- A sequence of positions or values that must be processed in order.
- The cost of moving from one item to the next depends only on their numeric difference (e.g., distance on a line, time proportional to difference).
- No need to look ahead or back beyond the immediate previous item.

Typical problems: total travel distance, minimum moves to make array elements equal, cumulative height changes, etc.

## Template

```python
def process_sequence(values, start=0):
    """
    Generic template for summing absolute differences between consecutive items.
    """
    total = 0
    cur = start
    for v in values:
        total += abs(v - cur)   # cost to move from cur to v
        cur = v                 # update current position
    return total
```

## LeetCode Problem Walkthrough

### Problem: 2452. Elevator Requests I

https://leetcode.com/problems/elevator-requests-i/

### Approach 1: Brute Force (step‑by‑step simulation)

**Algorithm**
1. Initialize `time = 0` and `cur = 0` (elevator starts at floor 0).
2. For each requested floor `r`:
   - While `cur != r`:
        - Move one floor toward `r` (`cur += 1` if `r > cur` else `cur -= 1`).
        - Increment `time` by 1 for each floor moved.
   - After the loop, the elevator is at `r`; proceed to the next request.
3. Return `time`.

**Implementation**

```python
class Solution:
    def elevatorRequests(self, n: int, requests: List[int]) -> int:
        time = 0
        cur = 0
        for r in requests:
            # move one floor at a time toward the target
            while cur != r:
                if r > cur:
                    cur += 1
                else:
                    cur -= 1
                time += 1          # each floor costs one second
        return time
```

**Complexity Analysis**
- Time complexity: O(Σ|requests[i] - previous|) – in the worst case each request could be up to `n` floors away, giving O(m·n) where m = len(requests). With constraints (n,m ≤ 100) this is fine, but not optimal.
- Space complexity: O(1) – only a few integer variables.

### Approach 2: Optimized using absolute difference

**Intuition**
Instead of moving one floor at a time, we can directly add the distance between the current floor and the next request, because the elevator moves at a constant speed of one floor per second. The total time is therefore the sum of `|r_i - r_{i-1}|` with `r_{-1} = 0`.

**Algorithm**
1. Set `time = 0` and `cur = 0`.
2. For each request `r` in `requests`:
   - `time += abs(r - cur)`
   - `cur = r`
3. Return `time`.

**Implementation**

```python
class Solution:
    def elevatorRequests(self, n: int, requests: List[int]) -> int:
        time = 0
        cur = 0
        for r in requests:
            time += abs(r - cur)   # direct distance in seconds
            cur = r
        return time
```

**Complexity Analysis**
- Time complexity: O(m) where m = len(requests) – one pass through the list.
- Space complexity: O(1) – constant extra space.

### Approach 3: Pythonic one‑liner using `zip` and `sum`

**Intuition**
The same logic can be expressed concisely by pairing each request with its predecessor (starting with 0) and summing the absolute differences.

**Algorithm**
1. Create an iterator that yields `[0] + requests` as the previous floors and `requests` as the current floors.
2. For each pair `(prev, cur)` compute `abs(cur - prev)` and accumulate the sum.
3. Return the total.

**Implementation**

```python
class Solution:
    def elevatorRequests(self, n: int, requests: List[int]) -> int:
        # prepend the starting floor 0 to the requests list
        return sum(abs(cur - prev) for prev, cur in zip([0] + requests, requests))
```

**Complexity Analysis**
- Time complexity: O(m) – same linear pass, just hidden inside the generator expression.
- Space complexity: O(1) – the zip creates an iterator, not a full list.

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

### Dry Run

We trace the optimized solution on the example `n = 5, requests = [2,1,4,3]`.

| Step | cur (before) | r (request) | |r - cur| (added time) | time (cumulative) | cur (after) |
|------|--------------|-------------|----------------------|-------------------|-------------|
| 0    | 0            | 2           | 2                    | 2                 | 2           |
| 1    | 2            | 1           | 1                    | 3                 | 1           |
| 2    | 1            | 4           | 3                    | 6                 | 4           |
| 3    | 4            | 3           | 1                    | 7                 | 3           |

The final `time` is 7 seconds, matching the expected output.

--- 

*This lecture follows the standard DSA‑mentor format: concept, recognition clues, reusable template, and a step‑by‑step walkthrough with multiple approaches and complexity analysis.*