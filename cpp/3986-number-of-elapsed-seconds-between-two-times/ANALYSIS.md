# Math & Geometry

## Video Solution

For more details about **Number of Elapsed Seconds Between Two Times**, watch the walkthrough at [https://www.youtube.com/watch?v=T0u5nwSA0w0](https://www.youtube.com/watch?v=T0u5nwSA0w0).

## Concept

To measure the elapsed time between two moments given as `HH:MM:SS`, we convert each timestamp into a total number of seconds since `00:00:00`.  
The conversion is straightforward:  

```
total_seconds = hours * 3600 + minutes * 60 + seconds
```

The answer is simply the difference `end_seconds - start_seconds`.  
(According to the problem statement, `endTime` is never earlier than `startTime`, so the result is non‑negative.)

## When to Use It

Use this conversion whenever you see:
- Timestamps in `HH:MM:SS` format.
- A need to compute a duration, difference, or interval between two times.
- Problems that ask for “seconds elapsed”, “time difference”, or similar.

## Template

```python
def to_seconds(t: str) -> int:
    """Convert 'HH:MM:SS' to total seconds."""
    h = int(t[0:2])
    m = int(t[3:5])
    s = int(t[6:8])
    return h * 3600 + m * 60 + s

def elapsed_seconds(startTime: str, endTime: str) -> int:
    return to_seconds(endTime) - to_seconds(startTime)
```

## LeetCode Problem Walkthrough

### Problem: 2744. Number of Elapsed Seconds Between Two Times

https://leetcode.com/problems/number-of-elapsed-seconds-between-two-times/

### Approach 1: Brute Force (direct substring extraction)

**Algorithm**
1. Extract the hour, minute, and second substrings from each time string using fixed positions.
2. Convert each substring to an integer.
3. Compute `total = hour*3600 + minute*60 + second`.
4. Return `end_total - start_total`.

**Implementation**

```python
class Solution:
    def secondsBetweenTimes(self, startTime: str, endTime: str) -> int:
        # helper to turn "HH:MM:SS" into seconds
        def to_sec(t: str) -> int:
            h = int(t[0:2])   # characters 0-1
            m = int(t[3:5])   # characters 3-4
            s = int(t[6:8])   # characters 6-7
            return h * 3600 + m * 60 + s

        return to_sec(endTime) - to_sec(startTime)
```

**Complexity Analysis**
- Time complexity: O(1) — we perform a constant amount of work (fixed‑length string slicing and arithmetic).
- Space complexity: O(1) — only a few integer variables are used.

### Approach 2: Using `split` (cleaner string parsing)

**Intuition**
Instead of manually slicing at known indices, we can split the string by the colon `:`. This makes the code more readable and less error‑prone if the format ever changes (while still O(1) because the string length is fixed).

**Algorithm**
1. Split `startTime` and `endTime` on `:` → `[HH, MM, SS]`.
2. Convert each part to `int`.
3. Compute total seconds with the same formula.
4. Return the difference.

**Implementation**

```python
class Solution:
    def secondsBetweenTimes(self, startTime: str, endTime: str) -> int:
        def to_sec(t: str) -> int:
            h, m, s = map(int, t.split(':'))
            return h * 3600 + m * 60 + s

        return to_sec(endTime) - to_sec(startTime)
```

**Complexity Analysis**
- Time complexity: O(1) — splitting a constant‑length string and mapping three integers.
- Space complexity: O(1) — only a few temporary variables.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Input: `startTime = "01:00:00"`, `endTime = "01:00:25"`

| Step | t          | h | m | s | total seconds |
|------|------------|---|---|---|---------------|
| 1    | startTime  | 1 | 0 | 0 | 1*3600 + 0*60 + 0 = 3600 |
| 2    | endTime    | 1 | 0 | 25| 1*3600 + 0*60 + 25 = 3625 |

Result = 3625 - 3600 = **25** seconds.

--- 

*This lecture follows the prescribed structure: concept, usage clues, reusable template, two approaches (brute‑force and split‑based) with intuition, algorithm, implementation, and complexity analysis, plus a concrete dry‑run.*