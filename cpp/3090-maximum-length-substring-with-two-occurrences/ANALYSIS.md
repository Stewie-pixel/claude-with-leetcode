# Sliding Window

## Video Solution

For more details about **Maximum Length Substring With Two Occurrences**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=K23Q6wqtBkI).

## Concept

The sliding window technique maintains a contiguous segment of the array/string using two pointers (`left` and `right`). We expand the window by moving `right` and, whenever the window violates a condition, we shrink it from the left until the condition is restored.  
For this problem the condition is: **no character appears more than twice** inside the window. We keep a frequency count of characters in the current window; if any count exceeds 2, we move `left` forward, decreasing the count of the character we leave behind, until all counts are ≤ 2 again. The window size (`right‑left+1`) is a candidate for the answer, and we track the maximum size seen.

**Real‑world analogy:** Imagine a buffet line where you can take at most two plates of each dish. You keep grabbing dishes (moving `right`). If you ever try to take a third plate of the same dish, you start putting back dishes from the front of your tray (moving `left`) until you have at most two of that dish again. The longest tray you ever held is the answer.

## When to Use It

Use sliding window when you see:
- A request for the **maximum/minimum length** of a **contiguous** subarray or substring.
- A condition that can be maintained incrementally (e.g., frequency limits, sum constraints, distinct‑element limits).
- Phrases like “at most K”, “exactly K”, “no more than”, “longest subarray where …”.

## Template

```python
def sliding_window(s: str, max_allowed: int) -> int:
    """
    Returns the longest substring length where each character appears
    at most `max_allowed` times.
    """
    freq = [0] * 26          # frequency of each lowercase letter
    left = 0
    best = 0

    for right, ch in enumerate(s):
        idx = ord(ch) - ord('a')
        freq[idx] += 1

        # shrink window while any character exceeds the limit
        while freq[idx] > max_allowed:
            freq[ord(s[left]) - ord('a')] -= 1
            left += 1

        # window [left, right] is valid
        best = max(best, right - left + 1)

    return best
```

## LeetCode Problem Walkthrough

### Problem: 3090. Maximum Length Substring With Two Occurrences

https://leetcode.com/problems/maximum-length-substring-with-two-occurrences/

### Approach 1: Brute Force

**Algorithm**
1. Enumerate every possible substring `s[i:j]` (O(n²) substrings).
2. For each substring, count frequencies of its characters.
3. If all frequencies ≤ 2, update the answer with its length.
4. Return the maximum length found.

**Implementation**

```python
class Solution:
    def maximumLengthSubstring(self, s: str) -> int:
        n = len(s)
        ans = 0
        for i in range(n):
            freq = [0] * 26
            for j in range(i, n):
                freq[ord(s[j]) - ord('a')] += 1
                # if any count exceeds 2, this substring is invalid;
                # longer substrings starting at i will also be invalid,
                # so we can break early.
                if freq[ord(s[j]) - ord('a')] > 2:
                    break
                ans = max(ans, j - i + 1)
        return ans
```

**Complexity Analysis**
- Time complexity: O(n²) – two nested loops over the string.
- Space complexity: O(1) – fixed-size frequency array of length 26.

### Approach 2: Sliding Window (Array Frequency)

**Intuition**
Instead of re‑counting frequencies for every substring, we can maintain a window that always satisfies the “at most two occurrences” rule. When we add a new character and its count goes above 2, we move the left border forward until the offending character’s count drops back to ≤ 2. This guarantees O(n) time because each pointer moves at most n steps.

**Algorithm**
- Use two pointers `left` and `right` to represent the current window.
- Keep an array `freq[26]` for character counts inside the window.
- Expand `right` one step at a time, updating `freq`.
- While the count of the newly added character exceeds 2, shrink the window from the left, decrementing the count of `s[left]`.
- After each expansion, the window is valid; update the answer with its size.

**Implementation**

```python
class Solution:
    def maximumLengthSubstring(self, s: str) -> int:
        freq = [0] * 26          # counts for 'a'..'z'
        left = 0
        best = 0

        for right, ch in enumerate(s):
            idx = ord(ch) - ord('a')
            freq[idx] += 1

            # If this character now appears >2 times, shrink from left
            while freq[idx] > 2:
                freq[ord(s[left]) - ord('a')] -= 1
                left += 1

            # Window [left, right] is valid
            best = max(best, right - left + 1)

        return best
```

**Complexity Analysis**
- Time complexity: O(n) – each index is visited at most twice (once by `right`, once by `left`).
- Space complexity: O(1) – constant‑size frequency array.

### Approach 3: Sliding Window with Early Exit (Optional Optimization)

**Intuition**
The naive sliding window already runs in O(n). A small tweak is to break the outer loop early when the remaining possible length (`n - left`) cannot beat the current best answer. This does not change asymptotic complexity but can cut constant factors for large inputs.

**Algorithm**
- Same as Approach 2, but before each iteration of `right`, check if `n - left <= best`. If true, no longer substring can improve the answer, so we stop.

**Implementation**

```python
class Solution:
    def maximumLengthSubstring(self, s: str) -> int:
        n = len(s)
        freq = [0] * 26
        left = 0
        best = 0

        for right, ch in enumerate(s):
            # Early stop: even if we take the whole suffix, we can't beat best
            if n - left <= best:
                break

            idx = ord(ch) - ord('a')
            freq[idx] += 1

            while freq[idx] > 2:
                freq[ord(s[left]) - ord('a')] -= 1
                left += 1

            best = max(best, right - left + 1)

        return best
```

**Complexity Analysis**
- Time complexity: O(n) – same as Approach 2; early exit may reduce constant work.
- Space complexity: O(1).

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the sliding‑window algorithm on the example `s = "bcbbbcba"`.

| Step | right | s[right] | freq[b] | freq[c] | freq[other] | left | Window (s[left:right+1]) | Window length | best |
|------|-------|----------|---------|---------|--------------|------|--------------------------|---------------|------|
| init | –     | –        | 0       | 0       | 0            | 0    | –                        | 0             | 0    |
| 0    | 0     | b        | 1       | 0       | 0            | 0    | b                        | 1             | 1    |
| 1    | 1     | c        | 1       | 1       | 0            | 0    | bc                       | 2             | 2    |
| 2    | 2     | b        | 2       | 1       | 0            | 0    | bcb                      | 3             | 3    |
| 3    | 3     | b        | 3 → >2  | 1       | 0            | 0    | b c b b (invalid)        | –             | –    |
|      |       | shrink   |         |         |              |      |                          |               |      |
| 3a   | –     | –        | 2 (removed s[0]) | 1 | 0 | 1 | c b b                    | 3             | 3    |
| 3b   | –     | –        | 2 (still >2) | 1 | 0 | 2 | b b                      | 2             | 3    |
| 3c   | –     | –        | 2 (still >2) | 1 | 0 | 3 | b                        | 1             | 3    |
| 4    | 4     | b        | 2       | 1       | 0            | 3    | b b                      | 2             | 3    |
| 5    | 5     | c        | 2       | 2       | 0            | 3    | b b c                    | 3             | 3    |
| 6    | 6     | b        | 3 → >2  | 2       | 0            | 3    | b b c b (invalid)        | –             | –    |
|      |       | shrink   |         |         |              |      |                          |               |      |
| 6a   | –     | –        | 2 (removed s[3]) | 2 | 0 | 4 | b c b                    | 3             | 3    |
| 7    | 7     | a        | 2       | 2       | a:1          | 4    | b c b a                  | 4             | 4    |

The maximum valid window length observed is **4**, corresponding to the substring `"bcba"` (indices 4‑7).

---

**Summary:**  
- Brute force checks all substrings (O(n²)).  
- Sliding window maintains a valid window in O(n) time with O(1) space.  
- The key insight is that the condition “at most two occurrences per character” is monotonic: adding a character can only violate it, and removing from the left can only restore it, enabling the two‑pointer technique.