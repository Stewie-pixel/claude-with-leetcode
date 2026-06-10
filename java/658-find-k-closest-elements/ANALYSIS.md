# Binary Search

## Video Solution

For more details about **Find K Closest Elements**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=o-YDQzHoaKM).

## Concept

Binary search can be used to find the **left bound** of a length‑`k` subarray that contains the `k` closest elements to `x`.  
Because the input array is sorted, if we know where the window starts, the window itself is determined (`[left, left+k-1]`).  
We compare the distances of the two candidates that could be the next element to include (`arr[mid]` vs `arr[mid+k]`) and move the search window toward the side with the smaller distance (or the smaller element on a tie).

## When to Use It

Use binary search on the answer space when you see:
- A **sorted** array.
- A request for a **contiguous** block of size `k` (or a window) that optimizes some monotonic condition.
- The condition can be expressed as “is the element at `mid` a better left bound than the element at `mid+k`?”  

In this problem the condition is:  
`x - arr[mid] > arr[mid+k] - x` (or the tie‑break rule) → move `left` to `mid+1`; otherwise keep `mid` as a candidate.

## Template

```python
def find_left_bound(arr, k, x):
    """
    Returns the starting index of the length-k window that holds the k closest
    elements to x.  The array is sorted in ascending order.
    """
    left, right = 0, len(arr) - k          # left can go up to len(arr)-k
    while left < right:
        mid = (left + right) // 2
        # If x is closer to arr[mid+k] than to arr[mid],
        # the window must start right of mid.
        if x - arr[mid] > arr[mid + k] - x:
            left = mid + 1
        else:
            right = mid
    return left
```

---

## LeetCode Problem Walkthrough

### Problem: 658. Find K Closest Elements

https://leetcode.com/problems/find-k-closest-elements/

### Approach 1: Brute Force (Sort by Distance)

**Algorithm**
1. Compute the absolute distance `|arr[i] - x|` for every element.
2. Sort the indices (or values) by distance, breaking ties by the actual value.
3. Take the first `k` elements from the sorted list.
4. Return them sorted in ascending order (as required).

**Implementation**

```python
class Solution:
    def findClosestElements(self, arr: List[int], k: int, x: int) -> List[int]:
        # Pair each element with its distance and value for tie‑breaking
        paired = [(abs(num - x), num) for num in arr]
        # Sort by distance, then by value
        paired.sort(key=lambda p: (p[0], p[1]))
        # Extract the values of the first k pairs
        closest = [val for _, val in paired[:k]]
        # Final output must be sorted
        closest.sort()
        return closest
```

**Complexity Analysis**
- Time complexity: O(n log n) — sorting `n` elements.
- Space complexity: O(n) — the list of pairs.

---

### Approach 2: Two Pointers (Shrink Window)

**Intuition**
Because the array is sorted, the `k` closest elements must form a contiguous subarray.  
Start with the whole array as a window `[left, right]`. While the window size exceeds `k`, discard the endpoint that is farther from `x` (or the larger endpoint on a tie). When the window size equals `k`, it contains the answer.

**Algorithm**
1. Initialize `left = 0`, `right = len(arr) - 1`.
2. While `right - left + 1 > k`:
   - Compare `abs(arr[left] - x)` and `abs(arr[right] - x)`.
   - If the left element is farther (or equal but larger), move `left` rightward.
   - Otherwise move `right` leftward.
3. Return `arr[left:right+1]`.

**Implementation**

```python
class Solution:
    def findClosestElements(self, arr: List[int], k: int, x: int) -> List[int]:
        left, right = 0, len(arr) - 1
        while right - left + 1 > k:
            if abs(arr[left] - x) > abs(arr[right] - x) or \
               (abs(arr[left] - x) == abs(arr[right] - x) and arr[left] > arr[right]):
                left += 1
            else:
                right -= 1
        return arr[left:right+1]
```

**Complexity Analysis**
- Time complexity: O(n - k) — each iteration removes one element; at most `n-k` steps.
- Space complexity: O(1) — only two pointers.

---

### Approach 3: Binary Search for Left Bound (Optimal)

**Intuition**
Instead of shrinking from both ends, we can directly locate the optimal starting index.  
If we pick a candidate start `mid`, the window is `[mid, mid+k-1]`.  
The only element that could improve the window by moving the start right is `arr[mid+k]` (the element just outside the window on the right).  
If `x` is closer to `arr[mid+k]` than to `arr[mid]`, the window must start right of `mid`; otherwise, `mid` is a viable start (or we need to look left).  
This yields a classic binary search on the range `[0, len(arr)-k]`.

**Algorithm**
1. Set `left = 0`, `right = len(arr) - k`.
2. While `left < right`:
   - Compute `mid = (left + right) // 2`.
   - If `x - arr[mid] > arr[mid + k] - x` (left side farther), set `left = mid + 1`.
   - Else set `right = mid`.
3. When the loop ends, `left` is the best start index.
4. Return `arr[left:left+k]`.

**Implementation**

```python
class Solution:
    def findClosestElements(self, arr: List[int], k: int, x: int) -> List[int]:
        left, right = 0, len(arr) - k
        while left < right:
            mid = (left + right) // 2
            # Compare distances of arr[mid] and arr[mid+k]
            if x - arr[mid] > arr[mid + k] - x:
                left = mid + 1
            else:
                right = mid
        return arr[left:left + k]
```

**Complexity Analysis**
- Time complexity: O(log (n - k)) — binary search over possible start positions.
- Space complexity: O(1) — only a few integer variables.

---

## Dry Run

**Input:** `arr = [1,2,3,4,5]`, `k = 4`, `x = 3`

We will trace the binary search approach (Approach 3).

| Step | left | right | mid | arr[mid] | arr[mid+k] | x - arr[mid] | arr[mid+k] - x | Condition (x - arr[mid] > arr[mid+k] - x) | New left | New right |
|------|------|-------|-----|----------|------------|--------------|----------------|-------------------------------------------|----------|-----------|
| 1    | 0    | 1     | 0   | 1        | 5          | 2            | 2              | False (2 > 2? no)                         | 0        | 0         |
| 2    | 0    | 0     | –   | –        | –          | –            | –              | Loop ends                                 |          |           |

Final start index = `left = 0`.  
Result = `arr[0:0+4] = [1,2,3,4]` → matches expected output.

---

**Summary**

- Brute force is simple but `O(n log n)`.
- Two‑pointer shrinkage works in linear time `O(n)`.
- Binary search on the left bound gives the optimal `O(log n)` solution while keeping `O(1)` extra space.  

Use the binary search template whenever you need to locate an optimal contiguous block in a sorted array.