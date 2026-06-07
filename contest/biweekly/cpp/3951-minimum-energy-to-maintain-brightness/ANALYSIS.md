# Merge Intervals

## Concept
Merging intervals is the process of combining overlapping or adjacent intervals into a minimal set of disjoint intervals that cover exactly the same points. Once merged, the total covered length is simply the sum of the lengths of the disjoint intervals.

## When to Use It
- You need the **total length** (or count) of points covered by a set of intervals.
- The problem involves **scheduling**, **resource allocation**, or **coverage** where overlapping periods should not be double‑counted.
- You see clues like “total time”, “union length”, “at least one interval covers”, or “merge overlapping intervals”.

## Template
```python
def merge_intervals(intervals):
    """
    Returns a list of merged, non‑overlapping intervals sorted by start.
    Each interval is a list/tuple [start, end] (inclusive).
    """
    if not intervals:
        return []

    # 1. Sort by start coordinate
    intervals.sort(key=lambda x: x[0])

    merged = []
    cur_start, cur_end = intervals[0]

    for start, end in intervals[1:]:
        # 2. If current interval overlaps or touches the previous one, extend it
        if start <= cur_end:                 # overlap (inclusive)
            cur_end = max(cur_end, end)
        else:
            # 3. No overlap – push the previous interval and start a new one
            merged.append([cur_start, cur_end])
            cur_start, cur_end = start, end

    # 4. Append the last interval
    merged.append([cur_start, cur_end])
    return merged

def covered_length(intervals):
    """Total number of integer points covered by the union of intervals."""
    total = 0
    for s, e in merge_intervals(intervals):
        total += e - s + 1          # inclusive length
    return total
```

## LeetCode Problem Walkthrough

### Problem: Minimum Energy to Maintain Brightness
https://leetcode.com/problems/minimum-energy-to-maintain-brightness/

---

### Approach 1: Brute Force (simulate each time unit)

**Algorithm**  
1. The minimum number of bulbs that must be on at any covered time unit is  
   `bulbs_needed = ceil(brightness / 3)` because each bulb can illuminate at most 3 distinct positions.  
2. Iterate over every integer time `t` from the smallest start to the largest end among all intervals.  
3. For each `t`, check whether it lies inside at least one interval (by scanning all intervals).  
4. If yes, add `bulbs_needed` to the answer; otherwise add 0.  

**Implementation**
```python
def minEnergy_brute(n, brightness, intervals):
    bulbs_needed = (brightness + 2) // 3          # ceil division
    if not intervals:
        return 0
    min_t = min(s for s, _ in intervals)
    max_t = max(e for _, e in intervals)
    energy = 0
    for t in range(min_t, max_t + 1):
        covered = any(s <= t <= e for s, e in intervals)
        if covered:
            energy += bulbs_needed
    return energy
```

**Complexity Analysis**  
- Time complexity: O((max_t - min_t + 1) * m) – in the worst case the time range can be up to 10⁹, making this approach infeasible.  
- Space complexity: O(1) extra.

---

### Approach 2: Merge Intervals (optimal)

**Intuition**  
The required number of bulbs is constant for every time unit that lies inside *any* interval. Therefore  
`total_energy = bulbs_needed × (total length of the union of all intervals)`.  
We only need to compute how many distinct time units are covered by at least one interval.

**Algorithm**  
1. Compute `bulbs_needed = ceil(brightness / 3)`.  
2. Merge the intervals to obtain a list of disjoint intervals.  
3. Sum the lengths (`end - start + 1`) of the merged intervals → `covered_time`.  
4. Return `bulbs_needed * covered_time`.

**Implementation**
```python
class Solution:
    def minEnergy(self, n: int, brightness: int, intervals: List[List[int]]) -> int:
        bulbs_needed = (brightness + 2) // 3          # ceil(brightness/3)

        if not intervals:
            return 0

        # 1. Sort by start
        intervals.sort(key=lambda x: x[0])

        covered_time = 0
        cur_start, cur_end = intervals[0]

        for s, e in intervals[1:]:
            if s <= cur_end:               # overlap (inclusive)
                cur_end = max(cur_end, e)
            else:
                # finalize previous block
                covered_time += cur_end - cur_start + 1
                cur_start, cur_end = s, e

        # add the last block
        covered_time += cur_end - cur_start + 1

        return bulbs_needed * covered_time
```

**Complexity Analysis**  
- Time complexity: O(m log m) – dominated by sorting (`m = len(intervals)`).  
- Space complexity: O(1) extra (apart from the input).

---

### Approach 3: Line Sweep with Events (alternative view)

**Intuition**  
Instead of explicitly merging, we can treat each interval as a “start” event (+1) and an “end+1” event (‑1). Sweeping through the sorted event points tells us when the coverage count is >0, allowing us to accumulate the covered length directly.

**Algorithm**  
1. Compute `bulbs_needed = ceil(brightness / 3)`.  
2. Build a list of events: `(start, +1)` and `(end + 1, -1)`.  
3. Sort events by coordinate.  
4. Sweep: keep a running `active` counter. Whenever `active` > 0, the segment from the previous coordinate to the current one is covered; add its length to `covered_time`.  
5. Return `bulbs_needed * covered_time`.

**Implementation**
```python
class Solution:
    def minEnergy(self, n: int, brightness: int, intervals: List[List[int]]) -> int:
        bulbs_needed = (brightness + 2) // 3
        events = []
        for s, e in intervals:
            events.append((s, 1))
            events.append((e + 1, -1))   # end is inclusive
        events.sort()

        covered_time = 0
        active = 0
        prev = None
        for point, delta in events:
            if prev is not None and active > 0:
                covered_time += point - prev
            active += delta
            prev = point

        return bulbs_needed * covered_time
```

**Complexity Analysis**  
- Time complexity: O(m log m) for sorting the 2m events.  
- Space complexity: O(m) for the event list.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

**Dry run on Example 1**  
`n = 5, brightness = 5, intervals = [[6,12]]`

1. `bulbs_needed = ceil(5/3) = 2`.  
2. Only one interval → merged list = `[[6,12]]`.  
3. Covered time = `12 - 6 + 1 = 7`.  
4. Energy = `2 * 7 = 14`.

| Step | cur_start | cur_end | Action                              | covered_time so far |
|------|-----------|---------|-------------------------------------|---------------------|
| init | 6         | 12      | start with first interval           | 0                   |
| end  | –         | –       | add length `12-6+1 = 7`             | 7                   |

Result: **14** (matches expected output).