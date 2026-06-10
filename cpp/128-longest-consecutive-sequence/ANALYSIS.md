# Arrays & Hashing

## Video Solution

For more details about **Longest Consecutive Sequence**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=P6RZZMu_maU).

## Concept

The core idea is to use a hash set for O(1) look‑ups so we can quickly check whether a number’s neighbors exist.  
Instead of scanning every possible start‑end pair (which would be O(n²)), we only begin counting a sequence when we know the current number is the **first** element of a consecutive run (i.e., `num‑1` is not in the set). From there we keep extending the run while `num+1`, `num+2`, … are present. Because each number is visited at most twice (once as a possible start, once while extending a run), the total time is O(n).

## When to Use It

Use a hash‑set based approach when you see:
- An unsorted array of integers.
- A request for the longest **consecutive** sequence (or similar “streak” problems).
- A required O(n) time solution.
- Clues like “consecutive”, “streak”, “sequence”, or “continuous”.

## Template

```python
def longest_consecutive(nums):
    """
    Returns the length of the longest consecutive elements sequence.
    Runs in O(n) time and O(n) extra space.
    """
    num_set = set(nums)          # O(n) build
    longest = 0

    for num in num_set:          # each element examined at most twice
        # Only start counting from the beginning of a sequence
        if num - 1 not in num_set:
            length = 1
            # Extend the sequence while the next number exists
            while num + length in num_set:
                length += 1
            longest = max(longest, length)

    return longest
```

## LeetCode Problem Walkthrough

### Problem: 128. Longest Consecutive Sequence

https://leetcode.com/problems/longest-consecutive-sequence/

### Approach 1: Brute Force (Sorting)

**Algorithm**
1. If the array is empty, return 0.
2. Sort the array – this brings equal or consecutive numbers together.
3. Scan the sorted array, keeping a `current_streak` that increments when the next number is exactly one greater than the previous, or resets when a gap or duplicate is found.
4. Track the maximum streak seen.

**Implementation**

```python
class Solution:
    def longestConsecutive(self, nums: List[int]) -> int:
        if not nums:
            return 0

        nums.sort()                     # O(n log n)
        max_len = cur_len = 1

        for i in range(1, len(nums)):
            if nums[i] == nums[i - 1] + 1:
                cur_len += 1
            elif nums[i] == nums[i - 1]:
                continue               # duplicate, ignore
            else:
                max_len = max(max_len, cur_len)
                cur_len = 1

        return max(max_len, cur_len)
```

**Complexity Analysis**
- Time complexity: O(n log n) — sorting dominates.
- Space complexity: O(1) — sort is in‑place (ignoring the small recursion stack).

### Approach 2: Hash Set (Optimal)

**Intuition**
By storing all numbers in a set we gain O(1) existence checks.  
A sequence can only start at a number that has no left neighbor (`num‑1` absent). Starting from each such left‑boundary guarantees we count each sequence exactly once, giving linear overall work.

**Algorithm**
1. Insert all numbers into a hash set.
2. Iterate over the set; for each `num`, if `num‑1` is not in the set, it is the start of a sequence.
3. From this start, repeatedly check `num+1`, `num+2`, … while they exist, counting the length.
4. Update the global maximum length.

**Implementation**

```python
class Solution:
    def longestConsecutive(self, nums: List[int]) -> int:
        num_set = set(nums)          # O(n) space
        longest = 0

        for num in num_set:
            # Only count from the beginning of a streak
            if num - 1 not in num_set:
                length = 1
                while num + length in num_set:
                    length += 1
                longest = max(longest, length)

        return longest
```

**Complexity Analysis**
- Time complexity: O(n) — each number is processed at most twice (once as a start, once while extending a streak).
- Space complexity: O(n) — the hash set stores all input numbers.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: `nums = [100, 4, 200, 1, 3, 2]`

| Step | num (from set) | num‑1 in set? | Action                               | current length | longest |
|------|----------------|---------------|--------------------------------------|----------------|---------|
| 1    | 100            | No (99 absent)| start streak at 100                  | 1 (100)        | 1       |
| 2    | 4              | Yes (3 present)| skip (not a streak start)            | –              | 1       |
| 3    | 200            | No (199 absent)| start streak at 200                  | 1 (200)        | 1       |
| 4    | 1              | No (0 absent) | start streak at 1 → 2 → 3 → 4        | 4              | 4       |
| 5    | 3              | Yes (2 present)| skip                                 | –              | 4       |
| 6    | 2              | Yes (1 present)| skip                                 | –              | 4       |

The longest consecutive sequence found is `[1,2,3,4]` with length **4**.