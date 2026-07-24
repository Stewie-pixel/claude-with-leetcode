# Two Pointers

## Video Solution

For more details about **Container With Most Water**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=UuiTKBwPgAo).

## Concept

The two‑pointer technique uses two indices that start at opposite ends of an array and move toward each other based on some condition. For the “Container With Most Water” problem, the area formed by two lines is limited by the shorter line (height) and the distance between them (width). By always moving the pointer at the shorter line, we never miss a potentially larger area while discarding configurations that cannot improve the answer.

## When to Use It

Use two pointers when you see:
- A problem that asks for a **maximum/minimum** value derived from pairs of elements.
- The array is **sorted** (or can be treated as sorted by some monotonic property).
- The decision to move left or right depends on a **comparison** (e.g., which side is smaller/larger).
- You need to achieve **O(n)** time instead of the naïve **O(n²)** pairwise check.

Typical patterns: “container with most water”, “trapping rain water”, “3‑sum”, “sorted array two‑sum”, “longest substring with at most K distinct characters”, etc.

## Template

```python
def two_pointer_template(arr):
    left, right = 0, len(arr) - 1          # start at both ends
    best = 0                               # or any appropriate initial value

    while left < right:
        # Compute current answer using arr[left] and arr[right]
        current = ...                       # problem‑specific formula
        best = max(best, current)          # or min, depending on goal

        # Move the pointer that is “worse” for the objective
        if arr[left] < arr[right]:         # example condition
            left += 1
        else:
            right -= 1

    return best
```

Replace the `...` with the problem‑specific calculation and adjust the pointer‑movement rule accordingly.

---

## LeetCode Problem Walkthrough

### Problem: 11. Container With Most Water

https://leetcode.com/problems/container-with-most-water/

### Approach 1: Brute Force

**Algorithm**  
Check every possible pair of lines, compute the water they can hold, and keep the maximum.

**Implementation**

```python
class Solution:
    def maxArea(self, height: List[int]) -> int:
        n = len(height)
        max_water = 0
        for i in range(n):
            for j in range(i + 1, n):
                # width is distance between lines, height is the shorter line
                water = min(height[i], height[j]) * (j - i)
                max_water = max(max_water, water)
        return max_water
```

**Complexity Analysis**

- Time complexity: O(n²) — we examine all `n·(n‑1)/2` pairs.  
- Space complexity: O(1) — only a few scalar variables are used.

### Approach 2: Two Pointers (Optimal)

**Intuition**  
The area is limited by the shorter line. If we move the pointer at the taller line inward, the width decreases and the height cannot increase beyond the current shorter line, so the area can only get smaller or stay the same. Conversely, moving the pointer at the shorter line gives a chance to find a taller line, which might increase the area despite the reduced width. Hence, we greedily advance the shorter side.

**Algorithm**

1. Initialize `left = 0`, `right = len(height) - 1`, `max_water = 0`.
2. While `left < right`:
   - Compute current area: `min(height[left], height[right]) * (right - left)`.
   - Update `max_water` if this area is larger.
   - If `height[left] < height[right]`, increment `left`; else decrement `right`.
3. Return `max_water`.

**Implementation**

```python
class Solution:
    def maxArea(self, height: List[int]) -> int:
        left, right = 0, len(height) - 1
        max_water = 0

        while left < right:
            # Current container's height is limited by the shorter line
            cur_height = min(height[left], height[right])
            cur_width = right - left
            max_water = max(max_water, cur_height * cur_width)

            # Move the pointer at the shorter line inward
            if height[left] < height[right]:
                left += 1
            else:
                right -= 1

        return max_water
```

**Complexity Analysis**

- Time complexity: O(n) — each element is visited at most once as either left or right pointer moves monotonically.
- Space complexity: O(1) — only constant extra space.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

We trace the algorithm on the example `height = [1,8,6,2,5,4,8,3,7]`.

| Step | left | height[left] | right | height[right] | width (right‑left) | current height | area | max_water | Action (move shorter side) |
|------|------|--------------|-------|---------------|--------------------|----------------|------|-----------|----------------------------|
| 0    | 0    | 1            | 8     | 7             | 8                  | 1              | 8    | 8         | left++ (1 < 7)             |
| 1    | 1    | 8            | 8     | 7             | 7                  | 7              | 49   | 49        | right-- (8 ≥ 7)            |
| 2    | 1    | 8            | 7     | 3             | 6                  | 3              | 18   | 49        | right-- (8 ≥ 3)            |
| 3    | 1    | 8            | 6     | 8             | 5                  | 8              | 40   | 49        | left++? (8 == 8 → move right) |
| 4    | 1    | 8            | 5     | 4             | 4                  | 4              | 16   | 49        | right-- (8 ≥ 4)            |
| 5    | 1    | 8            | 4     | 5             | 3                  | 5              | 15   | 49        | right-- (8 ≥ 5)            |
| 6    | 1    | 8            | 3     | 2             | 2                  | 2              | 4    | 49        | right-- (8 ≥ 2)            |
| 7    | 1    | 8            | 2     | 6             | 1                  | 6              | 6    | 49        | left++? (8 ≥ 6 → move right) |
| 8    | 1    | 8            | 1     | 8             | 0 (loop ends)      | –              | –    | 49        | loop stops                 |

The maximum area encountered is **49**, matching the expected answer.

---