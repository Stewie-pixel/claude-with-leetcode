# Merge Intervals

## Video Solution

For more details about **Remove Covered Intervals**, watch the walkthrough at [https://www.youtube.com/watch?v=nhAsMabiVkM](https://www.youtube.com/watch?v=nhAsMabiVkM)

## Concept

When we need to remove intervals that are completely covered by another interval, we can sort the intervals in a way that makes the covering relationship easy to detect with a single linear scan.

* Sort by **start** ascending.  
* If two intervals have the same start, sort by **end** **descending** – the longer interval comes first, so any later interval with the same start is guaranteed to be covered.
* After sorting, we walk through the list keeping track of the **largest end** seen so far.  
  * If the current interval’s end is larger than that maximum, it is **not** covered → we count it and update the maximum.  
  * Otherwise its end ≤ current max → it is covered by a previous interval and can be ignored.

This reduces the problem to O(n log n) time for sorting plus O(n) for the scan.

## When to Use It

Use the “sort‑and‑sweep” interval pattern when you see:

* A list of intervals and a relationship defined by one interval containing another (covered, overlapping, merging, etc.).
* Questions asking for the number of intervals that survive after discarding those that are contained.
* Phrases like “remove covered intervals”, “merge overlapping intervals”, “minimum number of arrows to burst balloons”, etc.

## Template

```python
# Generic interval sweep after sorting
def process_intervals(intervals, key):
    # 1. Sort according to the problem‑specific rule
    intervals.sort(key=key)

    # 2. Sweep line – keep track of a running value (e.g., max end, min start)
    running = None          # depends on the problem
    result = 0              # or whatever we are counting

    for interval in intervals:
        # 3. Decide whether the current interval contributes to the answer
        if condition(interval, running):
            result += 1
            running = update(running, interval)

    return result
```

## LeetCode Problem Walkthrough

### Problem: 1288. Remove Covered Intervals

https://leetcode.com/problems/remove-covered-intervals/

### Approach 1: Brute Force

**Algorithm**

For each interval `i`, check every other interval `j`.  
If there exists a `j` such that `j.start ≤ i.start` **and** `i.end ≤ j.end`, then `i` is covered and can be discarded.  
Count the intervals that are **not** covered by any other interval.

**Implementation**

```python
class Solution:
    def removeCoveredIntervals(self, intervals: List[List[int]]) -> int:
        n = len(intervals)
        uncovered = 0

        for i in range(n):
            covered = False
            for j in range(n):
                if i == j:
                    continue
                # does j cover i ?
                if intervals[j][0] <= intervals[i][0] and intervals[i][1] <= intervals[j][1]:
                    covered = True
                    break
            if not covered:
                uncovered += 1
        return uncovered
```

**Complexity Analysis**

- Time complexity: O(n²) — we compare every pair of intervals.  
- Space complexity: O(1) — only a few integer variables are used.

### Approach 2: Sort by Start, then End Descending (Greedy Sweep)

**Intuition**

If we sort intervals by their start coordinate, any interval that could cover the current one must appear **before** it in the list (its start is ≤ current start).  
When starts are equal, placing the longer interval first guarantees that a later interval with the same start is automatically covered.  
During a left‑to‑right sweep, the furthest right end we have seen so far tells us whether the current interval extends beyond all previous ones; if not, it is covered.

**Algorithm**

1. Sort `intervals` by `(start, -end)`.  
2. Initialise `count = 0` and `max_end = -1`.  
3. Iterate through the sorted list:  
   * If `end > max_end`, the interval is not covered → increment `count` and set `max_end = end`.  
   * Otherwise, skip it (it is covered).  
4. Return `count`.

**Implementation**

```python
class Solution:
    def removeCoveredIntervals(self, intervals: List[List[int]]) -> int:
        # 1. Sort by start ascending, end descending
        intervals.sort(key=lambda x: (x[0], -x[1]))

        count = 0
        max_end = -1

        for start, end in intervals:
            # 2. If this interval reaches farther than any seen before, it survives
            if end > max_end:
                count += 1
                max_end = end
            # else: covered by a previous interval → ignore

        return count
```

**Complexity Analysis**

- Time complexity: O(n log n) — dominated by the sorting step.  
- Space complexity: O(1) — sorting is in‑place; only a few variables are used.

### Approach 3: Alternative Sweep (Start Ascending, End Ascending) + Overlap Check

**Intuition**

Another way to think about the problem is to count how many intervals **add** new coverage beyond what we have already seen.  
If we sort by start ascending **and** end ascending, we can keep track of the **minimum** end of the current “active” group.  
When we encounter an interval whose start is greater than the previous interval’s end, we know the previous group is finished and we can start counting a new group.  
Within a group, only the interval with the largest end survives; all others are covered.  
This approach arrives at the same O(n log n) bound but may be easier to reason about for some.

**Algorithm**

1. Sort intervals by `(start, end)`.  
2. Initialise `ans = 0` and `prev_end = -1`.  
3. For each `(s, e)` in sorted list:  
   * If `e > prev_end`: this interval adds new coverage → `ans += 1`.  
   * Update `prev_end = max(prev_end, e)` (the farthest right we have covered so far).  
4. Return `ans`.

**Implementation**

```python
class Solution:
    def removeCoveredIntervals(self, intervals: List[List[int]]) -> int:
        intervals.sort(key=lambda x: (x[0], x[1]))
        ans = 0
        prev_end = -1

        for s, e in intervals:
            if e > prev_end:
                ans += 1
            prev_end = max(prev_end, e)
        return ans
```

**Complexity Analysis**

- Time complexity: O(n log n) — sorting dominates.  
- Space complexity: O(1) — only constant extra space.

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the greedy sweep (Approach 2) on the first example.

**Input:** `[[1,4],[3,6],[2,8]]`

### Step‑by‑step

| Step | Interval (start, end) | max_end before | Condition `end > max_end`? | Action                              | max_end after |
|------|-----------------------|----------------|----------------------------|-------------------------------------|---------------|
| 1    | (1, 4)                | -1             | **Yes** (4 > -1)           → count = 1               | 4                                   |
| 2    | (2, 8)                | 4              | **Yes** (8 > 4)            → count = 2               | 8                                   |
| 3    | (3, 6)                | 8              | **No**  (6 ≤ 8)            → skip (covered)          | 8                                   |

**Result:** `count = 2`, which matches the expected output.

---

**Summary**

* The brute‑force method checks every pair – simple but O(n²).  
* Sorting by start ascending and end descending lets us sweep once, keeping only the furthest right end seen so far – O(n log n) time, O(1) space.  
* An equivalent sweep sorting by both coordinates ascending works as well, reinforcing the pattern that after ordering, a single running aggregate (here the maximum end) decides whether an interval contributes to the answer.  

Commit this lecture as `lectures/2025-09-26-merge-intervals.md`.