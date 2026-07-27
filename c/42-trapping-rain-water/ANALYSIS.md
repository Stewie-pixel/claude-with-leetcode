# Two Pointers

## Video Solution

For more details about **Trapping Rain Water**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=KFdHpOlz8hs).

## Concept

The two‑pointer technique processes an array from both ends simultaneously.  
We keep track of the highest bar seen so far from the left (`maxLeft`) and from the right (`maxRight`).  
At any step, the amount of water that can be trapped above the current bar is determined by the **smaller** of these two maxima, because water will overflow over the lower side.  
We then move the pointer that points to the smaller height inward, updating the corresponding max and accumulating water.

This eliminates the need for extra storage (as in the DP prefix‑max solution) while still giving us the information needed to compute water at each position.

## When to Use It

Use the two‑pointer pattern when you see:

- Problems that ask for a value derived from **both sides** of an array (e.g., container with most water, trapping rain water).
- A need to reduce **O(n) extra space** to **O(1)** while keeping linear time.
- Situations where the decision for the current index depends on the **maximum/minimum** encountered so far from each direction.
- The array can be processed incrementally from the ends toward the middle.

## Template

```python
def two_pointer_template(arr):
    """
    Generic two‑pointer skeleton for problems where we need
    information from both ends (e.g., max/min prefix/suffix).
    """
    left, right = 0, len(arr) - 1
    # state variables that capture information from each side
    left_state, right_state = 0, 0   # e.g., max seen so far
    result = 0

    while left < right:
        # Process the side with the smaller current value
        if arr[left] < arr[right]:
            # Update left state
            if arr[left] >= left_state:
                left_state = arr[left]
            else:
                # Use left_state to compute contribution
                result += left_state - arr[left]
            left += 1
        else:
            # Update right state
            if arr[right] >= right_state:
                right_state = arr[right]
            else:
                result += right_state - arr[right]
            right -= 1

    return result
```

Replace the state update/computation logic with the specifics of the problem.

---

## LeetCode Problem Walkthrough

### Problem: 42. Trapping Rain Water  
https://leetcode.com/problems/trapping-rain-water/

### Approach 1: Brute Force  

**Algorithm**  
For each position `i`, scan left to find the highest bar (`leftMax`) and scan right to find the highest bar (`rightMax`).  
Water above `i` = `max(0, min(leftMax, rightMax) - height[i])`.  
Sum this value for all indices.

**Implementation**

```python
def trap_brute_force(height):
    n = len(height)
    total = 0
    for i in range(n):
        left_max = max(height[:i+1]) if i > 0 else height[i]
        right_max = max(height[i:]) if i < n-1 else height[i]
        water = min(left_max, right_max) - height[i]
        if water > 0:
            total += water
    return total
```

**Complexity Analysis**  
- Time complexity: O(n²) — each index scans up to O(n) elements on both sides.  
- Space complexity: O(1) — only a few scalar variables.

---

### Approach 2: DP (Prefix / Suffix Max Arrays)  

**Intuition**  
The brute force repeats the same left‑max and right‑max scans for every index.  
If we pre‑compute the maximum height to the left of each position and the maximum height to the right, we can obtain the answer in a single linear pass.

**Algorithm**  
1. Build `maxLeft[i] = max(height[0..i])`.  
2. Build `maxRight[i] = max(height[i..n-1])`.  
3. For each `i`, water += `max(0, min(maxLeft[i], maxRight[i]) - height[i])`.

**Implementation**

```python
def trap_dp(height):
    n = len(height)
    if n == 0:
        return 0

    max_left = [0] * n
    max_right = [0] * n

    max_left[0] = height[0]
    for i in range(1, n):
        max_left[i] = max(max_left[i-1], height[i])

    max_right[n-1] = height[n-1]
    for i in range(n-2, -1, -1):
        max_right[i] = max(max_right[i+1], height[i])

    total = 0
    for i in range(n):
        total += min(max_left[i], max_right[i]) - height[i]
    return total
```

**Complexity Analysis**  
- Time complexity: O(n) — three linear passes.  
- Space complexity: O(n) — two auxiliary arrays of size n.

---

### Approach 3: Two Pointers (Optimal)  

**Intuition**  
Instead of storing the full prefix/suffix maxima, we notice that water at a position depends only on the **lower** of the two maxima.  
If `height[left] < height[right]`, then the left side’s maximum is the limiting factor (because the right side has at least `height[right]` which is ≥ the current left height).  
Thus we are examining).  
Therefore we can safely compute water using `maxLeft` and move the left pointer inward.  
The symmetric case holds when `height[right] ≤ height[left]`.  
This lets us update the answer on the fly with O(1) extra space.

**Algorithm**  
1. Initialize `left = 0`, `right = n-1`, `maxLeft = 0`, `maxRight = 0`, `ans = 0`.  
2. While `left < right`:  
   - If `height[left] < height[right]`:  
        * If `height[left] >= maxLeft`: update `maxLeft`.  
        * Else: add `maxLeft - height[left]` to `ans`.  
        * Increment `left`.  
   - Else (right side is smaller or equal):  
        * If `height[right] >= maxRight`: update `maxRight`.  
        * Else: add `maxRight - height[right]` to `ans`.  
        * Decrement `right`.  
3. Return `ans`.

**Implementation**

```python
def trap_two_pointers(height):
    """
    Two‑pointer O(n) time, O(1) space solution.
    """
    left, right = 0, len(height) - 1
    max_left, max_right = 0, 0
    water = 0

    while left < right:
        if height[left] < height[right]:
            if height[left] >= max_left:
                max_left = height[left]
            else:
                water += max_left - height[left]
            left += 1
        else:
            if height[right] >= max_right:
                max_right = height[right]
            else:
                water += max_right - height[right]
            right -= 1

    return water
```

**Complexity Analysis**  
- Time complexity: O(n) — each element is visited at most once.  
- Space complexity: O(1) — only a few integer variables.

---

### Provide a Visual Demonstration  

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example  

We dry‑run the two‑pointer algorithm on the classic example:

```
Input: height = [0,1,0,2,1,0,1,3,2,1,2,1]
```

| Step | left | right | height[left] | height[right] | maxLeft | maxRight | Water added | Total water |
|------|------|-------|--------------|---------------|---------|----------|-------------|-------------|
| 0    | 0    | 11    | 0            | 1             | 0       | 0        | 0           | 0           |
| 1    | 1    | 11    | 1            | 1             | 0→1     | 0        | 0           | 0           |
| 2    | 1    | 10    | 1            | 2             | 1       | 0→2      | 0           | 0           |
| 3    | 2    | 10    | 0            | 2             | 1       | 2        | 1‑0 = 1     | 1           |
| 4    | 3    | 10    | 2            | 2             | 1→2     | 2        | 0           | 1           |
| 5    | 3    | 9     | 2            | 1             | 2       | 2        | 2‑1 = 1     | 2           |
| 6    | 3    | 8     | 2            | 2             | 2       | 2        | 0           | 2           |
| 7    | 3    | 7     | 2            | 3             | 2       | 2→3      | 0           | 2           |
| 8    | 4    | 7     | 1            | 3             | 2       | 3        | 2‑1 = 1     | 3           |
| 9    | 5    | 7     | 0            | 3             | 2       | 3        | 2‑0 = 2     | 5           |
|10    | 6    | 7     | 1            | 3             | 2       | 3        | 2‑1 = 1     | 6           |
|11    | 7    | 7     | stop         |               |         |          |             | **6**       |

The algorithm accumulates **6** units of trapped water, matching the expected output.