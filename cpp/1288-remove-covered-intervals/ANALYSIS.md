# Merge Intervals

## Video Solution

For more details about **Remove Covered Intervals**, watch the walkthrough at [https://www.youtube.com/watch?v=nhAsMabiVkM](https://www.youtube.com/watch?v=nhAsMabiVkM).

## Concept

When we say an interval `[a, b)` is *covered* by another `[c, d)`, it means `c ≤ a` and `b ≤ d`.  
Think of intervals as strips on a number line. If one strip starts at the same point (or earlier) and ends at the same point (or later), it completely hides the shorter strip underneath.  
After sorting intervals by their start point, any interval that ends **no later** than the maximum end we have seen so far is guaranteed to be covered by a previous interval.

## When to Use It

Use this pattern when you see:
- A list of intervals and you need to discard those that are “inside” another interval.
- Phrases like “covered by”, “contained within”, or “remove redundant intervals”.
- The relationship depends only on the ordering of start and end points.

## Template

```python
# 1. Sort intervals: start ascending, end descending (so longer intervals come first when starts tie)
intervals.sort(key=lambda x: (x[0], -x[1]))

# 2. Sweep through, keep track of the farthest right end seen
max_end = -1
remaining = 0
for l, r in intervals:
    if r > max_end:          # this interval extends beyond what we've covered → not covered
        remaining += 1
        max_end = r
    # else: it is covered → skip
return remaining
```

---

## LeetCode Problem Walkthrough

### Problem: 1288. Remove Covered Intervals
https://leetcode.com/problems/remove-covered-intervals/

### Approach 1: Brute Force

**Algorithm**
For each interval, check every other interval to see if it covers the current one. If none does, count it as remaining.

**Implementation**
```python
class Solution:
    def removeCoveredIntervals(self, intervals: List[List[int]]) -> int:
        n = len(intervals)
        remaining = 0
        for i in range(n):
            li, ri = intervals[i]
            covered = False
            for j in range(n):
                if i == j:
                    continue
                lj, rj = intervals[j]
                if lj <= li and ri <= rj:   # j covers i
                    covered = True
                    break
            if not covered:
                remaining += 1
        return remaining
```

**Complexity Analysis**
- Time complexity: O(n²) — we compare each pair of intervals.
- Space complexity: O(1) — only a few scalar variables are used.

### Approach 2: Sort + Greedy Sweep (Optimized)

**Intuition**
If we sort intervals by their start point ascending, any interval that could cover the current one must appear **before** it in the sorted order.  
When starts are equal, the longer interval (larger end) should come first so that it is seen before the shorter one it could cover.  
During a left‑to‑right sweep, we keep the maximum end seen so far; an interval whose end ≤ this maximum is covered.

**Algorithm**
1. Sort intervals by `(start, -end)`.
2. Initialize `max_end = -1` and `count = 0`.
3. For each interval `[l, r]`:
   - If `r > max_end`, it is not covered → increment `count` and update `max_end = r`.
   - Otherwise, skip (it is covered).
4. Return `count`.

**Implementation**
```python
class Solution:
    def removeCoveredIntervals(self, intervals: List[List[int]]) -> int:
        # 1. Sort by start ascending, end descending
        intervals.sort(key=lambda x: (x[0], -x[1]))
        
        max_end = -1
        remaining = 0
        for l, r in intervals:
            if r > max_end:          # extends farthest → not covered
                remaining += 1
                max_end = r
        return remaining
```

**Complexity Analysis**
- Time complexity: O(n log n) — dominated by the sort.
- Space complexity: O(1) — sorting is in‑place (ignoring Timsort's auxiliary space).

### Approach 3: Event Sweep (Alternative View)

**Intuition**
We can treat each interval as two events: a start (`+1`) and an end (`-1`).  
If we sweep from left to right and maintain a count of active intervals, an interval is *not* covered precisely when its start point sees a **new** maximum end that hasn’t been reached before.  
This reduces to the same logic as Approach 2 but makes the reasoning explicit with event ordering.

**Algorithm**
1. Create events: `(start, 0, end)` where the middle `0` distinguishes start from end.
2. Sort events by start ascending; for equal starts, process start before end.
3. Keep `furthest_end = -1`. When we encounter a start event, if its associated `end` > `furthest_end`, the interval is not covered → increment answer and update `furthest_end`.
4. End events are ignored for counting; they only affect ordering when starts tie.

**Implementation**
```python
class Solution:
    def removeCoveredIntervals(self, intervals: List[List[int]]) -> int:
        # Build start events only; we need the end value for comparison
        starts = [(l, r) for l, r in intervals]
        # Sort by start asc, end desc (same as Approach 2)
        starts.sort(key=lambda x: (x[0], -x[1]))
        
        furthest = -1
        ans = 0
        for l, r in starts:
            if r > furthest:
                ans += 1
                furthest = r
        return ans
```

**Complexity Analysis**
- Time complexity: O(n log n) — sorting the start events.
- Space complexity: O(n) — the list of start events (can be done in‑place if preferred).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

We trace Approach 2 on the example `intervals = [[1,4],[3,6],[2,8]]`.

1. **Sort** by `(start, -end)` → `[[1,4], [2,8], [3,6]]`.
2. Sweep:

| Step | Interval (l, r) | max_end before | r > max_end? | Action                              | max_end after | remaining |
|------|-----------------|----------------|--------------|-------------------------------------|---------------|-----------|
| 1    | (1, 4)          | -1             | Yes (4 > -1) | Count it, update max_end            | 4             | 1         |
| 2    | (2, 8)          | 4              | Yes (8 > 4)  | Count it, update max_end            | 8             | 2         |
| 3    | (3, 6)          | 8              | No (6 ≤ 8)   | Skip (covered by [2,8])             | 8             | 2         |

**Result:** 2 remaining intervals, matching the expected output.
