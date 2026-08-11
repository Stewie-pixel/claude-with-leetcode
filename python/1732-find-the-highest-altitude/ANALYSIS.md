# Prefix Sum

## Video Solution

For more details about **Find the Highest Altitude**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=AGJHLmVIrDk).

## Concept

Prefix sum is a technique where we compute the cumulative sum of an array up to each index.  
Imagine hiking a trail where each step tells you how much higher or lower you go compared to the previous point. To know your current elevation, you add up all the step changes from the start. The highest elevation you ever reach is simply the maximum of those running totals.

## When to Use It

Use prefix sum (or running total) when you see:
- Problems asking for cumulative values (e.g., total altitude, profit, score)  
- Queries about sub‑array sums or cumulative properties  
- A need to transform an array of differences into actual values  
- Constraints that allow a single linear pass (O(n) time)

## Template

```python
# Generic prefix‑sum / running‑total pattern
def prefix_sum(arr):
    # running total of elements seen so far
    running = 0
    # optional: store each prefix value if needed later
    prefixes = []
    for x in arr:
        running += x          # update the running total
        prefixes.append(running)  # record prefix sum at this index
    return prefixes
```

## LeetCode Problem Walkthrough

### Problem: 1732. Find the Highest Altitude

https://leetcode.com/problems/the-highest-altitude/

### Approach 1: Brute Force – Store All Altitudes

**Algorithm**
1. Start with altitude `0` and store it in a list `altitudes`.
2. Iterate through the `gain` array, each time adding the current gain to the last altitude and appending the result.
3. After the loop, return the maximum value in `altitudes`.

**Implementation**

```python
class Solution:
    def largestAltitude(self, gain: List[int]) -> int:
        # Store every altitude encountered
        altitudes = [0]          # starting point
        for g in gain:
            altitudes.append(altitudes[-1] + g)
        # The highest point is the max of all altitudes
        return max(altitudes)
```

**Complexity Analysis**
- Time complexity: O(n) — we traverse `gain` once.  
- Space complexity: O(n) — we keep an auxiliary list of size n+1.

### Approach 2: One‑Pass with O(1) Space (Optimized)

**Intuition**
We only need the *largest* altitude ever reached, not the whole history.  
While scanning the gains we can keep a running altitude and update a `max_seen` variable whenever the running altitude exceeds it. This eliminates the extra array.

**Algorithm**
1. Initialize `curr = 0` (current altitude) and `highest = 0`.
2. For each `g` in `gain`:
   - Update `curr += g`.
   - If `curr` is greater than `highest`, set `highest = curr`.
3. After processing all gains, return `highest`.

**Implementation**

```python
class Solution:
    def largestAltitude(self, gain: List[int]) -> int:
        curr = 0          # altitude at the current point
        highest = 0       # max altitude seen so far
        for g in gain:
            curr += g               # move to next point
            if curr > highest:
                highest = curr      # record new peak
        return highest
```

**Complexity Analysis**
- Time complexity: O(n) — single pass through `gain`.  
- Space complexity: O(1) — only two integer variables are used.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimized algorithm on Example 1: `gain = [-5,1,5,0,-7]`.

## Dry Run

Input: `gain = [-5, 1, 5, 0, -7]`

| Step | g   | curr (after adding g) | highest (max so far) | Action                              |
| ---- | --- | --------------------- | -------------------- | ----------------------------------- |
| 0    | —   | 0                     | 0                    | start                               |
| 1    | -5  | -5                    | 0                    | highest unchanged                  |
| 2    | 1   | -4                    | 0                    | highest unchanged                  |
| 3    | 5   | 1                     | 1                    | update highest to 1                |
| 4    | 0   | 1                     | 1                    | highest unchanged                  |
| 5    | -7  | -6                    | 1                    | highest unchanged                  |

Final `highest` = 1, which matches the expected output.