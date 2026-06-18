# Fast & Slow Pointers

## Video Solution

For more details about **Find the Duplicate Number**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=wjYnzkAhcNk)

## Concept

Treat the array as a linked list where each index points to the value at that index (`i → nums[i]`). Because the numbers are in `[1, n]` and there are `n+1` entries, at least one value is visited twice, creating a cycle. The entry point of this cycle is the duplicate number. Floyd’s Tortoise and Hare algorithm finds the cycle’s entry using two pointers moving at different speeds, requiring only constant extra space and without modifying the array.

## When to Use It

- The problem guarantees exactly one duplicate in an array of size `n+1` with values `1…n`.
- You need **O(1) extra space** and must not modify the input.
- The structure resembles a linked list with a cycle (e.g., “find the entrance to a cycle”).
- Appears in follow‑up questions that ask for linear time, constant space solutions.

## Template

```python
def find_duplicate(nums):
    # Phase 1: find intersection point of two runners.
    slow = fast = 0
    while True:
        slow = nums[slow]          # move 1 step
        fast = nums[nums[fast]]    # move 2 steps
        if slow == fast:
            break

    # Phase 2: find entrance to the cycle.
    slow2 = 0
    while slow != slow2:
        slow = nums[slow]      # move 1 step
        slow2 = nums[slow2]    # move 1 step
    return slow
```

---

## LeetCode Problem Walkthrough

### Problem: 287. Find the Duplicate Number

https://leetcode.com/problems/find-the-duplicate-number/

### Approach 1: Brute Force

**Algorithm**  
Check every pair of indices; if two positions hold the same value, return it.

**Implementation**

```python
class Solution:
    def findDuplicate(self, nums: List[int]) -> int:
        n = len(nums)
        for i in range(n):
            for j in range(i + 1, n):
                if nums[i] == nums[j]:
                    return nums[i]
        return -1  # should never happen given constraints
```

**Complexity Analysis**

- Time complexity: O(n²) — each element is compared with all later elements.  
- Space complexity: O(1) — only a few integer variables are used.

### Approach 2: Binary Search on Value Range

**Intuition**  
For a candidate value `mid`, count how many numbers in the array are ≤ `mid`.  
If the count exceeds `mid`, the duplicate must be ≤ `mid` (pigeonhole principle); otherwise it is > `mid`. This lets us binary‑search the answer without extra space.

**Algorithm**  
1. Set `low = 1`, `high = n` (where `n = len(nums)-1`).  
2. While `low < high`:  
   - `mid = (low + high) // 2`  
   - Count `cnt = sum(1 for x in nums if x <= mid)`  
   - If `cnt > mid`: duplicate is in `[low, mid]` → `high = mid`  
   - Else: duplicate is in `[mid+1, high]` → `low = mid + 1`  
3. Return `low` (or `high`) as the duplicate.

**Implementation**

```python
class Solution:
    def findDuplicate(self, nums: List[int]) -> int:
        low, high = 1, len(nums) - 1
        while low < high:
            mid = (low + high) // 2
            cnt = sum(1 for x in nums if x <= mid)
            if cnt > mid:
                high = mid          # duplicate is in lower half
            else:
                low = mid + 1       # duplicate is in upper half
        return low
```

**Complexity Analysis**

- Time complexity: O(n log n) — each of the log n iterations scans the whole array.  
- Space complexity: O(1) — only a few counters are used.

### Approach 3: Floyd’s Tortoise and Hare (Fast & Slow Pointers)

**Intuition**  
Interpret the array as a function `f(i) = nums[i]`. Starting from index `0` and repeatedly applying `f` generates a sequence that must eventually repeat because there are only `n+1` possible values but `n+1` steps. The first repeated value creates a cycle; the duplicate is the entry point of this cycle. Two pointers moving at speeds 1 and 2 are guaranteed to meet inside the cycle; then resetting one pointer to the start and moving both at speed 1 finds the cycle’s entrance.

**Algorithm**  
1. **Phase 1** – Find intersection: move `slow` by one step, `fast` by two steps until they meet.  
2. **Phase 2** – Find entrance: reset `slow2` to start; move both `slow` and `slow2` one step at a time until they meet; that meeting point is the duplicate.

**Implementation**

```python
class Solution:
    def findDuplicate(self, nums: List[int]) -> int:
        # Phase 1: find intersection point inside the cycle
        slow = fast = 0
        while True:
            slow = nums[slow]          # 1 step
            fast = nums[nums[fast]]    # 2 steps
            if slow == fast:
                break

        # Phase 2: find entrance to the cycle (the duplicate)
        slow2 = 0
        while slow != slow2:
            slow = nums[slow]      # 1 step
            slow2 = nums[slow2]    # 1 step
        return slow
```

**Complexity Analysis**

- Time complexity: O(n) — each pointer traverses the list at most a constant number of times.  
- Space complexity: O(1) — only a few integer variables.

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

### Dry Run (Floyd’s Algorithm) on `nums = [1,3,4,2,2]`

```
| Phase | Step | slow (value) | fast (value) | Action                                 |
|-------|------|--------------|--------------|----------------------------------------|
| 1     | start| 0            | 0            | both start at index 0                  |
| 1     | 1    | nums[0]=1    | nums[nums[0]]=nums[1]=3 | move slow 1, fast 2 steps          |
| 1     | 2    | nums[1]=3    | nums[nums[3]]=nums[2]=4 | move slow 1, fast 2 steps          |
| 1     | 3    | nums[3]=2    | nums[nums[4]]=nums[2]=4 | move slow 1, fast 2 steps          |
| 1     | 4    | nums[2]=4    | nums[nums[4]]=nums[2]=4 | move slow 1, fast 2 steps → meet   |
|       |      | (slow=4, fast=4)                       | intersection found                |
| 2     | reset| slow2=0      |              | slow2 starts at beginning             |
| 2     | 1    | slow=nums[4]=2 | slow2=nums[0]=1 | advance both 1 step                 |
| 2     | 2    | slow=nums[2]=4 | slow2=nums[1]=3 | advance both 1 step                 |
| 2     | 3    | slow=nums[4]=2 | slow2=nums[3]=2 | advance both 1 step → meet          |
|       |      | (both = 2)                               | duplicate found = 2               |
```

The algorithm correctly identifies `2` as the repeated number.