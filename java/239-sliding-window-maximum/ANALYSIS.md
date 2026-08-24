# Sliding Window

## Video Solution

For more details about **Sliding Window Maximum**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=DfljaUwZsOk)

## Concept

The sliding window technique processes contiguous sub‑arrays (windows) of a fixed size `k` while moving from left to right.  
For the *maximum* in each window we need a data structure that can:

* give the current maximum in O(1)
* discard elements that fall out of the window
* insert new elements efficiently  

A **monotonic deque** (double‑ended queue) stores indices in decreasing order of their values. The front always holds the index of the current window’s maximum, and we pop from the back while the new element is larger – this maintains the decreasing order.

*Analogy*: Imagine a line of people waiting for a ride, ordered by height tallest → shortest. When a new person arrives, anyone shorter than them can never become the tallest while the new person is in front, so they leave the line. The person at the front is always the tallest (the maximum) for the current window.

## When to Use It

Use a sliding window with a monotonic deque when you see:

* A fixed‑size window (`k`) moving over an array
* Need to compute **max** (or **min**) for each window
* The array is one‑dimensional and contiguous
* Constraints are large (n up to 10⁵) → O(n·k) is too slow

## Template

```python
from collections import deque
from typing import List

def sliding_window_max(nums: List[int], k: int) -> List[int]:
    """
    Returns the maximum for each sliding window of size k.
    """
    n = len(nums)
    if n == 0 or k == 0:
        return []

    # deque will store indices; values at those indices are in decreasing order
    dq = deque()
    result = []

    for i in range(n):
        # 1️⃣ Remove indices that are out of the current window
        if dq and dq[0] <= i - k:
            dq.popleft()

        # 2️⃣ Remove from back all indices whose value is <= current value
        #    (they can never be maximum while current element is in the window)
        while dq and nums[dq[-1]] <= nums[i]:
            dq.pop()

        # 3️⃣ Add current index
        dq.append(i)

        # 4️⃣ The front holds the max for windows that have reached size k
        if i >= k - 1:
            result.append(nums[dq[0]])

    return result
```

## LeetCode Problem Walkthrough

### Problem: 239. Sliding Window Maximum

https://leetcode.com/problems/sliding-window-maximum/

---

### Approach 1: Brute Force

**Algorithm**
For each possible window start `i` (0 … n‑k), scan the `k` elements in that window to find the maximum, then store it.

**Implementation**

```python
class Solution:
    def maxSlidingWindow(self, nums: List[int], k: int) -> List[int]:
        n = len(nums)
        ans = []
        for i in range(n - k + 1):
            window_max = nums[i]
            for j in range(i, i + k):
                if nums[j] > window_max:
                    window_max = nums[j]
            ans.append(window_max)
        return ans
```

**Complexity Analysis**
- Time complexity: O(n·k) — we examine k elements for each of the (n‑k+1) windows.
- Space complexity: O(1) — only a few scalar variables besides the output list.

---

### Approach 2: Max‑Heap (Priority Queue)

**Intuition**
A heap can give the current maximum in O(1) time, but we must discard elements that leave the window. By storing pairs `(-value, index)` we turn Python’s min‑heap into a max‑heap. When the top of the heap is outside the window we pop it away.

**Algorithm**
1. Push the first `k` elements into the heap.
2. The heap top (after discarding out‑of‑range indices) is the window maximum.
3. Slide the window: add the new element, then repeatedly pop heap tops whose index ≤ i‑k (out of range).
4. Record the current top’s value.

**Implementation**

```python
import heapq
from typing import List

class Solution:
    def maxSlidingWindow(self, nums: List[int], k: int) -> List[int]:
        n = len(nums)
        if n * k == 0:
            return []
        if k == 1:
            return nums

        # max‑heap simulated by negating values
        heap = []
        for i in range(k):
            heapq.heappush(heap, (-nums[i], i))

        ans = [-heap[0][0]]  # current max

        for i in range(k, n):
            heapq.heappush(heap, (-nums[i], i))
            # remove elements that fell out of the window
            while heap[0][1] <= i - k:
                heapq.heappop(heap)
            ans.append(-heap[0][0])

        return ans
```

**Complexity Analysis**
- Time complexity: O(n·log k) — each element is pushed once and popped at most once; heap ops cost log k.
- Space complexity: O(k) — the heap stores at most k elements.

---

### Approach 3: Monotonic Deque (Optimal)

**Intuition**
We maintain a deque of indices where the corresponding values are in **strictly decreasing** order.  
* The front always holds the index of the largest value in the current window.  
* When a new element arrives, we discard from the back all indices whose value ≤ new value — they can never become maximum while the newer (and larger/equal) element stays in the window.  
* We also pop from the front if its index is out of the window’s left bound.

**Algorithm**
1. Iterate `i` from 0 to n‑1.
2. Remove front indices that are ≤ i‑k (out of window).
3. Remove back indices while `nums[back] ≤ nums[i]`.
4. Append `i` to the deque.
5. Once `i ≥ k‑1`, the front of the deque is the max for the window ending at `i`; append `nums[front]` to answer.

**Implementation**

```python
from collections import deque
from typing import List

class Solution:
    def maxSlidingWindow(self, nums: List[int], k: int) -> List[int]:
        n = len(nums)
        if n * k == 0:
            return []
        if k == 1:
            return nums

        dq = deque()          # stores indices, values decreasing
        ans = []

        for i in range(n):
            # 1️⃣ Remove indices that are out of this window
            if dq and dq[0] <= i - k:
                dq.popleft()

            # 2️⃣ Maintain decreasing order: drop smaller/equal values
            while dq and nums[dq[-1]] <= nums[i]:
                dq.pop()

            # 3️⃣ Add current index
            dq.append(i)

            # 4️⃣ Record max for windows that have reached size k
            if i >= k - 1:
                ans.append(nums[dq[0]])

        return ans
```

**Complexity Analysis**
- Time complexity: O(n) — each index is inserted and removed from the deque at most once.
- Space complexity: O(k) — the deque never holds more than k indices.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimal deque algorithm on the example:

```
nums = [1, 3, -1, -3, 5, 3, 6, 7], k = 3
```

| Step | i | nums[i] | Deque (front → back) after cleanup | Action taken | Window (i‑k+1 … i) | Current max |
|------|---|---------|------------------------------------|--------------|--------------------|-------------|
| 0    | 0 | 1       | [0]                                | add 0        | –                  | –           |
| 1    | 1 | 3       | [1]                                | pop 0 (1≤3), add 1 | –            | –           |
| 2    | 2 | -1      | [1,2]                              | keep 1, add 2 (since -1<3) | [0,2] → [1,3,-1] | nums[1]=3 |
| 3    | 3 | -3      | [1,2,3]                            | keep all (‑3 < -1) | [1,3] → [3,-1,-3] | nums[1]=3 |
| 4    | 4 | 5       | [4]                                | pop 3,2,1 (‑3,‑1,3 ≤5), add 4 | [2,4] → [-1,-3,5] | nums[4]=5 |
| 5    | 5 | 3       | [4,5]                              | keep 4 (5>3), add 5 | [3,5] → [-3,5,3] | nums[4]=5 |
| 6    | 6 | 6       | [6]                                | pop 5,4 (3,5 ≤6), add 6 | [4,6] → [5,3,6] | nums[6]=6 |
| 7    | 7 | 7       | [7]                                | pop 6 (6≤7), add 7 | [5,7] → [3,6,7] | nums[7]=7 |

Resulting maxima: `[3,3,5,5,6,7]` – matches the expected output.

---