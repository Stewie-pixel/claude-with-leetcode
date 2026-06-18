# Two Pointers

## Video Solution

For more details about **Container With Most Water**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=UuiTKBwPgAo).

## Concept

The two‑pointer technique uses two indices that start at opposite ends of an array and move toward each other based on some condition.  
Think of trying to fill a bucket with water using two vertical walls. The amount of water you can hold is limited by the shorter wall and the distance between them. By starting with the widest possible container (first and last line) and then moving the pointer that points to the shorter line inward, we never miss a better answer because any container formed with the taller line and a narrower width would be limited by the same short line.

## When to Use It

Use two pointers when you see:
- Problems asking for a maximum/minimum value that depends on two indices (e.g., area, distance, sum).
- The objective can be expressed as `f(i, j)` where moving one index monotonically improves or does not worsen the answer.
- The input is sorted or you can arrange pointers at the extremes (start/end) and move them inward.
- Specific to this problem: maximize `width * min(height[i], height[j])`.

## Template

```python
left, right = 0, len(arr) - 1          # start at both ends
best = 0                               # store the best answer found

while left < right:
    # compute current answer using arr[left] and arr[right]
    current = ...                      # problem‑specific formula
    best = max(best, current)

    # decide which pointer to move
    if arr[left] < arr[right]:
        left += 1                      # move the pointer with the smaller value
    else:
        right -= 1

return best
```

## LeetCode Problem Walkthrough

### Problem: 11. Container With Most Water

https://leetcode.com/problems/container-with-most-water/

### Approach 1: Brute Force

**Algorithm**  
Check every possible pair of lines, compute the water they can hold, and keep the maximum.

**Implementation**

```python
def maxArea_brute(height):
    n = len(height)
    max_water = 0
    for i in range(n):
        for j in range(i + 1, n):
            # width is distance between lines, height is the shorter line
            water = (j - i) * min(height[i], height[j])
            max_water = max(max_water, water)
    return max_water
```

**Complexity Analysis**

- Time complexity: O(n²) — we examine all n·(n‑1)/2 pairs.  
- Space complexity: O(1) — only a few scalar variables are used.

### Approach 2: Two Pointers (Optimal)

**Intuition**  
The area is limited by the shorter line. Starting with the widest container (indices 0 and n‑1) gives the largest possible width. If we move the pointer at the taller line inward, the width can only decrease and the height is still bounded by the same short line, so the area cannot increase. Therefore, we safely discard the shorter line by moving that pointer inward, hoping to find a taller line that compensates for the reduced width.

**Algorithm**  
1. Initialize `left = 0`, `right = len(height) - 1`, `max_water = 0`.  
2. While `left < right`:  
   - Compute current area = `(right - left) * min(height[left], height[right])`.  
   - Update `max_water` if current area is larger.  
   - If `height[left] < height[right]`, increment `left`; else decrement `right`.  
3. Return `max_water`.

**Implementation**

```python
def maxArea(height):
    """
    Returns the maximum area of water that can be contained.
    :param height: List[int] - heights of vertical lines
    :return: int - maximum water area
    """
    left, right = 0, len(height) - 1
    max_water = 0

    while left < right:
        # width between the two lines
        width = right - left
        # container height limited by the shorter line
        if height[left] < height[right]:
            current_height = height[left]
            area = width * current_height
            max_water = max(max_water, area)
            left += 1               # move the shorter line
        else:
            current_height = height[right]
            area = width * current_height
            max_water = max(max_water, area)
            right -= 1              # move the shorter line

    return max_water
```

**Complexity Analysis**

- Time complexity: O(n) — each element is visited at most once as one of the pointers moves inward.  
- Space complexity: O(1) — only a constant number of variables are used.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Input: `height = [1,8,6,2,5,4,8,3,7]`

```
| Step | left | right | height[left] | height[right] | width | min_height | area | max_water | Action               |
|------|------|-------|--------------|---------------|-------|------------|------|-----------|----------------------|
| 1    | 0    | 8     | 1            | 7             | 8     | 1          | 8    | 8         | height[left] < height[right] → left++ |
| 2    | 1    | 8     | 8            | 7             | 7     | 7          | 49   | 49        | height[left] >= height[right] → right-- |
| 3    | 1    | 7     | 8            | 3             | 6     | 3          | 18   | 49        | height[left] >= height[right] → right-- |
| 4    | 1    | 6     | 8            | 8             | 5     | 8          | 40   | 49        | height[left] >= height[right] → right-- |
| 5    | 1    | 5     | 8            | 4             | 4     | 4          | 16   | 49        | height[left] >= height[right] → right-- |
| 6    | 1    | 4     | 8            | 5             | 3     | 3          | 9    | 49        | height[left] >= height[right] → right-- |
| 7    | 1    | 3     | 8            | 2             | 2     | 2          | 4    | 49        | height[left] >= height[right] → right-- |
| 8    | 1    | 2     | 8            | 6             | 1     | 1          | 1    | 49        | height[left] >= height[right] → right-- |
| loop ends (left >= right)                                                               |
```

The maximum area found is **49**, matching the expected output.