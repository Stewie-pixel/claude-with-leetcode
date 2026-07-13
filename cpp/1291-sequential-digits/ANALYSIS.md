# Backtracking

## Video Solution

For more details about **Sequential Digits**, watch the walkthrough at [https://www.youtube.com/watch?v=Q-ca65wRJyI](https://www.youtube.com/watch?v=Q-ca65wRJyI)

## Concept

Backtracking is a general algorithm for generating all (or some) solutions to a problem by building candidates incrementally and abandoning a candidate as soon as it determines that the candidate cannot possibly be completed to a valid solution.  
Think of it like exploring a maze: you walk down a path, and as soon as you hit a wall you backtrack to the last intersection and try a different direction.

## When to Use It

Use backtracking when you see:
- “Generate all …”, “List all …”, “Enumerate …”
- Constraints are small enough that the total number of candidates is manageable (often exponential, but the actual search space is pruned heavily)
- The problem can be solved by making a sequence of choices, where each choice leads to a sub‑problem of the same type

In **Sequential Digits** we need to list every integer whose digits increase by exactly 1. The set of such numbers is tiny (only 36 possible values), so we can generate them all with backtracking and then keep those that fall inside `[low, high]`.

## Template

```python
def backtrack(candidate):
    # 1. If candidate is a complete solution, process it
    if is_solution(candidate):
        process(candidate)
        return

    # 2. Otherwise, try to extend the candidate
    for choice in choices(candidate):
        # Prune branches that cannot lead to a solution
        if not is_valid(candidate + [choice]):
            continue
        # Choose
        candidate.append(choice)
        # Explore
        backtrack(candidate)
        # Un‑choose (backtrack)
        candidate.pop()
```

*`is_solution`* tells us we have built a full number;  
*`process`* records the number if it lies in the required range;  
*`choices`* returns the possible next digits (only `last_digit + 1` when it ≤ 9);  
*`is_valid`* checks that the prefix can still become a sequential number (i.e., the last digit is ≤ 8 so we can still add one more).

---

# LeetCode Problem Walkthrough

### Problem: 1291. Sequential Digits  
https://leetcode.com/problems/sequential-digits/

---

## Approach 1: Brute Force – Check Every Number

**Algorithm**
1. Iterate `num` from `low` to `high` (inclusive).
2. For each `num`, extract its digits and verify that each digit is exactly one more than the previous digit.
3. If the check passes, add `num` to the answer list.

**Implementation**

```python
class Solution:
    def sequentialDigits(self, low: int, high: int) -> List[int]:
        def is_sequential(x: int) -> bool:
            prev = x % 10
            x //= 10
            while x:
                cur = x % 10
                if cur + 1 != prev:          # digits must increase by 1 when read right‑to‑left
                    return False
                prev = cur
                x //= 10
            return True

        ans = []
        for num in range(low, high + 1):
            if is_sequential(num):
                ans.append(num)
        return ans
```

**Complexity Analysis**
- Time complexity: O((high‑low+1) * D) – we inspect every number in the range and each digit (D ≤ 10). In the worst case (`high = 10^9`) this is far too slow.
- Space complexity: O(1) extra (output list not counted).

---

## Approach 2: Generate All Sequential Numbers (String Slicing)

**Intuition**
All sequential numbers are substrings of the string `"123456789"`.  
For example, `"234"` appears starting at index 1 with length 3.  
Thus we can enumerate every possible length (2 … 9) and every valid start position, convert the substring to an integer, and keep it if it lies in `[low, high]`.  
Because the source string has only 9 digits, there are at most 36 candidates – constant time.

**Algorithm**
1. Let `digits = "123456789"`.
2. For each length `len` from 2 to 9:
   - For each start index `i` from 0 to `9‑len`:
     - Extract `sub = digits[i:i+len]`.
     - Convert `sub` to integer `num`.
     - If `low ≤ num ≤ high`, append `num` to answer.
3. Return the answer (already generated in increasing order).

**Implementation**

```python
class Solution:
    def sequentialDigits(self, low: int, high: int) -> List[int]:
        digits = "123456789"
        ans = []
        for length in range(2, 10):               # length of the number
            for start in range(0, 10 - length):   # valid start positions
                num = int(digits[start:start + length])
                if low <= num <= high:
                    ans.append(num)
        return ans
```

**Complexity Analysis**
- Time complexity: O(36) ≈ O(1) – we generate at most 9 + 8 + … + 2 = 36 numbers.
- Space complexity: O(1) extra (output list not counted).

---

## Approach 3: Breadth‑First Generation (Queue)

**Intuition**
We can build sequential numbers digit by digit: start with each digit 1‑9 as a queue entry, then repeatedly pop a number, append the next possible digit (`last_digit + 1` if ≤ 9), and push the new number back.  
This explores the search tree level‑by‑level and naturally produces numbers in increasing order.

**Algorithm**
1. Initialise a queue with the integers 1 … 9.
2. While the queue is not empty:
   - Pop the front number `cur`.
   - If `low ≤ cur ≤ high`, add `cur` to answer.
   - Let `last_digit = cur % 10`. If `last_digit < 9`, push `cur * 10 + (last_digit + 1)` into the queue.
3. Return the answer.

**Implementation**

```python
from collections import deque

class Solution:
    def sequentialDigits(self, low: int, high: int) -> List[int]:
        q = deque(range(1, 10))   # start with single‑digit numbers
        ans = []
        while q:
            cur = q.popleft()
            if low <= cur <= high:
                ans.append(cur)
            last = cur % 10
            if last < 9:          # we can extend the sequence
                q.append(cur * 10 + last + 1)
        return ans
```

**Complexity Analysis**
- Time complexity: O(36) ≈ O(1) – each sequential number is enqueued and dequeued at most once.
- Space complexity: O(36) ≈ O(1) for the queue (output not counted).

---

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the **string‑slicing** approach (Approach 2) on the sample `low = 100`, `high = 300`.

The candidates generated in order are:

| length | start | substring | num | in range? |
|--------|-------|-----------|-----|-----------|
| 2      | 0     | "12"      | 12  | No        |
| 2      | 1     | "23"      | 23  | No        |
| 2      | 2     | "34"      | 34  | No        |
| 2      | 3     | "45"      | 45  | No        |
| 2      | 4     | "56"      | 56  | No        |
| 2      | 5     | "67"      | 67  | No        |
| 2      | 6     | "78"      | 78  | No        |
| 2      | 7     | "89"      | 89  | No        |
| 3      | 0     | "123"     | 123 | **Yes**   |
| 3      | 1     | "234"     | 234 | **Yes**   |
| 3      | 2     | "345"     | 345 | No (>300) |
| 3      | 3     | "456"     | 456 | No        |
| …      | …     | …         | …   | …         |
| 4      | 0     | "1234"    | 1234| No (>300) |
| …      | …     | …         | …   | …         |

Only **123** and **234** satisfy the range, so the final answer is `[123, 234]`.

---

**Summary**  
- Brute force checks every number – simple but inefficient.  
- Generating all sequential numbers from the fixed string `"123456789"` is constant‑time and easy to implement.  
- A BFS/queue approach offers the same complexity while illustrating the backtracking idea of extending a prefix digit by digit.  

All three approaches produce the correct, sorted list because we generate candidates in increasing order.