# Sliding Window

## Video Solution

For more details about **Shortest and Lexicographically Smallest Beautiful String**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=C0pzkWR_278).

## Concept

A sliding window maintains a contiguous segment of the string while we expand the right end and contract the left end to keep a certain property (here, the number of `'1'` characters).  
By moving the window in O(n) time we can examine every candidate that satisfies the condition without checking all O(n²) substrings explicitly.

## When to Use It

Use sliding window when the problem asks for a **contiguous substring** that satisfies a **count‑based condition** (e.g., exactly *k* ones, at most *k* zeros, etc.) and you need the **shortest** or **longest** such substring.  
Typical clues:  
- “substring”, “contiguous”, “window”  
- condition involves a running total (count of 1’s, sum, etc.)  
- we want to minimize/maximize length while the condition holds.

## Template

```python
def sliding_window(s: str, k: int) -> str:
    left = 0                 # left boundary of the window
    ones = 0                 # number of '1's inside the window
    best_len = float('inf')  # length of the best answer found so far
    best_str = ""            # lexicographically smallest answer of that length

    for right in range(len(s)):
        # expand window to include s[right]
        if s[right] == '1':
            ones += 1

        # shrink from left while we have too many ones (> k)
        while ones > k and left <= right:
            if s[left] == '1':
                ones -= 1
            left += 1

        # when we have exactly k ones, try to discard leading zeros
        # to make the window as short as possible while keeping k ones
        if ones == k:
            # move left past any zeros that do not affect the count of ones
            while left <= right and s[left] == '0':
                left += 1

            cur_len = right - left + 1
            cur_str = s[left:right+1]

            if cur_len < best_len or (cur_len == best_len and cur_str < best_str):
                best_len = cur_len
                best_str = cur_str

    return best_str
```

## LeetCode Problem Walkthrough

### Problem: 2904. Shortest and Lexicographically Smallest Beautiful String

https://leetcode.com/problems/shortest-and-lexicographically-smallest-beautiful-string/

### Approach 1: Brute Force

**Algorithm**  
Enumerate every possible substring, count the number of `'1'` characters, and keep track of the shortest length that meets the requirement. Among substrings of that minimal length, retain the lexicographically smallest one.

**Implementation**

```python
class Solution:
    def shortestBeautifulSubstring(self, s: str, k: int) -> str:
        n = len(s)
        best_len = float('inf')
        best_str = ""

        for i in range(n):
            ones = 0
            for j in range(i, n):
                if s[j] == '1':
                    ones += 1
                if ones == k:
                    cur_len = j - i + 1
                    cur_str = s[i:j+1]
                    if cur_len < best_len or (cur_len == best_len and cur_str < best_str):
                        best_len = cur_len
                        best_str = cur_str
                # if ones > k we can break early because extending further will only increase ones
                if ones > k:
                    break

        return best_str if best_len != float('inf') else ""
```

**Complexity Analysis**

- Time complexity: O(n²) — two nested loops over the string.  
- Space complexity: O(1) — only a few integer variables are used.

### Approach 2: Sliding Window (find minimal length)

**Intuition**  
Instead of checking every substring, we can maintain a window that always contains at most `k` ones. When the window contains exactly `k` ones, we know it is a candidate. By moving the left pointer past unnecessary leading zeros we shrink the window to its smallest possible size while still holding `k` ones. This yields the minimal length in linear time.

**Algorithm**  
1. Expand the right end of the window, counting ones.  
2. If the count exceeds `k`, move the left end forward until the count is ≤ `k`.  
3. Whenever the count equals `k`, tighten the left end over any leading zeros (they do not affect the ones count) to obtain the shortest window ending at `right`.  
4. Track the globally shortest length seen; if a tie occurs, keep the lexicographically smaller string.

**Implementation**

```python
class Solution:
    def shortestBeautifulSubstring(self, s: str, k: int) -> str:
        left = 0
        ones = 0
        best_len = float('inf')
        best_str = ""

        for right, ch in enumerate(s):
            if ch == '1':
                ones += 1

            # shrink while we have too many ones
            while ones > k and left <= right:
                if s[left] == '1':
                    ones -= 1
                left += 1

            # when we have exactly k ones, discard leading zeros
            if ones == k:
                while left <= right and s[left] == '0':
                    left += 1

                cur_len = right - left + 1
                cur_str = s[left:right+1]

                if cur_len < best_len or (cur_len == best_len and cur_str < best_str):
                    best_len = cur_len
                    best_str = cur_str

        return best_str if best_len != float('inf') else ""
```

**Complexity Analysis**

- Time complexity: O(n) — each index is visited at most twice (once by `right`, once by `left`).  
- Space complexity: O(1) — only a few scalar variables.

### Approach 3: Sliding Window with Direct Lexicographic Tie‑break

**Intuition**  
The previous approach already yields the shortest length; we only need to ensure that, when multiple windows share that length, we pick the lexicographically smallest one. Since we examine windows in increasing order of `right`, we can update the answer whenever we find a window of the same best length that is lexicographically smaller.

**Algorithm**  
Identical to Approach 2, but we keep the update rule that favors a smaller string when lengths are equal.

**Implementation**  
(The code is the same as Approach 2; the explanation highlights the tie‑break.)

**Complexity Analysis**

- Time complexity: O(n) — same reasoning as Approach 2.  
- Space complexity: O(1) — same reasoning as Approach 2.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the example `s = "100011001"`, `k = 3`.

```
| right | s[right] | ones | left before shrink | left after shrink (ones>k) | left after zero‑trim | window (s[left:right]) | length | current best |
|-------|----------|------|--------------------|----------------------------|----------------------|------------------------|--------|--------------|
| 0     | 1        | 1    | 0                  | 0                          | 0                    | "1"                    | 1      | –            |
| 1     | 0        | 1    | 0                  | 0                          | 0                    | "10"                   | 2      | –            |
| 2     | 0        | 1    | 0                  | 0                          | 0                    | "100"                  | 3      | –            |
| 3     | 0        | 1    | 0                  | 0                          | 0                    | "1000"                 | 4      | –            |
| 4     | 1        | 2    | 0                  | 0                          | 0                    | "10001"                | 5      | –            |
| 5     | 1        | 3    | 0                  | 0                          | 0                    | "100011"               | 6      | "100011" (len6) |
| 6     | 0        | 3    | 0                  | 0                          | 0                    | "1000110"              | 7      | –            |
| 7     | 0        | 3    | 0                  | 0                          | 0                    | "10001100"             | 8      | –            |
| 8     | 1        | 4    | 0                  | move left until ones≤3:    |                      |                        |        |              |
|       |          |      |                    | left=1 (removed '1') → ones=3 | left=1                |                        |        |              |
|       |          |      |                    | trim leading zeros: s[1]='0' → left=2 | left=2 | window = s[2:9] = "0011001" → but leading zeros trimmed further? Actually we trim zeros while s[left]=='0': left=2 ('0') -> left=3 ('0') -> left=4 ('1') stop. | left=4 | window = s[4:9] = "11001" | length=5 | "11001" (len5 <6) |
```

After processing the whole string, the shortest length found is 5 and the lexicographically smallest substring of that length is `"11001"`, which matches the expected answer.

---

**Summary**  
- Use sliding window to achieve O(n) time.  
- Maintain a count of `'1'` inside the window.  
- When the count equals `k`, shrink leading zeros to obtain the minimal‑length window ending at the current `right`.  
- Track the globally shortest length and, on ties, the lexicographically smallest string.  

Commit this lecture as `/lectures/2025-08-24-sliding-window.md`.