# Merge Intervals

## Video Solution

For more details about **Minimum Energy to Maintain Brightness**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=5dg_gAym75I)

## Concept

Merging intervals is a classic sweep‑line technique.  
When you have a list of ranges `[start, end]` and you need to know the total length covered by the union of those ranges, you sort them by start coordinate and then walk through, extending the current interval whenever the next one overlaps (or touches). Whenever a gap appears, you add the length of the finished interval to the answer and start a new current interval.

The same idea appears whenever a problem asks for:

* total time during which at least one condition holds,
* the length of the union of segments,
* or the minimum number of points needed to intersect all intervals.

## When to Use It

Use interval merging when you see any of the following clues in the statement:

* “At each time unit … must be satisfied” together with a list of time intervals.
* “Return the total length covered by the given segments.”
* “Find the minimum number of … so that every interval contains at least one …”
* Overlapping ranges, meeting times, or busy‑schedule problems.

## Template

```python
def merge_intervals(intervals):
    """
    intervals: List[List[int]]  # each interval is [start, end] (inclusive)
    returns:   total length of the union of all intervals
    """
    if not intervals:
        return 0

    # 1. Sort by start coordinate
    intervals.sort(key=lambda x: x[0])

    total = 0
    cur_start, cur_end = intervals[0]

    for s, e in intervals[1:]:
        # 2. If overlapping or directly adjacent, extend current interval
        if s <= cur_end + 1:               # +1 makes touching intervals merge as well
            cur_end = max(cur_end, e)
        else:
            # 3. No overlap – finalize current interval
            total += cur_end - cur_start + 1
            cur_start, cur_end = s, e

    # Add the last interval
    total += cur_end - cur_start + 1
    return total
```

*Note*: Adjust the `+1` depending on whether intervals are inclusive/exclusive and whether touching intervals should be merged.

## LeetCode Problem Walkthrough

### Problem: 2742. Minimum Energy to Maintain Brightness  
https://leetcode.com/problems/minimum-energy-to-maintain-brightness/

---

### Approach 1: Brute Force (Check every time unit, try every bulb count)

**Algorithm**  
1. Build the set of all time units that appear in at least one interval (the union).  
2. For each possible number of bulbs `k` from 1 to `n`:
   * Greedily place bulbs to cover as many positions as possible (each bulb lights up to three consecutive cells).  
   * If with `k` bulbs we can illuminate at least `brightness` positions, then `k` is sufficient.  
   * The first `k` that works is the minimum bulbs needed.  
3. Energy = `min_bulbs * total_active_time`.

Because we may have to try up to `n` bulb counts and each check scans `n` positions, this is far too slow for the given constraints (`n` up to 10⁶, intervals up to 10⁵).

**Implementation**

```python
class Solution:
    def minEnergy(self, n: int, brightness: int, intervals: List[List[int]]) -> int:
        # ---- build the set of active times (could be huge, so we avoid this in practice) ----
        active = set()
        for s, e in intervals:
            active.update(range(s, e + 1))
        total_time = len(active)

        # ---- brute force search for minimal bulbs ----
        for bulbs in range(1, n + 1):
            # greedy placement: put a bulb, skip its illuminated range, repeat
            illuminated = 0
            i = 0
            while i < n and illuminated < brightness:
                illuminated += 3          # a bulb lights i, i+1, i+2 (clipped at ends)
                i += 3                    # move past the illuminated segment
            if illuminated >= brightness:
                return bulbs * total_time
        return 0  # never reached
```

**Complexity Analysis**  
- Time: O(|active| + n²) in the worst case (building the set can be O(10⁹) and the double loop is O(n²)).  
- Space: O(|active|) to store the active time units (could be huge).  

This approach is infeasible for the limits.

---

### Approach 2: Greedy Insight – Minimum bulbs = ceil(brightness / 3)

**Intuition**  
A single bulb can illuminate at most three distinct positions (itself and its two neighbours).  
If we place bulbs so that their illuminated ranges do **not** overlap, each bulb contributes exactly three newly lit positions.  
Overlap can only reduce the number of newly lit positions, therefore the *fewest* bulbs needed to reach a required illumination `brightness` is the ceiling of `brightness / 3`.  
Once we know how many bulbs must be on at any moment, the problem reduces to: **how many time units require those bulbs to be on?**  
That is simply the total length of the union of all given intervals, because outside the intervals we may keep all bulbs off.

**Algorithm**  
1. Compute `min_bulbs = (brightness + 2) // 3` (integer ceiling).  
2. Merge the intervals to obtain the total covered time `total_time`.  
3. Answer = `min_bulbs * total_time`.

**Implementation**

```python
class Solution:
    def minEnergy(self, n: int, brightness: int, intervals: List[List[int]]) -> int:
        # 1️⃣ Minimum number of bulbs needed at any active moment
        min_bulbs = (brightness + 2) // 3   # ceil(brightness/3)

        # 2️⃣ Merge intervals to get total active time
        if not intervals:
            return 0
        intervals.sort(key=lambda x: x[0])
        total_time = 0
        cur_start, cur_end = intervals[0]

        for s, e in intervals[1:]:
            if s <= cur_end + 1:                 # overlap or touch → merge
                cur_end = max(cur_end, e)
            else:
                total_time += cur_end - cur_start + 1
                cur_start, cur_end = s, e
        total_time += cur_end - cur_start + 1    # last interval

        # 3️⃣ Energy = bulbs × time
        return min_bulbs * total_time
```

**Complexity Analysis**  
- Time: `O(m log m)` where `m = len(intervals)` (sorting dominates).  
- Space: `O(1)` extra besides the input (in‑place sort).  

This meets the constraints easily (`m ≤ 10⁵`).

---

### Approach 3: One‑Pass Merge Without Extra Variables (Optional Variant)

**Intuition**  
The logic of Approach 2 can be tightened slightly by reusing the same variables and avoiding an extra `if` for the final interval by appending a sentinel interval `[∞, ∞]` (or by handling after the loop). The asymptotic complexity stays the same; this version is just a stylistic alternative.

**Algorithm**  
Same as Approach 2, but after sorting we iterate through intervals and accumulate length directly, resetting the current interval whenever a gap is found.

**Implementation**

```python
class Solution:
    def minEnergy(self, n: int, brightness: int, intervals: List[List[int]]) -> int:
        min_bulbs = (brightness + 2) // 3
        if not intervals:
            return 0

        intervals.sort(key=lambda x: x[0])
        total = 0
        start, end = intervals[0]

        for s, e in intervals[1:]:
            if s <= end + 1:          # overlap / adjacency
                end = max(end, e)
            else:
                total += end - start + 1
                start, end = s, e
        total += end - start + 1

        return min_bulbs * total
```

**Complexity Analysis**  
- Time: `O(m log m)`  
- Space: `O(1)`  

---

### Dry Run (Impact: HIGH | Category: explanation | Tags: dry-run, trace, example)

**Input**: `n = 5, brightness = 5, intervals = [[6,12]]`

1. `min_bulbs = (5 + 2) // 3 = 7 // 3 = 2`  
2. Merge intervals: only one interval → `total_time = 12 - 6 + 1 = 7`  
3. Energy = `2 * 7 = 14`

| Step | cur_start | cur_end | Action                              |
|------|-----------|---------|-------------------------------------|
| init | 6         | 12      | first interval sets current         |
| end  | –         | –       | add length 7 → total_time = 7       |
| result | –       | –       | answer = 2 bulbs * 7 time = 14      |

The dry run matches the expected output.

--- 

**Summary**  
* Identify that each bulb lights at most three cells → `min_bulbs = ceil(brightness/3)`.  
* Reduce the problem to finding the total length of the union of active intervals via the merge‑intervals pattern.  
* Multiply the two quantities to obtain the minimum total energy.  

This approach runs in `O(m log m)` time and `O(1)` extra space, fitting comfortably within the limits.